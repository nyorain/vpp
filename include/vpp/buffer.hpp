#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>

#include <memory>

namespace vpp
{

class Buffer : public Resource
{
protected:
	vk::Buffer buffer_ {};
	std::unique_ptr<DeviceMemory::Entry> memoryEntry_;

protected:
	void destroy();

public:
	Buffer() = default;
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	Buffer(DeviceMemoryAllocator& allctr, const vk::BufferCreateInfo& info,
		vk::MemoryPropertyFlags mflags = {});
	~Buffer();

	Buffer(Buffer&& other) noexcept;
	Buffer& operator=(Buffer&& other) noexcept;

	void swap(Buffer& other) noexcept;

	const DeviceMemory::Entry& memoryEntry() const { return *memoryEntry_; }
	vk::Buffer vkBuffer() const { return buffer_; }
	MemoryMap memoryMap() const;
};

};
