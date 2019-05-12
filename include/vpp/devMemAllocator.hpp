// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp> // vpp::Resource
#include <vpp/memory.hpp> // vpp::DeviceMemory
#include <vpp/util/span.hpp> // nytl::Span
#include <vpp/util/allocation.hpp> // vpp::Allocation

#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include <deque> // std::deque
#include <unordered_map> // std::unordered_map
#include <variant> // std::variant

namespace vpp {

/// Groups together multiple memory resources on as few DeviceMemory objects
/// as possible. Can be used manually but usually used implicitly via
/// vpp::Buffer and vpp::Image.
/// Can't be moved since memory resources might reference it during
/// deferred initialization.
class DeviceMemoryAllocator : public Resource {
public:
	using ReservationID = vk::DeviceSize;

	/// Allocated span on a device memory object.
	struct Allocation {
		DeviceMemory& memory;
		DeviceMemory::Allocation allocation;
	};

public:
	DeviceMemoryAllocator(const Device& dev);
	~DeviceMemoryAllocator();

	DeviceMemoryAllocator(DeviceMemoryAllocator&&) = delete;
	DeviceMemoryAllocator& operator=(DeviceMemoryAllocator) = delete;

	/// Requests memory for the given memory resource type.
	/// If an id is given, it can be later used to allocate that memory
	/// or cancel that reservation again (one of the two *must* happen
	/// otherwise the reserved memory might be wasted).
	/// - reqs: The MemoryRequirements queried for the given resource.
	///   Might have been modified by the caller to fit additional requirements
	///   (such as selected a preferred memory type)
	void reserve(AllocationType, const vk::MemoryRequirements&,
		ReservationID* id = nullptr);

	/// Cancels a pending reservation.
	void cancel(ReservationID) noexcept;

	/// Allocates memory for the given reservation id.
	/// The reservation must come from this Allocator object and must not
	/// have been cancelled.
	Allocation alloc(ReservationID);
	Allocation alloc(AllocationType, const vk::MemoryRequirements&);

	/// Allocates and associated device memory for all pending requests.
	void alloc();

	/// Returns all memories that this allocator manages.
	const auto& memories() const { return memories_; }

protected:
	/// Represents the requirements of a pending memory request.
	/// Holds all needed information for allocating memory for the
	/// associated resource.
	struct Requirement {
		ReservationID id {};
		vk::DeviceSize size {};
		vk::DeviceSize alignment {};
		std::uint32_t memoryTypes {};
		AllocationType type {};
	};

	struct Reservation {
		ReservationID id {};
		DeviceMemory* memory {};
		DeviceMemory::Allocation allocation {};
	};

protected:
	// utility global functions
	static bool supportsType(const Requirement&, unsigned int type) noexcept;
	static bool supportsType(uint32_t bits, unsigned int type) noexcept;

	// utility allocation functions
	void allocate(unsigned int type);
	void allocate(unsigned int type, nytl::Span<const Requirement> reqs);
	bool findMem(Requirement& req, Reservation& info);
	std::vector<Requirement>::iterator findReq(ReservationID);
	std::vector<Reservation>::iterator findRes(ReservationID);
	void queryTypes(); // returned in tmpTypeMap_
	unsigned int findBestType(uint32_t typeBits) const;

	// needed in alloc algorithm, cached vector of it below
	struct PendingReservation {
		ReservationID id;
		DeviceMemory::Allocation allocation;
		AllocationType type;
	};

protected:
	std::vector<Reservation> reservations_;
	std::vector<Requirement> requirements_;
	std::deque<DeviceMemory> memories_; // list of owned memory objects
	ReservationID lastReservation_ {};

	// cache for algorithms
	std::vector<Requirement> tmpRequirements_;
	std::vector<PendingReservation> tmpReservations_;
	std::array<std::vector<Requirement*>, 32> tmpOccurences_;
	std::array<std::vector<Requirement*>, 32> tmpTypeMap_;
};

} // namespace vpp
