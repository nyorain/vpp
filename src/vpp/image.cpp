#include <vpp/image.hpp>
#include <utility>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	vk::MemoryRequirements reqs;
	vk::createImage(vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(vkDevice(), image_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(mflags, reqs.memoryTypeBits()));
	device().deviceMemoryAllocator().request(image_, reqs, info.tiling(), memoryEntry_);
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
	memoryEntry_.allocate();
}

void Image::fill(const std::uint8_t& data, std::size_t size, vk::Format format,
	const vk::Extent3D& extent) const
{
	//TODO
}

//static convinience attachment instances
ViewableImage::CreateInfo ViewableImage::defaultDepth2D {
	{
		{},
		vk::ImageType::e2D,
		vk::Format::D16UnormS8Uint,
		{},
		1, 1,
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::Optimal,
		vk::ImageUsageFlagBits::DepthStencilAttachment | vk::ImageUsageFlagBits::Sampled,
		vk::SharingMode::Exclusive,
		0, nullptr, vk::ImageLayout::Undefined
	},
	{
		{}, {},
		vk::ImageViewType::e2D,
		vk::Format::D16UnormS8Uint,
		{},
		{
			vk::ImageAspectFlagBits::Depth | vk::ImageAspectFlagBits::Stencil,
			0, 1, 0, 1
		}
	}
};

ViewableImage::CreateInfo ViewableImage::defaultColor2D {
	{
		{},
		vk::ImageType::e2D,
		vk::Format::B8G8R8A8Unorm,
		{},
		1, 1,
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::Optimal,
		vk::ImageUsageFlagBits::ColorAttachment | vk::ImageUsageFlagBits::InputAttachment,
		vk::SharingMode::Exclusive,
		0, nullptr, vk::ImageLayout::Undefined
	},
	{
		{}, {},
		vk::ImageViewType::e2D,
		vk::Format::B8G8R8A8Unorm,
		{},
		{
			vk::ImageAspectFlagBits::Color,
			0, 1, 0, 1
		}
	}
};

//attachment
ViewableImage::ViewableImage(const Device& dev, const CreateInfo& info)
{
	initMemoryLess(dev, info.imageInfo, info.imageMemoryFlags);
	initMemoryResources(info.viewInfo);
}

ViewableImage::~ViewableImage()
{
	destroy();
}

ViewableImage::ViewableImage(ViewableImage&& other) noexcept
{
	swap(*this, other);
}

ViewableImage& ViewableImage::operator=(ViewableImage&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

void swap(ViewableImage& a, ViewableImage& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.imageView_, b.imageView_);
}

void ViewableImage::destroy()
{
	if(vkImageView()) vk::destroyImageView(vkDevice(), vkImageView(), nullptr);

	image_ = {};
}

void ViewableImage::initMemoryLess(const Device& dev, const vk::ImageCreateInfo& info,
	vk::MemoryPropertyFlags flags)
{
	image_ = Image(dev, info, flags);
}

void ViewableImage::initMemoryResources(vk::ImageViewCreateInfo info)
{
	image_.assureMemory();
	info.image(vkImage());
	vk::createImageView(vkDevice(), &info, nullptr, &imageView_);
}

}
