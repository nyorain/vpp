#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp>
#include <vpp/resource.hpp>
#include <vpp/allocator.hpp>

namespace vpp
{

///Non-virtual base class for all memory resources, i.e. buffers and images.
///Since this class is not virtual it should not (and cannot) be used to delete
///images or buffer dynamically (i.e. with delete).
class MemoryResource : public ResourceReference<MemoryResource>
{
public:
	///Assures that there is device memory associated with this resource.
	void init() const;

	///Checks if this memory resource was initialized yet and if not it will be initialized.
	///Will be implicitly called on member functions that require it.
	void assureMemory() const;

	///Creates a memory map for this memory resource (if there is none) and returns a view to it.
	///\exception std::logic_error If the resource was allocated on device local memory.
	///Use the mappable() member function or check the memoryEntry manually to determine
	///whether this resource was allocated on mappable memory.
	MemoryMapView memoryMap() const;

	///Returns whether the resource was allocated on hostVisible (mappable) memory.
	bool mappable() const;

	///Returns the memoryEntry for this memory resource.
	const MemoryEntry& memoryEntry() const { return memoryEntry_; }

	///Returns the size in bytes this resource takes in gpu memory.
	std::size_t size() const { return memoryEntry().size(); }

	const MemoryEntry& resourceRef() const { return memoryEntry(); }

protected:
	MemoryResource() = default;
	~MemoryResource() = default;

	MemoryResource(MemoryResource&& other) noexcept = default;
	MemoryResource& operator=(MemoryResource&& other) noexcept = default;

protected:
	MemoryEntry memoryEntry_;
};

}
