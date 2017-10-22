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
		nytl::Span<const vk::Format> formats,
		vk::ImageUsageFlags, vk::ImageAspectFlags,
		vk::ImageTiling = vk::ImageTiling::optimal,
		vk::SampleCountBits = vk::SampleCountBits::e1,
		vk::ImageLayout = vk::ImageLayout::undefined,
		vk::ImageCreateFlags = {},
		vk::FormatFeatureFlags additional = {});

	/// Default color viewable image info. 
	static std::optional<ViewableImageCreateInfo> color(
		const Device& dev, const vk::Extent3D& size,
		nytl::Span<const vk::Format> formats = {
			vk::Format::r8g8b8a8Unorm
		}, vk::ImageUsageFlags = 
			vk::ImageUsageBits::transferDst |
			vk::ImageUsageBits::sampled,
		vk::ImageTiling = vk::ImageTiling::optimal,
		vk::SampleCountBits = vk::SampleCountBits::e1,
		vk::ImageLayout = vk::ImageLayout::undefined,
		vk::ImageCreateFlags = {},
		vk::FormatFeatureFlags additional = {});

	/// Default depth viewable image info.
	static std::optional<ViewableImageCreateInfo> depth(
		const Device& dev, const vk::Extent3D& size, 
		nytl::Span<const vk::Format> formats = {
			vk::Format::d32Sfloat,
			vk::Format::d32SfloatS8Uint,
			vk::Format::d24UnormS8Uint,
			vk::Format::d16UnormS8Uint,
			vk::Format::d16Unorm
		}, vk::ImageUsageFlags = 
			vk::ImageUsageBits::depthStencilAttachment | 
			vk::ImageUsageBits::sampled,
		vk::ImageTiling = vk::ImageTiling::optimal,
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


#include <vpp/device.hpp>
#include <vkpp/functions.hpp>

namespace vpp {

bool supportedUsage(vk::FormatFeatureFlags features, vk::ImageUsageFlags usages)
{
	static constexpr struct {
		vk::ImageUsageBits usage;
		vk::FormatFeatureBits feature;
	} maps[] = {
		{vk::ImageUsageBits::sampled, vk::FormatFeatureBits::sampledImage},
		{vk::ImageUsageBits::storage, vk::FormatFeatureBits::storageImage},
		{vk::ImageUsageBits::colorAttachment, vk::FormatFeatureBits::colorAttachment},
		{vk::ImageUsageBits::depthStencilAttachment, vk::FormatFeatureBits::depthStencilAttachment},

		// TODO: only use/test for when vkpp supports maintenance1 extension
		//  and it is enabled on device
		// {vk::ImageUsageBits::transferSrc, vk::FormatFeatureBits::transferSrcKHR},
		// {vk::ImageUsageBits::transferDst, vk::FormatFeatureBits::transferDstKHR},
	};

	for(const auto& map : maps) {
		if((usages & map.usage) && !(features & map.feature)) {
			return false;
		}
	}

	return true;
}

bool supportedUsage(vk::FormatFeatureFlags features, vk::BufferUsageFlags usages)
{
	static constexpr struct {
		vk::BufferUsageBits usage;
		vk::FormatFeatureBits feature;
	} maps[] = {
		{vk::BufferUsageBits::uniformTexelBuffer, vk::FormatFeatureBits::uniformTexelBuffer},
		{vk::BufferUsageBits::storageTexelBuffer, vk::FormatFeatureBits::storageTexelBuffer},
		{vk::BufferUsageBits::vertexBuffer, vk::FormatFeatureBits::vertexBuffer},
	};

	for(const auto& map : maps) {
		if((usages & map.usage) && !(features & map.feature)) {
			return false;
		}
	}

	return true;
}

bool supported(const Device& dev, vk::Format format, 
	const vk::ImageCreateInfo& info, vk::FormatFeatureFlags additional)
{
	auto props = vk::getPhysicalDeviceFormatProperties(
		dev.vkPhysicalDevice(), format);
	auto features = (info.tiling == vk::ImageTiling::linear) ?
		props.linearTilingFeatures : props.optimalTilingFeatures;

	auto imgProps = vk::getPhysicalDeviceImageFormatProperties(
		dev.vkPhysicalDevice(), format, info.imageType, info.tiling, 
		info.usage, info.flags);

	return ((features & additional) == additional &&
		supportedUsage(features, info.usage) &&
		info.extent.width <= imgProps.maxExtent.width &&
		info.extent.height <= imgProps.maxExtent.height &&
		info.extent.depth <= imgProps.maxExtent.depth &&
		info.mipLevels <= imgProps.maxMipLevels && 
		info.arrayLayers <= imgProps.maxArrayLayers &&
		(imgProps.sampleCounts & info.samples));
}

bool supported(const Device& dev, vk::Format format, 
	vk::BufferUsageFlags use, vk::FormatFeatureFlags additional)
{
	auto props = vk::getPhysicalDeviceFormatProperties(
		dev.vkPhysicalDevice(), format);
	return (props.bufferFeatures & additional == additional) &&
		supportedUsage(props.bufferFeatures, use);
}

vk::Format findSupported(const Device& dev, nytl::Span<const vk::Format> formats, 
	const vk::ImageCreateInfo& info, vk::FormatFeatureFlags additional)
{
	for(auto format : formats) {
		if(supported(dev, format, info, additional)) {
			return format;
		}
	}

	return vk::Format::undefined;
}

vk::Format findSupported(const Device& dev, nytl::Span<const vk::Format> formats, 
	vk::BufferUsageFlags use, vk::FormatFeatureFlags additional)
{
	for(auto format : formats) {
		if(supported(dev, format, use, additional)) {
			return format;
		}
	}

	return vk::Format::undefined;
}

std::optional<ViewableImageCreateInfo> ViewableImageCreateInfo::general(
	const Device& dev, const vk::Extent3D& size, 
	nytl::Span<const vk::Format> formats, vk::ImageUsageFlags usage,
	vk::ImageAspectFlags aspect, vk::ImageTiling tiling, 
	vk::SampleCountBits samples, vk::ImageLayout layout,
	vk::ImageCreateFlags flags, vk::FormatFeatureFlags additional)
{
	ViewableImageCreateInfo ret;
	ret.img.flags = flags;
	ret.img.extent.width = size.width;
	ret.img.extent.height = size.height ? size.height : 1u;
	ret.img.extent.depth = size.depth ? size.depth : 1u;
	ret.img.imageType = size.depth > 1 ? 
		vk::ImageType::e3d : size.height > 1 ?
		vk::ImageType::e2d : vk::ImageType::e1d;
	ret.img.initialLayout = layout;
	ret.img.tiling = tiling;
	ret.img.arrayLayers = ret.img.mipLevels = 1u;
	ret.img.samples = samples;
	ret.img.sharingMode = vk::SharingMode::exclusive;
	ret.img.usage = usage;
	ret.img.format = findSupported(dev, formats, ret.img, additional);

	if(ret.img.format == vk::Format::undefined) {
		return {};
	}

	ret.view.viewType = size.depth > 1 ? 
		vk::ImageViewType::e3d : size.height > 1 ?
		vk::ImageViewType::e2d : vk::ImageViewType::e1d;
	ret.view.format = ret.img.format;
	ret.view.components = {}; // identity everywhere
	ret.view.subresourceRange = {aspect, 0, 1, 0, 1};

	return ret;
}

std::optional<ViewableImageCreateInfo> ViewableImageCreateInfo::color(
	const Device& dev, const vk::Extent3D& size, 
	nytl::Span<const vk::Format> formats, vk::ImageUsageFlags usage,
	vk::ImageTiling tiling, vk::SampleCountBits samples,
	vk::ImageLayout layout, vk::ImageCreateFlags flags,
	vk::FormatFeatureFlags additional)
{
	return general(dev, size, formats, usage, vk::ImageAspectBits::color,
		tiling, samples, layout, flags, additional);
}

std::optional<ViewableImageCreateInfo> ViewableImageCreateInfo::depth(
	const Device& dev, const vk::Extent3D& size, 
	nytl::Span<const vk::Format> formats, vk::ImageUsageFlags usage,
	vk::ImageTiling tiling, vk::SampleCountBits samples,
	vk::ImageLayout layout, vk::ImageCreateFlags flags,
	vk::FormatFeatureFlags additional)
{
	return general(dev, size, formats, usage, vk::ImageAspectBits::depth,
		tiling, samples, layout, flags, additional);
}

} // namespace vpp