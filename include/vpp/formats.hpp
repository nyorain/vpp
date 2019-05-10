// Copyright (c) 2016-2019 nyorain
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

/// Returns whether the given FormatFeatureFlags support the given usage.
bool supportedUsage(vk::FormatFeatureFlags, vk::ImageUsageFlags);
bool supportedUsage(vk::FormatFeatureFlags, vk::BufferUsageFlags);

/// Returns whether the format of the given ImageCreateInfo supports the
/// other parameters.
bool supported(const Device&, const vk::ImageCreateInfo&,
	vk::FormatFeatureFlags additional = {});

/// Returns whether the given format is valid for the given use case.
bool supported(const Device&, vk::Format, vk::BufferUsageFlags,
	vk::FormatFeatureFlags additional = {});

/// Selects the first format from the list that supports the given
/// use case. Returns vk::Format::undefined if none is supported.
vk::Format findSupported(const Device&, nytl::Span<const vk::Format>,
	const vk::ImageCreateInfo&, vk::FormatFeatureFlags additional = {});
vk::Format findSupported(const Device&, nytl::Span<const vk::Format>,
	vk::BufferUsageFlags, vk::FormatFeatureFlags additional = {});

/// Returns the number of mipmap levels needed for a full mipmap
/// chain for an image with the given extent.
unsigned mipmapLevels(const vk::Extent2D& extent);

/// Combines vk::ImageCreateInfo and vk::ImageViewCreatInfo and
/// offers default initializers.
struct ViewableImageCreateInfo {
	vk::ImageCreateInfo img {}; // info to create the image
	vk::ImageViewCreateInfo view {}; // info to create the view

	ViewableImageCreateInfo() = default;

	/// 2D default constructor, 1 layer, 1 sample
	ViewableImageCreateInfo(vk::Format format,
		vk::ImageAspectBits aspect, vk::Extent2D size,
		vk::ImageUsageBits usage,
		vk::ImageTiling tiling = vk::ImageTiling::optimal,
		uint32_t mipLevels = 1u);
};

} // namespace vpp
