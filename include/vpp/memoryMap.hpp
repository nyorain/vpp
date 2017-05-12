// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp> // vpp::ResourceReference
#include <vpp/util/allocation.hpp> // vpp::Allocation

namespace vpp {

/// Represents a mapped range of a vulkan DeviceMemory.
/// There shall never be more than one MemoryMap object for on DeviceMemory object.
/// The MemoryMap class is usually never used directly, but rather accessed through a
/// MemoryMapView.
/// Instances of this class cannot be created manually but must be indirectly
/// retrieved by a DeviceMemory object.
/// This class is not threadsafe.
class MemoryMap : public ResourceReference<MemoryMap> {
public:
	/// Assures that the range given by allocation is included in the map.
	/// Might remap the mapped range.
	void remap(const Allocation& allocation);

	/// Makes sure the mapped data is visible on the device.
	/// If memory is coherent, this function will have no effect.
	void flush() const;

	/// Reloads the device memory into mapped memory, i.e. makes sure writes by the device
	/// are made visible.
	/// If the memory is coherent, this function will have no effect.
	void reload() const;

	/// Returns whether this object is valid.
	/// If it is not, any operations on it may result in undefined behavior.
	bool valid() const noexcept { return ptr_; }

	const vk::DeviceMemory& vkMemory() const noexcept;
	const Allocation& allocation() const noexcept { return allocation_; }
	size_t offset() const noexcept { return allocation().offset; }
	size_t size() const noexcept { return allocation().size; }
	uint8_t* ptr() const noexcept { return static_cast<uint8_t*>(ptr_); }
	const DeviceMemory& memory() const noexcept { return *memory_; }
	bool coherent() const noexcept;

	vk::MappedMemoryRange mappedMemoryRange() const noexcept;

	const DeviceMemory& resourceRef() const noexcept { return *memory_; }
	friend void swap(MemoryMap& a, MemoryMap& b) noexcept;

protected:
	friend class MemoryMapView;
	friend class DeviceMemory;

	MemoryMap()  = default;
	MemoryMap(const DeviceMemory& memory, const Allocation& alloc);
	~MemoryMap();

	MemoryMap(MemoryMap&& rhs) noexcept { swap(*this, rhs); }
	MemoryMap& operator=(MemoryMap rhs) noexcept { swap(*this, rhs); return *this; }

	void ref() noexcept;
	void unref() noexcept;
	void unmap();

protected:
	const DeviceMemory* memory_ {nullptr};
	Allocation allocation_ {};
	size_t views_ {};
	void* ptr_ {nullptr};
};

/// A view into a mapped memory range.
/// Makes it possible to write/read from multiple allocations on a mapped memory.
/// Objects are always retrieved by a DeviceMemory object.
/// This class is not threadsafe.
class MemoryMapView : public ResourceReference<MemoryMapView> {
public:
	MemoryMapView() noexcept = default;
	~MemoryMapView();

	MemoryMapView(MemoryMapView&& lhs) noexcept { swap(lhs); }
	MemoryMapView& operator=(MemoryMapView lhs) noexcept { swap(lhs); return *this; }

	/// Makes sure the mapped data is visible on the device.
	/// Not needed when memory is coherent, look at vkFlushMappedMemoryRanges.
	/// Can be checked with coherent().
	void flush() const;

	/// Reloads the device memory into mapped memory, i.e. makes sure writes by the device
	/// are made visible. Not needed when memory is coherent, look at
	/// vkInvalidateMappedMemoryRanges. Can be checked with coherent().
	void reload() const;

	/// Returns whether the view is valid.
	bool valid() const noexcept { return memoryMap_; }

	MemoryMap& memoryMap() const noexcept { return *memoryMap_; }
	const DeviceMemory& memory() const noexcept { return memoryMap().memory(); }
	vk::DeviceMemory vkMemory() const noexcept { return memoryMap().vkMemory(); }
	const Allocation& allocation() const noexcept { return allocation_; }
	size_t offset() const noexcept { return allocation().offset; }
	size_t size() const noexcept { return allocation().size; }
	uint8_t* ptr() const noexcept;
	bool coherent() const noexcept;

	vk::MappedMemoryRange mappedMemoryRange() const noexcept;

	const MemoryMap& resourceRef() const noexcept { return *memoryMap_; }
	void swap(MemoryMapView& lhs) noexcept;

protected:
	friend class DeviceMemory;
	MemoryMapView(MemoryMap& map, const Allocation& range);

protected:
	MemoryMap* memoryMap_ {};
	Allocation allocation_ {};
};

} // namespace vpp
