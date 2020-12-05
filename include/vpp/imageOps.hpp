// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp>
#include <vkpp/structs.hpp>

namespace vpp {

/// Returns the memory address for an image texel.
/// Does not support compressed image formats.
/// - layout: The queried layout of the image, obtained via
///   vk::getImageSubresourceLayout.
/// - texelSize: The size (in bytes) of one texel.
///   Can e.g. be obtained from formatSize (with the images format).
/// - x,y,z,layer: The coordinates of the texel which address should be returned.
[[nodiscard]] VPP_API vk::DeviceSize texelAddress(const vk::SubresourceLayout& layout,
	unsigned texelSize, unsigned x, unsigned y, unsigned z = 0,
	unsigned layer = 0);

/// Returns the pixel number of a given texel in a tightly, linearly layout
/// image, dimension order: mips, layers, depth, height, width.
/// To obtain the buffer offset, multiply the address with the format size
/// of the image (see imageBufferOffset).
/// Useful since that's the format in which image ranges are returned
/// from retrieveStagingRange (and variations).
/// Mainly interesting for images with multiple mip levels since the
/// offset formula is not trivial in that case (since each level
/// has different size).
/// This cannot be used to compute the address of a texel in a linear
/// vulkan image, see vpp::texelAddress for that (above).
/// - extent: size of the linear image
/// - numLayers: number of layers the image has
/// - mip: the mip of the texel to compute the texel number for.
///   `mip < mipmapLevels(extent)` must hold, i.e. if the
///   mip level can't exist for the given extent
/// - layer: the layer of the texel to compute the texel number for
///   `layer < numLayers` must hold.
/// - x,y,z: coordinates of the texel in the given mip, layer
/// - firstMip: which mip begins at texel number 0.
///   `firstMip <= mip` must hold. Note that 'mip' is not relative
///   to baseMip but absolute.
[[nodiscard]] VPP_API vk::DeviceSize tightTexelNumber(vk::Extent3D extent,
	unsigned numLayers, unsigned mip, unsigned layer,
	unsigned x = 0u, unsigned y = 0u, unsigned z = 0u,
	unsigned firstMip = 0u);

/// Returns the number of texels in the specified image subresource range
/// in a tight, linear layout.
[[nodiscard]] VPP_API vk::DeviceSize tightTexelCount(vk::Extent3D extent,
	unsigned numLayers, unsigned numMips, unsigned firstMip = 0u);

/// Equivalent to tightTexelNumber(extent, 1, 0, 0, x, y, z),
/// i.e. returns the texel number in a single layer for an image
/// of the given size.
[[nodiscard]] inline vk::DeviceSize tightLayerTexelNumber(vk::Extent3D extent,
		unsigned x, unsigned y, unsigned z = 0) {
	return z * (extent.height * extent.width) + y * extent.width + x;
}

// TODO: properly test the functions for multiple mips and multiple layers.
// They might still have issues.
// TODO(low): we could support SubresourceRanges for the 'Map' functions
// as well.

/// Fills the given image with the given data by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and
/// linear tiling.
/// The image must not be in use (you probably have to use a barrier).
/// - data: Tightly packed data.
///   Must be in row-major order and large enough for the given extent.
///   The size of data will be expected to equal exactly:
///   extent.w * extent.h * extent.d * formatSize(format)
void VPP_API fillMap(const Image&, vk::Format, const vk::Extent3D& size,
	nytl::Span<const std::byte> data, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});

/// Retrieves the contents of the given image by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and
/// linear tiling.
/// The image must not be in use (you probably have to use a barrier).
/// The returned data will be tightly packed.
[[nodiscard]] VPP_API std::vector<std::byte> retrieveMap(const Image&, vk::Format,
	const vk::Extent3D& size, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});

// TODO, NOTE: the commands below don't insert buffer/image barriers
// automatically, you usually will need them (a barrier before this call
// on the reading resource and a barrier after the call on the written
// resource)

/// Records the command for fillStaging into the given
/// command buffer. The returned BufferRange must stay valid until
/// the command buffer has completed execution.
/// If the command buffer is never submitted, it may be destructed.
/// The command buffer must be in recording state.
/// - data: Tightly packed data.
///   Must be in row-major order and large enough for the given extent.
///   The size of data will be expected to equal exactly the sum of
///   formatSize(format) * subresource.layerCount * width * height * depth
///   for all mip levels in the given range (note that width, height, depth
///   might vary for each mip level). This is equivalent to
///   formatSize(format) * tightTexelNumber(extent, nl, mm, 0, 0, 0, 0, bm) with
///   nl = subresource.layerCount
///   mm = subresource.baseMipLevel + subresource.levelCount
///   bm = subresource.baseMipLevel.
///   The data is expected to be layouted as computed by tightTexelNumber,
///   miplevels -> layers -> depth -> height -> width.
/// - size: extent to fill in the image in absolute mip level 0.
///   Note that filling just a part of multiple levels of an image
///   might be a bad idea, you usually want to pass the full image size
///   here if multiple mip levels are filled.
/// - offset: offset into the image in absolute mip level 0.
///   When the fills any mip other than mip level 0, offset must be 0.
[[nodiscard]]
SubBuffer VPP_API fillStagingRange(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, vk::Extent3D size, nytl::Span<const std::byte> data,
	const vk::ImageSubresourceRange& range, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer VPP_API fillStagingLayers(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size, nytl::Span<const std::byte> data,
	const vk::ImageSubresourceLayers&, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer VPP_API fillStaging(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size, nytl::Span<const std::byte> data,
	const vk::ImageSubresource&, const vk::Offset3D& offset = {});

/// Records the commands for retrieveStaging into the
/// given command buffer. The returned, mappable buffer range will hold
/// the given images data when the given command buffer finishes
/// execution. The BufferRange must stay valid until the commandbuffer
/// finishes execution. If the commandbuffer is never submitted, the
/// BufferRange may be destroyed.
/// The command buffer must be in recording state.
/// The returned data will be tightly packed,
/// mipLevels -> layers- > depth- > height -> width.
/// When multiple mip levels are downloaded, calculating invidual
/// image addresses isn't trivial, see the tightTexelNumber function..
[[nodiscard]]
SubBuffer VPP_API retrieveStagingRange(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, vk::Extent3D size,
	const vk::ImageSubresourceRange&, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer VPP_API retrieveStagingLayers(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size,
	const vk::ImageSubresourceLayers&, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer VPP_API retrieveStaging(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size,
	const vk::ImageSubresource&, const vk::Offset3D& offset = {});

} // namespace vpp
