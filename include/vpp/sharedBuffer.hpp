// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/buffer.hpp>
#include <vpp/resource.hpp>
#include <deque>
#include <variant>

namespace vpp {

/// Like vpp::Buffer but keeps track of sub-allocated BufferRanges.
/// Can be used to share a buffer.
/// See also BufferAllocator.
/// Also implements a mechanism to make sure all allocations are spaced
/// according to the devices nonCoherentAtomAlign property.
/// Keep in mind the vulkan limitations and mechanisms of using buffers
/// with multiple queue families when sharing one.
class VPP_API SharedBuffer : public Buffer {
public:
	using Allocation = BasicAllocation<vk::DeviceSize>;

public:
	SharedBuffer(DeviceMemoryAllocator&, const vk::BufferCreateInfo&,
		unsigned int memBits = ~0u);
	SharedBuffer(DeviceMemoryAllocator&, vk::Buffer, vk::DeviceSize size,
		unsigned int memBits = ~0u);
	SharedBuffer(DeviceMemory&, const vk::BufferCreateInfo&);
	SharedBuffer(InitData&, DeviceMemoryAllocator&,
		const vk::BufferCreateInfo&, unsigned int memBits = ~0u);

	~SharedBuffer();

	/// Not movable to allow SubBuffer to reference it.
	SharedBuffer(SharedBuffer&&) noexcept = delete;
	SharedBuffer& operator=(SharedBuffer&&) noexcept = delete;

	/// Tries to allocate a range with the given size and align.
	/// Returns an empty (i.e. size == 0) allocation if there
	/// is not enough free space left.
	Allocation alloc(vk::DeviceSize size, vk::DeviceSize align = 0u);

	/// Frees the given allocation. Undefined behavior if the
	/// allocation is invalid (i.e. not allocated on this SharedBuffer).
	void free(const Allocation&);

	auto& allocations() { return allocations_; }
	const auto& allocations() const { return allocations_; }
	auto size() const { return size_; }

protected:
	// sorted by position for efficient allocation/release
	std::vector<Allocation> allocations_;
	vk::DeviceSize size_;
};

/// Cases in which you might care for buffer alignment:
///  - use it with a DrawIndirectCommand at the beginning (align 4)
///  - descriptor buffer (align min<BufType>BufferOffsetAlignment)
///  - using it as vertex buffer. Needs component type alignment

/// Allocates BufferRanges on owned SharedBuffers.
/// Useful to reuse (espeically short-lived) buffers like staging
/// transfer buffers or small buffers like ubos.
/// Keep in mind that all allocated SubBuffers can only be used
/// on the same queue family.
class VPP_API BufferAllocator {
public:
	using ReservationID = vk::DeviceSize;

	/// Allocated span on a buffer object.
	struct Allocation {
		SharedBuffer& buffer;
		SharedBuffer::Allocation allocation;
	};

public:
	BufferAllocator() = default;
	BufferAllocator(const Device&); // uses the devices default memory allocator
	BufferAllocator(DeviceMemoryAllocator& allocator);
	~BufferAllocator(); // erase reserved allocations before destroying buffers

	/// Can't be moved since SubBuffers might reference it during
	/// initializaiton.
	BufferAllocator(BufferAllocator&&) = delete;
	BufferAllocator& operator=(BufferAllocator&&) = delete;

	void init(DeviceMemoryAllocator& allocator);

	/// Reserves the given requirements.
	/// Useful to allow grouping many SubBuffers on one Buffer.
	/// Optionally returns the id associated with the reservation which can
	/// later be use to allocate or cancel it.
	[[nodiscard]] ReservationID reserve(vk::DeviceSize size, vk::BufferUsageFlags,
		unsigned int memBits = ~0u, vk::DeviceSize align = 0u);

	/// Allocates a buffer range with the given requirements.
	/// Note that alignment will automatically include physical device
	/// alignment requirements associated with the given usages.
	[[nodiscard]] Allocation alloc(ReservationID reservation);
	[[nodiscard]] Allocation alloc(vk::DeviceSize size, vk::BufferUsageFlags,
		unsigned int memBits = ~0u, vk::DeviceSize align = 0u);

	/// Cancels the given reservation.
	void cancel(ReservationID reservation);

	/// == A lot of things can wrong here, use with caution ==
	/// Tries to move the given allocator into this one,
	/// effectively merging them. This is not only possible when
	/// 'rhs' has no pending reservations/requirements (i.e. no resource still
	/// in initialization referencing it). Returns false if this
	/// is not the case and will not modify rhs in that case.
	/// If the operation can be done, returns false and will effectively
	/// empty/reset rhs.
	/// If 'this' was already initialized, 'this' and 'rhs' must both
	/// use the same vpp::Device. Will not modify the memory types
	/// this allocator is restricted to. Note that if rhs used a different
	/// device memory allocator, this BufferAllocator will get buffers
	/// allocated via that allocator and might allocate SubBuffers on
	/// it in future. Relevant for memory type restrictions.
	/// Can be used as conditional move operator, i.e. works even if
	/// 'this' is uninitialized. 'rhs' must be initialized in any case.
	[[nodiscard]] bool tryMergeBuffers(BufferAllocator&& rhs);

	const auto& buffers() const { return buffers_; }
	const auto& reservations() const { return reservations_; }
	const auto& requirements() const { return requirements_; }

	auto& devMemAllocator() const { return *devMemAlloc_; }
	auto& device() const { return devMemAllocator().device(); }
	bool initialized() const { return bool(devMemAlloc_); }

protected:
	struct Requirement {
		ReservationID id {};
		vk::DeviceSize size {};
		vk::DeviceSize align {};
		vk::BufferUsageFlags usage {};
		unsigned int memBits {};
	};

	struct Reservation {
		ReservationID id {};
		SharedBuffer* buffer {};
		SharedBuffer::Allocation allocation {};
	};

	struct Buffer {
		Buffer(DeviceMemoryAllocator&, vpp::BufferHandle, unsigned int mbits,
			vk::DeviceSize size, vk::BufferUsageFlags usage);

		std::unique_ptr<SharedBuffer> buffer;
		vk::BufferUsageFlags usage {};
	};

protected:
	DeviceMemoryAllocator* devMemAlloc_;

	// all owned shared buffers
	// we use unique_ptrs since SharedBuffer objects can't be moved.
	// std::deque wouldn't allow the tryMerge operation
	std::vector<Buffer> buffers_;
	std::vector<Reservation> reservations_;
	std::vector<Requirement> requirements_;
	ReservationID id_ {}; // last id for counting

	// cache for alloc algorithm
	std::vector<Requirement> tmpRequirements_;
	std::vector<Reservation> tmpReservations_;
};

/// Sub-allocated range of a buffer. See also SharedBuffer.
/// Before destructing a SubBuffer, i.e. before returning that allocated
/// range to the SharedBuffer, it must be ensured that all writes and
/// reads to/from it have finished, usually a pipeline barrier is required.
class VPP_API SubBuffer {
public:
	using Allocation = SharedBuffer::Allocation;
	struct InitData {
		InitData() = default;
		InitData(InitData&&) noexcept;
		InitData& operator=(InitData&&) noexcept;
		~InitData(); // cancels reservation

		BufferAllocator* allocator {};
		BufferAllocator::ReservationID reservation {};
	};

public:
	SubBuffer() = default;

	/// Will automatically make sure that alignment makes sense with
	/// the given usage flags, e.g. will make sure it's at least
	/// minUniformBufferOffsetAlignment of the associated physical device
	/// if usage flags include uniform buffer.
	SubBuffer(BufferAllocator&, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits = ~0u,
		vk::DeviceSize align = 0u);
	SubBuffer(InitData&, BufferAllocator&, vk::DeviceSize size,
		vk::BufferUsageFlags usage, unsigned memBits = ~0u,
		vk::DeviceSize align = 0u);
	SubBuffer(SharedBuffer&, const Allocation& allocation);
	~SubBuffer();

	SubBuffer(SubBuffer&& rhs) noexcept { swap(*this, rhs); }
	SubBuffer& operator=(SubBuffer rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}

	/// When this SubBuffer was constructed with the deferred constructor,
	/// will finish initialization. Otherwise undefined behaviour.
	void init(InitData&);

	const SharedBuffer& buffer() const { return *buffer_; }
	const Allocation& allocation() const { return allocation_; }
	vk::DeviceSize offset() const { return allocation().offset; }
	vk::DeviceSize size() const { return allocation().size; }
	auto end() const { return vpp::end(allocation_); }

	bool mappable() const noexcept { return buffer().mappable(); }
	MemoryMapView memoryMap(vk::DeviceSize offset = 0,
		vk::DeviceSize size = vk::wholeSize) const;

	const Device& device() const { return buffer().device(); }
	friend void swap(SubBuffer&, SubBuffer&) noexcept;

	// NOTE: there is no vkHandle() member or conversion operator
	// to vk::Buffer here since when passing a SubBuffer to vulkan
	// functions you always have to pass the offset as well.
	// A SubBuffer isn't a vk::Buffer, just an owned span on it.

protected:
	SharedBuffer* buffer_ {};
	Allocation allocation_ {};
};

} // namespace vpp
