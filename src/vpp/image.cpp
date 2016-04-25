#include <vpp/image.hpp>
#include <utility>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	memoryEntry_.reset(new DeviceMemoryAllocator::Entry());

	vk::MemoryRequirements reqs;
	vk::createImage(vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(vkDevice(), image_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(mflags, reqs.memoryTypeBits()));
	device().deviceMemoryAllocator().request(image_, reqs, info.tiling(), *memoryEntry_);
}

Image::Image(Image&& other) noexcept
{
	this->swap(other);
}

Image& Image::operator=(Image&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

Image::~Image()
{
	destroy();
}

void Image::swap(Image& other) noexcept
{
	using std::swap;

	swap(image_, other.image_);
	swap(memoryEntry_, other.memoryEntry_);
	swap(device_, other.device_);
}

void Image::destroy()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage(), nullptr);

	memoryEntry_ = {};
	image_ = {};
	Resource::destroy();
}

MemoryMap Image::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
}

void Image::assureMemory() const
{
	memoryEntry_->allocate();
}

}
