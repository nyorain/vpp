// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/sharedBuffer.hpp>
#include <vpp/queue.hpp>
#include <vkpp/structs.hpp>
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
		shared_(&buf), allocation_(alloc) {

	dlg_assert(buf.vkHandle());
	dlg_assert(alloc.size != 0);
}

SubBuffer::SubBuffer(BufferAllocator& alloc, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits,
		vk::DeviceSize align) {
	auto [shared, a] = alloc.alloc(size, usage, memBits, align);
	shared_ = &shared;
	allocation_ = a;
}

SubBuffer::SubBuffer(DeferTag, BufferAllocator& alloc, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits,
		vk::DeviceSize align) : allocator_(&alloc) {
	 alloc.reserve(size, usage, memBits, align, &allocation_.offset);
}

SubBuffer::~SubBuffer() {
	if(allocation_.size > 0) {
		dlg_assert(shared_);
		shared_->free(allocation_);
	} else if(allocation_.offset > 0) {
		dlg_assert(allocator_);
		allocator_->cancel(allocation_.offset);
	}
}

void SubBuffer::init() {
	if(allocation_.size == 0u) {
		dlg_assert(allocator_);
		dlg_assert(allocation_.offset > 0u);
		auto [shared, alloc] = allocator_->alloc(offset());
		shared_ = &shared;
		allocation_ = alloc;
	}
}

MemoryMapView SubBuffer::memoryMap() const {
	// buffer().memoryMap will warn if buffer is not mappable
	return buffer().memoryMap(offset(), size());
}

void swap(SubBuffer& a, SubBuffer& b) noexcept {
	using RR = ResourceReference<SubBuffer>;
	using std::swap;
	swap(static_cast<RR&>(a), static_cast<RR&>(b));
	swap(a.shared_, b.shared_);
	swap(a.allocation_, b.allocation_);
}

// SharedBuffer
SharedBuffer::SharedBuffer(const Device& dev, const vk::BufferCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* allocator) :
		Buffer(dev, info, memBits, allocator), size_(info.size) {
}

SharedBuffer::SharedBuffer(const Device& dev, const vk::BufferCreateInfo& info,
	DeviceMemory& mem) : Buffer(dev, info, mem), size_(info.size) {
}

SharedBuffer::SharedBuffer(DeferTag defer, const Device& dev,
	const vk::BufferCreateInfo& info, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		Buffer(defer, dev, info, memBits, alloc), size_(info.size) {
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
	auto t = memoryEntry().memory()->type();
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
		Resource(dev) {
}

void BufferAllocator::reserve(vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned int memBits,
		vk::DeviceSize align, Reservation* reservation) {

	dlg_assert(size > 0);
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	align = std::max(align, usageAlignment(device(), usage));
	auto& back = reqs_.emplace_back();

	back.size = size;
	back.align = align;
	back.usage = usage;
	back.memBits = memBits;

	if(reservation) {
		if(++id_ == 0u) {
			++id_;
		}

		back.id = id_;
		*reservation = id_;
	}
}

BufferAllocator::Allocation BufferAllocator::alloc(Reservation reservation) {
	dlg_assert(reservation);

	auto it = std::find_if(reqs_.begin(), reqs_.end(), [&](const auto& val)
		{ return val.id == reservation; });
	bool req = it != reqs_.end();
	SharedBuffer* rbuf {};
	SharedBuffer::Allocation ralloc {};
	if(req) {
		auto [b, a] = alloc(it->size, it->usage, it->align, it->memBits);
		rbuf = &b;
		ralloc = a;
	}

	auto rit = std::find_if(reservations_.begin(), reservations_.end(),
		[&](const auto& val) { return val.id == reservation; });
	dlg_assert(rit != reservations_.end());
	if(!req) {
		auto [b, a] = alloc(rit->size, rit->usage, rit->align, rit->memBits);
		rbuf = &b;
		ralloc = a;
	}

	reservations_.erase(rit);
	return {*rbuf, ralloc};
}

BufferAllocator::Allocation BufferAllocator::alloc(vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits, vk::DeviceSize align) {

	dlg_assert(size > 0);
	dlg_assertm(memBits, "invalid (too few) memBits given");
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	align = std::max(align, usageAlignment(device(), usage));

	// TODO: really dumb algorithm atm, greedy af
	for(auto& buf : buffers_) {
		auto* mem = buf.buffer.memoryEntry().memory();
		dlg_assert(mem);

		// check buffer has use and memBits we need
		if((buf.usage & usage) != usage || !(memBits & (1 << mem->type()))) {
			continue;
		}

		auto alloc = buf.buffer.alloc(size, align);
		if(alloc.size != 0) {
			dlg_assert(alloc.size == size);
			return {buf.buffer, alloc};
		}
	}

	// allocate a new buffer
	// gather all matching buffer information
	vk::BufferCreateInfo createInfo;
	createInfo.size = size;
	createInfo.usage = usage;

	for(auto& req : reqs_) {
		// TODO: bad idea (greedy)
		// the way we align we might allocate a bit too much but
		// that shouldn't be a problem
		auto mem = memBits & req.memBits;
		if(mem) {
			createInfo.usage |= req.usage;
			createInfo.size = vpp::align(createInfo.size, req.align);
			createInfo.size += req.size;
			memBits = mem;
			req.size = 0u; // mark for removal
			reservations_.push_back(req);
		}
	}

	reqs_.erase(std::remove_if(reqs_.begin(), reqs_.end(), [](const auto& r)
		{ return r.size == 0; }), reqs_.end());

	buffers_.emplace_back(device(), createInfo, memBits);
	auto alloc = buffers_.back().buffer.alloc(size);
	dlg_assert(alloc.size == size);
	return {buffers_.back().buffer, alloc};
}

void BufferAllocator::cancel(Reservation r) {
	auto it = std::find_if(reqs_.begin(), reqs_.end(), [&](const auto& val)
		{ return val.id == r; });
	if(it != reqs_.end()) {
		reqs_.erase(it);
		return;
	}

	auto rit = std::find_if(reservations_.begin(), reservations_.end(),
		[&](const auto& val) { return val.id == r; });
	dlg_assert(rit != reservations_.end());
	reservations_.erase(rit);
}

BufferAllocator::Buffer::Buffer(const Device& dev,
	const vk::BufferCreateInfo& info, unsigned int mbits) :
		buffer(dev, info, mbits), usage(info.usage) {
}

// utility
int transferQueueFamily(const Device& dev, const Queue** queue) {
	// we do not only query a valid queue family but a valid queue and then
	// chose its queue family to ensure that the device has a queue for the
	// queried queue family
	auto* q = dev.queue(vk::QueueBits::transfer);
	if(!q) {
		q = dev.queue(vk::QueueBits::graphics);
	}
	if(!q) {
		q = dev.queue(vk::QueueBits::compute);
	}
	if(!q) {
		return -1;
	}

	if(queue) {
		*queue = q;
	}

	return q->family();
}

} // namespace vpp
