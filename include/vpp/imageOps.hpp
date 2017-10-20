// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp>
#include <vpp/transferWork.hpp>
#include <vpp/vulkan/structs.hpp>

namespace vpp {

// TODO: functions implicitly assume things. Can probably be done in a
// better interface
// TODO: default stage masks? 

// TODO: fill/retrieve:
//  - allow compressed formats. Some way to allow filling a compressed image with
//	  uncompressed data? doing the compressing on the gpu?
// - parameter for preseving the layout, i.e. changing its back to the
//   original value if it has to be chagned


/// Returns the size of the given format in bits.
/// E.g. vk::Format::r8g8b8a8* will return 32, since it has 4 * 8 = 32 bits
/// For compressed formats this function will return the size of one block.
/// \sa formatSize, blockSize
unsigned int formatSizeBits(vk::Format);

/// Returns the size in bytes of the given format.
/// E.g. vk::Format::r8g8b8a8* will return 4, since it has 4 * 8 bits = 4 bytes.
/// For compressed formats this function will return the size of one block.
/// \sa blockSize, formatSizeBits
unsigned int formatSize(vk::Format);

/// Returns the size of one compressed block of a compressed vulkan format.
/// If the given format is not a compressed format, {1, 1} is returned.
/// For vk::Format::undefined, {0, 0} is returned
/// \sa formatSize, formatSizeBits
vk::Extent2D blockSize(vk::Format);


/// Fills the given image with the given data by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and 
/// linear tiling. The given data must be tightly packed.
/// The image must not be in use.
void fillMap(const Image&, nytl::Span<std::byte> data, vk::Format,
	const vk::Extent3D& size, const vk::ImageSubresource&, 
	const vk::Offset3D& offset = {});

/// Retrieves the contents of the given image by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and 
/// linear tiling. The returned data will be tightly packed.
/// The image must not be in use.
std::vector<std::byte> retrieveMap(const Image&, vk::Format, 
	const vk::Extent3D& size, const vk::ImageSubresource&, 
	const vk::Offset3D& offset = {});

/// Fills the given image by using a temporary staging buffer and then
/// copying the contents. The image must have been created with
/// the transferDst usage and must be in transferDstOptimal or general
/// layout. The image must not be in use.
/// The given data must be tightly packed.
void fillCopy(const Image&, nytl::Span<std::byte> data, vk::Format format,
	const vk::Extent3D& size, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});

/// Retrieves the contents of the given image by copying in to a staging
/// buffer. The image must have been created with the transferSrc usage and 
/// must be in transferDstOptimal or general layout. 
/// The image must not be in use.
/// The returned data will be tightly packed.
void retrieveCopy(DownloadWork& work, const Image&, vk::Format format,
	const vk::Extent3D& size, const vk::ImageSubresource&, 
	const vk::Offset3D& offset = {});


/// Records the command for changing the layout of the given image into the
/// given CommandBuffer.
/// The given CommandBuffer must be in recording state
void changeLayoutCommand(vk::CommandBuffer, vk::Image, 
	vk::ImageLayout oldLayout, vk::PipelineStageFlags srcStage,
	vk::ImageLayout newLayout, vk::PipelineStageFlags dstStage,
	const vk::ImageSubresourceRange&);

// TODO
/*
/// Changes the layout of a given image and returns the associated work ptr.
WorkPtr  changeLayout(const Device&, vk::Image, 
	vk::ImageLayout oldLayout, vk::PipelineStageFlags srcStage,
	vk::ImageLayout newLayout, vk::PipelineStageFlags dstStage,
	const vk::ImageSubresourceRange&);

inline WorkPtr changeLayout(const vpp::Image& img,
	vk::ImageLayout oldLayout, vk::PipelineStageFlags srcStage,
	vk::ImageLayout newLayout, vk::PipelineStageFlags dstStage,
	const vk::ImageSubresourceRange& range)
{ 
	return changeLayout(img.device(), img, 
		oldLayout, srcStage,
		newLayout, dstStage, range); 
}

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
*/

} // namespace vpp