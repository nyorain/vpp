// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/devMemAllocator.hpp>
#include <vpp/buffer.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>
#include <algorithm>

namespace vpp {

// DeviceMemoryAllocator
DeviceMemoryAllocator::DeviceMemoryAllocator(const Device& dev) :
	Resource(dev) {
}

DeviceMemoryAllocator::~DeviceMemoryAllocator() {
	dlg_assertlm(dlg_level_warn, requirements_.empty(),
		"~DeviceMemoryAllocator: pending requirements left");
	dlg_assertlm(dlg_level_warn, reservations_.empty(),
		"~DeviceMemoryAllocator: pending reservations left");
}

void DeviceMemoryAllocator::reserve(AllocationType type,
		const vk::MemoryRequirements& reqs, ReservationID* rid) {
	dlg_assertm(reqs.size, "req memory size 0");
	dlg_assertm(reqs.memoryTypeBits, "req no memory type bits");

	auto id = ReservationID {};
	if(rid) {
		if(++lastReservation_ == 0u) { // wrap
			++lastReservation_;
		}

		id = *rid = lastReservation_;
	}

	Requirement req {
		id,
		reqs.size,
		reqs.alignment,
		reqs.memoryTypeBits,
		type,
	};

	requirements_.push_back(req);
}

void DeviceMemoryAllocator::cancel(ReservationID id) noexcept {
	auto req = findReq(id);
	if(req != requirements_.end()) {
		requirements_.erase(req);
		return;
	}

	auto res = findRes(id);
	dlg_assertm(res != reservations_.end(), "Invalid ReservationID: {}", id);
	dlg_assert(res->memory);
	res->memory->free(res->allocation);
	reservations_.erase(res);
}

bool DeviceMemoryAllocator::findMem(Requirement& req, Reservation& info) {
	for(auto& mem : memories_) {
		if(!supportsType(req, mem.type())) {
			continue;
		}

		auto alloc = mem.alloc(req.size, req.alignment, req.type);
		if(alloc.size == 0) {
			continue;
		}

		info.id = req.id;
		info.memory = &mem;
		info.allocation = alloc;

		return true;
	}

	return false;
}

std::vector<DeviceMemoryAllocator::Requirement>::iterator
DeviceMemoryAllocator::findReq(ReservationID id) {
	return std::find_if(requirements_.begin(), requirements_.end(),
		[&](const auto& r) { return r.id == id; });
}

std::vector<DeviceMemoryAllocator::Reservation>::iterator
DeviceMemoryAllocator::findRes(ReservationID id) {
	return std::find_if(reservations_.begin(), reservations_.end(),
		[&](const auto& r) { return r.id == id; });
}

void DeviceMemoryAllocator::alloc() {
	dlg_assertlm(dlg_level_debug, !requirements_.empty(),
		"alloc called without pending requests");

	// try to find space for them
	Reservation res;
	for(auto it = requirements_.begin(); it != requirements_.end();) {
		if(findMem(*it, res)) {
			reservations_.push_back(res);
			it = requirements_.erase(it);
		} else {
			++it;
		}
	}

	// if we could find space for all requirements, there is no more to do
	if(requirements_.empty()) {
		return;
	}

	// otherwise allocate remaining type on new memoeries
	queryTypes();
	auto& typeMap = tmpTypeMap_; // set by queryTypes
	auto& reqs = tmpRequirements_;
	for(auto i = 0u; i < 32; ++i) {
		if(typeMap[i].empty()) {
			continue;
		}

		reqs.clear();
		reqs.reserve(typeMap[i].size());
		for(auto& t : typeMap[i]) {
			reqs.push_back(*t);
		}

		allocate(i, reqs);
	}

	requirements_.clear(); // all requirements were allocated
}

DeviceMemoryAllocator::Allocation
DeviceMemoryAllocator::alloc(ReservationID id) {

	auto rit = findRes(id);
	if(rit != reservations_.end()) {
		auto ret = Allocation{*rit->memory, rit->allocation};
		reservations_.erase(rit);
		return ret;
	}

	auto req = findReq(id);
	dlg_assertm(req != requirements_.end(), "Invalid ReservationID");

	// first of all try to find a free spot in the already existent memories
	Reservation res;
	if(findMem(*req, res)) {
		requirements_.erase(req);
		return {*res.memory, res.allocation};
	}

	// finding free memory failed, so query the memory type with the most
	// requests and on which the given entry can be allocated and then alloc
	// and bind all reqs for this type
	auto type = findBestType(req->memoryTypes);
	allocate(type);

	rit = findRes(id);
	dlg_assert(rit != reservations_.end());
	auto ret = Allocation{*rit->memory, rit->allocation};
	reservations_.erase(rit);
	return ret;
}

DeviceMemoryAllocator::Allocation
DeviceMemoryAllocator::alloc(AllocationType type,
		const vk::MemoryRequirements& memReqs) {
	ReservationID id;
	reserve(type, memReqs, &id);
	return alloc(id);
}

void DeviceMemoryAllocator::allocate(unsigned int type) {
	dlg_assertlm(dlg_level_debug, !requirements_.empty(),
		"allocate called without pending requests for type");

	auto& reqs = tmpRequirements_;
	reqs.clear();
	reqs.reserve(requirements_.size());
	for(auto it = requirements_.begin(); it != requirements_.end();) {
		if(!supportsType(*it, type)) {
			++it;
			continue;
		}

		reqs.push_back(*it);
		it = requirements_.erase(it);
	}

	allocate(type, reqs);
}

void DeviceMemoryAllocator::allocate(unsigned int type,
		nytl::Span<const Requirement> requirements) {
	dlg_assertm(type <= 32, "invalid memory type to allocate");
	dlg_assertm(!requirements.empty(), "empty requirements to allocate");

	auto gran = device().properties().limits.bufferImageGranularity;

	vk::DeviceSize offset = 0;
	bool applyGran = false;

	// holds the reservations we must patch and store after creating
	// the memory object
	auto& reservations = tmpReservations_;
	reservations.clear();
	reservations.reserve(requirements.size());

	// iterate through all reqs and place the ones that may be allocated on the
	// given type there. First all linear resources, then all optimal
	// resources.
	for(auto& req : requirements) {
		if(req.type == AllocationType::optimal) {
			applyGran = true;
			continue;
		}

		if(req.alignment) {
			offset = vpp::align(offset, req.alignment);
		}

		if(req.id) {
			reservations.push_back({req.id, {offset, req.size}, req.type});
		}
		offset += req.size;
	}

	// apply granularity if there were already resources placed and there are
	// ones to be placed
	if(offset > 0 && applyGran && gran) {
		offset = vpp::align(offset, gran);
	}

	// now all optimal resources
	for(auto& req : requirements) {
		if(req.type != AllocationType::optimal) {
			continue;
		}

		if(req.alignment) {
			offset = vpp::align(offset, req.alignment);
		}

		if(req.id) {
			reservations.push_back({req.id, {offset, req.size}, req.type});
		}
		offset += req.size;
	}

	// now the needed size is known and the requirements to be allocated have
	// their offsets the last offset value now equals the needed size
	auto info = vk::MemoryAllocateInfo {offset, type};
	auto& mem = memories_.emplace_back(device(), info);

	// insert reservations where needed
	reservations_.reserve(reservations_.size() + reservations.size());
	for(auto& r : reservations) {
		Reservation res;
		res.id = r.id;
		res.allocation = r.allocation;
		res.memory = &mem;
		reservations_.push_back(res);
		mem.allocSpecified(r.allocation, r.type);
	}
}

void
DeviceMemoryAllocator::queryTypes() {
	// XXX: this implementation does not always return the best result, but the
	//  algorithms complexity is quadratic (in the number of requirements) and
	//  the problem is NP-complete so good enough i guess.

	// this function implements an algorithm to choose the best type for
	// each requirement from its typebits, so that in the end there will
	// be as few allocations as possible needed.

	dlg_assertm(!requirements_.empty(), "queryTypes: no pending requirements");

	// tmpTypeMap_: returned information, holds for each type the requirements
	//   that should be allocated on it
	auto& typeMap = tmpTypeMap_;
	for(auto i = 0u; i < 32; ++i) {
		typeMap[i].clear();
	}

	// tmpOccurences_: map holding the current count of the different types
	auto& occurences = tmpOccurences_; // will be cleared in first countOccurences
	auto numOccurences = 0u; // total number in occurences

	// function to count the occurences of the different memory types and store
	// them in the occurences map
	auto countOccurences = [&]() {
		numOccurences = 0u;
		for(auto i = 0u; i < 32; ++i) {
			occurences[i].clear();
			for(auto& req : requirements_) {
				if(supportsType(req, i)) {
					++numOccurences;
					occurences[i].push_back(&req);
				}
			}
		}
	};

	// initial occurences count
	countOccurences();

	// while there are requirements left that have not been moved into ret
	while(numOccurences > 0) {

		// find the least occuring type
		// bestID is after this the memory type with the fewest requirements
		auto best = requirements_.size() + 1; // cant be bigger than that

		// holds memory type with the least requirements atm, updated in
		// each iteration
		std::uint32_t bestID = 0u;
		for(auto i = 0u; i < 32; ++i) {
			auto count = occurences[i].size();
			if(count > 0 && count < best) {
				best = occurences[i].size();
				bestID = i;
			}
		}

		// function to determine if other types besides the given are supported
		auto othersSupported = [](const Requirement& req, unsigned int type) {
			for(auto i = 0u; i < 32; ++i) {
				if(i != type && supportsType(req, i)) {
					return true;
				}
			}
			return false;
		};

		// remove the type with the fewest occurences
		// check if any of the reqs that can be allocated on bestID can ONLY be
		// allocated on it. Then there has to be an allocation of type bestID
		// made and all allocations that can be made on this type can be
		// assiocated with it and sorted out. otherwise the type wont be
		// considered for allocations any further.
		bool canBeRemoved = true;
		for(auto& req : occurences[bestID]) {
			if(!othersSupported(*req, bestID)) {
				canBeRemoved = false;
				break;
			}
		}

		// if it can be removed there must be at least one allocation for the
		// given type. Push all reqs that can be allocated on the type to ret
		// and remove them from occurences
		if(canBeRemoved) {
			// remove the bestID type bit from all requirement type bits,
			// to reduce the problems complexity
			for(auto& req : occurences[bestID]) {
				req->memoryTypes &= ~(1 << bestID);
			}

			// erase the occurences for bestID
			// this is the only change to occurences so it does not have to be
			// recounted
			numOccurences -= occurences[bestID].size();
			occurences[bestID].clear();
		} else {
			// set the requirements typebits to 0, indicating that it has a
			// matching type this makes it no longer have any effect on
			// countOccurences which makes sense, since we "removed" it.
			// one could alternatively really remove this from the
			// requirements_ vector, but this would be less efficient, the
			// vector will be cleared for all later on.
			for(auto& req : occurences[bestID]) {
				req->memoryTypes = 0;
			}

			// insert the requirements that will be allocated on bestID into ret
			auto& o = occurences[bestID];
			typeMap[bestID].insert(typeMap[bestID].end(), o.begin(), o.end());

			// the occurences have to be recounted since the occurences for type
			// bits other than best bestID (which will have 0 now) do not
			// count longer (they have a type)
			countOccurences();
		}
	}
}

// greedy, simply returns the type with the most requirements.
unsigned int DeviceMemoryAllocator::findBestType(uint32_t typeBits) const {
	dlg_assertm(typeBits != 0, "findBestType with 0 typeBits");

	auto best = 0;
	auto bestID = 0;

	// init count to with one, so even if there are no matching reqs at all,
	// at least a supported type bit is returned.
	for(auto i = 0u; i < 32; ++i) {
		auto count = 1;
		if(!supportsType(typeBits, i)) {
			continue;
		}

		for(auto& req : requirements_) {
			if(supportsType(req, i)) {
				++count;
			}
		}

		if(count > best) {
			best = count;
			bestID = i;
		}
	}

	return bestID;
}

bool DeviceMemoryAllocator::supportsType(uint32_t typeBits,
		unsigned int type) noexcept {
	return (typeBits & (1 << type));
}

bool DeviceMemoryAllocator::supportsType(const Requirement& req,
		unsigned int type) noexcept {
	return supportsType(req.memoryTypes, type);
}

} // namespace vpp
