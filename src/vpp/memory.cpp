#include <vpp/memory.hpp>
#include <vpp/vk.hpp>

#include <iostream>
#include <algorithm>

namespace vpp
{

//MemoryMap
MemoryMap::MemoryMap(const DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
	if(!(memory.propertyFlags() & vk::MemoryPropertyBits::hostVisible))
		throw std::logic_error("vpp::MemoryMap: trying to map device local memory");

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
	return memory_->vkDeviceMemory();
}

void MemoryMap::flushRanges() const
{
	if(coherent())
	{
		std::cout << "vpp::MemoryMap::flushRanges: called but not needed, mem coherent\n";
		return;
	}

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMap::invalidateRanges() const
{
	if(coherent())
	{
		std::cout << "vpp::MemoryMap::invalidateRanges: called but not needed, mem coherent\n";
		return;
	}

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

bool MemoryMap::coherent() const
{
	return memory().propertyFlags() & vk::MemoryPropertyBits::hostCoherent;
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

void MemoryMapView::flushRanges() const
{
	if(coherent()) return;

	auto range = mappedMemoryRange();
	vk::flushMappedMemoryRanges(vkDevice(), 1, range);
}

void MemoryMapView::invalidateRanges() const
{
	if(coherent()) return;

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, range);
}

std::uint8_t* MemoryMapView::ptr() const
{
	return memoryMap().ptr() + allocation().offset - memoryMap().offset();
}

bool MemoryMapView::coherent() const
{
	return memory().propertyFlags() & vk::MemoryPropertyBits::hostCoherent;
}

void swap(MemoryMapView& a, MemoryMapView& b) noexcept
{
	using std::swap;

	swap(a.memoryMap_, b.memoryMap_);
	swap(a.allocation_, b.allocation_);
}

//Memory
DeviceMemory::DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info)
	: Resource(dev)
{
	typeIndex_ = info.memoryTypeIndex;
	size_ = info.allocationSize;
	flags_ = dev.memoryProperties().memoryTypes[typeIndex_].propertyFlags;

	memory_ = vk::allocateMemory(vkDevice(), info);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex)
	: Resource(dev)
{
	typeIndex_ = typeIndex;
	size_ = size;
	flags_ = dev.memoryProperties().memoryTypes[typeIndex_].propertyFlags;

	vk::MemoryAllocateInfo info;
	info.allocationSize = size_;
	info.memoryTypeIndex = typeIndex_;

	memory_ = vk::allocateMemory(vkDevice(), info);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, vk::MemoryPropertyFlags flags)
	: Resource(dev)
{
	flags_ = flags;
	typeIndex_ = device().memoryType(flags);
	size_ = size;

	vk::MemoryAllocateInfo info;
	info.allocationSize = size_;
	info.memoryTypeIndex = typeIndex_;

	memory_ = vk::allocateMemory(vkDevice(), info);
}
DeviceMemory::~DeviceMemory()
{
	if(!allocations_.empty())
		std::cerr << "vpp::~DeviceMemory: there are " << allocations_.size() << "allocs left\n";

	if(vkDeviceMemory()) vk::freeMemory(vkDevice(), memory_, nullptr);
}

Allocation DeviceMemory::alloc(std::size_t size, std::size_t alignment, AllocationType type)
{
	auto allocation = allocatable(size, alignment, type);
	if(allocation.size > 0) return allocSpecified(allocation.offset, allocation.size, type);

	throw std::runtime_error("vpp::DeviceMemory::alloc: not enough memory left");
}

Allocation DeviceMemory::allocSpecified(std::size_t offset, std::size_t size, AllocationType type)
{
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

	static constexpr AllocationEntry start = {{0, 0}, AllocationType::none};
	auto granularity = device().properties().limits.bufferImageGranularity;


	//checks for best possible allocation
	//the best allocation wastes the least space for alignment or granularity reqs
	Allocation best = {};
	std::size_t bestWaste = -1;

	const AllocationEntry* old = &start;
	for(auto& alloc : allocations_)
	{
		vk::DeviceSize alignedOffset = std::ceil(old->allocation.end() / alignment) * alignment;

		//check for granularity between prev and to be inserted
		if(old->type != AllocationType::none && old->type != type)
			alignedOffset = std::ceil(alignedOffset / granularity) * granularity;

		//check for granularity between next and to be inserted
		auto end = alignedOffset + size;
		if(alloc.type != AllocationType::none && alloc.type != type)
			end = std::ceil(end / granularity) * granularity;

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
	vk::DeviceSize alignedOffset = std::ceil(old->allocation.end() / alignment) * alignment;

	if(old->type != AllocationType::none && old->type != type)
		alignedOffset = std::ceil(alignedOffset / granularity) * granularity;

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

void DeviceMemory::free(const Allocation& alloc)
{
	for(auto it = allocations_.cbegin(); it != allocations_.cend(); ++it) {
		if(it->allocation.offset == alloc.offset && it->allocation.size == alloc.size) {
			allocations_.erase(it);
			return;
		}
	}
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
	if(!(propertyFlags() & vk::MemoryPropertyBits::hostVisible))
		throw std::logic_error("vpp::DeviceMemory::map: not mappable.");

	if(!mapped()) memoryMap_ = MemoryMap(*this, allocation);
	else memoryMap_.remap(allocation);

	return MemoryMapView(memoryMap_, allocation);
}

}
