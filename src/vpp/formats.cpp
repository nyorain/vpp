// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/formats.hpp>
#include <vpp/device.hpp>
#include <vkpp/functions.hpp>
#include <cmath>

namespace vpp {

bool supportedUsage(vk::FormatFeatureFlags features, vk::ImageUsageFlags usages) {
	static constexpr struct {
		vk::ImageUsageBits usage;
		vk::FormatFeatureBits feature;
	} maps[] = {
		{vk::ImageUsageBits::sampled, vk::FormatFeatureBits::sampledImage},
		{vk::ImageUsageBits::storage, vk::FormatFeatureBits::storageImage},
		{vk::ImageUsageBits::colorAttachment, vk::FormatFeatureBits::colorAttachment},
		{vk::ImageUsageBits::depthStencilAttachment,
			vk::FormatFeatureBits::depthStencilAttachment},
		// vulkan 1.1. We would need to know whether application uses 1.1
		// {vk::ImageUsageBits::transferSrc, vk::FormatFeatureBits::transferSrc},
		// {vk::ImageUsageBits::transferDst, vk::FormatFeatureBits::transferDst},
	};

	for(const auto& map : maps) {
		if((usages & map.usage) && !(features & map.feature)) {
			return false;
		}
	}

	return true;
}

bool supportedUsage(vk::FormatFeatureFlags features, vk::BufferUsageFlags usages) {
	static constexpr struct {
		vk::BufferUsageBits usage;
		vk::FormatFeatureBits feature;
	} maps[] = {
		{vk::BufferUsageBits::uniformTexelBuffer,
			vk::FormatFeatureBits::uniformTexelBuffer},
		{vk::BufferUsageBits::storageTexelBuffer,
			vk::FormatFeatureBits::storageTexelBuffer},
		{vk::BufferUsageBits::vertexBuffer,
			vk::FormatFeatureBits::vertexBuffer},
	};

	for(const auto& map : maps) {
		if((usages & map.usage) && !(features & map.feature)) {
			return false;
		}
	}

	return true;
}

// TODO: fix try/catch when vkpp has proper error forwarding
bool supported(const Device& dev, const vk::ImageCreateInfo& info,
		vk::FormatFeatureFlags additional) {
	auto props = vk::getPhysicalDeviceFormatProperties(
		dev.vkPhysicalDevice(), info.format);
	auto features = (info.tiling == vk::ImageTiling::linear) ?
		props.linearTilingFeatures : props.optimalTilingFeatures;

	try {
		auto imgProps = vk::getPhysicalDeviceImageFormatProperties(
			dev.vkPhysicalDevice(), info.format, info.imageType, info.tiling,
			info.usage, info.flags);
		return ((features & additional) == additional &&
			supportedUsage(features, info.usage) &&
			info.extent.width <= imgProps.maxExtent.width &&
			info.extent.height <= imgProps.maxExtent.height &&
			info.extent.depth <= imgProps.maxExtent.depth &&
			info.mipLevels <= imgProps.maxMipLevels &&
			info.arrayLayers <= imgProps.maxArrayLayers &&
			(imgProps.sampleCounts & info.samples));
	} catch(const vk::VulkanError& err) {
		if(err.error == vk::Result::errorFormatNotSupported) {
			return false;
		} else {
			// we can't handle the error in this case; rethrow
			throw;
		}
	}
}

bool supported(const Device& dev, vk::Format format,
		vk::BufferUsageFlags use, vk::FormatFeatureFlags additional) {
	try {
		auto props = vk::getPhysicalDeviceFormatProperties(
			dev.vkPhysicalDevice(), format);
		return ((props.bufferFeatures & additional) == additional) &&
			supportedUsage(props.bufferFeatures, use);
	} catch(const vk::VulkanError& err) {
		if(err.error == vk::Result::errorFormatNotSupported) {
			return false;
		} else {
			// we can't handle the error in this case; rethrow
			throw;
		}
	}
}

vk::Format findSupported(const Device& dev, nytl::Span<const vk::Format> formats,
		const vk::ImageCreateInfo& info, vk::FormatFeatureFlags additional) {
	auto copy = info;
	for(auto format : formats) {
		copy.format = format;
		if(supported(dev, copy, additional)) {
			return format;
		}
	}

	return vk::Format::undefined;
}

vk::Format findSupported(const Device& dev, nytl::Span<const vk::Format> formats,
		vk::BufferUsageFlags use, vk::FormatFeatureFlags additional) {
	for(auto format : formats) {
		if(supported(dev, format, use, additional)) {
			return format;
		}
	}

	return vk::Format::undefined;
}

// complete mipmap chain as specified in
// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap11.html#resources-image-miplevel-sizing
unsigned mipmapLevels(const vk::Extent2D& extent) {
	return 1 + std::floor(std::log2(std::max(extent.width, extent.height)));
}

ViewableImageCreateInfo::ViewableImageCreateInfo(vk::Format format,
		vk::ImageAspectBits aspect, vk::Extent2D size,
		vk::ImageUsageFlags usage, vk::ImageTiling tiling,
		uint32_t mipLevels) {
	img.extent = {size.width, size.height, 1u};
	img.imageType = vk::ImageType::e2d;
	img.usage = usage;
	img.arrayLayers = 1u;
	img.mipLevels = mipLevels;
	img.tiling = tiling;
	img.format = format;
	img.samples = vk::SampleCountBits::e1;
	img.sharingMode = vk::SharingMode::exclusive;

	view.format = format;
	view.subresourceRange = {aspect, 0, mipLevels, 0, 1};
	view.components = {}; // identity
	view.viewType = vk::ImageViewType::e2d;
}

} // namespace vpp
