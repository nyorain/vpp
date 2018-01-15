// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/buffer.hpp>
#include <vpp/resource.hpp>
#include <deque>

namespace vpp {

/// Like vpp::Buffer but keeps track of sub-allocated BufferRanges.
/// Can be used to share a buffer.
/// See also BufferAllocator.
/// Also implements a mechanism to make sure all allocations are spaced
/// according to the devices nonCoherentAtomAlign property.
class SharedBuffer : public Buffer {
public:
	using Allocation = BasicAllocation<vk::DeviceSize>;

	/// Whether the nonCoherentAtomAlign feature is enabled.
	/// If this is true, will make sure allocations are spaced
	/// in a way that allows mapping and accessing them independently
	/// even on non-hostCoherent memory.
	/// Makes no sense to set on non-hostVisible or hostCoherent memory.
	/// Note that changing this only affects future allocations, so you
	/// probably don't want to call this while there are active
	/// allocations.
	bool nonCoherentAtomAlign {false};

public:
	using Buffer::Buffer;
	~SharedBuffer();

	/// Not movable to allow BufferRanges to reference it.
	SharedBuffer(SharedBuffer&&) noexcept = delete;
	SharedBuffer& operator=(SharedBuffer&&) noexcept = delete;

	/// Tries to allocate a range with the given size and align.
	/// Returns an empty (i.e. size == 0) allocation if there
	/// is not enough free space left.
	Allocation alloc(vk::DeviceSize size, vk::DeviceSize align = 0u);

	/// Frees the given allocation. Undefined behavior if the
	/// allocation is invalid (i.e. not allocated on this SharedBuffer).
	void free(const Allocation&);

	/// Returns all current allocations.
	const auto& allocations() const { return allocations_; }

protected:
	// sorted by position for efficient allocation/release
	std::vector<Allocation> allocations_;
};

/// Sub-allocated range of a buffer.
/// See also SharedBuffer.
class BufferRange : public ResourceReference<BufferRange> {
public:
	using Allocation = SharedBuffer::Allocation;

public:
	BufferRange() = default;
	BufferRange(SharedBuffer&, vk::DeviceSize size, vk::DeviceSize align = 0u);
	BufferRange(SharedBuffer&, const Allocation& allocation);
	~BufferRange();

	BufferRange(BufferRange&& rhs) noexcept { swap(*this, rhs); }
	BufferRange& operator=(BufferRange rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	const SharedBuffer& buffer() const { return *shared_; }
	const Allocation& allocation() const { return allocation_; }
	vk::DeviceSize offset() const { return allocation().offset; }
	vk::DeviceSize size() const { return allocation().size; }

	bool mappable() const noexcept { return buffer().mappable(); }
	MemoryMapView memoryMap() const;

	const SharedBuffer& resourceRef() const { return *shared_; }
	friend void swap(BufferRange&, BufferRange&) noexcept;

protected:
	SharedBuffer* shared_ {};
	Allocation allocation_ {};
};

/// Allocates BufferRanges on owned SharedBuffers.
/// Useful to reuse (espeically short-lived) buffers like staging
/// transfer buffers or small buffers like ubos.
class BufferAllocator : public vpp::Resource {
public:
	BufferAllocator() = default;
	BufferAllocator(const Device& dev);
	~BufferAllocator() = default;

	BufferAllocator(BufferAllocator&&) = default;
	BufferAllocator& operator=(BufferAllocator&&) = default;

	/// Reserves the given requirements.
	/// The next time alloc is called, these are initialized additionally.
	/// Can be used to group alloc groups together in a
	/// deferred-initialization-like manner.
	/// This does not automatically reserve anything, only
	/// causes the next buffer that has to be created and matches
	/// the given requirements to be larger (using the size parameter).
	/// If you wish to map the buffer, you have to pass true as mappable.
	void reserve(bool mappable, vk::DeviceSize size, vk::BufferUsageFlags,
		vk::DeviceSize align = 0u, unsigned int memBits = ~0u);

	/// Allocates a buffer range with the given requirements.
	/// If you wish to map the buffer, you have to pass true as mappable.
	BufferRange alloc(bool mappable, vk::DeviceSize size, vk::BufferUsageFlags,
		vk::DeviceSize align = 0u, unsigned int memBits = ~0u);

	/// Optimizes the buffer allocations. Will recreate all unused buffers
	/// as one big buffer. Makes sense to call when no/few buffer rangers
	/// are allocated but the memory will still be needed.
	void optimize();

	/// Releases all currently unused shared buffers.
	void shrink();

	const auto& buffers() const { return buffers_; }

protected:
	struct Requirement {
		vk::DeviceSize size {};
		vk::BufferUsageFlags usage {};
		unsigned int memBits {};
		bool mappable {};
	};

	struct Buffer {
		Buffer(const Device&, const vk::BufferCreateInfo&, unsigned int mbits);

		SharedBuffer buffer;
		vk::BufferUsageFlags usage;
	};

	std::deque<Buffer> buffers_;
	std::vector<Requirement> reqs_;
};

/// Returns a queue family that supports graphics, compute or transfer operations
/// and can therefore be used for transfer operations.
/// Guarantees that there exists at least one queue for the given device with
/// the returned queue family.
/// Returns -1 if there is no such family, although there usually should be.
/// If queue if not nullptr, will store a pointer to a queue of the returned
/// family into it.
int transferQueueFamily(const Device& dev, const Queue** queue);

} // namespace vpp
