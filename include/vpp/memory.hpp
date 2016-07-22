#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/utility/allocation.hpp>

#include <memory>
#include <map>
#include <vector>
#include <vector>

namespace vpp
{

///Represents a mapped range of a vulkan DeviceMemory.
///There shall never be more than one MemoryMap object for on DeviceMemory object.
///The MemoryMap class is usually never used directly, but rather accessed through a
///MemoryMapView.
class MemoryMap : public ResourceReference<MemoryMap>
{
public:
	MemoryMap()  = default;
	MemoryMap(const DeviceMemory& memory, const Allocation& alloc);
	~MemoryMap();

	MemoryMap(MemoryMap&& other) noexcept;
	MemoryMap& operator=(MemoryMap other) noexcept;

	///Might remaps the mapped range to assure it also includes the given allocation.
	void remap(const Allocation& allocation);

	///Makes sure the mapped data is visibile on the device.
	///If memory is coherent, this function will have no effect.
	void flush() const;

	///Reloads the device memory into mapped memory, i.e. makes sure writes by the device
	///are made visible.
	///If the memory is coherent, this function will have no effect.
	void reload() const;

	const vk::DeviceMemory& vkMemory() const;
	const Allocation& allocation() const { return allocation_; }
	std::size_t offset() const { return allocation().offset; }
	std::size_t size() const { return allocation().size; }
	std::uint8_t* ptr() const { return static_cast<std::uint8_t*>(ptr_); }
	const DeviceMemory& memory() const { return *memory_; }
	bool coherent() const;

	vk::MappedMemoryRange mappedMemoryRange() const;

	const DeviceMemory& resourceRef() const { return *memory_; }
	friend void swap(MemoryMap& a, MemoryMap& b) noexcept;

protected:
	friend class MemoryMapView;

	void ref();
	void unref();
	void unmap();

protected:
	const DeviceMemory* memory_ {nullptr};
	Allocation allocation_ {};
	std::size_t views_ {};
	void* ptr_ {nullptr};
};


///A view into a mapped memory range.
///Makes it possible to write/read from multiple allocations on a mapped memory.
class MemoryMapView : public ResourceReference<MemoryMapView>
{
public:
	MemoryMapView() = default;
	MemoryMapView(MemoryMap& map, const Allocation& range);
	~MemoryMapView();

	MemoryMapView(MemoryMapView&& other) noexcept;
	MemoryMapView& operator=(MemoryMapView other) noexcept;

	///Makes sure the mapped data is visibile on the device.
	///Not needed when memory is coherent, look at vkFlushMappedMemoryRanges.
	///Can be checked with coherent().
	void flush() const; //XXX: raname flush?

	///Reloads the device memory into mapped memory, i.e. makes sure writes by the device
	///are made visible. Not needed when memory is coherent, look at vkInvalidateMappedMemoryRanges.
	///Can be checked with coherent().
	void reload() const; //XXX: rename invalidte/reload?

	MemoryMap& memoryMap() const { return *memoryMap_; }
	const DeviceMemory& memory() const { return memoryMap().memory(); }
	const vk::DeviceMemory& vkMemory() const { return memoryMap().vkMemory(); }
	const Allocation& allocation() const { return allocation_; }
	std::size_t offset() const { return allocation().offset; }
	std::size_t size() const { return allocation().size; }
	std::uint8_t* ptr() const;
	bool coherent() const;

	vk::MappedMemoryRange mappedMemoryRange() const;

	const MemoryMap& resourceRef() const { return *memoryMap_; }
	friend void swap(MemoryMapView& a, MemoryMapView& b) noexcept;

protected:
	MemoryMap* memoryMap_ {};
	Allocation allocation_ {};
};

///Specifies the different types of allocation on a memory object.
enum class AllocationType
{
	none = 0,
	linear = 1,
	optimal = 2,
	sparse = 4,
	sparseAlias = 8
};

///DeviceMemory class that keeps track of its allocated and freed areas.
///Makes it easy to resuse memory as well as bind multiple memoryRequestors to one allocation.
///Note that there are additional rules for allocating device memory on vulkan (like e.g. needed
///offsets between image and buffer allocations) which are not checked/stored by this class, this
///has to be done externally.
class DeviceMemory : public ResourceHandle<vk::DeviceMemory>
{
public:
	struct AllocationEntry
	{
		Allocation allocation;
		AllocationType type;
	};

public:
	DeviceMemory() = default;
	DeviceMemory(const Device& dev, const vk::MemoryAllocateInfo& info);
	DeviceMemory(const Device& dev, std::uint32_t size, std::uint32_t typeIndex);
	DeviceMemory(const Device& dev, std::uint32_t size, vk::MemoryPropertyFlags flgs);
	~DeviceMemory();

	///DeviceMemory is NonMovable since all memory resources will keep references to
	///their memory objects. Therfore its location has not to change.
	DeviceMemory(DeviceMemory&& other) noexcept = delete;
	DeviceMemory& operator=(DeviceMemory&& other) noexcept = delete;

	///Tries to allocate a memory part that matches the given size and aligment requirements.
	///If there is not enough free space left, a std::runtime_error will be thrown.
	///The size parameter has to be not null, otherwise a std::logic_error will be thrown.
	///One can test if there is enough space for the needed allocation with the
	///allocatable() member function.
	Allocation alloc(std::size_t size, std::size_t aligment, AllocationType type);

	///Tests if an allocation for the given requirements can be made.
	///Will return an empty (size = 0) allocation if it is not possible or the theoretically
	///possible alloction. This can then (before any other allocate call) safely be allocated
	///with a call to allocSpecified.
	///Notice that this call itself does NOT reserve any memory for the caller so the memory
	///range of the returned allocatin shall not be used.
	Allocation allocatable(std::size_t size, std::size_t aligment, AllocationType type) const;

	///Allocates the specified memory part. Does not check for matched requirements or if
	///the specified space is already occupied, so this function have to be used with care.
	///This function can be useful if the possibility of a given allocation was checked before
	///with a call to the allocatable function (than the returned range can safely be allocated)
	///with this function. It might also be useful if one wants to manage the memory reservation
	///itself externally and can therefore assure that the given range can be allocated.
	Allocation allocSpecified(std::size_t offset, std::size_t size, AllocationType type);

	///Frees the given allocation. Will throw a std::logic_error if the given allocation is not
	///part of this Memory object.
	///\return false if the allocation could not be found, true otherwise.
	bool free(const Allocation& alloc);

	///Returns the the biggest (continuously) allocatable block.
	///This does not mean that an allocation of this size can be made, since there are also
	///alignment or granularity requirements which will effectively "shrink" this block.
	std::size_t biggestBlock() const;

	///Returns the total amount of free bytes.
	std::size_t totalFree() const;

	///Returns the total size this DeviceMemory object has.
	std::size_t size() const;

	///Returns the currently mapped range, or nullptr if there is none.
	MemoryMap* mapped() { return (memoryMap_.ptr()) ? &memoryMap_ : nullptr; }

	///Returns the currently mapped range, or nullptr if there is none.
	const MemoryMap* mapped() const { return (memoryMap_.ptr()) ? &memoryMap_ : nullptr; }

	///Maps the specified memory range.
	///Will throw a std::logic_error if this memory is not mappeble.
	MemoryMapView map(const Allocation& allocation);

	vk::MemoryPropertyFlags properties() const;
	bool mappable() const;

	unsigned int type() const { return type_; }
	const std::vector<AllocationEntry> allocations() const { return allocations_; }

protected:
	std::vector<AllocationEntry> allocations_ {}; //use sorted container?
	std::size_t size_ {};

	unsigned int type_ {};
	MemoryMap memoryMap_ {}; //the current memory map, or invalid object
};

}
