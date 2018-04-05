// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp>
#include <vpp/transferWork.hpp>
#include <vkpp/structs.hpp>

namespace vpp {

// TODO: fill/retrieve:
//  - allow compressed formats. Some way to allow filling a compressed image with
//	  uncompressed data? doing the compressing on the gpu?
//  - allow to only pass vk::image where it makes sense

/// Returns the memory address for an image texel.
/// Does not support compressed image formats.
/// \param texelSize the size (in bytes) of one texel.
/// Can e.g. be obtained from formatSize (with the images format).
vk::DeviceSize texelAddress(const vk::SubresourceLayout& layout,
	unsigned int texelSize, unsigned int x, unsigned int y, unsigned int z = 0,
	unsigned int layer = 0);

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
/// linear tiling.
/// The image must not be in use (you probably have to use a barrier).
/// \param data Tightly packed data.
/// The data must be in row-major order and large enough for the given extent.
/// The size of data will be expected to be exactly
/// extent.w * extent.h * extent.d * formatSize(format).
void fillMap(const Image&, vk::Format, const vk::Extent3D& size,
	nytl::Span<std::byte> data, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});

/// Retrieves the contents of the given image by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and
/// linear tiling.
/// The image must not be in use (you probably have to use a barrier).
/// The returned data will be tightly packed and have the size extent.w *
/// extent.h * extent.d * formatSize(format).
std::vector<std::byte> retrieveMap(const Image&, vk::Format,
	const vk::Extent3D& size, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});


/// Fills the given image by using a temporary staging buffer and then
/// copying the contents. The image must have been created with
/// the transferDst usage and must be in transferDstOptimal or general
/// layout. The image must not be in use.
/// The passed layout must be the image layout at the time this
/// commands are executed on the device.
/// The image must not be in use (you probably have to use a barrier).
UploadWork fillStaging(const Image&, vk::Format, vk::ImageLayout,
	const vk::Extent3D& size, nytl::Span<const std::byte> data,
	const vk::ImageSubresource&, const vk::Offset3D& offset = {},
	QueueSubmitter* = {});

/// Records the command for fillStaging (see above) into the given
/// command buffer. The returned BufferRange must stay valid until
/// the command buffer has completed execution.
/// If the command buffer is never submitted, it may be destructed.
/// The command buffer must be in recording state.
[[nodiscard]]
SubBuffer fillStaging(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size, nytl::Span<const std::byte> data,
	const vk::ImageSubresource&, const vk::Offset3D& offset = {});

/// Retrieves the contents of the given image by copying in to a staging
/// buffer. The image must have been created with the transferSrc usage and
/// must be in transferDstOptimal or general layout.
/// The image must not be in use.
/// The image must not be in use (you probably have to use a barrier).
DownloadWork retrieveStaging(const Image&, vk::Format, vk::ImageLayout,
	const vk::Extent3D& size, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {}, QueueSubmitter* = {});

/// Records the commands for retrieveStaging (see above) into the
/// given command buffer. The returned, mappable buffer range will hold
/// the given images data when the given command buffer finishes
/// execution. The BufferRange must stay valid until the commandbuffer
/// finishes execution. If the commandbuffer is never submitted, the
/// BufferRange may be destroyed.
/// The command buffer must be in recording state.
[[nodiscard]]
SubBuffer retrieveStaging(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});


/// Records the command for changing the layout of the given image.
/// You have to pass all required arguments, does not perform any heuristics.
void changeLayout(vk::CommandBuffer, vk::Image,
	vk::ImageLayout ol, vk::PipelineStageFlags srcs, vk::AccessFlags srca,
	vk::ImageLayout nl, vk::PipelineStageFlags dsts, vk::AccessFlags dsta,
	const vk::ImageSubresourceRange&);

CommandWork<void> changeLayout(vk::Image,
	vk::ImageLayout ol, vk::PipelineStageFlags srcs, vk::AccessFlags srca,
	vk::ImageLayout nl, vk::PipelineStageFlags dsts, vk::AccessFlags dsta,
	const vk::ImageSubresourceRange&, QueueSubmitter&);

} // namespace vpp
