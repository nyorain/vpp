// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/image.hpp>
#include <vpp/formats.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// Image
Image::Image(const Device& dev, const vk::ImageCreateInfo& info,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		Image(dev, vk::createImage(dev, info), info.tiling, memBits, alloc) {
}

Image::Image(const Device& dev, vk::Image image, vk::ImageTiling tiling,
		unsigned int memBits, vpp::DeviceMemoryAllocator* alloc)  {
	InitData data;
	*this = {data, dev, image, tiling, memBits, alloc};
	init(data);
}

Image::Image(const Device& dev, const vk::ImageCreateInfo& info,
		DeviceMemory& mem) : ImageHandle(dev, vk::createImage(dev, info)) {
	auto reqs = vk::getImageMemoryRequirements(dev, vkHandle());
	dlg_assertm(reqs.memoryTypeBits & (1 << mem.type()), "Invalid memory type");

	auto allocType = info.tiling == vk::ImageTiling::linear ?
		AllocationType::linear : AllocationType::optimal;
	auto alloc = mem.alloc(reqs.size, reqs.alignment, allocType);
	if(alloc.size == 0) {
		throw std::runtime_error("Failed to alloc from memory");
	}

	memory_ = &mem;
	offset_ = alloc.offset;
}

Image::Image(const Device& dev, vk::Image image, DeviceMemory& memory,
	vk::DeviceSize memOffset) :
		ImageHandle(dev, image),
		MemoryResource(memory, memOffset) {
}

Image::Image(InitData& data, const Device& dev, vk::Image image,
	vk::ImageTiling tiling, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		ImageHandle(dev, image) {
	dlg_assert(image);
	dlg_assert(memBits);

	auto reqs = vk::getImageMemoryRequirements(dev, vkHandle());
	reqs.memoryTypeBits &= memBits;
	dlg_assertm(reqs.memoryTypeBits, "Image: No memory type bits left");
	dlg_assert(reqs.size > 0);
	auto type = tiling == vk::ImageTiling::linear ?
		AllocationType::linear : AllocationType::optimal;

	data.allocator = alloc ? alloc : &dev.deviceAllocator();
	data.allocator->reserve(type, reqs, &data.reservation);
}

Image::Image(InitData& data, const Device& dev, const vk::ImageCreateInfo& info,
	unsigned int mbits, vpp::DeviceMemoryAllocator* alloc) :
		Image(data, dev, vk::createImage(dev, info), info.tiling, mbits, alloc) {
}

void Image::init(InitData& data) {
	MemoryResource::init(data);
	vk::bindImageMemory(device(), vkHandle(), memory(), memoryOffset());
}

// ViewableImage
ViewableImage::ViewableImage(const Device& dev,
	const vk::ImageCreateInfo& imgInfo, const vk::ImageViewCreateInfo& viewInfo,
	unsigned int memBits, vpp::DeviceMemoryAllocator* alloc) :
		ViewableImage(Image{dev, imgInfo, memBits, alloc}, viewInfo) {
}

ViewableImage::ViewableImage(const Device& dev,
	const ViewableImageCreateInfo& info, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		ViewableImage(dev, info.img, info.view, memBits, alloc) {
}

ViewableImage::ViewableImage(Image&& img, vk::ImageViewCreateInfo ivi) :
		image_(std::move(img)) {
	dlg_assert(image_.vkHandle());
	ivi.image = image_;
	imageView_ = {device(), ivi};
}

ViewableImage::ViewableImage(Image&& image, ImageView&& view) :
	image_(std::move(image)), imageView_(std::move(view)) {
}

ViewableImage::ViewableImage(InitData& data, const Device& dev,
	const vk::ImageCreateInfo& info, unsigned int memBits,
	vpp::DeviceMemoryAllocator* alloc) :
		image_(data, dev, info, memBits, alloc) {
}

ViewableImage::ViewableImage(InitData&, Image&& img) : image_(std::move(img)) {
}

void ViewableImage::init(InitData& data, vk::ImageViewCreateInfo ivi) {
	dlg_assert(image_.vkHandle());
	dlg_assert(!imageView_.vkHandle());

	image_.init(data);
	ivi.image = image_;
	imageView_ = {device(), ivi};
}

} // namespace vpp
