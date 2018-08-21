// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp> // vpp::MemoryResource

namespace vpp {

/// RAII vk::Buffer wrapper.
/// Does not handle any additional functionality
class BufferHandle : public ResourceHandle<vk::Buffer> {
public:
	BufferHandle() = default;
	BufferHandle(const Device&, const vk::BufferCreateInfo&);
	BufferHandle(const Device&, vk::Buffer);
	~BufferHandle();

	BufferHandle(BufferHandle&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(BufferHandle rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// Combines Buffer and MemoryResource, i.e. a buffer that knows
/// its bound memory. Does not support sparse memory bindings.
class Buffer : public BufferHandle, public MemoryResource {
public:
	Buffer() = default;

	/// The various types of constructors:
	/// - Transfer ownership vs create
	///   * (1,3,5): BufferCreateInfo parameter: create a new buffer
	///   * (2,4,6): vk::Buffer parameter: transfer ownerhip of existent buffer
	///     Note that for (2,6), the buffer must NOT already be bound to memory
	/// - Allocate mechanism. You can either use/pass
	///   * (1,2): using a DeviceMemoryAllocator, if none is given using
	///     the default threadlocal allocator of the given device.
	///     Guarantees that the memory is allocated on a memory type
	///     contained in memBits (e.g. to assure it's allocated
	///     on hostVisible memory).
	///   * (3): allocate on a specific DeviceMemory object.
	///     Will throw std::runtime_error if the DeviceMemory fails
	///     to allocate enough memory. The DeviceMemory must
	///     be allocated on a type that is supported for the
	///     created buffer (the vulkan spec gives some guarantess there).
	///   * (4): Will pass ownership of the memory entry which must be
	///     in allocated state and bound to the buffer.
	/// - Deferred? See the vpp doc for deferred initialization
	///   * (1,2,3) bind the buffer immediately to memory. For (1,2) this
	///     means to immediately request memory, which might result
	///     in a vkAllocateMemory call
	///   * (5, 6) does not bind the buffer to memory, only when
	///     ensureMemory is called. Already issues a reserving request
	///     to the DeviceMemoryAllocator, might result in less
	///     memory allocations made if multiple resources are created deferred.
	Buffer(const Device&, const vk::BufferCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	Buffer(const Device&, vk::Buffer, unsigned int memBits = ~0u,
		vpp::DeviceMemoryAllocator* = {});

	Buffer(const Device&, const vk::BufferCreateInfo&, DeviceMemory&);
	Buffer(const Device&, vk::Buffer, MemoryEntry&&);

	/// Creates the buffer without any bound memory.
	/// You have to call the ensureMemory function later on to
	/// make sure memory is bound to the buffer.
	Buffer(DeferTag, const Device&, const vk::BufferCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	Buffer(DeferTag, const Device&, vk::Buffer, unsigned int memBits = ~0u,
		vpp::DeviceMemoryAllocator* = {});

	Buffer(Buffer&& rhs) noexcept = default;
	Buffer& operator=(Buffer&& rhs) noexcept = default;

	/// To be called when the buffer was initialized with
	/// a deferred constructor. Will make sure the buffer
	/// has bound memory.
	void init() { ensureMemory(); }
};

/// RAII wrapper around a vulkan buffer view.
class BufferView : public ResourceHandle<vk::BufferView> {
public:
	BufferView() = default;
	BufferView(const Device&, const vk::BufferViewCreateInfo&);
	BufferView(const Device&, vk::BufferView);
	~BufferView();

	BufferView(BufferView&& rhs) noexcept { swap(*this, rhs); }
	BufferView& operator=(BufferView rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

} // namespace vpp
