#include <vpp/allocator.hpp>
#include <vpp/vk.hpp>
#include <cmath>

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

	Requirement req;
	req.type = RequirementType::buffer;
	req.size = reqs.size;
	req.alignment = reqs.alignment;
	req.memoryTypes = reqs.memoryTypeBits;
	req.buffer = requestor;

	requirements_.push_back(req);
}

void DeviceMemoryAllocator::request(vk::Image requestor, const vk::MemoryRequirements& reqs,
	vk::ImageTiling tiling, Entry& entry)
{
	if(reqs.size == 0)
		throw std::logic_error("vpp::DeviceMemAllocator::request: allocation size of 0 not allowed");

	entry = {};
	entry.allocator_ = this;

	using ReqType = RequirementType;

	Requirement req;
	req.type = (tiling == vk::ImageTiling::linear) ? ReqType::linearImage : ReqType::optimalImage;
	req.size = reqs.size;
	req.alignment = reqs.alignment;
	req.memoryTypes = reqs.memoryTypeBits;
	req.image = requestor;

	requirements_.push_back(req);
}

bool DeviceMemoryAllocator::removeRequest(const Entry& entry)
{
	auto it = findReq(entry);
	if(it == requirements_.end()) return false;
	requirements_.erase(it);
	return true;
}

bool DeviceMemoryAllocator::moveEntry(const Entry& oldOne, Entry& newOne)
{
	auto req = findReq(oldOne);
	if(req == requirements_.end()) return false;

	req->entry = &newOne;
	return true;
}

void DeviceMemoryAllocator::minimizeAllocations()
{
	//some code to allocate on already existent memories with free space
}

DeviceMemory* DeviceMemoryAllocator::findMem(Requirement& req)
{
	for(auto& mem : memories_)
	{
		if(!(req.memoryTypes & (1 << mem->typeIndex()))) continue;
		auto allocation = mem->allocatable(req.size, req.alignment, toAllocType(req.type));
		if(allocation.size == 0) continue;

		mem->allocSpecified(allocation.offset, allocation.size, toAllocType(req.type));

		//can be allocated on memory, allocate and bind it
		if(req.type == RequirementType::buffer)
			vk::bindBufferMemory(vkDevice(), req.buffer, mem->vkDeviceMemory(), allocation.offset);
		else
			vk::bindImageMemory(vkDevice(), req.image, mem->vkDeviceMemory(), allocation.offset);

		return mem.get();
	}

	return nullptr;
}

std::map<unsigned int, std::size_t> DeviceMemoryAllocator::sizeMap()
{
	//holding the total size for each needed deivceMemory
	std::map<unsigned int, std::size_t> sMap;

	//physical device granularity (space between different allocation types)
	auto gran = device().properties().limits.bufferImageGranularity;

	//hold the (virtually registered) lat allocation on the given deviceMemory
	std::map<unsigned int, AllocationType> lastAlloc;

	//add bufferRequirements to sizeMap
	for(auto i = 0u; i < 32; ++i) {
		auto itbuf = bufferRequirements_.find(i);
		if(itbuf != bufferRequirements_.cend()) {
			for(auto& req : itbuf->second) {
				auto align = req.requirements.alignment;
				if(sMap[i] % align) sMap[i] = sMap[i] - (sMap[i] % align) + align;
				req.offset = sMap[i];

				sMap[i] += req.requirements.size;
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

				auto align = req.requirements.alignment;
				if(sMap[i] % align) sMap[i] = sMap[i] - (sMap[i] % align) + align;
				req.offset = sMap[i];

				sMap[i] += req.requirements.size;
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
			auto alloc = memory->allocSpecified(buf.offset, buf.requirements.size,
				AllocationType::linear);
			buf.entry->memory_ = memory.get();
			buf.entry->allocation_ = alloc;

			vk::bindBufferMemory(vkDevice(), buf.requestor, memory->vkDeviceMemory(), buf.offset);
		}

		//images
		for(auto& img : imageRequirements_[entry.first]) {
			auto alloc = memory->allocSpecified(img.offset, img.requirements.size, img.type);
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
	auto req = findReq(entry);
	if(req == requirements_.end()) return false;

	//this function makes sure the given entry is allocated
	//first of all try to find a free spot in the already existent memories
	if(findMem(*req)) 
	{
		requirements_.erase(req);
		return true;
	}

	//finding free memory failed, so query the memory type with the most requests and on
	//which the given entry can be allocated and then alloc and bind all reqs for this type
	auto type = findBestType(req->memoryTypes);
	allocate(type);

	return true;
}

void DeviceMemoryAllocator::allocate(unsigned int type)
{
	auto gran = device().properties().limits.bufferImageGranularity;

	vk::DeviceSize offset = 0;
	bool applyGran = false;

	std::vector<std::pair<Requirement*, unsigned int>> offsets;

	//iterate through all reqs and place the ones that may be allocated on the given type
	//there. First all linear resources, then all optimal resources.
	for(auto& req : requirements_)
	{
		if(!suppportsType(req, type)) continue;
		if(req.type != RequirementType::buffer)
		{
			applyGran = true;
			continue;
		}

		offset = std::ceil(offset / req.alignment) * req.alignment;
		offsets.push_back({&req, offset});
		offset += req.size;
	}

	//apply granularity if there were already resources placed and there are ones to be placed
	if(offset > 0 && applyGran) offset = std::ceil(offset / gran) * gran;

	//now all optimal resources
	for(auto& req : requirements_)
	{
		if(!suppportsType(req, type) || req.type == RequirementType::buffer) continue;
		offset = std::ceil(offset / req.alignment) * req.alignment;
		offsets.push_back({&req, offset});
		offset += req.size;
	}

	//now the needed size is known and the requirements to be allocated have their offsets
	auto mem = std::make_unique<DeviceMemory>(offset, type);

	//bind and alloc all to be allocated resources
	for(auto& res : offsets)
	{
		auto& req = *res.first;
		auto& offset = res.second;
		auto& entry = *req.entry;

		entry.allocation_ = mem->allocSpecified(offset, req.size, toAllocType(req.type));
		entry.memory_ = mem.get();

		if(res.first->type == RequirementType::buffer)
		{
			vk::bindBufferMemory(vkDevice(), res.first->buffer, mem->vkDeviceMemory(), offset);
		}
		else
		{
			vk::bindImageMemory(vkDevice(), res.first->image, mem->vkDeviceMemory(), offset);
		}
	}
	
	//erase the allocated requirements
	for(auto it = requirements_.begin(); it < requirements_.end();)
	{
		for(auto& res : offsets)
		{
			if(&(*it) == res.first) it = requirements_.erase(it);
			else ++it;
		}
	}

	memories_.push_back(std::move(mem));
}

std::vector<std::pair<unsigned int, DeviceMemoryAllocator::Requirement*>> 
DeviceMemoryAllocator::queryTypes()
{
	//vector to return
	std::vector<std::pair<unsigned int, Requirement*>> ret;
	ret.reserve(requirements_.size());

	//this functions implements an algorithm to choose the best type for each requirement from
	//its typebits, so that in the end there will be as few allocations as possible needed.
	//map holding the current count of the different types
	std::map<unsigned int, std::vector<Requirement*>> occurences;

	//function to count the occurences of the different memory types and store them in
	//the occurences map
	auto countOccurences = [&]()
			{
				for(auto& req : requirements_)
					for(auto i = 0u; i < 32; ++i)	
						if(suppportsType(req, i)) occurences[i].push_back(&req);
			};

	//initial occurences count
	countOccurences();

	//while there are requirements left that have not been moved into ret
	//alternative condition: while(!occurences.empty())
	while(ret.size() < requirements_.size())
	{
		//find the least counted type
		auto best = 0u;
		std::uint32_t bestID = 0u;	
		for(auto& occ : occurences)
		{
			if(occ.second.size() > best)
			{
				best = occ.second.size();
				bestID = occ.first;
			}
		}

		//function to determine if other types besides the given are supported
		auto othersSupported = [](const Requirement& req, unsigned int type)
			{
				for(auto i = 0u; i < 32 && i != type; ++i)	
					if(suppportsType(req, i)) return true;
				return false;
			};

		//remove the type with the fewest occurences
		bool canBeRemoved = true;
		for(auto& req : occurences[bestID])
			if(!othersSupported(*req, bestID)) canBeRemoved = false;

		//if it can be removed there must be at least one allocation for the given type
		//enter all reqs that can be allocated on the type and remove them from occurences
		if(!canBeRemoved)
		{
			for(auto& req : occurences[bestID])
			{
				//set the requirements typebits to 0, indicating that it has a matching type
				req->memoryTypes = 0;

				//add requirements to ret
				ret.push_back({bestID, req});
			}

			//explicitly remove the occurences for the chosen bit.
			//the other references to the now removed since type found requirements stay, but
			//sice their memoryTypes member is 0, they will no longer be counted
			occurences.erase(occurences.find(bestID));

			//the occurences have to be recounted since the occurences for type bits other than
			//best bestID (which will have 0 now) do not count longer (they have a type)
			countOccurences();
		}
		else
		{
			//remove the bestID type bit from all requirement type bits, to reduce the problems
			//complexity
			for(auto& req : occurences[bestID])
				req->memoryTypes &= (~bestID);

			//erase the occurences for bestID
			occurences.erase(occurences.find(bestID));
		}

	}

	return ret;
}

std::vector<DeviceMemory*> DeviceMemoryAllocator::memories() const
{
	std::vector<DeviceMemory*> ret;
	ret.reserve(memories_.size());
	for(auto& mem : memories_) ret.push_back(mem.get());
	return ret;
}

AllocationType DeviceMemoryAllocator::toAllocType(RequirementType type)
{
	switch(type)
	{
		case RequirementType::buffer:
		case RequirementType::linearImage: return AllocationType::linear;
		case RequirementType::optimalImage: return AllocationType::optimal;
		default: return AllocationType::none;
	}
}

unsigned int DeviceMemoryAllocator::findBestType(std::uint32_t typeBits) const
{
	auto best = 0;
	auto bestID = 0;

	for(auto i = 0u; i < 32; ++i)
	{
		//start with one, so even if there are no matching reqs at all, at least a supported
		//type bit is returned.
		auto count = 1; 
		if(!suppportsType(typeBits, i)) continue;
		for(auto& req : requirements_) if(suppportsType(req, i)) ++count;

		if(count > best)
		{
			best = count;
			bestID = i;
		}
	}

	return bestID;
}

bool DeviceMemoryAllocator::suppportsType(std::uint32_t typeBits, unsigned int type)
{
	return (typeBits & (1 << type));
}

bool DeviceMemoryAllocator::suppportsType(const Requirement& req, unsigned int type)
{
	return suppportsType(req.memoryTypes, type);
}

}
