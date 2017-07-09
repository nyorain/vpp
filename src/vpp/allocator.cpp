// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/allocator.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/log.hpp>
#include <algorithm>

namespace vpp {

// DeviceMemoryAllocator
DeviceMemoryAllocator::DeviceMemoryAllocator(const Device& dev) : Resource(dev)
{
}

DeviceMemoryAllocator::~DeviceMemoryAllocator()
{
	dlg_check("~DeviceMemoryAllocator", {
		if(!requirements_.empty())
			vpp_warn("There are requirements left");
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

	swap(static_cast<Resource&>(a), static_cast<Resource&>(b));
	swap(a.requirements_, b.requirements_);
	swap(a.memories_, b.memories_);
}

void DeviceMemoryAllocator::request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
	vk::BufferUsageFlags usage, MemoryEntry& entry)
{
	dlg_check("DeviceMemoryAllocator::request(buffer)", {
		if(!requestor) vpp_error("buffer is nullHandle");
		if(!reqs.size) vpp_error("allocation size 0");
		if(!reqs.memoryTypeBits) vpp_error("no memory type bits");
	});

	entry = {};
	entry.allocator_ = this;

	Requirement req;
	req.type = RequirementType::buffer;
	req.size = reqs.size;
	req.alignment = reqs.alignment;
	req.memoryTypes = reqs.memoryTypeBits;
	req.buffer = requestor;
	req.entry = &entry;

	// apply additional device limits alignments
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
	dlg_check("vpp::DeviceMemoryAllocator::request(image)", {
		if(!requestor) vpp_error("image is nullHandle");
		if(!reqs.size) vpp_error("allocation size 0");
		if(!reqs.memoryTypeBits) vpp_error("no memory type bits");
		if(tiling != vk::ImageTiling::linear && tiling != vk::ImageTiling::optimal)
			vpp_error("invalid image tiling");
	});

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

void DeviceMemoryAllocator::removeRequest(const MemoryEntry& entry) noexcept
{
	auto req = findReq(entry);
	dlg_check("DeviceMemoryAllocator::removeRequest", {
		if(req == requirements_.end()) vpp_error("could not find entry");
	});

	requirements_.erase(req);
}

void DeviceMemoryAllocator::moveEntry(const MemoryEntry& oldOne, MemoryEntry& newOne) noexcept
{
	auto req = findReq(oldOne);
	dlg_check("DeviceMemoryAllocator::moveEntry", {
		if(req == requirements_.end()) vpp_error("could not find old entry");
		if(newOne.allocated()) vpp_warn("new entry is already allocated");
		if(newOne.allocator() != this) vpp_warn("new entry has invalid allocator");
	});

	req->entry = &newOne;
}

DeviceMemory* DeviceMemoryAllocator::findMem(Requirement& req)
{
	for(auto& mem : memories_) {
		if(!supportsType(req, mem->type())) continue;

		auto allocation = mem->allocatable(req.size, req.alignment, toAllocType(req.type));
		if(allocation.size == 0) continue;

		// can be allocated on memory, allocate and bind it
		mem->allocSpecified(allocation.offset, allocation.size, toAllocType(req.type));
		if(req.type == RequirementType::buffer) {
			vk::bindBufferMemory(device(), req.buffer, *mem, allocation.offset);
		} else {
			vk::bindImageMemory(device(), req.image, *mem, allocation.offset);
		}

		req.entry->allocation_ = allocation;
		req.entry->memory_ = mem.get();
		return mem.get();
	}

	return nullptr;
}

DeviceMemoryAllocator::Requirements::iterator
DeviceMemoryAllocator::findReq(const MemoryEntry& entry)
{
	return std::find_if(requirements_.begin(), requirements_.end(),
		[&](const auto& r) { return r.entry == &entry; });
}

void DeviceMemoryAllocator::allocate()
{
	dlg_check("DeviceMemoryAllocator::allocate()", {
		if(requirements_.empty()) vpp_warn("there are no pending requests");
	});

	// try to find space for them
	auto it = requirements_.begin();
	while(it != requirements_.end()) {
		if(findMem(*it)) it = requirements_.erase(it);
		else ++it;
	}

	// if we could find space for all requirements, there is no more to do
	if(requirements_.empty()) return;

	// otherwise allocate remaining type on new memoeries
	const auto& map = queryTypes();
	for(auto& type : map) allocate(type.first, type.second);
	requirements_.clear(); // all requirements can be removed
}

void DeviceMemoryAllocator::allocate(const MemoryEntry& entry)
{
	auto req = findReq(entry);

	dlg_check("vpp::DeviceMemoryAllocator::allocate(entry)", {
		if(req == requirements_.end()) vpp_error("could not find entry");
		if(entry.allocated()) vpp_warn("entry already allocated");
		if(entry.allocator() != this) vpp_warn("invalid entry allocator");
	});

	// this function makes sure the given entry is allocated
	// first of all try to find a free spot in the already existent memories
	if(findMem(*req)) {
		requirements_.erase(req);
		return;
	}

	// finding free memory failed, so query the memory type with the most requests and on
	// which the given entry can be allocated and then alloc and bind all reqs for this type
	auto type = findBestType(req->memoryTypes);
	allocate(type);
}

void DeviceMemoryAllocator::allocate(unsigned int type)
{
	std::vector<Requirement*> reqs;

	for(auto& req : requirements_)
		if(supportsType(req, type))
			reqs.push_back(&req);

	dlg_check("DeviceMemoryAllocator::allocate(type)", {
		if(requirements_.empty()) vpp_warn("there are no reqsests for type");
	});

	allocate(type, reqs);

	// remove allocated reqs
	auto contained = [&](Requirement& req){
		for(auto& r : reqs) if(r == &req) return true;
		return false;
	};

	auto newEnd = std::remove_if(requirements_.begin(), requirements_.end(), contained);
	requirements_.erase(newEnd, requirements_.end());
}

void DeviceMemoryAllocator::allocate(unsigned int type,
	nytl::Span<Requirement* const> requirements)
{
	dlg_check("DeviceMemoryAllocator::allocate(type, reqs)", {
		if(requirements.empty()) vpp_warn("empty reqs span passed");
		if(type > 32) vpp_warn("invalid memory type");
	});

	auto gran = device().properties().limits.bufferImageGranularity;

	vk::DeviceSize offset = 0;
	bool applyGran = false;

	std::vector<std::pair<Requirement*, unsigned int>> offsets;
	offsets.reserve(requirements.size());

	// iterate through all reqs and place the ones that may be allocated on the given type
	// there. First all linear resources, then all optimal resources.
	for(auto& req : requirements) {
		if(req->type == RequirementType::optimalImage) {
			applyGran = true;
			continue;
		}

		if(req->alignment) offset = vpp::align(offset, req->alignment);
		offsets.push_back({req, offset});
		offset += req->size;
	}

	// apply granularity if there were already resources placed and there are ones to be placed
	if(offset > 0 && applyGran && gran)
		offset = vpp::align(offset, gran);

	// now all optimal resources
	for(auto& req : requirements) {
		if(req->type != RequirementType::optimalImage) continue;

		if(req->alignment) offset = vpp::align(offset, req->alignment);
		offsets.push_back({req, offset});
		offset += req->size;
	}

	// now the needed size is known and the requirements to be allocated have their offsets
	// the last offset value now equals the needed size
	auto mem = std::make_unique<DeviceMemory>(device(), offset, type);

	// bind and alloc all to be allocated resources
	for(auto& res : offsets) {
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


std::unordered_map<unsigned int, std::vector<DeviceMemoryAllocator::Requirement*>>
DeviceMemoryAllocator::queryTypes()
{
	// XXX: probably one of the places where a custom host allocator would really speed things up
	// XXX: probably this can be done in an easier way. This algorithm is rather complex

	// this function implements an algorithm to choose the best type for each requirement from
	// its typebits, so that in the end there will be as few allocations as possible needed.

	dlg_check("DeviceMemoryAllocator::queryTypes", {
		if(requirements_.empty()) vpp_warn("there are no pending requests");
	});

	// vector to return, holds requirements that have a type
	std::unordered_map<unsigned int, std::vector<Requirement*>> ret;

	// map holding the current count of the different types
	std::unordered_map<unsigned int, std::vector<Requirement*>> occurences;

	// function to count the occurences of the different memory types and store them in
	// the occurences map
	auto countOccurences = [&]() {
		occurences.clear();
		for(auto& req : requirements_) {
			for(auto i = 0u; i < 32; ++i) {
				if(supportsType(req, i)) {
					std::vector<Requirement*> vec({&req});
					auto it = occurences.find(i);
					if(it != occurences.cend()) it->second.push_back(&req);
					else occurences.emplace(i, std::vector<Requirement*>{&req});
				}
			}
		}
	};

	// initial occurences count
	countOccurences();

	// while there are requirements left that have not been moved into ret
	while(!occurences.empty()) {

		// find the least occuring type
		// bestID is after this the memory type with the fewest requirements
		auto best = requirements_.size() + 1; // cant be bigger than that
		std::uint32_t bestID = 0u; // will hold the memory type with the LEAST requirements atm
		for(auto& occ : occurences) {
			if(occ.second.size() < best) {
				best = occ.second.size();
				bestID = occ.first;
			}
		}

		// function to determine if other types besides the given are supported
		auto othersSupported = [](const Requirement& req, unsigned int type) {
			for(auto i = 0u; i < 32; ++i)
				if(i != type && supportsType(req, i)) return true;
			return false;
		};

		// remove the type with the fewest occurences
		// check if any of the reqs that can be allocated on bestID can ONLY be allocated
		// on it. Then there has to be an allocation of type bestID made and all allocations that
		// can be made on this type can be assiocated with it and sorted out.
		// otherwise the type wont be considered for allocations any further.
		bool canBeRemoved = true;
		auto it = occurences.find(bestID);
		for(auto& req : it->second)
			if(!othersSupported(*req, bestID)) canBeRemoved = false;

		// if it can be removed there must be at least one allocation for the given type
		// push all reqs that can be allocated on the type to ret and remove them from occurences
		if(canBeRemoved) {
			//remove the bestID type bit from all requirement type bits,
			//to reduce the problems complexity
			for(auto& req : it->second)
				req->memoryTypes &= ~(1 << bestID);

			//erase the occurences for bestID
			//this is the only change to occurences so it does not have to be recounted
			occurences.erase(bestID);
		} else {
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

AllocationType DeviceMemoryAllocator::toAllocType(RequirementType type) noexcept
{
	switch(type) {
		case RequirementType::buffer:
		case RequirementType::linearImage: return AllocationType::linear;
		case RequirementType::optimalImage: return AllocationType::optimal;
		default: return AllocationType::none;
	}
}

unsigned int DeviceMemoryAllocator::findBestType(uint32_t typeBits) const
{
	dlg_check("vpp::DeviceMemoryAllocator::findBestType", {
		if(typeBits == 0) vpp_error("typeBits == 0");
	});

	auto best = 0;
	auto bestID = 0;

	// init count to with one, so even if there are no matching reqs at all,
	// at least a supported type bit is returned.
	for(auto i = 0u; i < 32; ++i) {
		auto count = 1;
		if(!supportsType(typeBits, i)) continue;
		for(auto& req : requirements_) if(supportsType(req, i)) ++count;

		if(count > best) {
			best = count;
			bestID = i;
		}
	}

	return bestID;
}

bool DeviceMemoryAllocator::supportsType(uint32_t typeBits, unsigned int type) noexcept
{
	return (typeBits & (1 << type));
}

bool DeviceMemoryAllocator::supportsType(const Requirement& req, unsigned int type) noexcept
{
	return supportsType(req.memoryTypes, type);
}

// MemoryEntry
MemoryEntry::MemoryEntry(DeviceMemory& memory, const Allocation& alloc)
	: memory_(&memory), allocation_(alloc)
{
	dlg_check("vpp::MemoryEntry::MemoryEntry", {
		if(alloc.size == 0) vpp_error("Invalid allocation parameter");
	});
}

MemoryEntry::MemoryEntry(MemoryEntry&& other) noexcept
{
	if(other.allocated()) {
		memory_ = other.memory_;
		other.memory_ = nullptr;
	} else if(other.allocator_) {
		allocator_ = other.allocator_;
		other.allocator_ = nullptr;
		allocator_->moveEntry(other, *this);
	}

	allocation_ = other.allocation_;
	other.allocation_ = {};
}

MemoryEntry& MemoryEntry::operator=(MemoryEntry&& other) noexcept
{
	// destroy
	if(!allocated() && allocator_) allocator_->removeRequest(*this);
	else if(allocated()) memory_->free(allocation_);

	// move
	if(other.allocated()) {
		memory_ = other.memory_;
		other.memory_ = nullptr;
	} else if(other.allocator_) {
		allocator_ = other.allocator_;
		other.allocator_ = nullptr;
		allocator_->moveEntry(other, *this);
	}

	allocation_ = other.allocation_;
	other.allocation_ = {};

	return *this;
}

MemoryEntry::~MemoryEntry()
{
	if(!allocated() && allocator_) allocator_->removeRequest(*this);
	else if(allocated()) memory_->free(allocation_);
}

MemoryMapView MemoryEntry::map() const
{
	auto mem = memory();
	if(!mem) throw std::logic_error("vpp::MemoryEntry::map: entry not bound to memory");
	return mem->map(allocation());
}

} // namespace vpp
