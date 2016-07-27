#include <vpp/buffer.hpp>
#include <vpp/vk.hpp>

#include <utility>
#include <cstring>

namespace vpp
{

Buffer::Buffer(const Device& dev, vk::Buffer buffer, vk::BufferUsageFlags usage,
	vk::MemoryPropertyFlags mflags)
{
	vkHandle() = buffer;
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits = dev.memoryTypeBits(mflags, reqs.memoryTypeBits);
	dev.deviceAllocator().request(vkHandle(), reqs, usage, memoryEntry_);
}

Buffer::Buffer(const Device& dev, vk::Buffer buffer, vk::BufferUsageFlags usage,
	std::uint32_t memoryTypeBits)
{
	vkHandle() = buffer;
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits &= reqs.memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, usage, memoryEntry_);
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	vkHandle() = vk::createBuffer(dev, info);
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits = dev.memoryTypeBits(mflags, reqs.memoryTypeBits);
	dev.deviceAllocator().request(vkHandle(), reqs, info.usage, memoryEntry_);
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, std::uint32_t memoryTypeBits)
{
	vkHandle() = vk::createBuffer(dev, info);
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, info.usage, memoryEntry_);
}

Buffer::~Buffer()
{
	if(vkHandle()) vk::destroyBuffer(device(), vkHandle());
}


}
