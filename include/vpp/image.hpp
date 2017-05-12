// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp> // vpp::MemoryResource
#include <vpp/work.hpp> // vpp::WorkPtr
#include <vpp/vulkan/structs.hpp> // vk::ImageCreateInfo

// TODO: fill/retrieve:
//  - allow compressed formats. Some way to allow filling a compressed image with
//	  uncompressed data? doing the compressing on the gpu?
// - parameter for preseving the layout, i.e. changing its back to the
//   original value if it has to be chagned

namespace vpp {

/// Representing a vulkan image on a device and having its own memory allocation bound to it.
/// The Image class does not store further information like size, type, format or layout.
/// All of this must be handled by the application to guarantee the best performance.
class Image : public MemoryResource<vk::Image> {
public:
	Image() = default;
	Image(const Device&, const vk::ImageCreateInfo&, unsigned int memoryTypeBits = ~0u);
	Image(const Device&, vk::Image, vk::ImageTiling, unsigned int memoryTypeBits = ~0u);
	Image(vk::Image, MemoryEntry&&);
	~Image();

	Image(Image&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(Image rhs) noexcept { swap(*this, rhs); return *this; }
};

/// Returns the size of the given format in bits.
/// E.g. vk::Format::r8g8b8a8* will return 4, since it has a size of 4 * 8 = 32 bits
/// For compressed formats this function will return the size of one block in bytes.
/// \sa formatSize, blockSize
unsigned int formatSizeBits(vk::Format);

/// Returns the size in bytes of the given format.
/// E.g. vk::Format::r8g8b8a8* will return 4, since it has a size of 4 * 8 bits = 32 bits = 4 bytes.
/// For compressed formats this function will return the size of one block in bytes.
/// \sa blockSize, formatSizeBits
unsigned int formatSize(vk::Format);

/// Returns the size of one compressed block of a compressed vulkan format.
/// If the given format is not a compressed format, {1, 1} is returned.
/// For vk::Format::undefined, {0, 0} is returned
/// \sa formatSize, formatSizeBits
vk::Extent2D blockSize(vk::Format);

/// Fills the given image with data.
/// There are two different methods for filling an image: memoryMap and transfer.
/// MemoryMap is used if the image is mappable and the allowMap param is true, otherwise
/// the transfer method is used (which is usually less efficient).
/// Some of the parameters are only needed for one of the two methods.
/// \param image The image to fill. Must have a non-compressed
/// format. Must not be multisampled and either be created on host visible memory or with the
/// transferDst usage bit set. If it was not created as sparse image, must be
/// fully bound to memory. Otherwise only the accessed parts must be bound.
/// \param data Tightly packed data.
/// The data must be in row-major order and large enough for the given extent.
/// The size of data will be expected to be extent.w * extent.h * extent.d * formatSize(format).
/// \param format The images format. Only important for the size, so if the images format
/// is r8g8b8a8*, passing any other format with the same size (like e.g. a8b8g8r8*) is fine.
/// \param layout The layout of the image when this work will be submitted.
/// Only needed if the image is filled per transfer. If the layout is not transferDstOptimal
/// or the general and the transfer method is used, it will be cahgned to
/// transferDstOptimal when the work is executed.
/// Note that if the returned work will change the layout, the reference
/// will be changed to the new layout, it will have when the work is finished.
/// \param extent The size of the image region to fill. Note that offset + extent must not be
/// larger than the images extent.
/// \param subres The subresource to fill (specifies e.g. layer to fill).
/// Must be set to valid values for the given image.
/// \param offset The offset of the image region to fill. By default no offset.
/// \param allowMap If set to false, the image fill always be filled using a transfer command
/// rather than mapping its memory. Needed e.g. if the image has an optimal tiling since
/// optimal tiling images cannot be filled using memory maps. True by default.
/// Note that if this is true the image must have been created with the transferDst
/// usage bit set.
/// If this is true and the image is mappable it must have linear tiling.
WorkPtr fill(const Image& image,
	const uint8_t& data,
	vk::Format format,
	vk::ImageLayout& layout,
	const vk::Extent3D& extent,
	const vk::ImageSubresource& subres,
	const vk::Offset3D& offset = {},
	bool allowMap = true);

/// Retrieves the data from the given image.
/// The image must be either allocated on host visible memory or must have the transferSrc bit set
/// as usage and must not be multisampled.
/// \param image The image to retrieve the data from. Must have a non-compressed
/// format. Must not be multisampled and either be created on host visible memory or with the
/// transferDst usage bit set. If it was not created as sparse image, must be
/// fully bound to memory. Otherwise only the accessed parts must be bound.
/// \param layout The current ImageLayout of the image.
/// This will only be used when the data is retrieved using a transfer.
/// Will be set to the ImageLayout the image will have when the returned work is completed
/// if it will be changed
/// \param allowMap If set to false, the image fill always be filled using a transfer command
/// rather than mapping its memory if possible. Needed e.g. if the image has an optimal tiling.
/// If this is set to false, the image must have been created with the transferDstSrc bit.
DataWorkPtr retrieve(const Image& image,
	vk::ImageLayout& layout,
	vk::Format format,
	const vk::Extent3D& extent,
	const vk::ImageSubresource& subres,
	const vk::Offset3D& offset = {},
	bool allowMap = true);

/// Records the command for changing the layout of the given image into the
/// given CommandBuffer.
/// The given CommandBuffer must be in recording state
void changeLayoutCommand(vk::CommandBuffer,
	vk::Image,
	vk::ImageLayout oldLayout,
	vk::ImageLayout newLayout,
	const vk::ImageSubresourceRange&);

/// Changes the layout of a given image and returns the associated work ptr.
WorkPtr changeLayout(const Device&,
	vk::Image,
	vk::ImageLayout oldLayout,
	vk::ImageLayout newLayout,
	const vk::ImageSubresourceRange&);

/// Changes the layout of a given image and returns the associated work ptr.
inline WorkPtr changeLayout(const Image& img,
	vk::ImageLayout oldLayout,
	vk::ImageLayout newLayout,
	const vk::ImageSubresourceRange& range)
{ return changeLayout(img.device(), img, oldLayout, newLayout, range); }

/// RAII wrapper around a vulkan image view.
class ImageView : public ResourceHandle<vk::ImageView>
{
public:
	ImageView() = default;
	ImageView(const Device& dev, const vk::ImageViewCreateInfo& info);
	ImageView(const Device& dev, vk::ImageView imageView);
	~ImageView();

	ImageView(ImageView&& lhs) noexcept { swap(*this, lhs); }
	ImageView& operator=(ImageView lhs) noexcept { swap(*this, lhs); return *this; }
};

/// Combines a vulkan image and an imageView for it.
/// Can be e.g. used for textures or framebuffer attachments.
class ViewableImage : public ResourceReference<ViewableImage> {
public:
	struct CreateInfo {
		vk::ImageCreateInfo imgInfo {}; // info to create the image
		vk::ImageViewCreateInfo viewInfo {}; // info to create the view
		unsigned int memoryTypeBits = ~0u; // memory type bits for the image memory
	};

	/// Some useful default create infos.
	/// Can e.g. be used for framebuffers or texures.
	/// Note that some properties like e.g. the size must be set manually afterwards.
	/// Note that these functions do NOT check for availability of the formats
	/// of attributes they return, they just use what is usually available.
	static CreateInfo defaultDepth2D();
	static CreateInfo defaultDepthStencil2D();
	static CreateInfo defaultColor2D();

public:
	ViewableImage() = default;
	ViewableImage(const Device& dev, const CreateInfo& info);
	~ViewableImage();

	ViewableImage(ViewableImage&& rhs) noexcept { swap(*this, rhs); }
	ViewableImage& operator=(ViewableImage rhs) noexcept { swap(*this, rhs); return *this; }

	void create(const Device&, const vk::ImageCreateInfo&, unsigned int memoryTypeBits = ~0u);
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

/// RAII wrapper for a vulkan sampler.
class Sampler : public ResourceHandle<vk::Sampler> {
public:
	Sampler() = default;
	Sampler(const Device& dev, const vk::SamplerCreateInfo& info);
	Sampler(const Device& dev, vk::Sampler sampler);
	~Sampler();

	Sampler(Sampler&& rhs) noexcept { swap(*this, rhs); }
	Sampler& operator=(Sampler rhs) noexcept { swap(*this, rhs); return *this; }
};

} // namespace vpp
