#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>

namespace vpp
{

class Buffer : public Resource
{
protected:
	vk::Buffer buffer_ {};
	DeviceMemory::Entry memoryEntry_;

protected:
	void destroy();

public:
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	Buffer(DeviceMemoryAllocator& allctr, const vk::BufferCreateInfo& info,
		vk::MemoryPropertyFlags mflags = {});
	~Buffer();

	Buffer(Buffer&& other);
	Buffer& operator=(Buffer&& other);

	const DeviceMemory::Entry& memoryEntry() const { return memoryEntry_; }
	vk::Buffer vkBuffer() const { return buffer_; }
	MemoryMap memoryMap() const;
};

};