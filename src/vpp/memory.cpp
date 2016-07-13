#include <vpp/memory.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/debug.hpp>

#include <iostream>
#include <algorithm>

namespace vpp
{

//MemoryMap
MemoryMap::MemoryMap(const DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
	if(!(memory.properties() & vk::MemoryPropertyBits::hostVisible))
		throw std::logic_error("vpp::MemoryMap: trying to map unmappable memory");

	ptr_ = vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {});
}

MemoryMap::MemoryMap(MemoryMap&& other) noexcept
{
	swap(*this, other);
}

MemoryMap& MemoryMap::operator=(MemoryMap other) noexcept
{
	unmap();
	swap(*this, other);
	return *this;
}

MemoryMap::~MemoryMap()
{
	unmap();
}

void MemoryMap::remap(const Allocation& allocation)
{
	//new allocation extent
	auto nbeg = std::min(allocation.offset, allocation_.offset);
	auto nsize = std::max(allocation.end(), allocation_.end()) - nbeg;

	//if new extent lay inside old do nothing
	if(offset() <= nbeg && offset() + size() >= nbeg + nsize) return;

	//else remap the memory
	vk::unmapMemory(vkDevice(), vkMemory());
	allocation_ = {nbeg, nsize};

	ptr_ = vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {});
}

void swap(MemoryMap& a, MemoryMap& b) noexcept
{
	using std::swap;

	swap(a.memory_, b.memory_);
	swap(a.allocation_, b.allocation_);
	swap(a.ptr_, b.ptr_);
	swap(a.views_, b.views_);
}

vk::MappedMemoryRange MemoryMap::mappedMemoryRange() const
{
	return {vkMemory(), offset(), size()};
}

const vk::DeviceMemory& MemoryMap::vkMemory() const
{
	return memory_->vkHandle();
}

void MemoryMap::flush() const
{
	VPP_DEBUG_CHECK(vpp::MemoryMap::flush,
	{
		if(coherent())
		{
			VPP_DEBUG_OUTPUT("Called on coherent memory. Not needed.");
			return;
		}
	})

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMap::reload() const
{
	VPP_DEBUG_CHECK(vpp::MemoryMap::invalidateRanges,
	{
		if(coherent())
		{
			VPP_DEBUG_OUTPUT("Called on coherent memory. Not needed.");
			return;
		}
	})

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

bool MemoryMap::coherent() const
{
	return memory().properties() & vk::MemoryPropertyBits::hostCoherent;
}

void MemoryMap::unmap()
{
	if(memory_ && vkMemory() && ptr() && size()) vk::unmapMemory(memory().vkDevice(), vkMemory());

	memory_ = nullptr;
	allocation_ = {};
	ptr_ = nullptr;
	views_ = 0;
}

void MemoryMap::ref()
{
	views_++;
}

void MemoryMap::unref()
{
	if(views_ <= 1) unmap();
	else views_--;
}

//MemoryMapView
MemoryMapView::MemoryMapView(MemoryMap& map, const Allocation& allocation)
	: memoryMap_(&map), allocation_(allocation)
{
	memoryMap_->ref();
}

MemoryMapView::~MemoryMapView()
{
	if(memoryMap_) memoryMap_->unref();
}

MemoryMapView::MemoryMapView(MemoryMapView&& other) noexcept
{
	swap(*this, other);
}

MemoryMapView& MemoryMapView::operator=(MemoryMapView other) noexcept
{
	swap(*this, other);
	return *this;
}

vk::MappedMemoryRange MemoryMapView::mappedMemoryRange() const
{
	return {vkMemory(), offset(), size()};
}

void MemoryMapView::flush() const
{
	VPP_DEBUG_CHECK(vpp::MemoryMapView::flush,
	{
		if(coherent())
		{
			VPP_DEBUG_OUTPUT("Called on coherent memory. Not needed.");
			return;
		}
	})

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMapView::reload() const
{
	VPP_DEBUG_CHECK(vpp::MemoryMapView::reload,
	{
		if(coherent())
		{
			VPP_DEBUG_OUTPUT("Called on coherent memory. Not needed.");
			return;
		}
	})

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

std::uint8_t* MemoryMapView::ptr() const
{
	return memoryMap().ptr() + allocation().offset - memoryMap().offset();
}

bool MemoryMapView::coherent() const
{
	return memory().properties() & vk::MemoryPropertyBits::hostCoherent;
}

void swap(MemoryMapView& a, MemoryMapView& b) noexcept
{
	using std::swap;

	swap(a.memoryMap_, b.memoryMap_);
	swap(a.allocation_, b.allocation_);
}

//Memory
DeviceMemory::DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info)
	: ResourceHandle(dev)
{
	type_ = info.memoryTypeIndex;
	size_ = info.allocationSize;

	vkHandle() = vk::allocateMemory(vkDevice(), info);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex)
	: ResourceHandle(dev)
{
	type_ = typeIndex;
	size_ = size;

	vk::MemoryAllocateInfo info;
	info.allocationSize = size_;
	info.memoryTypeIndex = type_;

	vkHandle() = vk::allocateMemory(vkDevice(), info);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, vk::MemoryPropertyFlags flags)
	: ResourceHandle(dev)
{
	type_ = device().memoryType(flags);
	size_ = size;

	vk::MemoryAllocateInfo info;
	info.allocationSize = size_;
	info.memoryTypeIndex = type_;

	vkHandle() = vk::allocateMemory(vkDevice(), info);
}
DeviceMemory::~DeviceMemory()
{
	VPP_DEBUG_CHECK(vpp::DeviceMemory::~DeviceMemory,
	{
		if(!allocations_.empty()) VPP_DEBUG_OUTPUT(allocations_.size(), "allocations left.");
	})

	if(vkHandle()) vk::freeMemory(vkDevice(), vkHandle(), nullptr);
}

Allocation DeviceMemory::alloc(std::size_t size, std::size_t alignment, AllocationType type)
{
	auto allocation = allocatable(size, alignment, type);
	if(allocation.size > 0) return allocSpecified(allocation.offset, allocation.size, type);

	throw std::runtime_error("vpp::DeviceMemory::alloc: not enough memory left");
}

Allocation DeviceMemory::allocSpecified(std::size_t offset, std::size_t size, AllocationType type)
{
	VPP_DEBUG_CHECK(vpp::DeviceMemory::allocSpecified,
	{
		if(size == 0)
		{
			VPP_DEBUG_OUTPUT("size is not allowed to be 0");
			return {};
		}

		for(auto& alloc : allocations_)
		{
			const auto& a = alloc.allocation;
			const auto& overlapping = (a.offset < offset) != (a.offset + a.size <= offset);
			const auto& inside = (a.offset >= offset) && (a.offset < offset + size);
			if(overlapping || inside)
			{
				VPP_DEBUG_OUTPUT("invalid params ", offset, ' ', size, ' ', a.offset, ' ', a.size);
				return {};
			}
		}

		if(type == AllocationType::none) VPP_DEBUG_OUTPUT("type is none. Could later cause aliasing");
	})

	AllocationEntry allocation = {{offset, size}, type};
	auto it = std::lower_bound(allocations_.begin(), allocations_.end(), allocation,
		[](auto& a, auto& b){ return a.allocation.offset < b.allocation.offset; });
	allocations_.insert(it, allocation);
	return allocations_.back().allocation;
}

Allocation DeviceMemory::allocatable(std::size_t size, std::size_t alignment,
	AllocationType type) const
{
	//TODO: better allocation finding algorithm.
	//atm the allocation with the least waste for alignment or granularity is chosen,
	//but there may result small gaps between the allocation which will likely never
	//be used. if an allocation fits a free segment between allocations really good, it should
	//be chosen.
	//
	//e.g. --allocation---- || -----A) free 10MB----- || -----allocation---- || ----B) free 20MB---
	//1) first call to allocatable: size 9MB, would fit free segment A as well as B
	//B is chosen since it results in less space waste
	//2) first call to allocatable: size 15MB, does now fit if none of the free segments
	//if the first allocation had chosen segment A this second allocation would now
	//fit in B.
	//
	//as new measurement the new sizes on both sides divided by the old (bigger) size could be
	//a taken in account (smaller = better) since the new sizes on both sides should be as small as
	//possible. true?

	//some additional checks/warning
	VPP_DEBUG_CHECK(vpp::DeviceMemory::allocatable,
	{
		if(type == AllocationType::none) VPP_DEBUG_OUTPUT("type is none. Can cause aliasing");

		if(alignment % 2)
		{
			VPP_DEBUG_OUTPUT("alignment param ", alignment, "not a power of 2");
			return {};
		}

		if(size == 0)
		{
			VPP_DEBUG_OUTPUT("size is not allowed to be 0");
			return {};
		}
	})

	static constexpr AllocationEntry start = {{0, 0}, AllocationType::none};
	auto granularity = device().properties().limits.bufferImageGranularity;


	//checks for best possible allocation
	//the best allocation wastes the least space for alignment or granularity reqs
	Allocation best = {};
	std::size_t bestWaste = -1;

	const AllocationEntry* old = &start;
	for(auto& alloc : allocations_)
	{
		vk::DeviceSize alignedOffset = align(old->allocation.end(), alignment);

		//check for granularity between prev and to be inserted
		if(old->type != AllocationType::none && old->type != type)
			alignedOffset = align(alignedOffset, granularity);

		//check for granularity between next and to be inserted
		auto end = alignedOffset + size;
		if(alloc.type != AllocationType::none && alloc.type != type)
			end = align(end, granularity);

		if(end < alloc.allocation.offset)
		{
			auto newWaste = alignedOffset - old->allocation.end();
			newWaste += end - (alignedOffset + size);
			if(newWaste < bestWaste)
			{
				bestWaste = newWaste;
				best = {alignedOffset, size};
			}
		}

		old = &alloc;
	}

	//check for segment AFTER the last allcation, since the loop just checks the segments
	//between two allocations
	//just copied from above with the "new allocation" alloc being an empty past-end allocation
	vk::DeviceSize alignedOffset = align(old->allocation.end(), alignment);

	if(old->type != AllocationType::none && old->type != type)
		alignedOffset = align(alignedOffset, granularity);

	if(alignedOffset + size <= this->size())
	{
		auto newWaste = alignedOffset - old->allocation.offset;
		if(newWaste < bestWaste)
		{
			bestWaste = newWaste;
			best = {alignedOffset, size};
		}
	}

	return best;
}

bool DeviceMemory::free(const Allocation& alloc)
{
	for(auto it = allocations_.cbegin(); it != allocations_.cend(); ++it)
	{
		if(it->allocation.offset == alloc.offset && it->allocation.size == alloc.size)
		{
			allocations_.erase(it);
			return true;
		}
	}

	VPP_DEBUG_OUTPUT_NOCHECK("vpp::DeviceMemory::free: could not find the given allocation");
	return false;
}

std::size_t DeviceMemory::biggestBlock() const
{
	std::size_t ret {0};
	std::size_t oldend {0};

	for(auto& alloc : allocations_) {
		if(alloc.allocation.offset - oldend > ret) ret = alloc.allocation.offset - oldend;
		oldend = alloc.allocation.offset + alloc.allocation.size;
	}

	return ret;
}

std::size_t DeviceMemory::totalFree() const
{
	std::size_t ret {0};
	for(auto& alloc : allocations_)
		ret += alloc.allocation.size;

	return size() - ret;
}
std::size_t DeviceMemory::size() const
{
	return size_;
}

MemoryMapView DeviceMemory::map(const Allocation& allocation)
{
	if(!(properties() & vk::MemoryPropertyBits::hostVisible))
		throw std::logic_error("vpp::DeviceMemory::map: not mappable.");

	if(!mapped()) memoryMap_ = MemoryMap(*this, allocation);
	else memoryMap_.remap(allocation);

	return MemoryMapView(memoryMap_, allocation);
}

vk::MemoryPropertyFlags DeviceMemory::properties() const
{
	return device().memoryProperties().memoryTypes[type()].propertyFlags;
}

bool DeviceMemory::mappable() const
{
	return properties() & vk::MemoryPropertyBits::hostVisible;
}

}
