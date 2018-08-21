// Copyright (c) 2016-2018 nyorain
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
	using Allocation = BasicAllocation<vk::DeviceSize>;

public:
	/// MemoryMap is NonMovable to allow views to keep references
	/// to the MemoryMap object (e.g. unref it on destruction).
	MemoryMap(MemoryMap&&) noexcept = delete;
	MemoryMap& operator=(MemoryMap&&) noexcept = delete;

	/// Assures that the range given by allocation is included in the map.
	/// Might remap the mapped range. If the memory is currently not
	/// mapped, will map it.
	void remap(const Allocation& allocation);

	/// Calls flushMappedMemoryRanges, see the vulkan spec.
	/// If memory is coherent, this function will have no effect (it
	/// will check everytime).
	void flush(const Allocation& = {0u, vk::wholeSize}) const;

	/// Calls invalidateMappedMemoryRanges, see the vulkan spec.
	/// If memory is coherent, this function will have no effect (it
	/// will check everytime).
	void invalidate(const Allocation& = {0u, vk::wholeSize}) const;

	/// Returns whether this object is valid.
	/// If it is not, any operations on it may result in undefined behavior.
	bool valid() const noexcept { return ptr_; }

	/// Note that pointer (and span) may change when the memory is remapped.
	/// So don't store them over a time where a remap might happen.
	std::byte* ptr() const noexcept { return static_cast<std::byte*>(ptr_); }
	nytl::Span<std::byte> span() const noexcept { return {ptr(), size()}; }
	nytl::Span<const std::byte> cspan() const noexcept { return {ptr(), size()}; }

	const vk::DeviceMemory& vkMemory() const noexcept;
	const Allocation& allocation() const noexcept { return allocation_; }
	vk::DeviceSize offset() const noexcept { return allocation().offset; }
	vk::DeviceSize size() const noexcept { return allocation().size; }
	const DeviceMemory& memory() const noexcept { return *memory_; }
	bool coherent() const noexcept;

	vk::MappedMemoryRange mappedMemoryRange() const noexcept;

	const DeviceMemory& resourceRef() const noexcept { return *memory_; }

protected:
	friend class MemoryMapView;
	friend class DeviceMemory;

	MemoryMap()  = default;
	~MemoryMap();

	void map(const DeviceMemory& memory, const Allocation& alloc);

	void ref() noexcept;
	void unref() noexcept;
	void unmap();

protected:
	const DeviceMemory* memory_ {nullptr};
	Allocation allocation_ {};
	unsigned int views_ {};
	void* ptr_ {nullptr};
};

/// A view into a mapped memory range.
/// Makes it possible to write/read from multiple allocations on a mapped memory.
/// Objects are always retrieved by a DeviceMemory object.
/// This class is not threadsafe.
class MemoryMapView : public ResourceReference<MemoryMapView> {
public:
	using Allocation = BasicAllocation<vk::DeviceSize>;

public:
	MemoryMapView() noexcept = default;
	~MemoryMapView();

	MemoryMapView(MemoryMapView&& lhs) noexcept { swap(lhs); }
	MemoryMapView& operator=(MemoryMapView lhs) noexcept {
		swap(lhs);
		return *this;
	}

	/// Calls flushMappedMemoryRanges, see the vulkan spec.
	/// If memory is coherent, this function will have no effect (it
	/// will check everytime).
	void flush() const;

	/// Calls invalidateMappedMemoryRanges, see the vulkan spec.
	/// If memory is coherent, this function will have no effect (it
	/// will check everytime).
	void invalidate() const;

	/// Returns whether the view is valid.
	bool valid() const noexcept { return memoryMap_; }

	/// Note that pointer (and span) may change when the memory is remapped.
	/// So don't store them over a time where a remap might happen.
	std::byte* ptr() const noexcept;
	nytl::Span<std::byte> span() const noexcept { return {ptr(), size()}; }
	nytl::Span<const std::byte> cspan() const noexcept { return {ptr(), size()}; }

	MemoryMap& memoryMap() const noexcept { return *memoryMap_; }
	const DeviceMemory& memory() const noexcept { return memoryMap().memory(); }
	vk::DeviceMemory vkMemory() const noexcept { return memoryMap().vkMemory(); }
	const Allocation& allocation() const noexcept { return allocation_; }
	vk::DeviceSize offset() const noexcept { return allocation().offset; }
	vk::DeviceSize size() const noexcept { return allocation().size; }
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
