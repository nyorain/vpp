// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/span.hpp>
#include <vkpp/structs.hpp>
#include <optional>

namespace vpp {

/// Various helper for choosing dynamic formats as well as default
/// ViewableImageCreateInfo constructors.
/// When choosing formats for your images or texelBuffer, the
/// best way (except for highly variable areas like depth/stencil)
/// is usually to use a format that is guaranteed to be supported
/// in the way you need it by the vulkan standard (section 31.3.3).

/// Returns the size of the given format in bits.
/// E.g. vk::Format::r8g8b8a8* will return 32, since it has 4 * 8 = 32 bits
/// For compressed formats this function will return the size of one block.
[[nodiscard]] unsigned int formatSizeBits(vk::Format);

/// Returns the size in bytes of the given format.
/// E.g. vk::Format::r8g8b8a8* will return 4, since it has 4 * 8 bits = 4 bytes.
/// For compressed formats this function will return the size of one block.
[[nodiscard]] unsigned int formatSize(vk::Format);

/// Returns the size of one compressed block of a compressed vulkan format.
/// If the given format is not a compressed format, {1, 1} is returned.
/// For vk::Format::undefined, {0, 0} is returned
[[nodiscard]] vk::Extent2D blockSize(vk::Format);

/// Returns whether the given FormatFeatureFlags support the given usage.
[[nodiscard]] bool supportedUsage(vk::FormatFeatureFlags, vk::ImageUsageFlags);
[[nodiscard]] bool supportedUsage(vk::FormatFeatureFlags, vk::BufferUsageFlags);

/// Returns whether the format of the given ImageCreateInfo supports the
/// other parameters.
[[nodiscard]] bool supported(const Device&, const vk::ImageCreateInfo&,
	vk::FormatFeatureFlags additional = {});

/// Returns whether the given format is valid for the given use case.
[[nodiscard]] bool supported(const Device&, vk::Format, vk::BufferUsageFlags,
	vk::FormatFeatureFlags additional = {});

/// Selects the first format from the list that supports the given
/// use case. Returns vk::Format::undefined if none is supported.
[[nodiscard]] vk::Format findSupported(const Device&, nytl::Span<const vk::Format>,
	const vk::ImageCreateInfo&, vk::FormatFeatureFlags additional = {});
[[nodiscard]] vk::Format findSupported(const Device&, nytl::Span<const vk::Format>,
	vk::BufferUsageFlags, vk::FormatFeatureFlags additional = {});

/// Returns the number of mipmap levels needed for a full mipmap
/// chain for an image with the given extent.
[[nodiscard]] unsigned mipmapLevels(const vk::Extent2D& extent);
[[nodiscard]] unsigned mipmapLevels(const vk::Extent3D& extent);

/// Returns the size of an image with given size at the given mip level.
/// Returns {1, 1, 1} if the mip level does not exist (i.e. too high).
[[nodiscard]] vk::Extent3D mipSize(vk::Extent3D size, unsigned l);

/// Combines vk::ImageCreateInfo and vk::ImageViewCreatInfo and
/// offers default initializers.
struct ViewableImageCreateInfo {
	vk::ImageCreateInfo img {}; // info to create the image
	vk::ImageViewCreateInfo view {}; // info to create the view

	ViewableImageCreateInfo() = default;

	/// 2D default constructor, 1 layer, 1 sample
	ViewableImageCreateInfo(vk::Format format,
		vk::ImageAspectBits aspect, vk::Extent2D size,
		vk::ImageUsageFlags usage,
		vk::ImageTiling tiling = vk::ImageTiling::optimal,
		uint32_t mipLevels = 1u);
};

} // namespace vpp
