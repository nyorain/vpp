// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp> // vpp::Resource
#include <vpp/memory.hpp> // vpp::DeviceMemory
#include <vpp/util/span.hpp> // nytl::Span
#include <vpp/util/allocation.hpp> // vpp::Allocation

#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include <unordered_map> // std::unordered_map
#include <deque> // std::deque

namespace vpp {

/// Makes it possible to allocate a few vk::DeviceMemory objects for many buffers/images.
/// Basically a memory pool that can be used in an asynchronous manner to allocate as few
/// different memory objects as possible. Will also reuse freed memory.
/// Can be used manually, but since the buffer and image (memoryResource)
/// classes already use it, manual usage is usually not required.
/// The api is nonetheless exposed publicly.
class DeviceMemoryAllocator : public Resource {
public:
	DeviceMemoryAllocator() = default;
	DeviceMemoryAllocator(const Device& dev);
	~DeviceMemoryAllocator();

	DeviceMemoryAllocator(DeviceMemoryAllocator&&) noexcept;
	DeviceMemoryAllocator& operator=(DeviceMemoryAllocator) noexcept;

	/// Requests memory for the given buffer and stores a (pending) reference to it into
	/// the given MemoryEntry.
	/// \param reqs The MemoryRequirements queried for the given buffer. Note that
	/// these can be modified (in a valid way like e.g. unsetting further memory
	/// type bits) by the caller.
	void request(vk::Buffer requestor, const vk::MemoryRequirements&,
		MemoryEntry&);

	/// Requests memory for the given vulkan image and stores a (pending) reference to it into
	/// the given MemoryEntry.
	/// \param reqs The MemoryRequirements queried for the given image. Note that
	/// these can be modified (in a valid way like e.g. unsetting further memory
	/// type bits) by the caller.
	/// \param tiling The ImageTiling for the given image.
	/// Needed to satisfy certain alignment and padding requirements
	void request(vk::Image requestor, const vk::MemoryRequirements&,
		vk::ImageTiling, MemoryEntry&);

	/// Removes the pending memory request for the given entry.
	/// Will have no effect if the old entry is not found other than outputting
	/// a warning if in debug mode.
	void removeRequest(const MemoryEntry&) noexcept;

	/// This function will be called when a stored entry is moved.
	/// Will remove the old entry and associate the new one with its request.
	/// More efficient than removing the old entry and creating a new one.
	/// Will have no effect if the old entry is not found other than outputting
	/// a warning if in debug mode.
	void moveEntry(const MemoryEntry& olde, MemoryEntry& newe) noexcept;

	/// Allocates and associated device memory for all pending requests.
	/// This will finish all pending memory requests.
	void allocate();

	/// Makes sure that the given entry has associated memory, i.e. finishes its memory request
	/// and removes it from the internal list of pending entries.
	/// Prefer the allocate overload without parameters that allocates all pending
	/// memory request since it might be more efficient overall.
	/// Will have no effect if the old entry is not found other than outputting
	/// a warning if in debug mode.
	void allocate(const MemoryEntry&);

	/// Returns all memories that this allocator manages.
	const auto& memories() const { return memories_; }
	static void swap(DeviceMemoryAllocator&, DeviceMemoryAllocator&) noexcept;
	friend void swap(DeviceMemoryAllocator& a, DeviceMemoryAllocator& b) noexcept {
		DeviceMemoryAllocator::swap(a, b);
	}

protected:
	/// The type of a request/requirement.
	/// Determines which union member is active and is relevant to satisfy
	/// all alignment, padding and layout requirements.
	enum class RequirementType {
		linearImage,
		optimalImage,
		buffer
	};

	/// Represents the requirements of a pending memory request.
	/// Holds all needed information for allocating memory for the
	/// associated resource.
	struct Requirement {
		RequirementType type {};
		vk::DeviceSize size {};
		vk::DeviceSize alignment {};
		std::uint32_t memoryTypes {};
		std::reference_wrapper<MemoryEntry> entry;
		union {
			vk::Buffer buffer;
			vk::Image image;
		};
	};

	using Requirements = std::vector<Requirement>;

protected:
	// utility global functions
	static AllocationType toAllocType(RequirementType reqType) noexcept;
	static bool supportsType(const Requirement& req, unsigned int type) noexcept;
	static bool supportsType(uint32_t bits, unsigned int type) noexcept;

	// utility allocation functions
	void allocate(unsigned int type);
	void allocate(unsigned int type, nytl::Span<Requirement* const> requirements);
	DeviceMemory* findMem(Requirement& req);
	Requirements::iterator findReq(const MemoryEntry& entry);
	std::unordered_map<unsigned int, std::vector<Requirement*>> queryTypes();
	unsigned int findBestType(uint32_t typeBits) const;

protected:
	Requirements requirements_; // list of pending requests
	std::deque<DeviceMemory> memories_; // list of owned memory objects
};

/// Represents an entry on a vulkan device memory which will be dynamically and asynchronously
/// allocated by a DeviceMemoryAllocator.
/// Objects of this type may have 3 states:
/// - invalid (defaulted constructed or moved from, no associated memory)
/// - pending (waiting for associated DeviceMemoryAllocator to allocate memory)
/// - bound (referencing an associated DeviceMemory object on which the Entry is bound)
/// Will automatically free the associated memory allocation (if any) on destruction or
/// unregister the pending memory request from the DeviceMemoryAllocator.
class MemoryEntry : public ResourceReference<MemoryEntry> {
public:
	using Allocation = DeviceMemory::Allocation;

public:
	MemoryEntry() = default;
	MemoryEntry(DeviceMemory& memory, const Allocation& alloc);
	~MemoryEntry();

	MemoryEntry(MemoryEntry&& other) noexcept;
	MemoryEntry& operator=(MemoryEntry&& other) noexcept;

	/// Will try to map the Memory and return a view to the location where this entry is placed.
	/// The MemoryEntry must be bound to memory.
	MemoryMapView map(vk::DeviceSize offset = 0,
		vk::DeviceSize size = vk::wholeSize) const;

	/// Returns whether this entry has an associated memory allocation, i.e. if it is currently
	/// in a bound state.
	bool allocated() const noexcept { return (allocation_.size > 0); }

	/// Assures that there is memory allocated and associated with this entry.
	/// Will have no effect if the entry already has an associated memory allocation.
	/// Results in undefined behavior if this MemoryEntry is in an invalid state, i.e.
	/// if it has no associated DeviceMemoryAllocator.
	void allocate() const { if(!allocated()) allocator_->allocate(*this); }

	auto* memory() const noexcept { return allocated() ? memory_ : nullptr; }
	auto* allocator() const noexcept { return allocated() ? nullptr : allocator_; }
	auto offset() const noexcept { return allocation_.offset; }
	auto size() const noexcept { return allocation_.size; }
	const Allocation& allocation() const noexcept { return allocation_; }

	Resource& resourceRef() const noexcept;

protected:
	friend class DeviceMemoryAllocator;

	// if there is an allocation associated with this entry (the allocation size is > 0)
	// the memory member will be active, otherwise the allocator.
	// so by default the allocator_ var will simple hold a nullptr and the allocation an empty
	// allocation {0, 0}. The allocation signals that is it not yet allocated and the
	// nullptr allocator var that it is invalid (i.e. not yet associated with an allocator).
	union {
		DeviceMemoryAllocator* allocator_ {};
		DeviceMemory* memory_;
	};

	Allocation allocation_ {};
};

} // namespace vpp
