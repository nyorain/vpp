// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/imageOps.hpp>
#include <vpp/transferWork.hpp> // vpp::transferWork
#include <vpp/queue.hpp> // vpp::Queue
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

/// Returns the memory address for an image for the given parameters.
vk::DeviceSize texelAddress(const vk::SubresourceLayout& layout,
		unsigned int texelSize, unsigned int x, unsigned int y,
		unsigned int z, unsigned int layer) {

	return layer * layout.arrayPitch + z * layout.depthPitch +
		y * layout.rowPitch + x * texelSize + layout.offset;
}

void fillMap(const Image& img, vk::Format format,
		const vk::Extent3D& size, nytl::Span<std::byte> data,
		const vk::ImageSubresource& subres, const vk::Offset3D& offset) {

	auto texSize = formatSize(format);

	dlg_assert(img.vkHandle());
	dlg_assert(img.mappable());
	dlg_assert(texSize > 0);
	dlg_assert(size.width != 0 && size.height != 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(data.size() == size.width * size.height * size.depth * texSize);
	dlg_assert(data.size() <= img.memorySize());

	auto sresLayout = vk::getImageSubresourceLayout(img.device(), img, subres);
	auto map = img.memoryEntry().map();

	// make it work for 2d images that specify a depth of zero
	auto depth = size.depth ? size.depth : 1u;
	auto doffset = 0u; // current data offset

	// copy row (width) after row
	for(unsigned int d = offset.z; d < offset.z + depth; ++d) {
		for(unsigned int h = offset.y; h < offset.y + size.height; ++h) {
			auto ptr = map.ptr() + texelAddress(sresLayout, texSize,
				offset.x, h, d, subres.arrayLayer);
			std::memcpy(ptr, data.data() + doffset, texSize * size.width);
			doffset += size.width * texSize;
		}
	}

	if(!map.coherent()) {
		map.flush();
	}
}

std::vector<std::byte> retrieveMap(const Image& img, vk::Format format,
		const vk::Extent3D& size, const vk::ImageSubresource& subres,
		const vk::Offset3D& offset) {

	const auto texSize = formatSize(format);
	const auto depth = size.depth ? size.depth : 1u;
	const auto byteSize = texSize * size.width * size.height * depth;

	dlg_assert(img.vkHandle());
	dlg_assert(img.mappable());
	dlg_assert(texSize > 0);
	dlg_assert(byteSize > 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(byteSize <= img.memorySize());
	dlg_assert(size.width != 0 && size.height != 0);

	std::vector<std::byte> data(byteSize);
	auto map = img.memoryMap();
	map.invalidate();

	auto sresLayout = vk::getImageSubresourceLayout(img.device(), img, subres);
	auto doffset = 0u; // dataOffset

	// copy row (width) after row
	for(unsigned int d = offset.z; d < offset.z + depth; ++d) {
		for(unsigned int h = offset.y; h < offset.y + size.height; ++h) {
			auto ptr = map.ptr() + texelAddress(sresLayout, texSize,
				offset.x, h, d, subres.arrayLayer);
			std::memcpy(data.data() + doffset, ptr, size.width);
			doffset += size.width * texSize;
		}
	}

	return data;
}

UploadWork fillStaging(const Image& img, vk::Format format,
		vk::ImageLayout layout, const vk::Extent3D& size,
		nytl::Span<const std::byte> data, const vk::ImageSubresource& subres,
		const vk::Offset3D& offset, QueueSubmitter* qsp) {

	auto& dev = img.device();
	auto& qs = qsp ? *qsp : dev.queueSubmitter();
	auto cmdBuf = dev.commandAllocator().get(qs.queue().family());

	vk::beginCommandBuffer(cmdBuf, {});
	auto range = fillStaging(cmdBuf, img, format, layout, size, data,
		subres, offset);
	vk::endCommandBuffer(cmdBuf);
	return {std::move(cmdBuf), qs, std::move(range)};
}

SubBuffer fillStaging(vk::CommandBuffer cmdBuf, const Image& img,
		vk::Format format, vk::ImageLayout layout, const vk::Extent3D& size,
		nytl::Span<const std::byte> data, const vk::ImageSubresource& subres,
		const vk::Offset3D& offset) {

	const auto texSize = formatSize(format);
	const auto depth = size.depth ? size.depth : 1u;

	dlg_assert(cmdBuf);
	dlg_assert(layout == vk::ImageLayout::transferDstOptimal ||
		layout == vk::ImageLayout::general);
	dlg_assert(img.vkHandle());
	dlg_assert(texSize > 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(data.size() == texSize * size.width * size.height * depth);
	dlg_assert(data.size() <= img.memorySize());
	dlg_assert(size.width != 0 && size.height != 0);

	// bufferOfset must be multiple of 4 and image format size
	auto align = img.device().properties().limits.optimalBufferCopyOffsetAlignment;
	align = std::max<vk::DeviceSize>(align, texSize);
	align = std::max<vk::DeviceSize>(align, 4u);
	auto stage = SubBuffer(img.device().bufferAllocator(), data.size(),
		vk::BufferUsageBits::transferSrc, img.device().hostMemoryTypes(), align);

	{
		auto map = stage.memoryMap();
		map.invalidate();
		std::memcpy(map.ptr(), data.data(), data.size());
	}

	auto buf = stage.buffer().vkHandle();
	auto boffset = stage.offset();
	vk::ImageSubresourceLayers layers {subres.aspectMask, subres.mipLevel,
		subres.arrayLayer, 1};
	vk::BufferImageCopy region {boffset, 0u, 0u, layers, offset,
		{size.width, size.height, depth}};

	vk::cmdCopyBufferToImage(cmdBuf, buf, img, layout, {region});
	return stage;
}

DownloadWork retrieveStaging(const Image& img, vk::Format format,
		vk::ImageLayout layout, const vk::Extent3D& size,
		const vk::ImageSubresource& subres, const vk::Offset3D& offset,
		QueueSubmitter* qsp) {

	auto& dev = img.device();
	auto& qs = qsp ? *qsp : dev.queueSubmitter();
	auto cmdBuf = dev.commandAllocator().get(qs.queue().family());

	vk::beginCommandBuffer(cmdBuf, {});
	auto range = retrieveStaging(cmdBuf, img, format, layout, size,
		subres, offset);
	vk::endCommandBuffer(cmdBuf);
	return {std::move(cmdBuf), qs, std::move(range)};
}

SubBuffer retrieveStaging(vk::CommandBuffer cmdBuf, const Image& img,
		vk::Format format, vk::ImageLayout layout, const vk::Extent3D& size,
		const vk::ImageSubresource& subres, const vk::Offset3D& offset) {

	const auto texSize = formatSize(format);
	const auto depth = size.depth ? size.depth : 1u;
	const auto byteSize = texSize * size.width * size.height * depth;

	dlg_assert(cmdBuf);
	dlg_assert(layout == vk::ImageLayout::transferSrcOptimal ||
		layout == vk::ImageLayout::general);
	dlg_assert(img.vkHandle());
	dlg_assert(texSize > 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(byteSize <= img.memorySize());
	dlg_assert(size.width != 0 && size.height != 0);

	// bufferOfset must be multiple of 4 and image format size
	auto align = img.device().properties().limits.optimalBufferCopyOffsetAlignment;
	align = std::max<vk::DeviceSize>(align, texSize);
	align = std::max<vk::DeviceSize>(align, 4u);
	auto stage = SubBuffer {img.device().bufferAllocator(), byteSize,
		vk::BufferUsageBits::transferDst, img.device().hostMemoryTypes(), align};

	auto buf = stage.buffer().vkHandle();
	auto boffset = stage.offset();
	vk::ImageSubresourceLayers layers {subres.aspectMask, subres.mipLevel,
		subres.arrayLayer, 1};
	vk::BufferImageCopy region {boffset, 0u, 0u, layers, offset,
		{size.width, size.height, depth}};

	vk::cmdCopyImageToBuffer(cmdBuf, img, layout, buf, {region});
	return stage;
}

void changeLayout(vk::CommandBuffer cmdBuf, vk::Image img,
		vk::ImageLayout ol, vk::PipelineStageFlags srcs, vk::AccessFlags srca,
		vk::ImageLayout nl, vk::PipelineStageFlags dsts, vk::AccessFlags dsta,
		const vk::ImageSubresourceRange& subres) {

	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = ol;
	barrier.newLayout = nl;
	barrier.image = img;
	barrier.subresourceRange = subres;
	barrier.srcAccessMask = srca;
	barrier.dstAccessMask = dsta;
	vk::cmdPipelineBarrier(cmdBuf, srcs, dsts, {}, {}, {}, {barrier});
}

CommandWork<void> changeLayout(vk::Image image,
		vk::ImageLayout ol, vk::PipelineStageFlags srcs, vk::AccessFlags srca,
		vk::ImageLayout nl, vk::PipelineStageFlags dsts, vk::AccessFlags dsta,
		const vk::ImageSubresourceRange& subres, QueueSubmitter& qs) {

	auto cmdBuf = qs.device().commandAllocator().get(qs.queue().family());
	vk::beginCommandBuffer(cmdBuf, {});
	changeLayout(cmdBuf, image, ol, srcs, srca, nl, dsts, dsta, subres);
	vk::endCommandBuffer(cmdBuf);
	return {qs, std::move(cmdBuf)};
}

// Utility functions
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
