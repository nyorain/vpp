#include <vpp/memory.hpp>
#include <iostream>

namespace vpp
{

//Entry
DeviceMemory::Entry::Entry(DeviceMemoryPtr memory, const Allocation& alloc)
	: memory_(memory), allocation_(alloc)
{
}

DeviceMemory::Entry::~Entry()
{
	if(memory_)
	{
		memory_->free(allocation_);
	}
}

//Memory
DeviceMemory::DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info)
	: Resource(dev)
{
	auto heapIndex = dev.memoryProperties().memoryTypes()[typeIndex_];
	flags_ = dev.memoryProperties().memoryHeaps()[heapIndex].flags();

	typeIndex_ = info.memoryTypeIndex();
	size_ = info.allocationSize();

	vk::allocateMemory(vkDevice(), &info, nullptr, &memory_);
}
DeviceMemory::DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex)
	: Resource(dev)
{
	auto heapIndex = dev.memoryProperties().memoryTypes()[typeIndex];
	flags_ = dev.memoryProperties().memoryHeaps()[heapIndex].flags();

	typeIndex_ = typeIndex;
	size_ = size;

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

DeviceMemory::Allocation DeviceMemory::alloc(std::size_t size, std::size_t aligment)
{
	static const Allocation start = {0, 0};

	const Allocation* old = &start;
	for(auto& alloc : allocations_)
	{
		auto alignedOffset = (old->offset & ~aligment);
		if(alignedOffset < old->offset) alignedOffset += aligment;

		if(alloc.offset - alignedOffset > size)
		{
			allocations_.emplace_back(alignedOffset, size);
			return allocations_.back();
		}
	}

	return {0, 0};
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

void DeviceMemoryAllocator::request(vk::Image requestor,  const vk::MemoryRequirements& reqs,
		DeviceMemory::Entry& entry)
{
	for(auto& type : imageRequirements_)
	{
		if(reqs.memoryTypeBits() & (1 << type.first))
		{
			type.second.push_back({requestor, reqs, &entry});
			return;
		}
	}

	unsigned int typeIndex = 0;
	while(((reqs.memoryTypeBits() >> typeIndex++) & 1) != 1);
	imageRequirements_[typeIndex].push_back({requestor, reqs, &entry});
}

void DeviceMemoryAllocator::allocate()
{
	std::vector<DeviceMemoryPtr> memories;
	std::map<unsigned int, std::size_t> sizeMap;

	for(auto i = 0u; i < 32; ++i)
	{
		auto itbuf = bufferRequirements_.find(i);
		if(itbuf != bufferRequirements_.cend())
		{
		}

	}

	for(auto i = 0u; i < 32; ++i)
	{
		auto itimg = imageRequirements_.find(i);	
		if(itimg != imageRequirements_.cend())
		{
		}
	}

	for(auto& entry : sizeMap)
	{
		memories.push_back(std::make_shared<DeviceMemory>(device(), entry.second, entry.first));

		for(auto& buf : bufferRequirements_[entry.first])
		{
			*buf.entry = DeviceMemory::Entry(memories.back(), memories.back()->alloc(0, 0, 0));
		}
	}
}

}
