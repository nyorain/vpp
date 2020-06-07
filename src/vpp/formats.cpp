// Copyright (c) 2016-2020 Jan Kelling
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

unsigned mipmapLevels(const vk::Extent3D& extent) {
	auto m = std::max(extent.width, std::max(extent.height, extent.depth));
	return 1 + std::floor(std::log2(m));
}

vk::Extent3D mipSize(vk::Extent3D size, unsigned l) {
	return {
		std::max(size.width >> l, 1u),
		std::max(size.height >> l, 1u),
		std::max(size.depth >> l, 1u),
	};
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

unsigned int formatSizeBits(vk::Format format) {
	using namespace vk;
	switch(format) {
		case Format::undefined: return 0;
		case Format::r4g4UnormPack8: return 8;
		case Format::r4g4b4a4UnormPack16: return 16;
		case Format::b4g4r4a4UnormPack16: return 16;
		case Format::r5g6b5UnormPack16: return 16;
		case Format::b5g6r5UnormPack16: return 16;
		case Format::r5g5b5a1UnormPack16: return 16;
		case Format::b5g5r5a1UnormPack16: return 16;
		case Format::a1r5g5b5UnormPack16: return 16;
		case Format::r8Unorm: return 8;
		case Format::r8Snorm: return 8;
		case Format::r8Uscaled: return 8;
		case Format::r8Sscaled: return 8;
		case Format::r8Uint: return 8;
		case Format::r8Sint: return 8;
		case Format::r8Srgb: return 8;
		case Format::r8g8Unorm: return 16;
		case Format::r8g8Snorm: return 16;
		case Format::r8g8Uscaled: return 16;
		case Format::r8g8Sscaled: return 16;
		case Format::r8g8Uint: return 16;
		case Format::r8g8Sint: return 16;
		case Format::r8g8Srgb: return 16;
		case Format::r8g8b8Unorm: return 24;
		case Format::r8g8b8Snorm: return 24;
		case Format::r8g8b8Uscaled: return 24;
		case Format::r8g8b8Sscaled: return 24;
		case Format::r8g8b8Uint: return 24;
		case Format::r8g8b8Sint: return 24;
		case Format::r8g8b8Srgb: return 24;
		case Format::b8g8r8Unorm: return 24;
		case Format::b8g8r8Snorm: return 24;
		case Format::b8g8r8Uscaled: return 24;
		case Format::b8g8r8Sscaled: return 24;
		case Format::b8g8r8Uint: return 24;
		case Format::b8g8r8Sint: return 24;
		case Format::b8g8r8Srgb: return 24;
		case Format::r8g8b8a8Unorm: return 32;
		case Format::r8g8b8a8Snorm: return 32;
		case Format::r8g8b8a8Uscaled: return 32;
		case Format::r8g8b8a8Sscaled: return 32;
		case Format::r8g8b8a8Uint: return 32;
		case Format::r8g8b8a8Sint: return 32;
		case Format::r8g8b8a8Srgb: return 32;
		case Format::b8g8r8a8Unorm: return 32;
		case Format::b8g8r8a8Snorm: return 32;
		case Format::b8g8r8a8Uscaled: return 32;
		case Format::b8g8r8a8Sscaled: return 32;
		case Format::b8g8r8a8Uint: return 32;
		case Format::b8g8r8a8Sint: return 32;
		case Format::b8g8r8a8Srgb: return 32;
		case Format::a8b8g8r8UnormPack32: return 32;
		case Format::a8b8g8r8SnormPack32: return 32;
		case Format::a8b8g8r8UscaledPack32: return 32;
		case Format::a8b8g8r8SscaledPack32: return 32;
		case Format::a8b8g8r8UintPack32: return 32;
		case Format::a8b8g8r8SintPack32: return 32;
		case Format::a8b8g8r8SrgbPack32: return 32;
		case Format::a2r10g10b10UnormPack32: return 32;
		case Format::a2r10g10b10SnormPack32: return 32;
		case Format::a2r10g10b10UscaledPack32: return 32;
		case Format::a2r10g10b10SscaledPack32: return 32;
		case Format::a2r10g10b10UintPack32: return 32;
		case Format::a2r10g10b10SintPack32: return 32;
		case Format::a2b10g10r10UnormPack32: return 32;
		case Format::a2b10g10r10SnormPack32: return 32;
		case Format::a2b10g10r10UscaledPack32: return 32;
		case Format::a2b10g10r10SscaledPack32: return 32;
		case Format::a2b10g10r10UintPack32: return 32;
		case Format::a2b10g10r10SintPack32: return 32;
		case Format::r16Unorm: return 16;
		case Format::r16Snorm: return 16;
		case Format::r16Uscaled: return 16;
		case Format::r16Sscaled: return 16;
		case Format::r16Uint: return 16;
		case Format::r16Sint: return 16;
		case Format::r16Sfloat: return 16;
		case Format::r16g16Unorm: return 32;
		case Format::r16g16Snorm: return 32;
		case Format::r16g16Uscaled: return 32;
		case Format::r16g16Sscaled: return 32;
		case Format::r16g16Uint: return 32;
		case Format::r16g16Sint: return 32;
		case Format::r16g16Sfloat: return 32;
		case Format::r16g16b16Unorm: return 48;
		case Format::r16g16b16Snorm: return 48;
		case Format::r16g16b16Uscaled: return 48;
		case Format::r16g16b16Sscaled: return 48;
		case Format::r16g16b16Uint: return 48;
		case Format::r16g16b16Sint: return 48;
		case Format::r16g16b16Sfloat: return 48;
		case Format::r16g16b16a16Unorm: return 64;
		case Format::r16g16b16a16Snorm: return 64;
		case Format::r16g16b16a16Uscaled: return 64;
		case Format::r16g16b16a16Sscaled: return 64;
		case Format::r16g16b16a16Uint: return 64;
		case Format::r16g16b16a16Sint: return 64;
		case Format::r16g16b16a16Sfloat: return 64;
		case Format::r32Uint: return 32;
		case Format::r32Sint: return 32;
		case Format::r32Sfloat: return 32;
		case Format::r32g32Uint: return 64;
		case Format::r32g32Sint: return 64;
		case Format::r32g32Sfloat: return 64;
		case Format::r32g32b32Uint: return 96;
		case Format::r32g32b32Sint: return 96;
		case Format::r32g32b32Sfloat: return 96;
		case Format::r32g32b32a32Uint: return 128;
		case Format::r32g32b32a32Sint: return 128;
		case Format::r32g32b32a32Sfloat: return 128;
		case Format::r64Uint: return 64;
		case Format::r64Sint: return 64;
		case Format::r64Sfloat: return 64;
		case Format::r64g64Uint: return 128;
		case Format::r64g64Sint: return 128;
		case Format::r64g64Sfloat: return 128;
		case Format::r64g64b64Uint: return 192;
		case Format::r64g64b64Sint: return 192;
		case Format::r64g64b64Sfloat: return 192;
		case Format::r64g64b64a64Uint: return 256;
		case Format::r64g64b64a64Sint: return 256;
		case Format::r64g64b64a64Sfloat: return 256;
		case Format::b10g11r11UfloatPack32: return 32;
		case Format::e5b9g9r9UfloatPack32: return 32;
		case Format::d16Unorm: return 16;
		case Format::x8D24UnormPack32: return 32;
		case Format::d32Sfloat: return 32;
		case Format::s8Uint: return 8;
		case Format::d16UnormS8Uint: return 24;
		case Format::d24UnormS8Uint: return 32;
		case Format::d32SfloatS8Uint: return 48;

		case Format::bc1RgbUnormBlock: return 64;
		case Format::bc1RgbSrgbBlock: return 64;
		case Format::bc1RgbaUnormBlock: return 64;
		case Format::bc1RgbaSrgbBlock: return 64;
		case Format::bc2UnormBlock: return 128;
		case Format::bc2SrgbBlock: return 128;
		case Format::bc3UnormBlock: return 128;
		case Format::bc3SrgbBlock: return 128;
		case Format::bc4UnormBlock: return 64;
		case Format::bc4SnormBlock: return 64;
		case Format::bc5UnormBlock: return 128;
		case Format::bc5SnormBlock: return 128;
		case Format::bc6hUfloatBlock: return 128;
		case Format::bc6hSfloatBlock: return 128;
		case Format::bc7UnormBlock: return 128;
		case Format::bc7SrgbBlock: return 128;
		case Format::etc2R8g8b8UnormBlock: return 64;
		case Format::etc2R8g8b8SrgbBlock: return 64;
		case Format::etc2R8g8b8a1UnormBlock: return 64;
		case Format::etc2R8g8b8a1SrgbBlock: return 64;
		case Format::etc2R8g8b8a8UnormBlock: return 128;
		case Format::etc2R8g8b8a8SrgbBlock: return 64;
		case Format::eacR11UnormBlock: return 64;
		case Format::eacR11SnormBlock: return 64;
		case Format::eacR11g11UnormBlock: return 128;
		case Format::eacR11g11SnormBlock: return 128;
		case Format::astc4x4UnormBlock: return 128;
		case Format::astc4x4SrgbBlock: return 128;
		case Format::astc5x4UnormBlock: return 128;
		case Format::astc5x4SrgbBlock: return 128;
		case Format::astc5x5UnormBlock: return 128;
		case Format::astc5x5SrgbBlock: return 128;
		case Format::astc6x5UnormBlock: return 128;
		case Format::astc6x5SrgbBlock: return 128;
		case Format::astc6x6UnormBlock: return 128;
		case Format::astc6x6SrgbBlock: return 128;
		case Format::astc8x5UnormBlock: return 128;
		case Format::astc8x5SrgbBlock: return 128;
		case Format::astc8x6UnormBlock: return 128;
		case Format::astc8x6SrgbBlock: return 128;
		case Format::astc8x8UnormBlock: return 128;
		case Format::astc8x8SrgbBlock: return 128;
		case Format::astc10x5UnormBlock: return 128;
		case Format::astc10x5SrgbBlock: return 128;
		case Format::astc10x6UnormBlock: return 128;
		case Format::astc10x6SrgbBlock: return 128;
		case Format::astc10x8UnormBlock: return 128;
		case Format::astc10x8SrgbBlock: return 128;
		case Format::astc10x10UnormBlock: return 128;
		case Format::astc10x10SrgbBlock: return 128;
		case Format::astc12x10UnormBlock: return 128;
		case Format::astc12x10SrgbBlock: return 128;
		case Format::astc12x12UnormBlock: return 128;
		case Format::astc12x12SrgbBlock: return 128;
		default: return 0;
	}
}

unsigned int formatSize(vk::Format format) {
	return formatSizeBits(format) / 8;
}

vk::Extent2D blockSize(vk::Format format) {
	using vk::Format;
	switch(format) {
	case Format::bc1RgbUnormBlock: return vk::Extent2D{4, 4};
		case Format::bc1RgbSrgbBlock: return {4, 4};
		case Format::bc1RgbaUnormBlock: return {4, 4};
		case Format::bc1RgbaSrgbBlock: return {4, 4};
		case Format::bc2UnormBlock: return {4, 4};
		case Format::bc2SrgbBlock: return {4, 4};
		case Format::bc3UnormBlock: return {4, 4};
		case Format::bc3SrgbBlock: return {4, 4};
		case Format::bc4UnormBlock: return {4, 4};
		case Format::bc4SnormBlock: return {4, 4};
		case Format::bc5UnormBlock: return {4, 4};
		case Format::bc5SnormBlock: return {4, 4};
		case Format::bc6hUfloatBlock: return {4, 4};
		case Format::bc6hSfloatBlock: return {4, 4};
		case Format::bc7UnormBlock: return {4, 4};
		case Format::bc7SrgbBlock: return {4, 4};
		case Format::etc2R8g8b8UnormBlock: return {4, 4};
		case Format::etc2R8g8b8SrgbBlock: return {4, 4};
		case Format::etc2R8g8b8a1UnormBlock: return {4, 4};
		case Format::etc2R8g8b8a1SrgbBlock: return {4, 4};
		case Format::etc2R8g8b8a8UnormBlock: return {4, 4};
		case Format::etc2R8g8b8a8SrgbBlock: return {4, 4};
		case Format::eacR11UnormBlock: return {4, 4};
		case Format::eacR11SnormBlock: return {4, 4};
		case Format::eacR11g11UnormBlock: return {4, 4};
		case Format::eacR11g11SnormBlock: return {4, 4};
		case Format::astc4x4UnormBlock: return {4, 4};
		case Format::astc4x4SrgbBlock: return {4, 4};
		case Format::astc5x4UnormBlock: return {5, 4};
		case Format::astc5x4SrgbBlock: return {5, 4};
		case Format::astc5x5UnormBlock: return {5, 5};
		case Format::astc5x5SrgbBlock: return {5, 5};
		case Format::astc6x5UnormBlock: return {6, 5};
		case Format::astc6x5SrgbBlock: return {6, 5};
		case Format::astc6x6UnormBlock: return {6, 6};
		case Format::astc6x6SrgbBlock: return {6, 6};
		case Format::astc8x5UnormBlock: return {8, 5};
		case Format::astc8x5SrgbBlock: return {8, 5};
		case Format::astc8x6UnormBlock: return {8, 6};
		case Format::astc8x6SrgbBlock: return {8, 6};
		case Format::astc8x8UnormBlock: return {8, 8};
		case Format::astc8x8SrgbBlock: return {8, 8};
		case Format::astc10x5UnormBlock: return {10, 5};
		case Format::astc10x5SrgbBlock: return {10, 5};
		case Format::astc10x6UnormBlock: return {10, 6};
		case Format::astc10x6SrgbBlock: return {10, 6};
		case Format::astc10x8UnormBlock: return {10, 8};
		case Format::astc10x8SrgbBlock: return {10, 8};
		case Format::astc10x10UnormBlock: return {10, 10};
		case Format::astc10x10SrgbBlock: return {10, 10};
		case Format::astc12x10UnormBlock: return {12, 10};
		case Format::astc12x10SrgbBlock: return {12, 10};
		case Format::astc12x12UnormBlock: return {12, 12};
		case Format::astc12x12SrgbBlock: return {12, 12};
		case vk::Format::undefined: return {0, 0};
		default: return {1, 1};
	}
}

} // namespace vpp
