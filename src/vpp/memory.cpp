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
	if(!(memory.propertyFlags() & vk::MemoryPropertyFlagBits::HostVisible))
		throw std::logic_error("vpp::MemoryMap: trying to map device local memory");

	vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {}, &ptr_);
}

MemoryMap::MemoryMap(MemoryMap&& other) noexcept
{
	swap(*this, other);
}

MemoryMap& MemoryMap::operator=(MemoryMap&& other) noexcept
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

	vk::mapMemory(vkDevice(), vkMemory(), offset(), size(), {}, &ptr_);
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
	vk::flushMappedMemoryRanges(vkDevice(), 1, &range);
}

void MemoryMap::invalidateRanges() const
{
	if(coherent())
	{
		std::cout << "vpp::MemoryMap::invalidateRanges: called but not needed, mem coherent\n";
		return;
	}

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, &range);
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
	vk::flushMappedMemoryRanges(vkDevice(), 1, &range);
}

void MemoryMapView::invalidateRanges() const
{
	if(coherent()) return;

	auto range = mappedMemoryRange();
	vk::invalidateMappedMemoryRanges(vkDevice(), 1, &range);
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

	vk::allocateMemory(vkDevice(), &info, nullptr, &memory_);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex)
	: Resource(dev)
{
	typeIndex_ = typeIndex;
	size_ = size;
	flags_ = dev.memoryProperties().memoryTypes()[typeIndex_].propertyFlags();

	vk::MemoryAllocateInfo info;
	info.allocationSize(size_);
	info.memoryTypeIndex(typeIndex_);
	vk::allocateMemory(vkDevice(), &info, nullptr, &memory_);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, vk::MemoryPropertyFlags flags)
	: Resource(dev)
{
	flags_ = flags;
	typeIndex_ = device().memoryType(flags);
	size_ = size;

	vk::MemoryAllocateInfo info;
	info.allocationSize(size_);
	info.memoryTypeIndex(typeIndex_);
	vk::allocateMemory(vkDevice(), &info, nullptr, &memory_);
}
DeviceMemory::~DeviceMemory()
{
	if(!allocations_.empty())
	{
		std::cerr << "vpp::~DeviceMemory: there are " << allocations_.size() << "allocs left\n";
	}

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
	static const AllocationEntry start = {{0, 0}, AllocationType::none};
	auto granularity = device().properties().limits().bufferImageGranularity();

	const AllocationEntry* old = &start;
	for(auto& alloc : allocations_) {
		auto alignedOffset = ((old->allocation.offset + alignment) & ~(alignment - 1));
		if(old->type != AllocationType::none && old->type != alloc.type)
			alignedOffset = (alignedOffset + granularity) & ~(granularity - 1);

		if((alloc.allocation.offset - alignedOffset) > size) return {alignedOffset, size};
		old = &alloc;
	}

	return {};
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
	if(!(propertyFlags() & vk::MemoryPropertyFlagBits::HostVisible))
		throw std::logic_error("vpp::DeviceMemory::map: not mappable.");

	if(!mapped()) memoryMap_ = MemoryMap(*this, allocation);
	else memoryMap_.remap(allocation);

	return MemoryMapView(memoryMap_, allocation);
}

}
