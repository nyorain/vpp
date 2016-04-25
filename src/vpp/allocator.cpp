#include <vpp/allocator.hpp>

namespace vpp
{

//Entry
DeviceMemoryAllocator::Entry::Entry(DeviceMemory* memory, const Allocation& alloc)
	: memory_(memory), allocation_(alloc)
{
}

DeviceMemoryAllocator::Entry::Entry(Entry&& other) noexcept
{
	this->swap(other);
}

DeviceMemoryAllocator::Entry& DeviceMemoryAllocator::Entry::operator=(Entry&& other) noexcept
{
	this->free();
	this->swap(other);
	return *this;
}

DeviceMemoryAllocator::Entry::~Entry()
{
	if(allocator_) allocator_->removeRequest(*this);
	free();
}

void DeviceMemoryAllocator::Entry::swap(Entry& other) noexcept
{
	using std::swap;

	swap(memory_, other.memory_);
	swap(allocation_, other.allocation_);
	swap(allocator_, other.allocator_);
}

void DeviceMemoryAllocator::Entry::free()
{
	if((memory_ != nullptr) && (allocation_.size > 0))
	{
		memory_->free(allocation_);
	}

	memory_ = nullptr;
	allocation_ = {};
}


//Allocator
DeviceMemoryAllocator::DeviceMemoryAllocator(const Device& dev) : Resource(dev)
{
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
	//RAII
	if(device_)allocate();
}

DeviceMemoryAllocator::DeviceMemoryAllocator(DeviceMemoryAllocator&& other) noexcept
{
	this->swap(other);
}
DeviceMemoryAllocator& DeviceMemoryAllocator::operator=(DeviceMemoryAllocator&& other) noexcept
{
	if(device_)allocate();
	this->swap(other);
	return *this;
}

void DeviceMemoryAllocator::swap(DeviceMemoryAllocator& other) noexcept
{
	using std::swap;

	swap(bufferRequirements_, other.bufferRequirements_);
	swap(imageRequirements_, other.imageRequirements_);
	swap(device_, other.device_);
}

void DeviceMemoryAllocator::request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
	Entry& entry)
{
	entry.allocator_ = this;

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
	vk::ImageTiling tiling, Entry& entry)
{
	entry.allocator_ = this;

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

bool DeviceMemoryAllocator::removeRequest(const Entry& entry)
{
	//check buffer
	for(auto& reqs : bufferRequirements_)
	{
		for(auto it = reqs.second.begin(); it != reqs.second.end(); ++it)
		{
			if(it->entry == &entry)
			{
				reqs.second.erase(it);
				return true;
			}
		}
	}

	//check image
	for(auto& reqs : imageRequirements_)
	{
		for(auto it = reqs.second.begin(); it != reqs.second.end(); ++it)
		{
			if(it->entry == &entry)
			{
				reqs.second.erase(it);
				return true;
			}
		}
	}

	//not found
	return false;
}

void DeviceMemoryAllocator::allocate()
{
	using AllocType = DeviceMemory::AllocationType;

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
		auto memory = std::make_unique<DeviceMemory>(device(), entry.second, entry.first);

		//buffers
		for(auto& buf : bufferRequirements_[entry.first])
		{
			auto alloc = memory->allocSpecified(buf.offset, buf.requirements.size(),
				AllocType::buffer);
			if(!buf.entry)
			{
				std::cerr << "vpp::DeviceMemoryAllocator: no buffer requirement entry\n";
			}
			else
			{
				*buf.entry = Entry(memory.get(), alloc);
				buf.entry->allocator_ = nullptr;
			}

			vk::bindBufferMemory(vkDevice(), buf.requestor, memory->vkDeviceMemory(), buf.offset);
		}

		//images
		for(auto& img : imageRequirements_[entry.first])
		{
			auto alloctype = (img.tiling == vk::ImageTiling::Linear) ? AllocType::imageLinear
				: AllocType::imageOptimal;
			auto alloc = memory->allocSpecified(img.offset, img.requirements.size(), alloctype);
			if(!img.entry)
			{
				std::cerr << "vpp::DeviceMemoryAllocator: no image requirement entry\n";
			}
			else
			{
				*img.entry = Entry(memory.get(), alloc);
				img.entry->allocator_ = nullptr;
			}

			vk::bindImageMemory(vkDevice(), img.requestor, memory->vkDeviceMemory(), img.offset);
		}
	}

	//clear
	imageRequirements_.clear();
	bufferRequirements_.clear();
}

}
