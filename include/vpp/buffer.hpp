#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>

#include <memory>
#include <type_traits>
#include <cstring>
#include <cmath>

namespace vpp
{

///Representing a vulkan buffer on a device.
///Can be filled and read, and stores a handle to the memory location it is allocated on (or
///the allocator that will take care of its allocation).
///Does not store additional information such as buffer usage type or memory layout, this
///must be handled by the application for best performance.
class Buffer : public MemoryResource<vk::Buffer>
{
public:
	Buffer() = default;
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, std::uint32_t memoryTypesBits);
	Buffer(const Device& dev, const vk::BufferCreateInfo& info,
		vk::MemoryPropertyFlags mflags = {});
	~Buffer();

	Buffer(Buffer&& other) noexcept = default;
	Buffer& operator=(Buffer&& other) noexcept = default;
};


};
