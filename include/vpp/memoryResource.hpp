// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/allocator.hpp>
#include <vpp/util/log.hpp>

namespace vpp {

/// Bbase class for all memory resources, i.e. buffers and images.
/// Holds an internal memory entry that is used to bind memory to this resource.
/// Since this class is not virtual it should not (and cannot) be used to delete
/// images or buffer dynamically (i.e. with delete).
template<typename H>
class MemoryResource : public ResourceReferenceHandle<MemoryResource<H>, H> {
public:
	/// Checks if this memory resource was initialized yet and if not it will be initialized.
	/// Will be implicitly called on member functions that require it.
	void assureMemory() const { memoryEntry().allocate(); }

	/// Creates a memory map for this memory resource (if there is none) and returns a view to it.
	/// Will automatically assure that there is memory bound for this resource.
	/// In debug mode, throws std::logic_error if the memory it is bound to cannot be mapped.
	MemoryMapView memoryMap() const;

	/// Returns whether the resource was allocated on hostVisible (mappable) memory.
	/// If the there was not yet memory allocated for this resource, false is returned.
	bool mappable() const { return memoryEntry().memory() && memoryEntry().memory()->mappable(); }

	/// Returns the memoryEntry for this memory resource.
	/// Can be used to get more information about the associated device memory.
	const MemoryEntry& memoryEntry() const { return memoryEntry_; }

	/// Returns the size in bytes this resource takes in device memory.
	/// Note that this might differ from the real buffer size e.g. if it was not
	/// already bound or if there were additional bytes allocated for the buffer.
	std::size_t memorySize() const { return memoryEntry().size(); }
	const MemoryEntry& resourceRef() const { return memoryEntry(); }

	template<typename R>
	friend void swap(MemoryResource<R>& a, MemoryResource<R>& b) noexcept;

protected:
	using ResourceReferenceHandle<MemoryResource<H>, H>::ResourceReferenceHandle;

	MemoryResource() = default;
	~MemoryResource() = default;

	// needed for default move operators
	friend class Buffer;
	friend class Image;

protected:
	MemoryEntry memoryEntry_;
};

// - implementation -
template<typename R>
MemoryMapView MemoryResource<R>::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
}

template<typename R>
void swap(MemoryResource<R>& a, MemoryResource<R>& b) noexcept
{
	using std::swap;
	using RRH = ResourceReferenceHandle<MemoryResource<R>, R>;

	swap(static_cast<RRH&>(a), static_cast<RRH&>(b));
	swap(a.memoryEntry_, b.memoryEntry_);
}

} // namespace vpp
