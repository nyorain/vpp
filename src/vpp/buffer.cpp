#include <vpp/buffer.hpp>
#include <utility>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	memoryEntry_.reset(new DeviceMemoryAllocator::Entry());

	vk::MemoryRequirements reqs;
	vk::createBuffer(vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(vkDevice(), buffer_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(reqs.memoryTypeBits(), mflags));
	device().deviceMemoryAllocator().request(buffer_, reqs, *memoryEntry_);
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
	Resource::destroy();
}

MemoryMap Buffer::memoryMap() const
{
	assureMemory();
	return MemoryMap(*memoryEntry_);
}

void Buffer::fill(const std::vector<BufferData>& data) const
{
	assureMemory();
	//TODO
}

void Buffer::assureMemory() const
{
	memoryEntry_->allocate();
}

}
