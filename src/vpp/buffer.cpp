#include <vpp/buffer.hpp>
#include <utility>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	vk::MemoryRequirements reqs;
	vk::createBuffer(vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(vkDevice(), buffer_, &reqs);

	auto type = device().memoryType(reqs.memoryTypeBits(), mflags);
	if(type == -1)
	{
		throw std::runtime_error("vpp::Image: no matching deviceMemoryType");
	}

	auto memory = std::make_shared<DeviceMemory>(dev, type, reqs.size());

	auto alloc = memory->alloc(reqs.size(), reqs.alignment());
	memoryEntry_ = DeviceMemory::Entry(memory, alloc);

	vk::bindBufferMemory(vkDevice(), buffer_, memory->vkDeviceMemory(), alloc.offset);
}

Buffer::Buffer(DeviceMemoryAllocator& allctr, const vk::BufferCreateInfo& info,
	vk::MemoryPropertyFlags mflags) : Resource(allctr.device())
{
	vk::MemoryRequirements reqs;
	vk::createBuffer(vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(vkDevice(), buffer_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(reqs.memoryTypeBits(), mflags));
	allctr.request(buffer_, reqs, memoryEntry_);
}

Buffer::Buffer(Buffer&& other) noexcept
{
	this->swap(other);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	Buffer swapper(std::move(other));
	this->swap(swapper);
	return *this;
}

Buffer::~Buffer()
{
	destroy();
}

void Buffer::swap(Buffer& other) noexcept
{
	using std::swap;

	swap(memoryEntry_, other.memoryEntry_);
	swap(buffer_, other.buffer_);
	swap(device_, other.device_);
}

void Buffer::destroy()
{
	if(vkBuffer()) vk::destroyBuffer(vkDevice(), vkBuffer(), nullptr);

	memoryEntry_ = {};
	buffer_ = {};
}

MemoryMap Buffer::memoryMap() const
{
	return MemoryMap(memoryEntry_);
}

}
