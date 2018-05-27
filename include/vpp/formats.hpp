// Copyright (c) 2016-2018 nyorain
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

/// Returns whether the given format is valid for the given use
/// case.
bool supported(const Device&, vk::Format, const vk::ImageCreateInfo&,
	vk::FormatFeatureFlags additional = {});
bool supported(const Device&, vk::Format, vk::BufferUsageFlags,
	vk::FormatFeatureFlags additional = {});

/// Selects the first format from the list that supports the given
/// use case. Returns vk::Format::undefined if none is supported.
vk::Format findSupported(const Device&, nytl::Span<const vk::Format>,
	const vk::ImageCreateInfo&, vk::FormatFeatureFlags additional = {});
vk::Format findSupported(const Device&, nytl::Span<const vk::Format>,
	vk::BufferUsageFlags, vk::FormatFeatureFlags additional = {});

/// Combines vk::ImageCreateInfo and vk::ImageViewCreatInfo and
/// offers default initializers.
struct ViewableImageCreateInfo {
	vk::ImageCreateInfo img {}; // info to create the image
	vk::ImageViewCreateInfo view {}; // info to create the view

	/// General function for creating a valid CreateInfo.
	/// All required settings must be given as arguments, uses
	/// the defaults seen here.
	/// Allowed to pass 0 as as third size parameter, will automatically
	/// be corrected to 1.
	/// Selects the first available format.
	/// If the usecase is not supported for any given format returns
	/// an empty object.
	static std::optional<ViewableImageCreateInfo> general(
		const Device& dev, const vk::Extent3D& size,
		vk::ImageUsageFlags, nytl::Span<const vk::Format> formats,
		vk::ImageAspectFlags,
		vk::ImageTiling = vk::ImageTiling::optimal,
		vk::SampleCountBits = vk::SampleCountBits::e1,
		vk::ImageLayout = vk::ImageLayout::undefined,
		vk::ImageCreateFlags = {},
		vk::FormatFeatureFlags additional = {});

	/// Default color viewable image info.
	/// NOTE: you might want to use Srgb color format.
	static std::optional<ViewableImageCreateInfo> color(
		const Device& dev, const vk::Extent3D& size,
		vk::ImageUsageFlags =
			vk::ImageUsageBits::transferDst |
			vk::ImageUsageBits::sampled,
		nytl::Span<const vk::Format> formats = {
			vk::Format::r8g8b8a8Unorm
		}, vk::ImageTiling = vk::ImageTiling::optimal,
		vk::SampleCountBits = vk::SampleCountBits::e1,
		vk::ImageLayout = vk::ImageLayout::undefined,
		vk::ImageCreateFlags = {},
		vk::FormatFeatureFlags additional = {});

	/// Default depth viewable image info.
	static std::optional<ViewableImageCreateInfo> depth(
		const Device& dev, const vk::Extent3D& size,
		vk::ImageUsageFlags =
			vk::ImageUsageBits::depthStencilAttachment |
			vk::ImageUsageBits::sampled,
		nytl::Span<const vk::Format> formats = {
			vk::Format::d32Sfloat,
			vk::Format::d32SfloatS8Uint,
			vk::Format::d24UnormS8Uint,
			vk::Format::d16UnormS8Uint,
			vk::Format::d16Unorm
		}, vk::ImageTiling = vk::ImageTiling::optimal,
		vk::SampleCountBits = vk::SampleCountBits::e1,
		vk::ImageLayout = vk::ImageLayout::undefined,
		vk::ImageCreateFlags = {},
		vk::FormatFeatureFlags additional = {});

	// TODO: depthStencil, stencil, additional color formats (other priorities)
	// TODO: add memBits to structure? to allow hostVisible/hostCoherent default
	//  initializers?
	// TODO: ability to override imageType (that is then included in
	//  format checks)?
};

} // namespace vpp
