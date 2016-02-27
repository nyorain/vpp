#include <vpp/memory.hpp>
#include <iostream>

namespace vpp
{

//Entry
DeviceMemory::Entry::Entry(DeviceMemoryPtr memory, const Allocation& alloc)
	: memory_(memory), allocation_(alloc)
{
}

DeviceMemory::Entry::Entry(Entry&& other)
{
	std::swap(memory_, other.memory_);
	std::swap(allocation_, other.allocation_);
}

DeviceMemory::Entry& DeviceMemory::Entry::operator=(Entry&& other)
{
	this->free();
	std::swap(memory_, other.memory_);
	std::swap(allocation_, other.allocation_);

	return *this;
}

DeviceMemory::Entry::~Entry()
{
	free();
}

void DeviceMemory::Entry::free()
{
	if(memory_ && allocation_.size > 0)
	{
		memory_->free(allocation_);
	}

	memory_.reset();
	allocation_ = {0, 0};
}

//Memory
DeviceMemory::DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info)
	: Resource(dev)
{
	typeIndex_ = info.memoryTypeIndex();
	size_ = info.allocationSize();

	//auto heapIndex = dev.memoryProperties().memoryTypes()[typeIndex_].heapIndex();
	flags_ = dev.memoryProperties().memoryTypes()[typeIndex_].propertyFlags();

	vk::allocateMemory(vkDevice(), &info, nullptr, &memory_);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex)
	: Resource(dev)
{
	typeIndex_ = typeIndex;
	size_ = size;

	//auto heapIndex = dev.memoryProperties().memoryTypes()[typeIndex].heapIndex();
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
	typeIndex_ = device().memoryType(~0u, flags);
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

DeviceMemory::Allocation DeviceMemory::alloc(std::size_t size, std::size_t alignment)
{
	static const Allocation start = {0, 0};

	const Allocation* old = &start;
	for(auto& alloc : allocations_)
	{
		auto alignedOffset = ((old->offset + alignment) & ~alignment);
		if((alloc.offset - alignedOffset) > size)
			return allocSpecified(alignedOffset, size);
	}

	return {0, 0};
}

DeviceMemory::Allocation DeviceMemory::allocSpecified(std::size_t offset, std::size_t size)
{
	allocations_.push_back({offset, size});
	return allocations_.back();
}

void DeviceMemory::free(const Allocation& alloc)
{
	for(auto it = allocations_.cbegin(); it != allocations_.cend(); ++it)
	{
		if(it->offset == alloc.offset && it->size == alloc.size)
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
		if(alloc.offset - oldend > ret) ret = alloc.offset - oldend;
		oldend = alloc.offset + alloc.size;
	}

	return ret;
}

std::size_t DeviceMemory::free() const
{
	std::size_t ret {0};
	for(auto& alloc : allocations_)
		ret += alloc.size;

	return size() - ret;
}
std::size_t DeviceMemory::size() const
{
	return size_;
}

//Manager
DeviceMemoryAllocator::DeviceMemoryAllocator(const Device& dev) : Resource(dev)
{
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
	//RAII
	allocate();
}

void DeviceMemoryAllocator::request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
	DeviceMemory::Entry& entry)
{
	for(auto& type : bufferRequirements_)
	{
		if(reqs.memoryTypeBits() & (1 << type.first))
		{
			type.second.push_back({requestor, reqs, &entry});
			return;
		}
	}

	std::uint32_t typeIndex = 0;
	while(((reqs.memoryTypeBits() >> typeIndex++) & 1) != 1);
	bufferRequirements_[typeIndex].push_back({requestor, reqs, &entry});
}

void DeviceMemoryAllocator::request(vk::Image requestor, const vk::MemoryRequirements& reqs,
	vk::ImageTiling tiling, DeviceMemory::Entry& entry)
{
	for(auto& type : imageRequirements_)
	{
		if(reqs.memoryTypeBits() & (1 << type.first))
		{
			type.second.push_back({requestor, reqs, tiling, &entry});
			return;
		}
	}

	unsigned int typeIndex = 0;
	while(((reqs.memoryTypeBits() >> typeIndex++) & 1) != 1);
	imageRequirements_[typeIndex].push_back({requestor, reqs, tiling, &entry});
}

void DeviceMemoryAllocator::allocate()
{
	enum class AllocType
	{
		none = 0,
		buffer,
		imageOptimal,
		imageLinear
	};

	//physical device granularity (space between different allocation types)
	auto granularity = device().properties().limits().bufferImageGranularity();

	//holding the total size for each needed deivceMemory
	std::map<unsigned int, std::size_t> sizeMap;

	//hold the (virtually registered) lat allocation on the given deviceMemory
	std::map<unsigned int, AllocType> lastAlloc;

	//add bufferRequirements to sizeMap
	for(auto i = 0u; i < 32; ++i)
	{
		auto itbuf = bufferRequirements_.find(i);
		if(itbuf != bufferRequirements_.cend())
		{
			for(auto& req : itbuf->second)
			{
				auto align = req.requirements.alignment();
				auto alignedOffset = ((sizeMap[i] + align) & ~align);
				req.offset = alignedOffset;

				sizeMap[i] = alignedOffset + req.requirements.size();
				lastAlloc[i] = AllocType::buffer;
			}
		}
	}

	//add ImageRrequirements to sizeMap, care for granularity
	for(auto i = 0u; i < 32; ++i)
	{
		auto itimg = imageRequirements_.find(i);
		if(itimg != imageRequirements_.cend())
		{
			for(auto& req : itimg->second)
			{
				auto allocType = (req.tiling == vk::ImageTiling::Optimal) ?
						AllocType::imageOptimal : AllocType::imageLinear;

				if(lastAlloc[i] != AllocType::none && allocType != lastAlloc[i])
				{
					sizeMap[i] = (sizeMap[i] + granularity) & ~granularity;
					lastAlloc[i] = allocType;
				}

				auto align = req.requirements.alignment();
				auto alignedOffset = ((sizeMap[i] + align) & ~align);
				req.offset = alignedOffset;

				sizeMap[i] = alignedOffset + req.requirements.size();
			}
		}
	}

	//allocate and bind DeviceMemory objects
	for(auto& entry : sizeMap)
	{
		auto memory = std::make_shared<DeviceMemory>(device(), entry.second, entry.first);

		//buffers
		for(auto& buf : bufferRequirements_[entry.first])
		{
			auto alloc = memory->allocSpecified(buf.offset, buf.requirements.size());
			if(!buf.entry)
			{
				std::cerr << "vpp::DeviceMemoryAllocator: no buffer requirement entry\n";
			}
			else
			{
				*buf.entry = DeviceMemory::Entry(memory, alloc);
			}

			vk::bindBufferMemory(vkDevice(), buf.requestor, memory->vkDeviceMemory(), buf.offset);
		}

		//images
		for(auto& img : imageRequirements_[entry.first])
		{
			auto alloc = memory->allocSpecified(img.offset, img.requirements.size());
			if(!img.entry)
			{
				std::cerr << "vpp::DeviceMemoryAllocator: no image requirement entry\n";
			}
			else
			{
				*img.entry = DeviceMemory::Entry(memory, alloc);
			}

			vk::bindImageMemory(vkDevice(), img.requestor, memory->vkDeviceMemory(), img.offset);
		}
	}
}

//MemoryMap
MemoryMap::MemoryMap(const DeviceMemory& memory, std::size_t offset, std::size_t size)
	: memory_(&memory), offset_(offset), size_(size)
{
	vk::mapMemory(memory.vkDevice(), vkMemory(), offset, size, {}, &ptr_);
}

MemoryMap::MemoryMap(const DeviceMemory& memory, const DeviceMemory::Allocation& alloc)
	: MemoryMap(memory, alloc.offset, alloc.size)
{
}

MemoryMap::MemoryMap(const DeviceMemory::Entry& entry)
	: MemoryMap(entry.memory(), entry.offset(), entry.size())
{
}

MemoryMap::MemoryMap(MemoryMap&& other)
{
	std::swap(memory_, other.memory_);
	std::swap(size_, other.size_);
	std::swap(offset_, other.offset_);
	std::swap(ptr_, other.ptr_);
}

MemoryMap& MemoryMap::operator=(MemoryMap&& other)
{
	unmap();

	std::swap(memory_, other.memory_);
	std::swap(size_, other.size_);
	std::swap(offset_, other.offset_);
	std::swap(ptr_, other.ptr_);

	return *this;
}

MemoryMap::~MemoryMap()
{
	unmap();
}

void MemoryMap::flushRanges() const
{
	//check for memory flags - function call needed?
	vk::MappedMemoryRange range {vkMemory(), offset(), size()};
	vk::flushMappedMemoryRanges(memory().vkDevice(), 1, &range);
}

void MemoryMap::invalidateRanges() const
{
	//check for memory flags - function call needed?
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
	size_ = 0;
	offset_ = 0;
	ptr_ = nullptr;
}

}
