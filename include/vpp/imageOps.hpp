// Copyright (c) 2016-2019 nyorain
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
vk::DeviceSize texelAddress(const vk::SubresourceLayout& layout,
	unsigned int texelSize, unsigned int x, unsigned int y, unsigned int z = 0,
	unsigned int layer = 0);

/// Returns the size of the given format in bits.
/// E.g. vk::Format::r8g8b8a8* will return 32, since it has 4 * 8 = 32 bits
/// For compressed formats this function will return the size of one block.
unsigned int formatSizeBits(vk::Format);

/// Returns the size in bytes of the given format.
/// E.g. vk::Format::r8g8b8a8* will return 4, since it has 4 * 8 bits = 4 bytes.
/// For compressed formats this function will return the size of one block.
unsigned int formatSize(vk::Format);

/// Returns the size of one compressed block of a compressed vulkan format.
/// If the given format is not a compressed format, {1, 1} is returned.
/// For vk::Format::undefined, {0, 0} is returned
vk::Extent2D blockSize(vk::Format);

/// Fills the given image with the given data by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and
/// linear tiling.
/// The image must not be in use (you probably have to use a barrier).
/// - data: Tightly packed data.
///   Must be in row-major order and large enough for the given extent.
///   The size of data will be expected to equal exactly:
///   extent.w * extent.h * extent.d * formatSize(format)
void fillMap(const Image&, vk::Format, const vk::Extent3D& size,
	nytl::Span<const std::byte> data, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});

/// Retrieves the contents of the given image by mapping it.
/// The image must be bound to hostVisible memory.
/// The image must have a suitable (preinitialized or general) layout and
/// linear tiling.
/// The image must not be in use (you probably have to use a barrier).
/// The returned data will be tightly packed.
std::vector<std::byte> retrieveMap(const Image&, vk::Format,
	const vk::Extent3D& size, const vk::ImageSubresource&,
	const vk::Offset3D& offset = {});

/// Records the command for fillStaging into the given
/// command buffer. The returned BufferRange must stay valid until
/// the command buffer has completed execution.
/// If the command buffer is never submitted, it may be destructed.
/// The command buffer must be in recording state.
/// - data: Tightly packed data.
///   Must be in row-major order and large enough for the given extent.
///   The size of data will be expected to equal exactly:
///   extent.w * extent.h * extent.d * formatSize(format)
[[nodiscard]]
SubBuffer fillStagingLayers(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size, nytl::Span<const std::byte> data,
	const vk::ImageSubresourceLayers&, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer fillStaging(vk::CommandBuffer, const Image&, vk::Format,
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
/// mipLevels, layers, depth, height, width. When multiple mip levels are
/// downloaded, calculating invidual image addresses isn't trivial,
/// see the bufferImageOffset function below.
[[nodiscard]]
SubBuffer retrieveStagingRange(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size,
	const vk::ImageSubresourceRange&, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer retrieveStagingLayers(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size,
	const vk::ImageSubresourceLayers&, const vk::Offset3D& offset = {});
[[nodiscard]]
SubBuffer retrieveStaging(vk::CommandBuffer, const Image&, vk::Format,
	vk::ImageLayout, const vk::Extent3D& size,
	const vk::ImageSubresource&, const vk::Offset3D& offset = {});

/// For an image with the given extent and a format of the given byte size,
/// downloaded with retrieveStagingRange, returns the address at which
/// the part of the image data for mip, layer begins.
/// TODO: extend for baseMipLevel != 0
vk::DeviceSize imageBufferOffset(unsigned formatSize, vk::Extent3D extent,
	unsigned numLayers, unsigned mip, unsigned layer);

} // namespace vpp
