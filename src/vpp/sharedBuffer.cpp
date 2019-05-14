// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/sharedBuffer.hpp>
#include <vpp/queue.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>
#include <algorithm>

namespace vpp {
namespace {

/// Returns the offset a SubBuffer with the given usage flags will
/// be created with.
vk::DeviceSize usageAlignment(const Device& dev, vk::BufferUsageFlags usage) {
	vk::DeviceSize ret = 0u;

	auto align = dev.properties().limits.minUniformBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::uniformBuffer && align > 0) {
		ret = std::max(ret, align);
	}

	align = dev.properties().limits.minTexelBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::uniformTexelBuffer && align > 0) {
		ret = std::max(ret, align);
	}

	align = dev.properties().limits.minStorageBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::storageBuffer && align > 0) {
		ret = std::max(ret, align);
	}

	return ret;
}

} // anon namespace

// BufferRange
SubBuffer::SubBuffer(SharedBuffer& buf, const Allocation& alloc) :
		buffer_(&buf), allocation_(alloc) {
	dlg_assert(buf.vkHandle());
	dlg_assert(alloc.size != 0);
}

SubBuffer::SubBuffer(BufferAllocator& alloc, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits,
		vk::DeviceSize align) {
	dlg_assert(memBits);
	auto [buffer, a] = alloc.alloc(size, usage, memBits, align);
	buffer_ = &buffer;
	allocation_ = a;
}

SubBuffer::SubBuffer(InitData& data, BufferAllocator& alloc,
		vk::DeviceSize size, vk::BufferUsageFlags usage, unsigned memBits,
		vk::DeviceSize align) {
	dlg_assert(memBits);
	data.allocator = &alloc;
	alloc.reserve(size, usage, memBits, align, &data.reservation);
}

SubBuffer::~SubBuffer() {
	if(buffer_) {
		dlg_assert(allocation_.size > 0);
		buffer_->free(allocation_);
	}
}

void SubBuffer::init(InitData& data) {
	dlg_assert(data.allocator && data.reservation);
	dlg_assert(!buffer_);

	auto [buf, alloc] = data.allocator->alloc(data.reservation);
	buffer_ = &buf;
	allocation_ = alloc;
	data.allocator = nullptr;
}

MemoryMapView SubBuffer::memoryMap(vk::DeviceSize offset,
		vk::DeviceSize size) const {
	size = (size == vk::wholeSize) ? this->size() : size;
	dlg_assert(offset + size <= this->size());
	offset += this->offset();
	return buffer().memoryMap(offset, size);
}

void swap(SubBuffer& a, SubBuffer& b) noexcept {
	using std::swap;
	swap(a.buffer_, b.buffer_);
	swap(a.allocation_, b.allocation_);
}

// InitData
SubBuffer::InitData::InitData(InitData&& rhs) noexcept {
	allocator = rhs.allocator;
	reservation = rhs.reservation;
	rhs.allocator = {};
	rhs.reservation = {};
}

SubBuffer::InitData& SubBuffer::InitData::operator=(
		InitData&& rhs) noexcept {
	this->~InitData();
	allocator = rhs.allocator;
	reservation = rhs.reservation;
	rhs.allocator = {};
	rhs.reservation = {};
	return *this;
}

SubBuffer::InitData::~InitData() {
	if(allocator) {
		dlg_assert(reservation);
		allocator->cancel(reservation);
	}
}

// SharedBuffer
SharedBuffer::SharedBuffer(const Device& dev, const vk::BufferCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* allocator) :
		Buffer(dev, info, memBits, allocator), size_(info.size) {
}

SharedBuffer::SharedBuffer(const Device& dev, vk::Buffer buf, vk::DeviceSize size,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Buffer(dev, buf, memBits, alloc), size_(size) {
}

SharedBuffer::SharedBuffer(const Device& dev, const vk::BufferCreateInfo& info,
	DeviceMemory& mem) : Buffer(dev, info, mem), size_(info.size) {
}

SharedBuffer::SharedBuffer(InitData& data, const Device& dev,
	const vk::BufferCreateInfo& info, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		Buffer(data, dev, info, memBits, alloc), size_(info.size) {
}

SharedBuffer::~SharedBuffer() {
	dlg_assertm(allocations_.empty(), "~SharedBuffer: allocations left");
}

SharedBuffer::Allocation SharedBuffer::alloc(vk::DeviceSize size,
		vk::DeviceSize alignment) {
	dlg_assert(size > 0);
	dlg_assertm(alignment == 1 || alignment % 2 == 0,
		"Alignment {} not power of 2", alignment);

	// TODO: allocation algorithm can be improved, greedy atm

	// respect the nonCoherentAtomSize if the buffer is allocated on hostVisible
	// and not hostCoherent memory
	auto t = memory().type();
	auto flags = device().memoryProperties().memoryTypes[t].propertyFlags;
	auto nonCoherentAtomAlign =
		flags & vk::MemoryPropertyBits::hostVisible &&
		!(flags & vk::MemoryPropertyBits::hostCoherent);

	Allocation old = {0, 0};
	vk::DeviceSize endAlign = 0u;
	if(nonCoherentAtomAlign) {
		endAlign = device().properties().limits.nonCoherentAtomSize;
		alignment = std::max(alignment, endAlign);
	}

	for(auto it = allocations_.begin(); it != allocations_.end(); ++it) {
		auto aligned = vpp::align(old.end(), alignment);
		if(aligned + vpp::align(size, endAlign) <= it->offset) {
			Allocation range = {aligned, size};
			allocations_.insert(it, range);
			return range;
		}

		old = *it;
	}

	// last
	// we don't have to check for nonCoherentAtom end align here
	auto aligned = vpp::align(old.end(), alignment);
	if(aligned + size <= this->size()) {
		Allocation range = {aligned, size};
		allocations_.push_back(range);
		return range;
	}

	return {};
}

void SharedBuffer::free(const Allocation& alloc) {
	auto it = std::find(allocations_.begin(), allocations_.end(), alloc);
	dlg_assertm(it != allocations_.end(), "free: invalid allocation");
	allocations_.erase(it);
}

// BufferAllocator
BufferAllocator::BufferAllocator(const Device& dev) :
	BufferAllocator(dev.deviceAllocator()) {
}

BufferAllocator::BufferAllocator(DeviceMemoryAllocator& memAlloc) :
	devMemAlloc_(memAlloc) {
}

BufferAllocator::~BufferAllocator() {
	// shared buffer will warn if there are remaining allocations
	// on destruction
	// for(auto& res : reservations_) {
	// 	dlg_assert(res.buffer && res.allocation.size);
	// 	res.buffer->free(res.allocation);
	// }

	dlg_assertlm(dlg_level_warn, requirements_.empty(),
		"~BufferAllocator: pending requirements left");
	dlg_assertlm(dlg_level_warn, reservations_.empty(),
		"~BufferAllocator: pending reservations left");
}

void BufferAllocator::reserve(vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned int memBits,
		vk::DeviceSize align, ReservationID* reservation) {

	dlg_assert(size > 0);
	dlg_assert(memBits);
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	Requirement req;
	req.size = size;
	req.align = std::max(align, usageAlignment(device(), usage));
	req.usage = usage;
	req.memBits = memBits;

	if(reservation) {
		if(++id_ == 0u) { // wrap; 0 is invalid reservation id
			++id_;
		}

		req.id = *reservation = id_;
	}

	requirements_.push_back(req);
}

BufferAllocator::Allocation BufferAllocator::alloc(ReservationID reservation) {
	dlg_assert(reservation);
	auto cmp = [&](const auto& val) { return val.id == reservation; };
	auto res = std::find_if(reservations_.begin(), reservations_.end(), cmp);
	if(res != reservations_.end()) {
		auto ret = Allocation{*res->buffer, res->allocation};
		reservations_.erase(res);
		return ret;
	}

	auto rit = std::find_if(requirements_.begin(), requirements_.end(), cmp);
	dlg_assert(rit != requirements_.end());
	auto req = *rit;
	requirements_.erase(rit); // important to erase before alloc
	auto ret = alloc(req.size, req.usage, req.memBits, req.align);
	return ret;
}

BufferAllocator::Allocation BufferAllocator::alloc(vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits, vk::DeviceSize align) {
	// TODO: really dumb, greedy algorithm at the moment

	dlg_assert(size > 0);
	dlg_assertm(memBits, "invalid memBits given");
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	align = std::max(align, usageAlignment(device(), usage));

	// check if there is still space available
	for(auto& buf : buffers_) {
		auto& mem = buf.buffer.memory();

		// check buffer has use and memBits we need
		if((buf.usage & usage) != usage || !(memBits & (1 << mem.type()))) {
			continue;
		}

		auto alloc = buf.buffer.alloc(size, align);
		if(alloc.size != 0) {
			dlg_assert(alloc.size == size);
			return {buf.buffer, alloc};
		}
	}

	// allocate a new buffer
	auto& remaining = tmpRequirements_;
	auto& reservations = tmpReservations_;
	remaining.reserve(requirements_.size());
	reservations.reserve(requirements_.size());

	auto imemBits = memBits;
	vk::BufferCreateInfo createInfo;
	createInfo.size = size;
	createInfo.usage = usage;

	// try to allocate as many requirements as possible on this new buffer
	for(auto& req : requirements_) {
		auto mem = imemBits & req.memBits;
		if(!mem) { // requirement can't be allocated in this batch
			remaining.push_back(req);
			continue;
		}

		auto off = vpp::align(createInfo.size, req.align);
		createInfo.usage |= req.usage;
		createInfo.size = off + req.size;
		imemBits = mem;
		if(req.id) {
			// buffer nullptr is filled in after buffer creation
			reservations.push_back({req.id, nullptr, {off, req.size}});
		}
	}

	auto buf = BufferHandle(device(), createInfo);
	auto memReqs = vk::getBufferMemoryRequirements(device(), buf);
	imemBits &= memReqs.memoryTypeBits;
	if(!imemBits) {
		// in this case we can't allocate the buffer on any memory
		// type we want to. Try again but only with the types that
		// vulkan supports.
		imemBits = memBits & memReqs.memoryTypeBits;
		dlg_assertm(imemBits, "Invalid allocation memBits requirement");
		remaining.clear();
		reservations.clear();
		buf = {};
		return alloc(size, usage, imemBits, align);
	}

	auto& nbuf = buffers_.emplace_back(device(), std::move(buf), imemBits,
		createInfo.size, createInfo.usage, devMemAlloc_);
	auto& allocs = nbuf.buffer.allocations(); // insert them manually
	allocs.push_back({0, size}); // first entry, the required one
	for(auto& res : reservations) {
		res.buffer = &nbuf.buffer;
		allocs.push_back(res.allocation);
	}

	requirements_.clear();
	requirements_.insert(requirements_.end(), remaining.begin(), remaining.end());
	reservations_.insert(reservations_.end(), reservations.begin(),
		reservations.end());

	remaining.clear();
	reservations.clear();

	return {nbuf.buffer, {0, size}};
}

void BufferAllocator::cancel(ReservationID reservation) {
	dlg_assert(reservation);
	auto cmp = [&](const auto& val) { return val.id == reservation; };
	auto res = std::find_if(reservations_.begin(), reservations_.end(), cmp);
	if(res != reservations_.end()) {
		dlg_assert(res->buffer && res->allocation.size);
		res->buffer->free(res->allocation);
		reservations_.erase(res);
		return;
	}

	auto req = std::find_if(requirements_.begin(), requirements_.end(), cmp);
	dlg_assert(req != requirements_.end());
	requirements_.erase(req);
}

BufferAllocator::Buffer::Buffer(const Device& dev,
	vpp::BufferHandle buf, unsigned int mbits, vk::DeviceSize size,
	vk::BufferUsageFlags xusage, DeviceMemoryAllocator& alloc) :
		buffer(dev, buf.release(), size, mbits, &alloc), usage(xusage) {
}

} // namespace vpp
