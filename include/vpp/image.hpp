#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp>
#include <vpp/allocator.hpp>
#include <vpp/work.hpp>
#include <vpp/vulkan/structs.hpp>

#include <memory>

namespace vpp
{

///Returns the size in bytes of the given format.
///E.g. vk::Format::r8g8b8a8* will return 4, since it has a size of 4 * 8 bits = 32 bits = 4 bytes.
///For compressed formats this function will return the size of one block in bytes.
///\sa blockSize
unsigned int formatSize(vk::Format format);

///Returns the size of one compressed block of a compressed vulkan format.
///If the given format is not a compressed format, {1, 1} is returned.
///For vk::Format::undefined, {0, 0} is returned
///\sa formatSize
vk::Extent2D blockSize(vk::Format format);


///Representing a vulkan image on a device and having its own memory allocation bound to it.
///The Image class does not store further information like size, type, format or layout.
///All of this must be handled by the application to guarantee the best performance.
class Image : public MemoryResource<vk::Image>
{
public:
	Image() = default;
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	Image(const Device& dev, const vk::ImageCreateInfo& info, std::uint32_t memoryTypeBits);
	~Image();

	Image(Image&& other) noexcept = default;
	Image& operator=(Image&& other) noexcept = default;
};

//TODO: allow compressed formats. Some way to allow filling a compressed image with
//	uncompressed data? doing the compressing on the gpu?

///Fills the given image with data.
///There are two different methods for filling an image: memoryMap and transfer.
///MemoryMap is used if the image is mappable and the allowMap param is true, otherwise
///the transferMethod is used.
///Some of the parameters are only needed for one of the two methods. If it is certain
///which method is used before calling this function they can be set to any value.
///\param image The image to retrieve the data from. Must have linear tiling and a non-compressed
///format. Must not be multisampled and either be created on host visible memory of with the
///transferDst usage bit set.
///\param data Tightly packed data.
///The data must be in row-major order and large enough for the given extent.
///The size of data will be expected to be extent.w * extent.h * extent.d * formatSize(format).
///\param format The images format. Only important for the size, so if the images format
///is r8g8b8a8*, passing a8b8g8r8* as format is fine.
///\param layout The layout of the image when this work will be submitted.
///Only needed if the image is retrieved per transfer. If the layout is not transferDstOptimal
///or the generel it will be changed to transferDstOptimal.
///\param extent The size of the image region to fill. Note that offset + extent must not be larger
///than the images extent.
///\param subres The Subresource to fill.
///\param offset The offset of the image region to fill. By default no offset.
///\param allowMap If set to false, the image fill always be filled using a transfer command
///rather than mapping its memory. Needed e.g. if the image has an optimal tiling since
///optimal tiling images cannot be filled using memory maps. True by default.
WorkPtr fill(const Image& image, const std::uint8_t& data, vk::Format format,
	vk::ImageLayout layout, const vk::Extent3D& extent, const vk::ImageSubresource& subres,
	const vk::Offset3D& offset = {}, bool allowMap = true);

///Retrieves the data from the given image.
///The image must be either allocated on host visible memory or must have the transferSrc bit set
///as usage and must not be multisampled.
///\param allowMap If set to false, the image fill always be filled using a transfer command
///rather than mapping its memory. Needed e.g. if the image has an optimal tiling.
DataWorkPtr retrieve(const Image& image, vk::ImageLayout layout, vk::Format format,
	const vk::Extent3D& extent, const vk::ImageSubresource& subres,
	const vk::Offset3D& offset = {}, bool allowMap = true);


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
	struct CreateInfo
	{
		vk::ImageCreateInfo imgInfo;
		vk::ImageViewCreateInfo viewInfo;
		vk::MemoryPropertyFlags memoryFlags; //memory flags for image
	};

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
	vk::Image vkImage() const { return image(); }

	const Image& resourceRef() const { return image_; }
	friend void swap(ViewableImage& a, ViewableImage& b) noexcept;

protected:
	Image image_;
	vk::ImageView imageView_ {};
};

///Sampler wrapper.
class Sampler : public ResourceHandle<vk::Sampler>
{
public:
	Sampler() = default;
	Sampler(const Device& dev, const vk::SamplerCreateInfo& info);
	~Sampler();

	Sampler(Sampler&& other) noexcept = default;
	Sampler& operator=(Sampler&& other) noexcept = default;
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
