// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/allocator.hpp>
#include <utility>

namespace vpp {

/// Represents a resource that might be bound to memory and keeps track
/// of this memory. Does not support any kind of sparse bindings.
class MemoryResource {
public:
	MemoryResource() = default;
	MemoryResource(MemoryEntry&& entry)
		: memoryEntry_(std::move(entry)) {}

	MemoryResource(MemoryResource&& rhs) noexcept = default;
	MemoryResource& operator=(MemoryResource&& rhs) noexcept = default;

	/// Checks if this memory resource was initialized yet and if not it
	/// will be initialized.
	void ensureMemory();

	/// Returns whether this resource is bound to hostVisible memory.
	bool mappable() const;

	/// Creates/Updates the memoryMap of the memory this resource is
	/// bound to and returns a view. Must be bound to hostVisible memory,
	/// i.e. mappable() must be true.
	MemoryMapView memoryMap(vk::DeviceSize offset = 0u,
		vk::DeviceSize size = vk::wholeSize) const;

	/// Returns the size this resource takes up in memory.
	/// Note that this often differs from the real size of the resource
	/// (e.g. buffer or image data size) due to metadata so it should
	/// not be used as that.
	vk::DeviceSize memorySize() const { return memoryEntry().size(); }
	const MemoryEntry& memoryEntry() const { return memoryEntry_; }

protected:
	MemoryEntry memoryEntry_;
};

} // namespace vpp
