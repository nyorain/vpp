// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/memory.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/debug.hpp>

#include <algorithm> // std::lower_bound
#include <string> // std::string
#include <stdexcept> // std::runtime_error

namespace vpp {

// DeviceMemory
DeviceMemory::DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info)
	: ResourceHandle(dev)
{
	type_ = info.memoryTypeIndex;
	size_ = info.allocationSize;

	handle_ = vk::allocateMemory(vkDevice(), info);
}

DeviceMemory::DeviceMemory(const Device& dev, uint32_t size, uint32_t typeIndex)
	: ResourceHandle(dev)
{
	type_ = typeIndex;
	size_ = size;

	vk::MemoryAllocateInfo info;
	info.allocationSize = size_;
	info.memoryTypeIndex = type_;

	handle_ = vk::allocateMemory(vkDevice(), info);
}

DeviceMemory::DeviceMemory(const Device& dev, uint32_t size, vk::MemoryPropertyFlags flags)
	: ResourceHandle(dev)
{
	type_ = device().memoryType(flags);
	size_ = size;

	vk::MemoryAllocateInfo info;
	info.allocationSize = size_;
	info.memoryTypeIndex = type_;

	handle_ = vk::allocateMemory(vkDevice(), info);
}

DeviceMemory::~DeviceMemory()
{
	VPP_DEBUG_CHECK("vpp::DeviceMemory::~DeviceMemory", {
		if(!allocations_.empty()) {
			std::string msg = std::to_string(allocations_.size()) + " allocations left:";
			for(auto& a : allocations_) {
				msg += "\n\t" + std::to_string(a.allocation.offset);
				msg += " " + std::to_string(a.allocation.size);
			}

			VPP_CHECK_WARN(msg);
		}
	})

	if(vkHandle()) vk::freeMemory(vkDevice(), vkHandle(), nullptr);
}

Allocation DeviceMemory::alloc(size_t size, size_t alignment, AllocationType type)
{
	auto allocation = allocatable(size, alignment, type);
	if(allocation.size > 0) return allocSpecified(allocation.offset, allocation.size, type);
	throw std::runtime_error("vpp::DeviceMemory::alloc: not enough memory left");
}

Allocation DeviceMemory::allocSpecified(size_t offset, size_t size, AllocationType type)
{
	VPP_DEBUG_CHECK("vpp::DeviceMemory::allocSpecified", {
		if(size == 0) VPP_CHECK_THROW("size is not allowed to be 0");
		for(auto& alloc : allocations_) {
			const auto& a = alloc.allocation;
			const auto& overlapping = (a.offset < offset) != (a.offset + a.size <= offset);
			const auto& inside = (a.offset >= offset) && (a.offset < offset + size);
			if(overlapping || inside) {
				VPP_CHECK_THROW("invalid params ", offset, ' ', size, ' ', a.offset, ' ', a.size);
				break;
			}
		}

		if(type == AllocationType::none)
			VPP_CHECK_THROW("type is none. Could later cause aliasing");
	})

	AllocationEntry allocation = {{offset, size}, type};
	auto it = std::lower_bound(allocations_.begin(), allocations_.end(), allocation,
		[](auto& a, auto& b){ return a.allocation.offset < b.allocation.offset; });

	allocations_.insert(it, allocation);
	return allocations_.back().allocation;
}

Allocation DeviceMemory::allocatable(size_t size, size_t alignment,
	AllocationType type) const
{
	// NOTE: allocation finding algorithm can be improved
	// atm the allocation with the least waste for alignment or granularity is chosen,
	// but there may result small gaps between the allocation which will likely never
	// be used. if an allocation fits a free segment between allocations really good, it should
	// be chosen.
	//
	// e.g. --allocation---- || -----A) free 10MB----- || -----allocation---- || ----B) free 20MB---
	// 1) first call to allocatable: size 9MB, would fit free segment A as well as B
	// B is chosen since it results in less space waste
	// 2) first call to allocatable: size 15MB, does now fit if none of the free segments
	// if the first allocation had chosen segment A this second allocation would now
	// fit in B.
	//
	// as new measurement the new sizes on both sides divided by the old (bigger) size could be
	// a taken in account (smaller = better) since the new sizes on both sides should be as small as
	// possible. true?

	VPP_DEBUG_CHECK("vpp::DeviceMemory::allocatable", {
		if(alignment % 2) VPP_CHECK_THROW("alignment param ", alignment, "not a power of 2");
		if(size == 0) VPP_CHECK_THROW("size is not allowed to be 0");
		if(type == AllocationType::none) VPP_CHECK_THROW("type is none. Can cause aliasing");
	})

	static constexpr AllocationEntry start = {{0, 0}, AllocationType::none};
	auto granularity = device().properties().limits.bufferImageGranularity;


	// checks for best possible allocation
	// the best allocation wastes the least space for alignment or granularity reqs
	Allocation best = {};
	size_t bestWaste = -1;

	const AllocationEntry* old = &start;
	for(auto& alloc : allocations_) {
		vk::DeviceSize alignedOffset = align(old->allocation.end(), alignment);

		// check for granularity between prev and to be inserted
		if(old->type != AllocationType::none && old->type != type)
			alignedOffset = align(alignedOffset, granularity);

		// check for granularity between next and to be inserted
		auto end = alignedOffset + size;
		if(alloc.type != AllocationType::none && alloc.type != type)
			end = align(end, granularity);

		if(end < alloc.allocation.offset) {
			auto newWaste = alignedOffset - old->allocation.end();
			newWaste += end - (alignedOffset + size);
			if(newWaste < bestWaste) {
				bestWaste = newWaste;
				best = {size_t(alignedOffset), size};
			}
		}

		old = &alloc;
	}

	// check for segment AFTER the last allcation, since the loop just checks the segments
	// between two allocations
	// just copied from above with the "new allocation" alloc being an empty past-end allocation
	vk::DeviceSize alignedOffset = align(old->allocation.end(), alignment);

	if(old->type != AllocationType::none && old->type != type)
		alignedOffset = align(alignedOffset, granularity);

	if(alignedOffset + size <= this->size()) {
		auto newWaste = alignedOffset - old->allocation.offset;
		if(newWaste < bestWaste) {
			bestWaste = newWaste;
			best = {::size_t(alignedOffset), size};
		}
	}

	return best;
}

void DeviceMemory::free(const Allocation& alloc)
{
	for(auto it = allocations_.cbegin(); it != allocations_.cend(); ++it) {
		if(it->allocation.offset == alloc.offset && it->allocation.size == alloc.size) {
			allocations_.erase(it);
			return;
		}
	}

	VPP_DEBUG_WARN("vpp::DeviceMemory::free: could not find the given allocation");
}

size_t DeviceMemory::largestFreeSegment() const noexcept
{
	size_t ret {0};
	size_t oldend {0};

	for(auto& alloc : allocations_) {
		if(alloc.allocation.offset - oldend > ret)
			ret = alloc.allocation.offset - oldend;

		oldend = alloc.allocation.end();
	}

	// potential last free block
	if(size_ - oldend > ret)
		ret = size_ - oldend;

	return ret;
}

size_t DeviceMemory::totalFree() const noexcept
{
	size_t ret {0};
	for(auto& alloc : allocations_)
		ret += alloc.allocation.size;

	return size() - ret;
}
size_t DeviceMemory::size() const noexcept
{
	return size_;
}

MemoryMapView DeviceMemory::map(const Allocation& allocation)
{
	if(!mapped()) memoryMap_ = MemoryMap(*this, allocation);
	else memoryMap_.remap(allocation);

	return MemoryMapView(memoryMap_, allocation);
}

vk::MemoryPropertyFlags DeviceMemory::properties() const noexcept
{
	return device().memoryProperties().memoryTypes[type()].propertyFlags;
}

bool DeviceMemory::mappable() const noexcept
{
	return properties() & vk::MemoryPropertyBits::hostVisible;
}

} // namespace vpp
