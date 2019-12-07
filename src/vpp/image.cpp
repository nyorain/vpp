// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/image.hpp>
#include <vpp/debug.hpp>
#include <vpp/formats.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// Image
Image::Image(DeviceMemoryAllocator& alloc, const vk::ImageCreateInfo& info,
	unsigned int memBits) : Image(alloc,
		vk::createImage(alloc.device(), info), info.tiling, memBits) {
}

Image::Image(DeviceMemoryAllocator& alloc, vk::Image image,
		vk::ImageTiling tiling, unsigned int memBits) {
	InitData data;
	*this = {data, alloc, image, tiling, memBits};
	init(data);
}

Image::Image(DeviceMemory& mem, const vk::ImageCreateInfo& info) :
		ImageHandle(mem.device(), vk::createImage(mem.device(), info)) {
	auto reqs = vk::getImageMemoryRequirements(mem.device(), vkHandle());
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

Image::Image(DeviceMemory& mem, vk::Image image, vk::DeviceSize memOffset) :
		ImageHandle(mem.device(), image),
		MemoryResource(mem, memOffset) {
}

Image::Image(InitData& data, DeviceMemoryAllocator& alloc, vk::Image image,
	vk::ImageTiling tiling, unsigned int memBits) :
		ImageHandle(alloc.device(), image) {
	dlg_assert(image);
	dlg_assert(memBits);

	auto reqs = vk::getImageMemoryRequirements(alloc.device(), vkHandle());
	reqs.memoryTypeBits &= memBits;
	dlg_assertm(reqs.memoryTypeBits, "Image: No memory type bits left");
	dlg_assert(reqs.size > 0);
	auto type = tiling == vk::ImageTiling::linear ?
		AllocationType::linear : AllocationType::optimal;

	data.allocator = &alloc;
	data.allocator->reserve(type, reqs, &data.reservation);
}

Image::Image(InitData& data, DeviceMemoryAllocator& alloc,
	const vk::ImageCreateInfo& info, unsigned int mbits) : Image(data, alloc,
		vk::createImage(alloc.device(), info), info.tiling, mbits) {
}

void Image::init(InitData& data) {
	MemoryResource::init(data);
	vk::bindImageMemory(device(), vkHandle(), memory(), memoryOffset());
}

// ViewableImage
ViewableImage::ViewableImage(DeviceMemoryAllocator& alloc,
	const vk::ImageCreateInfo& imgInfo, const vk::ImageViewCreateInfo& viewInfo,
	unsigned int memBits) :
		ViewableImage(Image{alloc, imgInfo, memBits}, viewInfo) {
}

ViewableImage::ViewableImage(DeviceMemoryAllocator& alloc,
	const ViewableImageCreateInfo& info, unsigned int memBits) :
		ViewableImage(alloc, info.img, info.view, memBits) {
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

ViewableImage::ViewableImage(InitData& data, DeviceMemoryAllocator& alloc,
	const vk::ImageCreateInfo& info, unsigned int memBits) :
		image_(data, alloc, info, memBits) {
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

inline namespace debug {

vk::Result nameHandle(const ViewableImage& vi, std::string name) {
	auto pos = name.length();
	name += ".image";
	if(auto res = nameHandle(vi.image(), name.c_str());
			res != vk::Result::success) {
		return res;
	}

	name.erase(pos);
	name += ".view";
	return nameHandle(vi.imageView(), name.c_str());
}

} // namespace debug
} // namespace vpp
