// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/sharedBuffer.hpp>
#include <vpp/queue.hpp>
#include <vkpp/structs.hpp>
#include <dlg/dlg.hpp>
#include <algorithm>

namespace vpp {

// BufferRange
SubBuffer::SubBuffer(SharedBuffer& buf, vk::DeviceSize size,
		vk::DeviceSize align) {

	dlg_assert(buf.vkHandle());
	auto alloc = buf.alloc(size, align);
	if(!alloc.size) {
		throw std::runtime_error("BufferRange: not enough SharedBuffer space");
	}

	shared_ = &buf;
	allocation_ = alloc;
}

SubBuffer::SubBuffer(SharedBuffer& buf, const Allocation& alloc) :
		shared_(&buf), allocation_(alloc) {

	dlg_assert(buf.vkHandle());
	dlg_assert(alloc.size != 0);
}

SubBuffer::~SubBuffer() {
	if(shared_) {
		shared_->free(allocation_);
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

// TODO: not sure if alignment reserving is always ok this way
void BufferAllocator::reserve(bool mappable, vk::DeviceSize size,
		vk::BufferUsageFlags usage, vk::DeviceSize align,
		unsigned int memBits) {

	dlg_assert(size > 0);
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	if(mappable) {
		memBits &= device().memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
		dlg_assertm(memBits, "reserve: invalid (too few) memBits given");
	}

	auto& back = reqs_.emplace_back();
	back.size = vpp::align(size, align);
	back.usage = usage;
	back.memBits = memBits;
	back.mappable = mappable;
}

SubBuffer BufferAllocator::alloc(bool mappable, vk::DeviceSize size,
		vk::BufferUsageFlags usage, vk::DeviceSize align,
		unsigned int memBits) {

	dlg_assert(size > 0);
	dlg_assertm(align == 1 || align % 2 == 0,
		"Alignment {} not power of 2", align);

	// TODO:
	// - really dumb algorithm atm, greedy af
	// - we curently just add all previous reservings... maybe
	//  subtract currently requested size (only if smaller than
	//  absolute size or sth.)?
	// - maybe always use hostCoherent memory types for mappable buffers?

	if(mappable) {
		memBits &= device().memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
		dlg_assertm(memBits, "alloc: invalid (too few) memBits given");
	}

	for(auto& buf : buffers_) {
		auto* mem = buf.buffer.memoryEntry().memory();
		dlg_assert(mem);

		// check buffer has use and memBits we need
		if((buf.usage & usage) != usage || !(memBits & (1 << mem->type()))) {
			continue;
		}

		// check the buffer is mappable
		if(mappable && !buf.buffer.nonCoherentAtomAlign) {
			auto props = buf.buffer.memoryEntry().memory()->properties();
			if(!(props & vk::MemoryPropertyBits::hostCoherent)) {
				continue;
			}
		}

		auto alloc = buf.buffer.alloc(size, align);
		if(alloc.size != 0) {
			dlg_assert(alloc.size == size);
			return SubBuffer(buf.buffer, alloc);
		}
	}

	// allocate a new buffer
	// gather all matching buffer information
	vk::BufferCreateInfo createInfo;
	createInfo.size = size;
	createInfo.usage = usage;

	bool createMappable = mappable;
	for(auto& req : reqs_) {
		auto mem = memBits & req.memBits;
		if(mem) {
			createMappable |= req.mappable;
			createInfo.usage |= req.usage;
			createInfo.size += req.size;
			memBits = mem;
		}
	}

	buffers_.emplace_back(device(), createInfo, memBits);
	auto props = buffers_.back().buffer.memoryEntry().memory()->properties();
	if(createMappable && !(props & vk::MemoryPropertyBits::hostCoherent)) {
		buffers_.back().buffer.nonCoherentAtomAlign = true;
	}

	auto alloc = buffers_.back().buffer.alloc(size);
	dlg_assert(alloc.size == size);
	return SubBuffer(buffers_.back().buffer, alloc);
}

void BufferAllocator::optimize() {
	dlg_warn("TODO: BufferAllocator::optimize");
}

void BufferAllocator::shrink() {
	dlg_warn("TODO: BufferAllocator::shrink");
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
