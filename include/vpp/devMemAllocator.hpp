// Copyright (c) 2016-2020 Jan Kelling
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
class VPP_API DeviceMemoryAllocator : public Resource {
public:
	using ReservationID = vk::DeviceSize;

	/// Allocated span on a device memory object.
	struct Allocation {
		DeviceMemory& memory;
		DeviceMemory::Allocation allocation;
	};

public:
	DeviceMemoryAllocator() = default;
	DeviceMemoryAllocator(const Device& dev);
	~DeviceMemoryAllocator();

	DeviceMemoryAllocator(DeviceMemoryAllocator&&) = delete;
	DeviceMemoryAllocator& operator=(DeviceMemoryAllocator) = delete;

	void init(const Device& dev);

	/// Requests memory for the given memory resource type.
	/// If an id is given, it can be later used to allocate that memory
	/// or cancel that reservation again (one of the two *must* happen
	/// if id is not nullptr since the memory will be reserved for the
	/// id to retrieve later on).
	/// When this is called without 'id' it just serves as general
	/// over-allocation hint the next time memory is allocated,
	/// and does not actually reserve memory.
	/// - reqs: The MemoryRequirements queried for the given resource.
	///   Might have been modified by the caller to fit additional requirements
	///   (such as selected a preferred memory type)
	[[nodiscard]] ReservationID reserve(AllocationType, const vk::MemoryRequirements&);

	/// Cancels a pending reservation.
	void cancel(ReservationID) noexcept;

	/// Allocates memory for the given reservation id.
	/// The reservation must come from this Allocator object and must not
	/// have been cancelled.
	[[nodiscard]] Allocation alloc(ReservationID);
	[[nodiscard]] Allocation alloc(AllocationType, const vk::MemoryRequirements&);

	/// Allocates and associated device memory for all pending requests.
	void alloc();

	/// == A lot of things can wrong here, use with caution ==
	/// Tries to move the given device memory allocator into this one,
	/// effectively merging them. This is not only possible when
	/// 'rhs' has no pending reservations/requirements (i.e. no resource still in
	/// initialization referencing it). Returns false if this
	/// is not the case and will not modify rhs in that case.
	/// If the operation can be done, returns false and will effectively
	/// empty/reset rhs.
	/// If 'this' was already initialized, 'this' and 'rhs' must both
	/// use the same vpp::Device. Will not modify the memory types
	/// this allocator is restricted to. Note that if rhs was restricted
	/// to less memory types than this allocator is, this allocator
	/// will gain memories not matching its restriction and might allocate
	/// on them in future.
	/// Can be used as conditional move operator, i.e. works even if
	/// 'this' is uninitialized.
	[[nodiscard]] bool tryMergeMemories(DeviceMemoryAllocator&& rhs);

	/// Restricts allocation to the given type bits.
	/// Note that if this is too restrictive, certain memory resources
	/// might be able to allocate memory anymore using this allocator
	/// (when the type bits they can be allocated on has not types
	/// in common with the given ones).
	/// Will not change the previously allocatoed memories, just
	/// future allocations.
	void restrict(std::uint32_t memoryTypeBits);

	/// Returns all memories that this allocator manages.
	const auto& memories() const { return memories_; }
	const auto& reservations() const { return reservations_; }
	const auto& requirements() const { return requirements_; }
	std::uint32_t restricted() const { return restrict_; }

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
	void alloc(unsigned int type);
	void alloc(unsigned int type, nytl::Span<const Requirement> reqs);
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
	std::uint32_t restrict_ {0xFFFFFFFF};

	std::vector<Reservation> reservations_;
	std::vector<Requirement> requirements_;

	// all owned memory objects
	// we use unique_ptrs since DeviceMemory objects can't be moved.
	// std::deque wouldn't allow the tryMerge operation
	std::vector<std::unique_ptr<DeviceMemory>> memories_;
	ReservationID lastReservation_ {}; // for id counting

	// cache for algorithms
	std::vector<Requirement> tmpRequirements_;
	std::vector<PendingReservation> tmpReservations_;
	std::array<std::vector<Requirement*>, 32> tmpOccurences_;
	std::array<std::vector<Requirement*>, 32> tmpTypeMap_;
};

} // namespace vpp
