#include <vpp/memory.hpp>
#include <iostream>

namespace vpp
{

//MemoryMap
MemoryMap::MemoryMap(const DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
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
	auto nbeg = std::min(allocation.offset, allocation_.offset);
	auto nsize = std::max(allocation.end(), allocation_.end()) - nbeg;

	if(offset() <= nbeg && offset() + size() >= nbeg + nsize) return;

	vk::unmapMemory(memory().vkDevice(), vkMemory());
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

vk::DeviceMemory MemoryMap::vkMemory() const
{
	return memory_->vkDeviceMemory();
}

void MemoryMap::flushRanges() const
{
	//TODO: check for memory flags - function call needed?
	vk::MappedMemoryRange range {vkMemory(), offset(), size()};
	vk::flushMappedMemoryRanges(memory().vkDevice(), 1, &range);
}

void MemoryMap::invalidateRanges() const
{
	//TODO: check for memory flags - function call needed?
	vk::MappedMemoryRange range {vkMemory(), offset(), size()};
	vk::invalidateMappedMemoryRanges(memory().vkDevice(), 1, &range);
}

void MemoryMap::unmap()
{
	if(memory_ && vkMemory() && ptr() && size())
	{
		vk::unmapMemory(memory().vkDevice(), vkMemory());
	}

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

std::uint8_t* MemoryMapView::ptr() const
{
	return memoryMap().ptr() + allocation().offset - memoryMap().offset();
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
	typeIndex_ = info.memoryTypeIndex();
	size_ = info.allocationSize();
	flags_ = dev.memoryProperties().memoryTypes()[typeIndex_].propertyFlags();

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
	static const Allocation start = {0, 0};

	const Allocation* old = &start;
	for(auto& alloc : allocations_)
	{
		auto alignedOffset = ((old->offset + alignment) & ~alignment);
		if((alloc.allocation.offset - alignedOffset) > size)
			return allocSpecified(alignedOffset, size, type);
	}

	return {0, 0};
}

Allocation DeviceMemory::allocSpecified(std::size_t offset, std::size_t size, AllocationType type)
{
	allocations_.push_back({{offset, size}, type});
	return allocations_.back().allocation;
}

void DeviceMemory::free(const Allocation& alloc)
{
	for(auto it = allocations_.cbegin(); it != allocations_.cend(); ++it)
	{
		if(it->allocation.offset == alloc.offset && it->allocation.size == alloc.size)
		{
			allocations_.erase(it);
			return;
		}
	}
}

std::size_t DeviceMemory::biggestBlock() const
{
	std::size_t ret {0};
	std::size_t oldend {0};

	for(auto& alloc : allocations_)
	{
		if(alloc.allocation.offset - oldend > ret) ret = alloc.allocation.offset - oldend;
		oldend = alloc.allocation.offset + alloc.allocation.size;
	}

	return ret;
}

std::size_t DeviceMemory::free() const
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
		throw std::logic_error("DeviceMemory::map: not mappable.");

	if(!mapped()) memoryMap_ = MemoryMap(*this, allocation);
	else memoryMap_.remap(allocation);

	return MemoryMapView(memoryMap_, allocation);
}

}
