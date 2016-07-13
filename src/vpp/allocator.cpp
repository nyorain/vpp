#include <vpp/allocator.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/debug.hpp>
#include <algorithm>

namespace vpp
{

//Entry
MemoryEntry::MemoryEntry(DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
}

MemoryEntry::MemoryEntry(MemoryEntry&& other) noexcept
{
	swap(*this, other);
}

MemoryEntry& MemoryEntry::operator=(MemoryEntry other) noexcept
{
	swap(*this, other);
	return *this;
}

MemoryEntry::~MemoryEntry()
{
	if(!allocated() && allocator_) allocator_->removeRequest(*this);
	else if(allocated()) memory_->free(allocation_);
}

void swap(MemoryEntry& a, MemoryEntry& b) noexcept
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

MemoryMapView MemoryEntry::map() const
{
	return memory()->map(allocation());
}

//Allocator
DeviceMemoryAllocator::DeviceMemoryAllocator(const Device& dev) : Resource(dev)
{
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
	VPP_DEBUG_CHECK(vpp::~DeviceMemoryAllocator,
	{
		if(!requirements_.empty()) VPP_DEBUG_OUTPUT("There are requirements left");
	});
}

DeviceMemoryAllocator::DeviceMemoryAllocator(DeviceMemoryAllocator&& other) noexcept
{
	swap(*this, other);
}
DeviceMemoryAllocator& DeviceMemoryAllocator::operator=(DeviceMemoryAllocator other) noexcept
{
	swap(*this, other);
	return *this;
}

void swap(DeviceMemoryAllocator& a, DeviceMemoryAllocator& b) noexcept
{
	using std::swap;

	swap(a.resourceBase(), b.resourceBase());
	swap(a.requirements_, b.requirements_);
	swap(a.memories_, b.memories_);
}

void DeviceMemoryAllocator::request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
	vk::BufferUsageFlags usage, MemoryEntry& entry)
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
	req.entry = &entry;

	//apply additional device limits alignments
	auto align = device().properties().limits.minUniformBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::uniformBuffer && align > 0)
		req.alignment = vpp::align(req.alignment, align);

	align = device().properties().limits.minTexelBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::uniformTexelBuffer && align > 0)
		req.alignment = vpp::align(req.alignment, align);

	align = device().properties().limits.minStorageBufferOffsetAlignment;
	if(usage & vk::BufferUsageBits::storageBuffer && align > 0)
		req.alignment = vpp::align(req.alignment, align);

	requirements_.push_back(req);
}

void DeviceMemoryAllocator::request(vk::Image requestor, const vk::MemoryRequirements& reqs,
	vk::ImageTiling tiling, MemoryEntry& entry)
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
	req.entry = &entry;

	requirements_.push_back(req);
}

bool DeviceMemoryAllocator::removeRequest(const MemoryEntry& entry)
{
	auto it = findReq(entry);
	if(it == requirements_.end())
	{
#ifndef NDEBUG
		std::cerr << "vpp::DevMemAllocator::removeRequest: could not find entry\n";
#endif
		return false;
	}

	requirements_.erase(it);
	return true;
}

bool DeviceMemoryAllocator::moveEntry(const MemoryEntry& oldOne, MemoryEntry& newOne)
{
	auto req = findReq(oldOne);
	if(req == requirements_.end())
	{
#ifndef NDEBUG
		std::cerr << "vpp::DevMemAllocator::moveRequest: could not find entry\n";
#endif
		return false;
	}

	req->entry = &newOne;
	return true;
}

DeviceMemory* DeviceMemoryAllocator::findMem(Requirement& req)
{
	for(auto& mem : memories_)
	{
		if(!supportsType(req, mem->type())) continue;
		auto allocation = mem->allocatable(req.size, req.alignment, toAllocType(req.type));
		if(allocation.size == 0) continue;

		mem->allocSpecified(allocation.offset, allocation.size, toAllocType(req.type));

		//can be allocated on memory, allocate and bind it
		if(req.type == RequirementType::buffer)
			vk::bindBufferMemory(device(), req.buffer, *mem, allocation.offset);
		else
			vk::bindImageMemory(device(), req.image, *mem, allocation.offset);

		req.entry->allocation_ = allocation;
		req.entry->memory_ = mem.get();
		return mem.get();
	}

	return nullptr;
}

DeviceMemoryAllocator::Requirements::iterator DeviceMemoryAllocator::findReq(const MemoryEntry& entry)
{
	return std::find_if(requirements_.begin(), requirements_.end(), [&](const auto& r)
		{ return r.entry == &entry; });
}

//TODO: all 4 allocate functions can be improved.
void DeviceMemoryAllocator::allocate()
{
	//try to find space for them
	for(auto it = requirements_.begin(); it != requirements_.end();)
	{
		if(findMem(*it)) it = requirements_.erase(it);
		else ++it;
	}

	if(requirements_.empty()) return;

	//allocate remaining types
	const auto& map = queryTypes(); //lifetime extension
	for(auto& type : map) allocate(type.first, type.second);
	requirements_.clear(); //all requirements can be removed
}

bool DeviceMemoryAllocator::allocate(const MemoryEntry& entry)
{
	auto req = findReq(entry);
	if(req == requirements_.end())
	{
#ifndef NDEBUG
		std::cerr << "vpp::DevMemAllocator::allocate: could not find entry\n";
#endif
		return false;
	}

	//this function makes sure the given entry is allocated
	//first of all try to find a free spot in the already existent memories
	if(findMem(*req))
	{
		requirements_.erase(req);
		return true;
	}

	//finding free memory failed, so query the memory type with the most requests and on
	//which the given entry can be allocated and then alloc and bind all reqs for this type
	//XXX: rather use here also queryTypes? could be more efficient seen for the whole amount
	//	allocations that have to be done. At the moment it will choose the type bit that has
	//	the highest amount of stored requirements, but some other type may be better (?) to reduce
	//	the amount of different allocations that have to be done.
	auto type = findBestType(req->memoryTypes);
	allocate(type);

	return true;
}

void DeviceMemoryAllocator::allocate(unsigned int type)
{
	std::vector<Requirement*> reqs;

	for(auto& req : requirements_)
		if(supportsType(req, type)) reqs.push_back(&req);

	allocate(type, reqs);

	//remove allocated reqs
	//TODO: use efficient std algorithm with less resize/moves
	for(auto& req : reqs)
	{
		for(auto it = requirements_.begin(); it != requirements_.end();)
		{
			if(req == &(*it)) it = requirements_.erase(it);
			else ++it;
		}
	}
}

void DeviceMemoryAllocator::allocate(unsigned int type, const Range<Requirement*>& requirements)
{
	auto gran = device().properties().limits.bufferImageGranularity;

	vk::DeviceSize offset = 0;
	bool applyGran = false;

	std::vector<std::pair<Requirement*, unsigned int>> offsets;
	offsets.reserve(requirements.size());

	//iterate through all reqs and place the ones that may be allocated on the given type
	//there. First all linear resources, then all optimal resources.
	for(auto& req : requirements)
	{
		if(req->type == RequirementType::optimalImage)
		{
			applyGran = true;
			continue;
		}

		if(req->alignment) offset = vpp::align(offset, req->alignment);
		offsets.push_back({req, offset});
		offset += req->size;
	}

	//apply granularity if there were already resources placed and there are ones to be placed
	if(offset > 0 && applyGran && gran) offset = vpp::align(offset, gran);

	//now all optimal resources
	for(auto& req : requirements)
	{
		if(req->type != RequirementType::optimalImage) continue;

		if(req->alignment) offset = vpp::align(offset, req->alignment);
		offsets.push_back({req, offset});
		offset += req->size;
	}

	//now the needed size is known and the requirements to be allocated have their offsets
	//the last offset value now equals the needed size
	auto mem = std::make_unique<DeviceMemory>(device(), offset, type);

	//bind and alloc all to be allocated resources
	for(auto& res : offsets)
	{
		auto& req = *res.first;
		auto& offset = res.second;
		auto& entry = *req.entry;

		entry.allocation_ = mem->allocSpecified(offset, req.size, toAllocType(req.type));
		entry.memory_ = mem.get();

		auto isBuffer = (res.first->type == RequirementType::buffer);
		if(isBuffer) vk::bindBufferMemory(device(), res.first->buffer, *mem, offset);
		else vk::bindImageMemory(vkDevice(), res.first->image, *mem, offset);
	}

	memories_.push_back(std::move(mem));
}


spm::map<unsigned int, std::pmr::vector<DeviceMemoryAllocator::Requirement*>>
DeviceMemoryAllocator::queryTypes()
{
	//XXX: probably one of the places where a custom host allocator would really speed things up
	//XXX: probably there is some really easy and trivial algorithm for it... pls find it...

	//this function implements an algorithm to choose the best type for each requirement from
	//its typebits, so that in the end there will be as few allocations as possible needed.

	//vector to return, holds requirements that have a type
	auto& allctr = device().hostMemoryResource();
	spm::map<unsigned int, std::pmr::vector<Requirement*>> ret(&allctr);

	//map holding the current count of the different types
	spm::map<unsigned int, std::pmr::vector<Requirement*>> occurences(&allctr);

	//function to count the occurences of the different memory types and store them in
	//the occurences map
	auto countOccurences = [&]()
	{
		occurences.clear();
		for(auto& req : requirements_)
		{
			for(auto i = 0u; i < 32; ++i)
			{
				if(supportsType(req, i))
				{
					std::pmr::vector<Requirement*> vec({&req}, {&allctr});

					auto it = occurences.find(i);
					if(it != occurences.cend()) it->second.push_back(&req);
					else occurences.emplace(i, std::pmr::vector<Requirement*>{&req});
				}
			}
		}
	};

	//initial occurences count
	countOccurences();

	//while there are requirements left that have not been moved into ret
	while(!occurences.empty())
	{
		//find the least occuring type
		//bestID is after this the memory type with the fewest requirements
		auto best = requirements_.size() + 1; //cant be bigger than that
		std::uint32_t bestID = 0u; //will hold the memory type with the LEAST requirements atm
		for(auto& occ : occurences)
		{
			if(occ.second.size() < best)
			{
				best = occ.second.size();
				bestID = occ.first;
			}
		}

		//function to determine if other types besides the given are supported
		auto othersSupported = [](const Requirement& req, unsigned int type)
		{
			for(auto i = 0u; i < 32; ++i)
				if(i != type && supportsType(req, i)) return true;
			return false;
		};

		//remove the type with the fewest occurences
		//check if any of the reqs that can be allocated on bestID can ONLY be allocated
		//on it. Then there has to be an allocation of type bestID made and all allocations that
		//can be made on this type can be assiocated with it and sorted out.
		//otherwise the type wont be considered for allocations any further.
		bool canBeRemoved = true;
		auto it = occurences.find(bestID);
		for(auto& req : it->second)
			if(!othersSupported(*req, bestID)) canBeRemoved = false;

		//if it can be removed there must be at least one allocation for the given type
		//push all reqs that can be allocated on the type to ret and remove them from occurences
		if(canBeRemoved)
		{
			//remove the bestID type bit from all requirement type bits,
			//to reduce the problems complexity
			for(auto& req : it->second)
				req->memoryTypes &= ~(1 << bestID);

			//erase the occurences for bestID
			//this is the only change to occurences so it does not have to be recounted
			occurences.erase(bestID);
		}
		else
		{
			//set the requirements typebits to 0, indicating that it has a matching type
			//this makes it no longer have any effect on countOccurences which makes sense,
			//since we "removed" it
			//one could alternatively really remove this from the requirements_ vector, but
			//this would be less efficient
			for(auto& req : it->second) req->memoryTypes = 0;

			//insert the requirements that will be allocated on bestID into ret
			ret.emplace(bestID, std::move(it->second));

			//explicitly remove the occurences for the chosen bit.
			//the other references to the now removed since type found requirements stay, but
			//sice their memoryTypes member is 0, they will no longer be counted
			occurences.erase(bestID);

			//the occurences have to be recounted since the occurences for type bits other than
			//best bestID (which will have 0 now) do not count longer (they have a type)
			countOccurences();
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
		case RequirementType::linearImage:
			return AllocationType::linear;
		case RequirementType::optimalImage:
			return AllocationType::optimal;
		default:
			return AllocationType::none;
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
		if(!supportsType(typeBits, i)) continue;
		for(auto& req : requirements_) if(supportsType(req, i)) ++count;

		if(count > best)
		{
			best = count;
			bestID = i;
		}
	}

	return bestID;
}

bool DeviceMemoryAllocator::supportsType(std::uint32_t typeBits, unsigned int type)
{
	return (typeBits & (1 << type));
}

bool DeviceMemoryAllocator::supportsType(const Requirement& req, unsigned int type)
{
	return supportsType(req.memoryTypes, type);
}

}
