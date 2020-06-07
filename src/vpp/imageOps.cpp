// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/imageOps.hpp>
#include <vpp/queue.hpp>
#include <vpp/formats.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>
#include <limits>

namespace vpp {

/// Returns the memory address for an image for the given parameters.
vk::DeviceSize texelAddress(const vk::SubresourceLayout& layout,
		unsigned int texelSize, unsigned int x, unsigned int y,
		unsigned int z, unsigned int layer) {
	return layer * layout.arrayPitch + z * layout.depthPitch +
		y * layout.rowPitch + x * texelSize + layout.offset;
}

void fillMap(const Image& img, vk::Format format,
		const vk::Extent3D& size, nytl::Span<const std::byte> data,
		const vk::ImageSubresource& subres, const vk::Offset3D& offset) {

	// make it work for 2d images that specify a depth of zero
	auto depth = size.depth ? size.depth : 1u;
	auto height = size.height ? size.height : 1u;
	auto fmtSize = formatSize(format);

	dlg_assert(img.vkHandle());
	dlg_assert(img.mappable());
	dlg_assert(fmtSize > 0);
	dlg_assert(size.width != 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(data.size() == size.width * height * depth * fmtSize);

	auto sresLayout = vk::getImageSubresourceLayout(img.device(), img, subres);

	auto mapBegin = texelAddress(sresLayout, fmtSize, offset.x,
		offset.y, offset.z, subres.arrayLayer);
	auto mapEnd = texelAddress(sresLayout, fmtSize, size.width,
		height - 1, depth - 1, subres.arrayLayer);
	auto map = img.memoryMap(mapBegin, mapEnd - mapBegin);

	auto doffset = 0u; // current data offset

	// copy row (width) after row
	for(unsigned int d = offset.z; d < offset.z + depth; ++d) {
		for(unsigned int h = offset.y; h < offset.y + height; ++h) {
			auto texOff = texelAddress(sresLayout, fmtSize,
				offset.x, h, d, subres.arrayLayer);
			auto ptr = map.ptr() + texOff - mapBegin;
			std::memcpy(ptr, data.data() + doffset, fmtSize * size.width);
			doffset += fmtSize * size.width;
		}
	}

	if(!map.coherent()) {
		map.flush();
	}
}

std::vector<std::byte> retrieveMap(const Image& img, vk::Format format,
		const vk::Extent3D& size, const vk::ImageSubresource& subres,
		const vk::Offset3D& offset) {

	const auto fmtSize = formatSize(format);
	const auto depth = size.depth ? size.depth : 1u;
	auto height = size.height ? size.height : 1u;
	const auto byteSize = fmtSize * size.width * height * depth;

	dlg_assert(img.vkHandle());
	dlg_assert(img.mappable());
	dlg_assert(fmtSize > 0);
	dlg_assert(byteSize > 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(size.width != 0);

	std::vector<std::byte> data(byteSize);

	auto sresLayout = vk::getImageSubresourceLayout(img.device(), img, subres);

	auto mapBegin = texelAddress(sresLayout, fmtSize, offset.x,
		offset.y, offset.z, subres.arrayLayer);
	auto mapEnd = texelAddress(sresLayout, fmtSize, size.width,
		height - 1, depth - 1, subres.arrayLayer);
	dlg_assert(mapEnd - mapBegin >= byteSize);
	auto map = img.memoryMap(mapBegin, mapEnd - mapBegin);

	if(!map.coherent()) {
		map.invalidate();
	}

	auto doffset = 0u; // dataOffset

	// copy row (width) after row
	for(unsigned int d = offset.z; d < offset.z + depth; ++d) {
		for(unsigned int h = offset.y; h < offset.y + height; ++h) {
			auto texOff = texelAddress(sresLayout, fmtSize,
				offset.x, h, d, subres.arrayLayer);
			auto ptr = map.ptr() + texOff - mapBegin;
			std::memcpy(data.data() + doffset, ptr, fmtSize * size.width);
			doffset += fmtSize * size.width;
		}
	}

	return data;
}

SubBuffer fillStagingRange(vk::CommandBuffer cb, const Image& img, vk::Format format,
		vk::ImageLayout layout, vk::Extent3D size, nytl::span<const std::byte> data,
		const vk::ImageSubresourceRange& range, const vk::Offset3D& offset) {

	const auto maxLvl = range.baseMipLevel + range.levelCount - 1;
	const auto fmtSize = std::uint64_t(formatSize(format));
	size.depth = std::max<vk::DeviceSize>(size.depth, 1u);
	size.height = std::max<vk::DeviceSize>(size.height, 1u);

	dlg_assert(cb);
	dlg_assert(range.levelCount > 0u);
	dlg_assert(range.layerCount > 0u);
	dlg_assert(img.vkHandle());
	dlg_assert(fmtSize > 0);
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	dlg_assert(size.width != 0);
	dlg_assert((size.width >> maxLvl) != 0 ||
		(size.height >> maxLvl) != 0 ||
		(size.depth >> maxLvl) != 0);
	// Offsets are not supported when filling something other than the base layer
	dlg_assert(!maxLvl || !offset.x);
	dlg_assert(!maxLvl || !offset.y);
	dlg_assert(!maxLvl || !offset.z);

	auto bsize = fmtSize * tightTexelCount(size, range.layerCount,
		range.levelCount, range.baseMipLevel);
	dlg_assert(data.size() == bsize);

	auto align = img.device().properties().limits.optimalBufferCopyOffsetAlignment;
	align = std::max<vk::DeviceSize>(align, fmtSize);
	align = std::max<vk::DeviceSize>(align, 4u);
	auto stage = SubBuffer(img.device().bufferAllocator(), bsize,
		vk::BufferUsageBits::transferSrc, img.device().hostMemoryTypes(), align);

	{
		auto map = stage.memoryMap();
		std::memcpy(map.ptr(), data.data(), data.size());
		map.flush();
	}

	auto buf = stage.buffer().vkHandle();
	auto boffset = stage.offset();

	vk::ImageSubresourceLayers subres;
	subres.aspectMask = range.aspectMask;
	subres.baseArrayLayer = range.baseArrayLayer;
	subres.layerCount = range.layerCount;
	for(auto i = 0u; i < range.levelCount; ++i) {
		auto l = range.baseMipLevel + i;
		subres.mipLevel = l;

		auto [iwidth, iheight, idepth] = mipSize(size, l);
		const auto ms = fmtSize * iwidth * iheight * idepth * range.layerCount;
		vk::BufferImageCopy region {boffset, 0u, 0u, subres, offset,
			{iwidth, iheight, idepth}};
		vk::cmdCopyBufferToImage(cb, buf, img, layout, {{region}});
		boffset += ms;
	}

	return stage;
}

SubBuffer fillStagingLayers(vk::CommandBuffer cb, const Image& img,
		vk::Format format, vk::ImageLayout layout, const vk::Extent3D& size,
		nytl::Span<const std::byte> data,
		const vk::ImageSubresourceLayers& subres, const vk::Offset3D& offset) {

	vk::ImageSubresourceRange layers;
	layers.aspectMask = subres.aspectMask;
	layers.baseArrayLayer = subres.baseArrayLayer;
	layers.layerCount = subres.layerCount;
	layers.baseMipLevel = subres.mipLevel;
	layers.levelCount = 1u;
	return fillStagingRange(cb, img, format, layout, size, data, layers, offset);
}

SubBuffer fillStaging(vk::CommandBuffer cb, const Image& img,
		vk::Format format, vk::ImageLayout layout, const vk::Extent3D& size,
		nytl::Span<const std::byte> data,
		const vk::ImageSubresource& subres, const vk::Offset3D& offset) {
	vk::ImageSubresourceRange layers;
	layers.aspectMask = subres.aspectMask;
	layers.baseArrayLayer = subres.arrayLayer;
	layers.layerCount = 1u;
	layers.baseMipLevel = subres.mipLevel;
	layers.levelCount = 1u;
	return fillStagingRange(cb, img, format, layout, size, data, layers, offset);
}

SubBuffer retrieveStagingRange(vk::CommandBuffer cb, const Image& img, vk::Format format,
		vk::ImageLayout layout, vk::Extent3D size,
		const vk::ImageSubresourceRange& range, const vk::Offset3D& offset) {

	const auto maxLvl = range.baseMipLevel + range.levelCount - 1;
	const auto fmtSize = std::uint64_t(formatSize(format));
	size.depth = std::max<vk::DeviceSize>(size.depth, 1u);
	size.height = std::max<vk::DeviceSize>(size.height, 1u);

	dlg_assert(cb); // valid command buffer handle
	dlg_assert(range.levelCount > 0u);
	dlg_assert(range.layerCount > 0u);
	dlg_assert(img.vkHandle()); // image is valid
	dlg_assert(fmtSize > 0); // format isn't undefined/weird
	dlg_assert(size.width != 0); // at least 1D texture
	// We don't support compressed textures yet.
	dlg_assert(blockSize(format).width == 1 && blockSize(format).height == 1);
	// first, check for reasonable level count.
	// If it's more, we might get problems with the shifting below.
	dlg_assert(maxLvl < 32);
	// Check that the size is valid on all mips
	dlg_assert((size.width >> maxLvl) != 0 ||
		(size.height >> maxLvl) != 0 ||
		(size.depth >> maxLvl) != 0);
	// Check that offset makes sense
	dlg_assert(offset.x >= 0 && offset.y >= 0 && offset.z >= 0);
	// Offsets are not supported when retrieving something other than the base layer
	dlg_assert(!maxLvl || !offset.x);
	dlg_assert(!maxLvl || !offset.y);
	dlg_assert(!maxLvl || !offset.z);

	auto bsize = fmtSize * tightTexelCount(size, range.layerCount,
		range.levelCount, range.baseMipLevel);

	// bufferOfset must be multiple of 4 and image format size
	auto align = img.device().properties().limits.optimalBufferCopyOffsetAlignment;
	align = std::max<vk::DeviceSize>(align, fmtSize);
	align = std::max<vk::DeviceSize>(align, 4u);
	auto stage = SubBuffer {img.device().bufferAllocator(), bsize,
		vk::BufferUsageBits::transferDst, img.device().hostMemoryTypes(), align};

	auto buf = stage.buffer().vkHandle();
	auto boffset = stage.offset();

	vk::ImageSubresourceLayers subres;
	subres.aspectMask = range.aspectMask;
	subres.baseArrayLayer = range.baseArrayLayer;
	subres.layerCount = range.layerCount;
	for(auto i = 0u; i < range.levelCount; ++i) {
		auto l = range.baseMipLevel + i;
		subres.mipLevel = l;

		auto [iwidth, iheight, idepth] = mipSize(size, l);
		const auto ms = fmtSize * iwidth * iheight * idepth * range.layerCount;

		vk::BufferImageCopy region {boffset, 0u, 0u, subres, offset,
			{iwidth, iheight, idepth}};
		vk::cmdCopyImageToBuffer(cb, img, layout, buf, {{region}});
		boffset += ms;
	}

	return stage;
}

SubBuffer retrieveStagingLayers(vk::CommandBuffer cb, const Image& img,
		vk::Format format, vk::ImageLayout layout, const vk::Extent3D& size,
		const vk::ImageSubresourceLayers& subres, const vk::Offset3D& offset) {
	vk::ImageSubresourceRange range;
	range.aspectMask = subres.aspectMask;
	range.baseArrayLayer = subres.baseArrayLayer;;
	range.layerCount = subres.layerCount;
	range.baseMipLevel = subres.mipLevel;
	range.levelCount = 1u;
	return retrieveStagingRange(cb, img, format, layout, size, range, offset);
}

SubBuffer retrieveStaging(vk::CommandBuffer cb, const Image& img,
		vk::Format format, vk::ImageLayout layout, const vk::Extent3D& size,
		const vk::ImageSubresource& subres, const vk::Offset3D& offset) {
	vk::ImageSubresourceRange range;
	range.aspectMask = subres.aspectMask;
	range.baseArrayLayer = subres.arrayLayer;
	range.baseMipLevel = subres.mipLevel;
	range.layerCount = 1u;
	range.levelCount = 1u;
	return retrieveStagingRange(cb, img, format, layout, size, range, offset);
}

vk::DeviceSize tightTexelCount(vk::Extent3D extent,
		unsigned numLayers, unsigned numMips, unsigned firstMip) {
	dlg_assert(firstMip + numMips <= mipmapLevels(extent));

	vk::DeviceSize off = 0u;
	for(auto i = firstMip; i < firstMip + numMips; ++i) {
		auto ie = mipSize(extent, i);
		off += ie.width * ie.height * ie.depth * numLayers;
	}

	return off;
}

vk::DeviceSize tightTexelNumber(vk::Extent3D extent,
		unsigned numLayers, unsigned mip, unsigned layer,
		unsigned x, unsigned y, unsigned z,
		unsigned firstMip) {
	dlg_assert(layer < numLayers);
	dlg_assert(mip < mipmapLevels(extent));
	dlg_assert(firstMip <= mip);

	vk::DeviceSize off = 0u;
	for(auto i = firstMip; i < mip; ++i) {
		auto ie = mipSize(extent, i);
		off += ie.width * ie.height * ie.depth * numLayers;
	}

	auto ie = mipSize(extent, mip);
	auto ltn = tightLayerTexelNumber(extent, x, y, z);
	return off + layer * ie.width * ie.height * ie.depth + ltn;
}

vk::DeviceSize imageBufferOffset(unsigned formatSize, vk::Extent3D extent,
		unsigned numLayers, unsigned mip, unsigned layer) {
	dlg_assert(layer < numLayers);
	dlg_assert(mip < mipmapLevels(extent));

	std::uint64_t off = 0u;
	for(auto i = 0u; i < mip; ++i) {
		off += extent.width * extent.height * extent.depth * numLayers * formatSize;

		extent.width = std::max(extent.width >> 1u, 1u);
		extent.height = std::max(extent.height >> 1u, 1u);
		extent.depth = std::max(extent.depth >> 1u, 1u);
	}

	auto s = extent.width * extent.height * extent.depth * formatSize;
	return off + layer * s;
}

} // namespace vpp
