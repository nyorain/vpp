// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>

#include <memory>
#include <type_traits>
#include <cstring>
#include <cmath>

namespace vpp {

/// Representing a vulkan buffer on a device.
/// Can be filled and read, and stores a handle to the memory location it is allocated on (or
/// the allocator that will take care of its allocation).
/// Does not store additional information such as buffer usage type or memory layout, this
/// must be handled by the application for best performance.
class Buffer : public MemoryResource<vk::Buffer> {
public:
	Buffer() = default;
	Buffer(const Device&, const vk::BufferCreateInfo&, vk::MemoryPropertyFlags = {});
	Buffer(const Device&, const vk::BufferCreateInfo&, unsigned int memoryTypesBits);
	Buffer(const Device&, vk::Buffer, vk::BufferUsageFlags, unsigned int memoryTypeBits);
	Buffer(const Device&, vk::Buffer, vk::BufferUsageFlags, vk::MemoryPropertyFlags = {});
	Buffer(vk::Buffer buffer, MemoryEntry&& entry);
	~Buffer();

	Buffer(Buffer&& other) noexcept = default;
	Buffer& operator=(Buffer&& other) noexcept = default;
};

} // namespace vpp
