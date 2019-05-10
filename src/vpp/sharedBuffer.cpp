// Copyright (c) 2016-2019 nyorain
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

MemoryMapView SubBuffer::memoryMap(vk::DeviceSize offset,
		vk::DeviceSize size) const {
	size = (size == vk::wholeSize) ? this->size() : size;
	dlg_assert(offset + size <= this->size());
	offset += this->offset();
	return buffer().memoryMap(offset, size);
}

void swap(SubBuffer& a, SubBuffer& b) noexcept {
	using std::swap;

	// union swap
	if(a.size()) {
		if(b.size()) {
			swap(a.shared_, b.shared_);
		} else {
			auto tmp = b.allocator_;
			b.allocator_ = a.allocator_;
			a.allocator_ = tmp;
		}
	} else {
		if(b.size()) {
			auto tmp = b.shared_;
			b.allocator_ = a.allocator_;
			a.shared_ = tmp;
		} else {
			swap(a.allocator_, b.allocator_);
		}
	}

	swap(a.allocation_, b.allocation_);
}

const Device& SubBuffer::device() const {
	if(allocation_.size) {
		return shared_->device();
	} else {
		return allocator_->device();
	}
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

BufferAllocator::~BufferAllocator() {
	// shared buffer will warn if there are remaining allocations
	// on destruction
	for(auto& reservation : reservations_) {
		if(auto* r = std::get_if<1>(&reservation.data); r) {
			if(r->buffer && r->allocation.size) {
				r->buffer->free(r->allocation);
			}
		}
	}
}

void BufferAllocator::reserve(vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned int memBits,
		vk::DeviceSize align, ReservationID* reservation) {

	dlg_assert(size > 0);
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	align = std::max(align, usageAlignment(device(), usage));
	auto& back = reservations_.emplace_back();

	Requirement req;
	req.size = size;
	req.align = align;
	req.usage = usage;
	req.memBits = memBits;
	back.data = {req};

	if(reservation) {
		if(++id_ == 0u) { // wrap
			++id_;
		}

		back.id = id_;
		*reservation = id_;
	}
}

BufferAllocator::Allocation BufferAllocator::alloc(ReservationID reservation) {
	dlg_assert(reservation);
	auto cmp = [&](const auto& val) { return val.id == reservation; };
	auto it = std::find_if(reservations_.begin(), reservations_.end(), cmp);
	dlg_assert(it != reservations_.end());

	auto r = std::move(*it);
	reservations_.erase(it); // important to erase first!
	if(auto* req = std::get_if<0>(&r.data); req) {
		auto [buf, a] = alloc(req->size, req->usage, req->memBits, req->align);
		return {buf, a};
	} else {
		auto& res = std::get<1>(r.data);
		return {*res.buffer, res.allocation};
	}
}

BufferAllocator::Allocation BufferAllocator::alloc(vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits, vk::DeviceSize align) {

	dlg_assert(size > 0);
	dlg_assertm(memBits, "invalid (too few) memBits given");
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	align = std::max(align, usageAlignment(device(), usage));

	// check if there is still space available
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

	for(auto it = reservations_.begin(); it != reservations_.end();) {
		auto reqp = std::get_if<0>(&it->data);
		if(!reqp) { // reservation, no requirement
			++it;
			continue;
		}
		auto& req = *reqp;

		// TODO: bad, greedy algorithm
		auto mem = memBits & req.memBits;
		if(mem) {
			auto off = vpp::align(createInfo.size, req.align);
			createInfo.usage |= req.usage;
			createInfo.size = off + req.size;
			memBits = mem;

			if(it->id) {
				// buffer is filled in after creation
				it->data = Reservation{nullptr, {off, req.size}};
				++it;
			} else {
				it = reservations_.erase(it);
			}
		}
	}

	auto& nbuf = buffers_.emplace_back(device(), createInfo, memBits);
	auto& allocs = nbuf.buffer.allocations(); // insert them manually
	allocs.push_back({0, size}); // first entry, the required one
	for(auto& r : reservations_) {
		if(auto* res = std::get_if<1>(&r.data); res && !res->buffer) {
			res->buffer = &nbuf.buffer;
			allocs.push_back(res->allocation);
		}
	}

	return {nbuf.buffer, {0, size}};
}

void BufferAllocator::cancel(ReservationID reservation) {
	dlg_assert(reservation);
	auto cmp = [&](const auto& val) { return val.id == reservation; };
	auto it = std::find_if(reservations_.begin(), reservations_.end(), cmp);
	dlg_assert(it != reservations_.end());
	auto r = std::move(*it);
	reservations_.erase(it);

	// if we already made the reserving allocation, free it!
	if(auto* req = std::get_if<1>(&r.data); req) {
		req->buffer->free(req->allocation);
	}
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
