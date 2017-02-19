// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp> // vpp::ResourceHandle
#include <vpp/memoryMap.hpp> // vpp::MemoryMap
#include <vpp/util/allocation.hpp> // vpp::Allocation
#include <vector> // std::vector

namespace vpp {

/// Specifies the different types of allocation on a memory object.
enum class AllocationType {
	none = 0,
	linear = 1,
	optimal = 2,
	sparse = 4,
	sparseAlias = 8
};

/// DeviceMemory class that keeps track of its allocated and freed areas.
/// Makes it easy to reuse memory as well as bind multiple memoryRequestors to one allocation.
/// Note that there are additional rules for allocating device memory on vulkan (like e.g. needed
/// offsets between image and buffer allocations) which are not checked/stored by this class, this
/// has to be done externally.
class DeviceMemory : public ResourceHandle<vk::DeviceMemory> {
public:
	struct AllocationEntry {
		Allocation allocation;
		AllocationType type;
	};

public:
	DeviceMemory() = default;
	DeviceMemory(const Device&, const vk::MemoryAllocateInfo&);
	DeviceMemory(const Device&, uint32_t size, uint32_t typeIndex);
	DeviceMemory(const Device&, uint32_t size, vk::MemoryPropertyFlags);
	~DeviceMemory();

	/// DeviceMemory is NonMovable since all memory resources will keep references to
	/// their memory objects. Therfore its location has not to change.
	DeviceMemory(DeviceMemory&&) noexcept = delete;
	DeviceMemory& operator=(DeviceMemory&&) noexcept = delete;

	/// Tries to allocate a memory part that matches the given size and alignment requirements.
	/// If there is not enough free space left, a std::runtime_error will be thrown.
	/// The size parameter has to be not null, otherwise a std::runtime_error will be thrown.
	/// One can test if there is enough space for the needed allocation with the
	/// allocatable() member function.
	/// Alignment must be a power of 2.
	Allocation alloc(size_t size, size_t aligment, AllocationType type);

	/// Tests if an allocation for the given requirements can be made.
	/// Will return an empty (size = 0) allocation if it is not possible or the theoretically
	/// possible allocation. This can then (before any other allocate call) safely be allocated
	/// with a call to allocSpecified.
	/// Notice that this call itself does NOT reserve any memory for the caller so the memory
	/// range of the returned allocation shall not be used.
	/// Alignment must be a power of 2.
	Allocation allocatable(size_t size, size_t aligment, AllocationType) const;

	/// Allocates the specified memory part. Does not check for matched requirements or if
	/// the specified space is already occupied, so this function have to be used with care.
	/// This function can be useful if the possibility of a given allocation was checked before
	/// with a call to the allocatable function (than the returned range can safely be allocated)
	/// with this function. It might also be useful if one wants to manage the memory reservation
	/// itself externally and can therefore assure that the given range can be allocated.
	Allocation allocSpecified(size_t offset, size_t size, AllocationType);

	/// Frees the given allocation. Will throw a std::logic_error if the given allocation is not
	/// part of this Memory object.
	void free(const Allocation& alloc);

	/// Returns the the biggest (continuously) allocatable block.
	/// This does not mean that an allocation of this size can be made, since there are also
	/// alignment or granularity requirements which will effectively "shrink" this block.
	size_t largestFreeSegment() const noexcept;

	/// Returns the total amount of free bytes.
	size_t totalFree() const noexcept;

	/// Returns the total size this DeviceMemory object has.
	size_t size() const noexcept;

	/// Returns the currently mapped range, or nullptr if there is none.
	MemoryMap* mapped() noexcept { return (memoryMap_.ptr()) ? &memoryMap_ : nullptr; }

	/// Returns the currently mapped range, or nullptr if there is none.
	const MemoryMap* mapped() const noexcept { return (memoryMap_.ptr()) ? &memoryMap_ : nullptr; }

	/// Maps the specified memory range.
	/// Will throw a std::logic_error if this memory is not mappeble.
	MemoryMapView map(const Allocation& allocation);

	vk::MemoryPropertyFlags properties() const noexcept;
	bool mappable() const noexcept;

	unsigned int type() const noexcept { return type_; }
	const std::vector<AllocationEntry>& allocations() const noexcept { return allocations_; }

protected:
	std::vector<AllocationEntry> allocations_ {};
	size_t size_ {};
	unsigned int type_ {};
	MemoryMap memoryMap_ {}; // the current memory map, may be invalid
};

} // namespace vpp
