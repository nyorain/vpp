#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>
#include <vpp/allocator.hpp>
#include <vpp/work.hpp>

#include <memory>

namespace vpp
{


///Representing a vulkan image on a device and having its own memory allocation bound to it.
///The Image class does not store further information like size, type, format or layout.
///All of this must be handled by the application to guarantee the best performance.
class Image : public MemoryResource
{
public:
	Image() = default;
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	Image(const Device& dev, const vk::ImageCreateInfo& info, std::uint32_t memoryTypeBits);
	~Image();

	Image(Image&& other) noexcept;
	Image& operator=(Image other) noexcept;

	const vk::Image& vkImage() const { return image_; }

	operator vk::Image() const { return vkImage(); }
	friend void swap(Image& a, Image& b) noexcept;

protected:
	vk::Image image_ {};
};

///Fills the given image with data.
///The image must be either allocated on host visible memory or must have the transferDst bit set
///as usage and must not be multisampled.
///\param dataSize The size of the given data. If vk::wholeSize, it will be assumed to be as large
///as the images memory size - offset.
///\param data Tightly packed data
///The data must be in linear order, independent from the images tiling.
///\param allowMap If set to false, the image fill always be filled using a transfer command
///rather than mapping its memory. Needed e.g. if the image has an optimal tiling.
WorkPtr fill(const Image& image, const std::uint8_t& data, vk::ImageLayout layout,
	const vk::Extent3D& extent, const vk::ImageSubresourceLayers& subres,
	vk::DeviceSize dataSize = vk::wholeSize, bool allowMap = true);

///Retrieves the data from the given image.
///The image must be either allocated on host visible memory or must have the transferSrc bit set
///as usage and must not be multisampled.
///\param allowMap If set to false, the image fill always be filled using a transfer command
///rather than mapping its memory. Needed e.g. if the image has an optimal tiling.
DataWorkPtr retrieve(const Image& image, vk::ImageLayout layout, const vk::Extent3D& extent,
	const vk::ImageSubresourceLayers& subres, bool allowMap = true);


///XXX: rather use vk::ImageSubresource param instead of only aspects and guessing
///Records the command for changing an image layout.
///\param cmdBuffer Command buffer which must be in recording state
void changeLayoutCommand(vk::CommandBuffer cmdBuffer, vk::Image img, vk::ImageLayout ol,
	vk::ImageLayout nl, vk::ImageAspectFlags aspects);

///\{
///Changes the layout of a given vulkan image and returns the associated work ptr.
WorkPtr changeLayout(const Device& dev, vk::Image img, vk::ImageLayout ol, vk::ImageLayout nl,
	vk::ImageAspectFlags aspect);
inline WorkPtr changeLayout(const Image& img, vk::ImageLayout ol, vk::ImageLayout nl,
	vk::ImageAspectFlags aspect) { return changeLayout(img.device(), img, ol, nl, aspect); }
///\}


///Viewable image, can be e.g. used as framebuffer attachment.
class ViewableImage : public ResourceReference<ViewableImage>
{
public:
	struct CreateInfo;

	///\{
	///Some useful default create infos.
	///Can e.g. be used for framebuffers or texures.
	///Note that some properties like e.g. the size must be set manually afterwards.
	static CreateInfo defaultDepth2D();
	static CreateInfo defaultColor2D();
	///\}

public:
	ViewableImage() = default;
	ViewableImage(const Device& dev, const CreateInfo& info);
	~ViewableImage();

	ViewableImage(ViewableImage&& other) noexcept;
	ViewableImage& operator=(ViewableImage other) noexcept;

	void create(const Device& dev, const vk::ImageCreateInfo& img, vk::MemoryPropertyFlags flgs = {});
	void init(const vk::ImageViewCreateInfo& info);

	const Image& image() const { return image_; }
	vk::ImageView vkImageView() const { return imageView_; }
	vk::Image vkImage() const { return image_.vkImage(); }

	const Image& resourceRef() const { return image_; }
	friend void swap(ViewableImage& a, ViewableImage& b) noexcept;

protected:
	Image image_;
	vk::ImageView imageView_ {};
};

///TODO:
///Vulkan image view.
class ImageView : public Resource
{
public:
protected:
	vk::ImageView imageView_ {};
};

};
