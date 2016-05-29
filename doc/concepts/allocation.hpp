#pragma once

#include <memory>
#include <vector>

namespace vpp
{

///Utility struct that represents an allocated range (offset + size).
struct Allocation
{
	std::size_t offset {0};
	std::size_t size {0};

	std::size_t end() const { return offset + size; }
};

///Abstracts the Placement of many "allocations" on a given range.
///Can be used e.g. to implement a custom memory allocator or to generally divide a buffer
///into smaller parts that can be used seperatly.
class AllocationManager
{
public:
	AllocationManager(std::size_t size) : size_(size) {}
	~AllocationManager();

	///Tries to allocate a memory part that matches the given size and aligment requirements.
	///If there is not enough free space left, a std::runtime_error will be thrown.
	///One can test if there is enough space for the needed allocation with the
	///allocatable() member function.
	std::size_t alloc(std::size_t size, std::size_t alignment);

	///Tries to reallocate the given allocation (position) to a bigger or smaller size.
	///If the given position is NOT a valid allocation, a std::logic_error will be thrown.
	///If the allocation is valid but cannot be resized, a std::runtime_error will be thrown.
	void realloc(std::size_t position, std::size_t newSize);

	///Frees the given allocation.
	///Will return false if the given allocation (position) is not valid.
	bool free(std::size_t position);

	///Tests if an allocation for the given requirements can be made.
	///Will return an empty (size = 0) allocation if it is not possible or the theoretically
	///possible alloction. This can then (before any other allocate call) safely be allocated
	///with a call to allocSpecified.
	///Notice that this call itself does NOT reserve any memory for the caller so the memory
	///range of the returned allocatin shall not be used.
	Allocation allocatable(std::size_t size, std::size_t alignment) const;

	///Allocates the specified memory part. Does not check for matched requirements or if
	///the specified space is already occupied, so this function have to be used with care.
	///This function can be useful if the possibility of a given allocation was checked before
	///with a call to the allocatable function (than the returned range can safely be allocated)
	///with this function. It might also be useful if one wants to manage the memory reservation
	///itself externally and can therefore assure that the given range can be allocated.
	std::size_t allocSpecified(const Allocation& allocation);

	///Returns the total size this DeviceMemory object has.
	std::size_t size() const { return size_; }

	///Returns the the biggest (continuously) allocatable block.
	///This does not mean that an allocation of this size can be made, since there are also
	///alignment requirements which may effectively "shrink" this block.
	std::size_t biggestBlock() const;

	///Returns the total amount of free bytes.
	std::size_t toalFree() const;

	///Returns the amount of allocations made.
	std::size_t allocationCount() const;

protected:
	std::size_t size_;
	std::vector<Allocation> allocations_;
};

}
