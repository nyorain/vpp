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

	reqs.memoryTypeBits(device().memoryTypeBits(mflags, reqs.memoryTypeBits()));
	device().deviceMemoryAllocator().request(buffer_, reqs, *memoryEntry_);
}

Buffer::Buffer(Buffer&& other) noexcept
{
	swap(*this, other);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

Buffer::~Buffer()
{
	destroy();
}

void swap(Buffer& a, Buffer& b) noexcept
{
	using std::swap;

	swap(b.memoryEntry_, a.memoryEntry_);
	swap(b.buffer_, a.buffer_);
	swap(b.device_, a.device_);
}

void Buffer::destroy()
{
	if(vkBuffer()) vk::destroyBuffer(vkDevice(), vkBuffer(), nullptr);

	memoryEntry_ = {};
	buffer_ = {};
	Resource::destroy();
}

MemoryMapView Buffer::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
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
