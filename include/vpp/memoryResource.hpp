// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memory.hpp>
#include <vpp/allocator.hpp>
#include <utility>

namespace vpp {

/// Represents a resource that might be bound to memory span and keeps track
/// of this memory. Does not support any kind of sparse bindings.
class MemoryResource {
public:
	struct InitData {
		DeviceMemoryAllocator* allocator {};
		DeviceMemoryAllocator::ReservationID reservation {};
	};

public:
	MemoryResource() = default;
	MemoryResource(DeviceMemory&, vk::DeviceSize offset);
	~MemoryResource();

	MemoryResource(MemoryResource&& rhs) noexcept { swap(*this, rhs); }
	MemoryResource& operator=(MemoryResource rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	/// When the two-step deferred constructor was used, this function
	/// will allocate the memory for this resource.
	void init(InitData& data);

	/// Returns whether this resource is bound to hostVisible memory.
	bool mappable() const;

	/// Creates/Updates the memoryMap of the memory this resource is
	/// bound to and returns a view. Must be bound to hostVisible memory,
	/// i.e. mappable() must be true.
	MemoryMapView memoryMap(vk::DeviceSize offset,
		vk::DeviceSize size) const;

	DeviceMemory& memory() const { return *memory_; }
	vk::DeviceSize memoryOffset() const { return offset_; }

	friend void swap(MemoryResource& a, MemoryResource& b);

protected:
	DeviceMemory* memory_ {};
	vk::DeviceSize offset_ {};
};

} // namespace vpp
