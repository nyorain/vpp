// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/memory.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

#include <algorithm> // std::lower_bound
#include <string> // std::string
#include <stdexcept> // std::runtime_error

namespace vpp {

// DeviceMemoryHandle
DeviceMemoryHandle::DeviceMemoryHandle(const Device& dev,
	const vk::MemoryAllocateInfo& info)
		: DeviceMemoryHandle(dev, vk::allocateMemory(dev, info)) {
}

DeviceMemoryHandle::DeviceMemoryHandle(const Device& dev, vk::DeviceMemory mem)
	: ResourceHandle(dev, mem) {
}

DeviceMemoryHandle::~DeviceMemoryHandle() {
	if(vkHandle()) {
		vk::freeMemory(vkDevice(), vkHandle());
	}
}

// DeviceMemory
DeviceMemory::DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info)
	: DeviceMemoryHandle(dev, info), size_(info.allocationSize),
		type_(info.memoryTypeIndex) {
}

DeviceMemory::DeviceMemory(const Device& dev, vk::DeviceMemory mem,
	vk::DeviceSize size, unsigned int memoryType)
		: DeviceMemoryHandle(dev, mem), size_(size), type_(memoryType) {
}

DeviceMemory::~DeviceMemory() {
	dlg_assertm(allocations_.empty(), "~DeviceMemory: allocations left");
}

DeviceMemory::Allocation DeviceMemory::alloc(vk::DeviceSize size,
		vk::DeviceSize alignment, AllocationType type) {

	auto allocation = allocatable(size, alignment, type);
	if(allocation.size > 0) {
		allocSpecified(allocation, type);
	}

	return allocation;
}

void DeviceMemory::allocSpecified(Allocation a, AllocationType type) {
	auto offset = a.offset;
	auto size = a.size;

	dlg_assertm(size != 0, "given size is 0");
	dlg_assertm(type != AllocationType::none, "given AllocationType::none");
	dlg_assertm(offset + size <= this->size(), "given size too large");

	AllocationEntry allocation = {{offset, size}, type};
	auto it = std::lower_bound(allocations_.begin(), allocations_.end(), allocation,
		[](auto& a, auto& b){ return a.allocation.offset < b.allocation.offset; });

	dlg_assertm(it == allocations_.begin() || (it - 1)->allocation.end() <= offset,
		"Overlapping allocSpecified (before allocation)");
	dlg_assertm(it == allocations_.end() || it->allocation.offset >= a.end(),
		"Overlapping allocSpecified (after allocation)");

	allocations_.insert(it, allocation);
}

DeviceMemory::Allocation DeviceMemory::allocatable(vk::DeviceSize size,
		vk::DeviceSize alignment, AllocationType type) const {

	// NOTE: allocation finding algorithm can be improved
	// atm the allocation with the least waste for alignment or granularity is chosen,
	// but there may result small gaps between the allocation which will likely never
	// be used. if an allocation fits a free segment between allocations really
	// good, it should be chosen.
	//
	// --[A: free 10MB]-- || --[alloc]-- || --[B: free 20MB]--
	// 1) first call to allocatable: size 9MB, would fit free segment A as well as B
	// B is chosen since it results in less space waste
	// 2) first call to allocatable: size 15MB, does now fit if none of the free segments
	// if the first allocation had chosen segment A this second allocation would now
	// fit in B.
	//
	// as new measurement the new sizes on both sides divided by the old (bigger)
	// size could be a taken in account (smaller = better) since the new sizes
	// on both sides should be as small as possible. true?

	dlg_assertm(alignment == 1 || alignment % 2 == 0,
		"Alignment {} not power of 2", alignment);
	dlg_assertm(size != 0, "given alloc size 0");
	dlg_assertm(type != AllocationType::none, "given AllocationType::none");

	static constexpr AllocationEntry start = {{0, 0}, AllocationType::none};
	auto granularity = device().properties().limits.bufferImageGranularity;

	// checks for best possible allocation
	// the best allocation wastes the least space for alignment or granularity
	// reqs
	Allocation best = {};
	vk::DeviceSize bestWaste = -1;

	const AllocationEntry* old = &start;
	for(auto& alloc : allocations_) {
		vk::DeviceSize alignedOffset = align(old->allocation.end(), alignment);

		// check for granularity between prev and to be inserted
		if(old->type != AllocationType::none && old->type != type) {
			alignedOffset = align(alignedOffset, granularity);
		}

		// check for granularity between next and to be inserted
		auto end = alignedOffset + size;
		if(alloc.type != AllocationType::none && alloc.type != type) {
			end = align(end, granularity);
		}

		if(end < alloc.allocation.offset) {
			auto newWaste = alignedOffset - old->allocation.end();
			newWaste += end - (alignedOffset + size);
			if(newWaste < bestWaste) {
				bestWaste = newWaste;
				best = {alignedOffset, size};
			}
		}

		old = &alloc;
	}

	// check for segment AFTER the last allcation, since the loop just checks
	// the segments between two allocations
	// just copied from above with the "new allocation" alloc being an empty
	// past-end allocation
	vk::DeviceSize alignedOffset = align(old->allocation.end(), alignment);

	if(old->type != AllocationType::none && old->type != type) {
		alignedOffset = align(alignedOffset, granularity);
	}

	if(alignedOffset + size <= this->size()) {
		auto newWaste = alignedOffset - old->allocation.offset;
		if(newWaste < bestWaste) {
			bestWaste = newWaste;
			best = {alignedOffset, size};
		}
	}

	return best;
}

void DeviceMemory::free(const Allocation& alloc) {
	auto it = std::find_if(allocations_.begin(), allocations_.end(),
		[&](auto& ae) { return ae.allocation == alloc; });
	dlg_assertm(it != allocations_.end(), "free: invalid allocation");
	allocations_.erase(it);
}

vk::DeviceSize DeviceMemory::largestFreeSegment() const noexcept {
	vk::DeviceSize ret {0};
	vk::DeviceSize oldend {0};

	for(auto& alloc : allocations_) {
		if(alloc.allocation.offset - oldend > ret) {
			ret = alloc.allocation.offset - oldend;
		}

		oldend = alloc.allocation.end();
	}

	// potential last free block
	if(size_ - oldend > ret) {
		ret = size_ - oldend;
	}

	return ret;
}

vk::DeviceSize DeviceMemory::totalFree() const noexcept {
	vk::DeviceSize ret {0};
	for(auto& alloc : allocations_) {
		ret += alloc.allocation.size;
	}

	return size() - ret;
}

MemoryMapView DeviceMemory::map(const Allocation& allocation) {
	// NOTE: any allocation can be mapped, don't get confused
	// by device limits minMemoryMapAlignment. It's a guarantee for
	// us, not something we have to fulfill [see spec]
	if(!mapped()) {
		memoryMap_.map(*this, allocation);
	} else {
		memoryMap_.remap(allocation);
	}

	return MemoryMapView(memoryMap_, allocation);
}

vk::MemoryPropertyFlags DeviceMemory::properties() const noexcept {
	return device().memoryProperties().memoryTypes[type()].propertyFlags;
}

bool DeviceMemory::mappable() const noexcept {
	return properties() & vk::MemoryPropertyBits::hostVisible;
}

MemoryMap* DeviceMemory::mapped() noexcept {
	return (memoryMap_.ptr()) ? &memoryMap_ : nullptr;
}

const MemoryMap* DeviceMemory::mapped() const noexcept {
	return (memoryMap_.ptr()) ? &memoryMap_ : nullptr;
}

} // namespace vpp
