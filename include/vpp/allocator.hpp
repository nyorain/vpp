// Copyright (c) 2017 nyorain
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
#include <map> // std::map
#include <cstdlib> // std::size_t

// TODO: use unordered_map for type requirements algorithm (?)

namespace vpp {

/// Makes it possible to allocate a few vk::DeviceMemory objects for many buffers/images.
/// Basically a memory pool that can be used in an asynchronous manner to allocate as few
/// different memory objects as possible. Will also reuse freed memory.
/// Can be used manually, but since the buffer and image (memoryResource)
/// classes already use it, manual usage is usually not required.
/// The api is nontheless exposed publicly.
class DeviceMemoryAllocator : public Resource {
public:
	DeviceMemoryAllocator() = default;
	DeviceMemoryAllocator(const Device& dev);
	~DeviceMemoryAllocator();

	DeviceMemoryAllocator(DeviceMemoryAllocator&& other) noexcept;
	DeviceMemoryAllocator& operator=(DeviceMemoryAllocator other) noexcept;

	/// Requests memory for the given buffer and stores a (pending) reference to it into
	/// the given MemoryEntry.
	/// \param reqs The MemoryRequirements queried for the given buffer. Note that
	/// these can be modified (in a valid way like e.g. unsetting further memory
	/// type bits) by the caller.
	/// \param usage The usage flags the buffer was created with and is used for.
	/// Needed to satisfy certain alignment and pedding requirements
	void request(vk::Buffer requestor, const vk::MemoryRequirements& reqs,
		vk::BufferUsageFlags usage, MemoryEntry& entry);

	/// Requests memory for the given vulkan image and stores a (pending) reference to it into
	/// the given MemoryEntry.
	/// \param reqs The MemoryRequirements queried for the given image. Note that
	/// these can be modified (in a valid way like e.g. unsetting further memory
	/// type bits) by the caller.
	/// \param tiling The ImageTiling for the given image.
	/// Needed to satisfy certain alignment and pedding requirements
	void request(vk::Image requestor, const vk::MemoryRequirements& reqs, vk::ImageTiling tiling,
		MemoryEntry& entry);

	/// Removes the pending memory request for the given entry.
	/// Returns false if the given entry could not be found.
	bool removeRequest(const MemoryEntry& entry);

	/// This function will be called when a stored entry is moved.
	/// Will return false if the given old entry could not be found and could
	/// therefore not be replaced by the new one.
	/// More efficient than removing the old entry and creating a new one.
	bool moveEntry(const MemoryEntry& oldOne, MemoryEntry& newOne);

	/// Allocates and associated device memory for all pending requests.
	/// This will finish all pending memory requests.
	void allocate();

	/// Makes sure that the given entry has associated memory, i.e. finishes its memory request
	/// and removes it from the interal list of pending entries.
	/// Will return false if the given entry cannot be found.
	bool allocate(const MemoryEntry& entry);

	/// Returns all memories that this allocator manages.
	std::vector<DeviceMemory*> memories() const;
	friend void swap(DeviceMemoryAllocator& a, DeviceMemoryAllocator& b) noexcept;

protected:
	/// The type of a request/requirement.
	/// Determines which union member is active and is relevant to satisy
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
		MemoryEntry* entry {};
		union { vk::Buffer buffer; vk::Image image; };
	};

	using Requirements = std::vector<Requirement>;

protected:
	// utility global functions
	static AllocationType toAllocType(RequirementType reqType);
	static bool supportsType(const Requirement& req, unsigned int type);
	static bool supportsType(std::uint32_t bits, unsigned int type);

	// utility allocation functions
	void allocate(unsigned int type);
	void allocate(unsigned int type, nytl::Span<Requirement* const> requirements);
	DeviceMemory* findMem(Requirement& req);
	Requirements::iterator findReq(const MemoryEntry& entry);
	std::map<unsigned int, std::vector<Requirement*>> queryTypes();
	unsigned int findBestType(std::uint32_t typeBits) const;

protected:
	Requirements requirements_; // list of pending requests
	std::vector<std::unique_ptr<DeviceMemory>> memories_; // list of owned memory objects
};

/// Represents an entry on a vulkan device memory which will be dynamically and asynchronously
/// allocated by a DeviceMemoryAllocator.
/// Objects of this type may have 3 states:
/// - invalid (defaulted constructed or moved from, no associated memory)
/// - pending (waiting for associated DeviceMemoryAllocator to allocate memory)
/// - bound (referncing an associated DeviceMemory object on which the Entry is bound)
/// Will automatically free the associated memory allocation (if any) on destruction or
/// unregister the pending memory request from the DeviceMemoryAllocator.
class MemoryEntry : public ResourceReference<MemoryEntry> {
public:
	MemoryEntry() = default;
	MemoryEntry(DeviceMemory& memory, const Allocation& alloc);
	~MemoryEntry();

	MemoryEntry(MemoryEntry&& other) noexcept;
	MemoryEntry& operator=(MemoryEntry other) noexcept;

	/// Will try to map the Memory and return a view to the location where this entry is placed.
	/// Throws a std::logic_error if the associated DeviceMemory is not mappable or the
	/// MemoryEntry has no associated memory.
	MemoryMapView map() const;

	/// Returns whether this entry has an associated memory allocation, i.e. if it is currently
	/// in a bound state.
	bool allocated() const { return (allocation_.size > 0); }

	/// Assures that there is memory allocated and associated with this entry.
	/// Will have no effect if the entry already has an associated memory allocation.
	/// Results in undefined behvaiour if this MemoryEntry is in an invalid state, i.e.
	/// if it has no associated DeviceMemoryAllocator.
	void allocate() const { if(!allocated()) allocator_->allocate(*this); }

	DeviceMemory* memory() const { return allocated() ? memory_ : nullptr; };
	DeviceMemoryAllocator* allocator() const { return allocated() ? nullptr : allocator_; };
	std::size_t offset() const { return allocation_.offset; };
	std::size_t size() const { return allocation_.size; }
	const Allocation& allocation() const { return allocation_; }

	Resource& resourceRef() const { if(allocated()) return *memory_; else return *allocator_; }
	friend void swap(MemoryEntry& a, MemoryEntry& b) noexcept;

protected:
	friend class DeviceMemoryAllocator;

	// if there is an allocation associated with this entry (the allocation size is > 0)
	// the memory member wil be active, otherwise the allocator.
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
