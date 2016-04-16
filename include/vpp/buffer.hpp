#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>

#include <memory>

namespace vpp
{

///Utility class for filling buffers.
class BufferData
{
public:
	const void* data = nullptr;
	std::size_t size = 0;
	std::size_t offset = 0;

public:
	BufferData() = default;

	template<typename T> BufferData(const T& obj, std::size_t xoffset = 0)
		: data(&obj), size(sizeof(T)), offset(xoffset) {}
};

///Representing a vulkan buffer on a device.
class Buffer : public Resource
{
protected:
	vk::Buffer buffer_ {};
	std::unique_ptr<DeviceMemoryAllocator::Entry> memoryEntry_;

protected:
	void destroy();

public:
	Buffer() = default;
	Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	~Buffer();

	Buffer(Buffer&& other) noexcept;
	Buffer& operator=(Buffer&& other) noexcept;

	void swap(Buffer& other) noexcept;

	const DeviceMemoryAllocator::Entry& memoryEntry() const { return *memoryEntry_; }
	vk::Buffer vkBuffer() const { return buffer_; }

	///Assures that there is device memory associated with this buffer.
	///Will be implicitly called on member functions that require it.
	void assureMemory() const;

	///Returns a vulkan memory map guard. Should only be called when buffer was created on a
	///host visible device memory heap and if the device memory was allocated.
	MemoryMap memoryMap() const;

	///Fills the buffer with the given data.
	///Does this either by memory mapping the buffer or by copying it via command buffer.
	///Should only be called if the device memory for the buffer was allocated.
	///Expects that buffer was created fillable, so either the buffer is memory mappable or
	///it is allowed to copy data into it and the device was created with a matching queue.
	///Note that this operation may be asnyc, so shall call Device::finishSetup to make sure the
	///buffer is really filled with the given data.
	void fill(const std::vector<BufferData>& data) const;
};

};
