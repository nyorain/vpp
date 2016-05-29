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
	swap(*this, other);
}

DeviceMemoryAllocator::Entry& DeviceMemoryAllocator::Entry::operator=(Entry&& other) noexcept
{
	this->free();
	swap(*this, other);
	return *this;
}

DeviceMemoryAllocator::Entry::~Entry()
{
	if(allocator_) allocator_->removeRequest(*this);
	free();
}

void swap(DeviceMemoryAllocator::Entry& a, DeviceMemoryAllocator::Entry& b) noexcept
{
	using std::swap;

	//signal the allocator (if there is any) that the entry has been moved
	//since the allocator stores references (addresses) of the entries
	if(!a.allocated() && a.allocator_) a.allocator_->moveEntry(a, b);
	if(!b.allocated() && b.allocator_) b.allocator_->moveEntry(b, a);

	//correclty swap the anonymous union
	//can be proably be done more elegant...

	//backup the memory or allocator values of a
	auto memTmp = a.allocated() ? a.memory() : nullptr;
	auto allocTmp = a.allocated() ? nullptr : a.allocator();

	//correclty "swap" the unions
	if(b.allocated()) a.memory_ = b.memory_;
	else a.allocator_ = b.allocator_;

	if(a.allocated()) b.memory_ = memTmp;
	else b.allocator_ = allocTmp;

	//swap allocations
	swap(a.allocation_, b.allocation_);
}

void DeviceMemoryAllocator::Entry::free()
{
	if(!allocated() && allocator_) allocator_->removeRequest(*this);
	else if(allocated()) memory_->free(allocation_);

	allocation_ = {};
	allocator_ = nullptr;
}

MemoryMapView DeviceMemoryAllocator::Entry::map() const
{
	return memory()->map(allocation());
}

//Allocator
DeviceMemoryAllocator::DeviceMemoryAllocator(const Device& dev) : Resource(dev)
{
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
	if(device_) allocate();
}

DeviceMemoryAllocator::DeviceMemoryAllocator(DeviceMemoryAllocator&& other) noexcept
{
	swap(*this, other);
}
DeviceMemoryAllocator& DeviceMemoryAllocator::operator=(DeviceMemoryAllocator&& other) noexcept
{
	if(device_) allocate();
	swap(*this, other);
	return *this;
}

void swap(DeviceMemoryAllocator& a, DeviceMemoryAllocator& b) noexcept
{
	using std::swap;

	swap(a.bufferRequirements_, b.bufferRequirements_);
	swap(a.imageRequirements_, b.imageRequirements_);
	swap(a.device_, b.device_);
	swap(a.memories_, b.memories_);
}

void DeviceMemoryAllocator::request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
	Entry& entry)
{
	if(!reqs.size)
		throw std::logic_error("vpp::DeviceMemAllocator::request: allocation size of 0 not allowed");

	entry = {};
	entry.allocator_ = this;

	for(auto& type : bufferRequirements_)
	{
		if(reqs.memoryTypeBits & (1 << type.first))
		{
			type.second.push_back({requestor, reqs, &entry});
			return;
		}
	}

	std::uint32_t typeIndex = 0;
	while(((reqs.memoryTypeBits >> typeIndex++) & 1) != 1);
	bufferRequirements_[typeIndex].push_back({requestor, reqs, &entry});
}

void DeviceMemoryAllocator::request(vk::Image requestor, const vk::MemoryRequirements& reqs,
	vk::ImageTiling tiling, Entry& entry)
{
	if(reqs.size == 0)
		throw std::logic_error("vpp::DeviceMemAllocator::request: allocation size of 0 not allowed");

	entry = {};
	entry.allocator_ = this;
	auto typ = (tiling == vk::ImageTiling::linear) ? AllocationType::linear : AllocationType::optimal;

	for(auto& type : imageRequirements_)
	{
		if(reqs.memoryTypeBits & (1 << type.first))
		{
			type.second.push_back({requestor, reqs, typ, &entry});
			return;
		}
	}

	unsigned int typeIndex = 0;
	while(((reqs.memoryTypeBits() >> typeIndex++) & 1) != 1);
	imageRequirements_[typeIndex].push_back({requestor, reqs, typ, &entry});
}

bool DeviceMemoryAllocator::removeRequest(const Entry& entry)
{
	//check buffer
	for(auto& reqs : bufferRequirements_) {
		for(auto it = reqs.second.begin(); it != reqs.second.end(); ++it) {
			if(it->entry == &entry) {
				reqs.second.erase(it);
				return true;
			}
		}
	}

	//check image
	for(auto& reqs : imageRequirements_) {
		for(auto it = reqs.second.begin(); it != reqs.second.end(); ++it) {
			if(it->entry == &entry) {
				reqs.second.erase(it);
				return true;
			}
		}
	}

	//not found
	return false;
}

bool DeviceMemoryAllocator::moveEntry(const Entry& oldOne, Entry& newOne)
{
	//check buffer
	for(auto& reqs : bufferRequirements_) {
		for(auto& req : reqs.second) {
			if(req.entry == &oldOne) {
				req.entry = &newOne;
				return true;
			}
		}
	}

	//check image
	for(auto& reqs : imageRequirements_) {
		for(auto& req : reqs.second) {
			if(req.entry == &oldOne) {
				req.entry = &newOne;
				return true;
			}
		}
	}

	//not found
	return false;
}

DeviceMemoryAllocator::BufReqs::iterator DeviceMemoryAllocator::findBufReq(const Entry& entry,
	unsigned int& type)
{
	for(auto& reqs : bufferRequirements_)
	{
		auto& b = reqs.second;
		for(auto it = b.begin(); it < b.end(); ++it)
		{
			if(it->entry == &entry)
			{
				type = reqs.first;
				return it;
			}
		}
	}

	type = 0;
	return {};
}

DeviceMemoryAllocator::ImgReqs::iterator DeviceMemoryAllocator::findImgReq(const Entry& entry,
	unsigned int& type)
{
	for(auto& reqs : imageRequirements_)
	{
		auto& b = reqs.second;
		for(auto it = b.begin(); it < b.end(); ++it)
		{
			if(it->entry == &entry)
			{
				type = reqs.first;
				return it;
			}
		}
	}

	type = 0;
	return {};
}

void DeviceMemoryAllocator::minimizeAllocations()
{
	//some code to allocate on already existent memories with free space
}

DeviceMemory* DeviceMemoryAllocator::findMem(const vk::MemoryRequirements& reqs, AllocationType type,
	Allocation& allocation)
{
	//try to find free memory that is able to allocate for the given requirements
	return nullptr;
}

std::map<unsigned int, std::size_t> DeviceMemoryAllocator::sizeMap()
{
	//holding the total size for each needed deivceMemory
	std::map<unsigned int, std::size_t> sMap;

	//physical device granularity (space between different allocation types)
	auto gran = device().properties().limits().bufferImageGranularity();

	//hold the (virtually registered) lat allocation on the given deviceMemory
	std::map<unsigned int, AllocationType> lastAlloc;

	//add bufferRequirements to sizeMap
	for(auto i = 0u; i < 32; ++i) {
		auto itbuf = bufferRequirements_.find(i);
		if(itbuf != bufferRequirements_.cend()) {
			for(auto& req : itbuf->second) {
				auto align = req.requirements.alignment();
				if(sMap[i] % align) sMap[i] = sMap[i] - (sMap[i] % align) + align;
				req.offset = sMap[i];

				sMap[i] += req.requirements.size();
			}
		}
	}

	//add ImageRequirements to sizeMap, care for granularity
	for(auto i = 0u; i < 32; ++i) {
		auto itimg = imageRequirements_.find(i);
		if(itimg != imageRequirements_.cend()) {
			for(auto& req : itimg->second) {
				if(lastAlloc[i] != AllocationType::none && req.type != lastAlloc[i]) {
					if(sMap[i] % gran) sMap[i] = sMap[i] - (sMap[i] % gran) + gran;
					lastAlloc[i] = req.type;
				}

				auto align = req.requirements.alignment();
				if(sMap[i] % align) sMap[i] = sMap[i] - (sMap[i] % align) + align;
				req.offset = sMap[i];

				sMap[i] += req.requirements.size();
			}
		}
	}

	return sMap;
}

//TODO: reimplement both allocate functions to be more effeicient (reuse old mem esp.)
void DeviceMemoryAllocator::allocate()
{
	minimizeAllocations();
	auto sMap = sizeMap();

	//allocate and bind DeviceMemory objects
	for(auto& entry : sMap) {
		auto memory = std::make_unique<DeviceMemory>(device(), entry.second, entry.first);

		//buffers
		for(auto& buf : bufferRequirements_[entry.first]) {
			auto alloc = memory->allocSpecified(buf.offset, buf.requirements.size(),
				AllocationType::linear);
			buf.entry->memory_ = memory.get();
			buf.entry->allocation_ = alloc;

			vk::bindBufferMemory(vkDevice(), buf.requestor, memory->vkDeviceMemory(), buf.offset);
		}

		//images
		for(auto& img : imageRequirements_[entry.first]) {
			auto alloc = memory->allocSpecified(img.offset, img.requirements.size(), img.type);
			img.entry->memory_ = memory.get();
			img.entry->allocation_ = alloc;

			vk::bindImageMemory(vkDevice(), img.requestor, memory->vkDeviceMemory(), img.offset);
		}

		memories_.push_back(std::move(memory));
	}

	//clear
	imageRequirements_.clear();
	bufferRequirements_.clear();
}

bool DeviceMemoryAllocator::allocate(const Entry& entry)
{
	//TODO
	allocate();
	return true;

	//try to find memory for the given entry
	constexpr static unsigned int undef = -1;

	unsigned int type = undef;
	Allocation allocation;

	auto buf = findBufReq(entry, type);
	if(type != undef) {
		auto mem = findMem(buf->requirements, AllocationType::linear, allocation);
		if(mem) {
			buf->entry->memory_ = mem;
			buf->entry->allocation_ = allocation;

			vk::bindBufferMemory(vkDevice(), buf->requestor, mem->vkDeviceMemory(), buf->offset);
			return true;
		}
	} else {
		auto img = findImgReq(entry, type);
		if(type == undef) return false; //cannot be found as buffer nor as image

		auto mem = findMem(img->requirements, AllocationType::linear, allocation);
		if(mem) {
			img->entry->memory_ = mem;
			img->entry->allocation_ = allocation;

			vk::bindImageMemory(vkDevice(), img->requestor, mem->vkDeviceMemory(), img->offset);
			return true;
		}
	}

	//allocate a new memory for the needed type
	//allocate(type);
	allocate();
	return true;
}

void DeviceMemoryAllocator::allocate(unsigned int type)
{
	//find all requests that might be allocated on this type an allocate them
}

std::vector<DeviceMemory*> DeviceMemoryAllocator::memories() const
{
	std::vector<DeviceMemory*> ret;
	ret.reserve(memories_.size());
	for(auto& mem : memories_) ret.push_back(mem.get());
	return ret;
}

}
