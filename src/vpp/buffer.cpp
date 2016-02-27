#include <vpp/buffer.hpp>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	vk::MemoryRequirements reqs;
	vk::createBuffer(vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(vkDevice(), buffer_, &reqs);

	auto type = device().memoryType(reqs.memoryTypeBits(), mflags);
	auto memory = std::make_shared<DeviceMemory>(dev, type, reqs.size());

	auto alloc = memory.alloc(reqs.allocationSize(), reqs.alignment());
	memoryEntry_ = DeviceMemory::Entry(std::move(memory), alloc);
}

Buffer::Buffer(DeviceMemoryAllocator& allctr, const vk::ImageCreateInfo& info,
	vk::MemoryPropertyFlags mflags) : Resource(allctr.device())
{
	vk::MemoryRequirements reqs;
	vk::createBuffer(vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(vkDevice(), buffer_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(reqs.memoryTypeBits(), mflags));
	allctr.request(buffer_, rqes, memoryEntry_);
}

Buffer::~Buffer()
{
	if(vkBuffer()) vk::destroyBuffer(vkDevice(), vkImage(), nullptr);
}

MemoryMap Buffer::memoryMap() const
{
	return MemoryMap(memoryEntry_);
}

}
