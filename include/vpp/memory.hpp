// Copyright (c) 2016-2018 nyorain
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

	// TODO (?)
	// sparse = 4,
	// sparseAlias = 8
};

/// RAII wrapper around vk::DeviceMemory.
/// Does not carry/manage any additional stuff, see vpp::DeviceMemory.
class DeviceMemoryHandle : public ResourceHandle<vk::DeviceMemory> {
public:
	DeviceMemoryHandle() = default;
	DeviceMemoryHandle(const Device&, const vk::MemoryAllocateInfo&);
	DeviceMemoryHandle(const Device&, vk::DeviceMemory);
	~DeviceMemoryHandle();

	DeviceMemoryHandle(DeviceMemoryHandle&& rhs) { swap(*this, rhs); }
	DeviceMemoryHandle& operator=(DeviceMemoryHandle rhs) { 
		swap(*this, rhs); 
		return *this;
	}
};

/// DeviceMemory class that keeps track of its allocated and freed areas.
/// Makes it easy to reuse memory as well as bind multiple memoryRequestors to one allocation.
/// Note that there are additional rules for allocating device memory on vulkan (like e.g. needed
/// offsets between image and buffer allocations) which are not checked/stored by this class, this
/// has to be done externally.
class DeviceMemory : public DeviceMemoryHandle {
public:
	using Allocation = BasicAllocation<vk::DeviceSize>;
	struct AllocationEntry {
		Allocation allocation;
		AllocationType type;
	};

public:
	DeviceMemory(const Device&, const vk::MemoryAllocateInfo&);
	DeviceMemory(const Device&, vk::DeviceMemory, vk::DeviceSize size, 
		unsigned int memoryType);
	~DeviceMemory();

	/// DeviceMemory is NonMovable to allow memory resources to keep references
	/// to the DeviceMemory object (e.g. to free it on destruction).
	DeviceMemory(DeviceMemory&&) noexcept = delete;
	DeviceMemory& operator=(DeviceMemory&&) noexcept = delete;

	/// Tries to allocate a memory part that matches the given size and 
	/// alignment requirements. Will return an empty (size == 0) allocation
	/// if there is not enough free space left.
	/// One can test if there is enough space for the needed allocation with the
	/// allocatable() member function (but then prefer to use
	/// allocSpecified for better performance).
	/// Alignment must be a power of 2, size not 0 and the given AllocationType
	/// valid must represent the type the allocation will be used for.
	Allocation alloc(vk::DeviceSize size, vk::DeviceSize aligment, 
		AllocationType type);

	/// Allocates the specified memory part. 
	/// Does not check for matched requirements or if the specified space 
	/// is already occupied, so this function has to be used with care.
	/// This function can be useful if the possibility of a given allocation 
	/// was checked before with a call to the allocatable function (then 
	/// the returned range can safely be allocated if no other allocation
	/// was made in between).
	/// It might also be useful if one wants to partly manage the memory 
	/// reservation itself externally and can therefore ensure that 
	/// the given range can be allocated (like e.g. the first allocation).
	void allocSpecified(Allocation, AllocationType);

	/// Frees the given allocation. Undefined behavior if the
	/// allocation is invalid (i.e. not allocated on this Memory).
	void free(const Allocation& alloc);

	/// Tests if an allocation for the given requirements can be made.
	/// Will return an empty (size = 0) allocation if it is not possible.
	/// Otherwise returns the allocation which could be made, which could then 
	/// (before any other allocate call) safely be allocated with a call to 
	/// allocSpecified. The call itself does not change the state of this
	/// object, i.e. does not NOT reserve memory or something like it.
	/// Alignment must be a power of 2, size not 0 and the given AllocationType
	/// valid must represent the type the allocation would be used for.
	/// Usually this function is not needed.
	Allocation allocatable(vk::DeviceSize size, vk::DeviceSize aligment, 
		AllocationType) const;

	/// Maps the specified memory range.
	/// Undefined behavior if the memory is not mappable, the caller must
	/// check this using mappable()
	MemoryMapView map(const Allocation& allocation);

	/// Returns the the biggest (continuously) allocatable block.
	/// This does not mean that an allocation of this size can be made, 
	/// since there are also alignment or granularity requirements which 
	/// will effectively "shrink" this block.
	vk::DeviceSize largestFreeSegment() const noexcept;

	/// Returns the total amount of free bytes.
	vk::DeviceSize totalFree() const noexcept;

	/// Returns the total size this DeviceMemory object has.
	vk::DeviceSize size() const noexcept { return size_; }

	/// Returns the currently mapped range, or nullptr if there is none.
	MemoryMap* mapped() noexcept;
	const MemoryMap* mapped() const noexcept;

	vk::MemoryPropertyFlags properties() const noexcept;
	bool mappable() const noexcept;

	unsigned int type() const noexcept { return type_; }
	const auto& allocations() const noexcept { return allocations_; }

protected:
	std::vector<AllocationEntry> allocations_ {};
	vk::DeviceSize size_ {};
	unsigned int type_ {};
	MemoryMap memoryMap_ {}; // the current memory map, may be invalid
};

} // namespace vpp
