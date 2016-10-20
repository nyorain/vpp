#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/allocator.hpp>

namespace vpp
{

///Non-virtual base class for all memory resources, i.e. buffers and images.
///Since this class is not virtual it should not (and cannot) be used to delete
///images or buffer dynamically (i.e. with delete).
template<typename H>
class MemoryResource : public ResourceReferenceHandle<MemoryResource<H>, H>
{
public:
	//XXX: why cant these made protected?
	MemoryResource(MemoryResource&& lhs) noexcept { swap(lhs); }
	MemoryResource& operator=(MemoryResource lhs) noexcept { swap(lhs); return *this; }
	~MemoryResource() = default;

	///Checks if this memory resource was initialized yet and if not it will be initialized.
	///Will be implicitly called on member functions that require it.
	void assureMemory() const { if(!memoryEntry().allocated()) memoryEntry().allocate(); }

	///Creates a memory map for this memory resource (if there is none) and returns a view to it.
	///\exception std::logic_error If the resource was allocated on device local memory.
	///Use the mappable() member function or check the memoryEntry manually to determine
	///whether this resource was allocated on mappable memory.
	MemoryMapView memoryMap() const
	{
		assureMemory();
		if(!mappable()) throw std::logic_error("vpp::MemoryResource::memoryMap: not mappable");
		return memoryEntry().map();
	}

	///Returns whether the resource was allocated on hostVisible (mappable) memory.
	///If the there was not yet memory allocated for this resource, false is returned.
	bool mappable() const { return memoryEntry().memory() && memoryEntry().memory()->mappable(); }

	///Returns the memoryEntry for this memory resource.
	const MemoryEntry& memoryEntry() const { return memoryEntry_; }

	///Returns the size in bytes this resource takes in gpu memory.
	std::size_t size() const { return memoryEntry().size(); } //XXX rename memorySize?

	const MemoryEntry& resourceRef() const { return memoryEntry(); }

	void swap(MemoryResource& lhs) noexcept
	{
		using std::swap;
		swap(this->resourceBase(), lhs.resourceBase());
		swap(memoryEntry_, lhs.memoryEntry_);
	}

protected:
	using ResourceReferenceHandle<MemoryResource<H>, H>::ResourceReferenceHandle;
	MemoryResource() = default;

protected:
	MemoryEntry memoryEntry_;
};

}
