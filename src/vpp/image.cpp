// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/image.hpp>
#include <vpp/formats.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// ImageHandle
ImageHandle::ImageHandle(const Device& dev, const vk::ImageCreateInfo& info) :
	ResourceHandle(dev, vk::createImage(dev, info))
{
}

ImageHandle::ImageHandle(const Device& dev, vk::Image buf) :
	ResourceHandle(dev, buf)
{
}

ImageHandle::~ImageHandle()
{
	if(vkHandle()) {
		vk::destroyImage(device(), vkHandle());
	}
}

// Image
Image::Image(const Device& dev, const vk::ImageCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Image(dev, vk::createImage(dev, info), info.tiling, memBits, alloc)
{
}

Image::Image(const Device& dev, vk::Image image, vk::ImageTiling tiling,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Image(defer, dev, image, tiling, memBits, alloc)
{
	ensureMemory();
}

Image::Image(const Device& dev, const vk::ImageCreateInfo& info,
	DeviceMemory& mem) :
		ImageHandle(dev, vk::createImage(dev, info))
{
	auto reqs = vk::getImageMemoryRequirements(dev, vkHandle());
	dlg_assertm(reqs.memoryTypeBits & (1 << mem.type()), "Invalid memory type");

	auto allocType = info.tiling == vk::ImageTiling::linear ?
		AllocationType::linear : AllocationType::optimal;
	auto alloc = mem.alloc(reqs.size, reqs.alignment, allocType);
	if(alloc.size == 0) {
		throw std::runtime_error("Failed to alloc from memory");
	}

	memoryEntry_ = {mem, alloc};
}

Image::Image(const Device& dev, vk::Image image, MemoryEntry&& entry) :
	ImageHandle(dev, image), MemoryResource(std::move(entry))
{
	dlg_assert(memoryEntry().allocated());
}

Image::Image(DeferTag, const Device& dev, vk::Image image,
	vk::ImageTiling tiling, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		ImageHandle(dev, image)
{
	dlg_assert(image);
	dlg_assert(memBits);

	alloc = alloc ? alloc : &dev.deviceAllocator();
	auto reqs = vk::getImageMemoryRequirements(dev, vkHandle());
	reqs.memoryTypeBits &= memBits;
	dlg_assertm(reqs.memoryTypeBits, "Image: No memory type bits left");
	dlg_assert(reqs.size > 0);

	dev.deviceAllocator().request(vkHandle(), reqs, tiling, memoryEntry_);
}

Image::Image(DeferTag, const Device& dev, const vk::ImageCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Image(defer, dev, vk::createImage(dev, info), info.tiling, memBits, alloc)
{
}

// ImageView
ImageView::ImageView(const Device& dev, const vk::ImageViewCreateInfo& info) :
	ImageView(dev, vk::createImageView(dev, info))
{
}

ImageView::ImageView(const Device& dev, vk::ImageView view) :
	ResourceHandle(dev, view)
{
}

ImageView::~ImageView()
{
	if(vkHandle()) {
		vk::destroyImageView(device(), vkHandle());
	}
}

// ViewableImage
ViewableImage::ViewableImage(const Device& dev,
	const vk::ImageCreateInfo& imgInfo, const vk::ImageViewCreateInfo& viewInfo,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		ViewableImage(Image{dev, imgInfo, memBits, alloc}, viewInfo)
{
}

ViewableImage::ViewableImage(const Device& dev,
	const ViewableImageCreateInfo& info, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		ViewableImage(dev, info.img, info.view, memBits, alloc)
{
}

ViewableImage::ViewableImage(Image&& image, const vk::ImageViewCreateInfo& info) :
	image_(std::move(image))
{
	dlg_assert(image_.vkHandle());
	init(info);
}

ViewableImage::ViewableImage(Image&& image, ImageView&& view) :
	image_(std::move(image)), imageView_(std::move(view))
{
}

ViewableImage::ViewableImage(DeferTag, const Device& dev,
	const vk::ImageCreateInfo& info, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) : image_(dev, info, memBits, alloc)
{
}

ViewableImage::ViewableImage(DeferTag, Image&& image) : image_(std::move(image))
{
}

void ViewableImage::init(vk::ImageViewCreateInfo info)
{
	dlg_assert(image_.vkHandle());
	dlg_assert(!imageView_.vkHandle());

	image_.ensureMemory();
	info.image = vkImage();
	imageView_ = {device(), info};
}

// Sampler
Sampler::Sampler(const Device& dev, const vk::SamplerCreateInfo& info) :
	Sampler(dev, vk::createSampler(dev, info))
{
}

Sampler::Sampler(const Device& dev, vk::Sampler sampler) :
	ResourceHandle(dev, sampler)
{
}

Sampler::~Sampler()
{
	if(vkHandle()) {
		vk::destroySampler(device(), vkHandle());
	}
}

} // namespace vpp
