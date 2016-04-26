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
	swap(*this, other);
}

Image& Image::operator=(Image&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

Image::~Image()
{
	destroy();
}

void swap(Image& a, Image& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.memoryEntry_, b.memoryEntry_);
	swap(a.device_, b.device_);
}

void Image::destroy()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage(), nullptr);

	memoryEntry_ = {};
	image_ = {};
	Resource::destroy();
}

MemoryMapView Image::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
}

void Image::assureMemory() const
{
	memoryEntry_->allocate();
}

}
