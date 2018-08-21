// Copyright (c) 2016-2018 nyorain
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
/// Keep in mind the vulkan limitations and mechanisms of using buffers
/// with multiple queue families when sharing one.
class SharedBuffer : public Buffer {
public:
	using Allocation = BasicAllocation<vk::DeviceSize>;

public:
	SharedBuffer(const Device&, const vk::BufferCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	SharedBuffer(const Device&, const vk::BufferCreateInfo&, DeviceMemory&);
	SharedBuffer(DeferTag, const Device&, const vk::BufferCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

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

	const auto& allocations() const { return allocations_; }
	auto size() const { return size_; }

protected:
	// sorted by position for efficient allocation/release
	std::vector<Allocation> allocations_;
	vk::DeviceSize size_;
};

/// Sub-allocated range of a buffer.
/// See also SharedBuffer.
class SubBuffer : public ResourceReference<SubBuffer> {
public:
	using Allocation = SharedBuffer::Allocation;

public:
	SubBuffer() = default;

	/// Will automatically make sure that alignment makes sense with
	/// the given usage flags, e.g. will make sure it's at least
	/// minUniformBufferOffsetAlignment of the associated physical device
	/// if usage flags include uniform buffer.
	SubBuffer(BufferAllocator&, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memoryTypeBits = ~0u,
		vk::DeviceSize align = 0u);
	SubBuffer(DeferTag, BufferAllocator&, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memoryTypeBits = ~0u,
		vk::DeviceSize align = 0u);
	SubBuffer(SharedBuffer&, const Allocation& allocation);
	~SubBuffer();

	SubBuffer(SubBuffer&& rhs) noexcept { swap(*this, rhs); }
	SubBuffer& operator=(SubBuffer rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	/// Only has to be called when the SubBuffer was constructed
	/// with the defer tag. Otherwise has no effect.
	void init();

	const SharedBuffer& buffer() const { return *shared_; }
	const Allocation& allocation() const { return allocation_; }
	vk::DeviceSize offset() const { return allocation().offset; }
	vk::DeviceSize size() const { return allocation().size; }
	auto end() const { return allocation_.end(); }

	bool mappable() const noexcept { return buffer().mappable(); }
	MemoryMapView memoryMap() const;

	const SharedBuffer& resourceRef() const { return *shared_; }
	friend void swap(SubBuffer&, SubBuffer&) noexcept;

protected:
	union {
		BufferAllocator* allocator_ {};
		SharedBuffer* shared_;
	};

	Allocation allocation_ {};
};

/// Cases in which you might care for buffer alignment:
///  - use it with a DrawIndirectCommand at the beginning (align 4)
///  - descriptor buffer (align min*BufferOffsetAlignment)
///  - using it as vertex buffer. Needs component type alignment

/// Allocates BufferRanges on owned SharedBuffers.
/// Useful to reuse (espeically short-lived) buffers like staging
/// transfer buffers or small buffers like ubos.
/// Keep in mind that all allocated SubBuffers can only be used
/// on the same queue family.
class BufferAllocator : public vpp::Resource {
public:
	using Reservation = vk::DeviceSize;
	using Allocation = std::pair<SharedBuffer&, SharedBuffer::Allocation>;

public:
	BufferAllocator() = default;
	BufferAllocator(const Device& dev);
	~BufferAllocator() = default;

	BufferAllocator(BufferAllocator&&) = default;
	BufferAllocator& operator=(BufferAllocator&&) = default;

	/// Reserves the given requirements.
	/// Useful to allow grouping many SubBuffers on one Buffer.
	/// Optionally returns the id associated with the reservation which can
	/// later be use to allocate or cancel it.
	void reserve(vk::DeviceSize size, vk::BufferUsageFlags,
		unsigned int memBits = ~0u, vk::DeviceSize align = 0u,
		Reservation* id = nullptr);

	/// Allocates a buffer range with the given requirements.
	/// Note that alignment will automatically include physical device
	/// alignment requirements associated with the given usages.
	Allocation alloc(Reservation reservation);
	Allocation alloc(vk::DeviceSize size, vk::BufferUsageFlags,
		unsigned int memBits = ~0u, vk::DeviceSize align = 0u);

	/// Cancels the given reservation.
	void cancel(Reservation reservation);

	const auto& buffers() const { return buffers_; }

protected:
	struct Requirement {
		Reservation id {};
		vk::DeviceSize size {};
		vk::DeviceSize align {};
		vk::BufferUsageFlags usage {};
		unsigned int memBits {};
	};

	struct Buffer {
		Buffer(const Device&, const vk::BufferCreateInfo&, unsigned int mbits);

		SharedBuffer buffer;
		vk::BufferUsageFlags usage {};
	};

	std::deque<Buffer> buffers_;
	std::vector<Requirement> reqs_;
	std::vector<Requirement> reservations_;
	Reservation id_ {};
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
