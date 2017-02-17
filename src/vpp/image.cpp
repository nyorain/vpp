// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/image.hpp>
#include <vpp/transfer.hpp> // vpp::TransferManager
#include <vpp/transferWork.hpp> // vpp::transferWork
#include <vpp/queue.hpp> // vpp::Queue
#include <vpp/util/debug.hpp> // VPP_DEBUG_CHECK
#include <vpp/vk.hpp>

#include <utility> // std::move, std::swap
#include <cstring> // std::memcpy
#include <memory> // std::make_unique
#include <vector> // std::vector

namespace vpp {
namespace {

/// Returns the memory address for an image for the given parameters.
vk::DeviceSize imageAddress(const vk::SubresourceLayout& layout, unsigned int texelSize,
	unsigned int x, unsigned int y, unsigned int z, unsigned int layer)
{
	return layer * layout.arrayPitch + z * layout.depthPitch + y * layout.rowPitch +
		x * texelSize + layout.offset;
}

} // anonymous util namespace

// Image
Image::Image(const Device& dev, const vk::ImageCreateInfo& info, unsigned int memoryTypeBits)
{
	handle_ = vk::createImage(dev.vkDevice(), info);
	auto reqs = vk::getImageMemoryRequirements(dev.vkDevice(), vkHandle());

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, info.tiling, memoryEntry_);
}

Image::Image(const Device& dev, vk::Image image, vk::ImageTiling tiling,
	unsigned int memoryTypeBits)
{
	handle_ = image;
	auto reqs = vk::getImageMemoryRequirements(dev.vkDevice(), vkHandle());
	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, tiling, memoryEntry_);
}

Image::Image(vk::Image image, MemoryEntry&& entry)
{
	handle_ = image;
	memoryEntry_ = std::move(entry);
}

Image::~Image()
{
	if(vkHandle()) vk::destroyImage(device(), vkHandle());
}

WorkPtr fill(const Image& image, const uint8_t& data, vk::Format format,
	vk::ImageLayout& layout, const vk::Extent3D& extent, const vk::ImageSubresource& subres,
	const vk::Offset3D& offset, bool allowMap)
{
	image.assureMemory();
	const auto texSize = formatSize(format);

	if(image.mappable() && allowMap) {

		// baiscally the size of the format in bytes. Can be computed from the given size/extent.
		auto sresLayout = vk::getImageSubresourceLayout(image.device(), image, subres);
		auto map = image.memoryEntry().map();

		// make it work for 2d images that specify a depth of zero
		auto depth = extent.depth ? extent.depth : 1;
		auto doffset = 0u; // current data offset
		for(unsigned int d = offset.z; d < offset.z + depth; ++d) {
			for(unsigned int h = offset.y; h < offset.y + extent.height; ++h) {
				auto ioff = imageAddress(sresLayout, texSize, offset.x, h, d, subres.arrayLayer);
				std::memcpy(map.ptr() + ioff, &data + doffset, texSize * extent.width);
				doffset += extent.width * texSize;
			}
		}

		if(!map.coherent()) map.flush();
		return std::make_unique<FinishedWork<void>>();
	} else {
		const auto byteSize = texSize * extent.width * extent.height * extent.depth;
		const Queue* queue;
		auto qFam = transferQueueFamily(image.device(), &queue);
		auto cmdBuffer = image.device().commandProvider().get(qFam);
		auto uploadBuffer = image.device().transferManager().buffer(byteSize);
		fill140(uploadBuffer.buffer(), raw(data, byteSize));

		vk::BufferImageCopy region;
		region.imageOffset = offset;
		region.imageExtent = extent;
		region.imageSubresource = {subres.aspectMask, subres.mipLevel, subres.arrayLayer, 1};

		vk::beginCommandBuffer(cmdBuffer, {});

		// change layout if needed
		if(layout != vk::ImageLayout::transferDstOptimal && layout != vk::ImageLayout::general) {
			changeLayoutCommand(cmdBuffer, image, layout, vk::ImageLayout::transferDstOptimal,
				{subres.aspectMask, subres.mipLevel, 1, subres.arrayLayer, 1});
			layout = vk::ImageLayout::transferDstOptimal;
		}

		vk::cmdCopyBufferToImage(cmdBuffer, uploadBuffer.buffer(), image, layout, {region});
		vk::endCommandBuffer(cmdBuffer);

		return std::make_unique<UploadWork>(std::move(cmdBuffer), *queue, std::move(uploadBuffer));
	}
}

DataWorkPtr retrieve(const Image& image, vk::ImageLayout& layout, vk::Format format,
	const vk::Extent3D& extent, const vk::ImageSubresource& subres, const vk::Offset3D& offset,
	bool allowMap)
{
	VPP_DEBUG_CHECK("vpp::retrieve(image)", {
		if(!image.memoryEntry().allocated()) VPP_CHECK_THROW("Image has no memory");
	})

	if(image.mappable() && allowMap) {
		std::vector<std::uint8_t> data(image.memorySize());
		auto map = image.memoryMap();

		//baiscally the size of the format in bytes. Can be computed from the given size/extent.
		const auto texelSize = formatSize(format);
		auto subresLayout = vk::getImageSubresourceLayout(image.device(), image, subres);

		auto depth = extent.depth ? extent.depth : 1;
		auto doffset = 0u; // dataOffset
		for(unsigned int d = offset.z; d < offset.z + depth; ++d) {
			for(unsigned int h = offset.y; h < offset.y + extent.height; ++h) {
				auto ioffset = imageAddress(subresLayout, texelSize, 0, h, d, subres.arrayLayer);
				std::memcpy(data.data() + doffset, map.ptr() + ioffset, extent.width);
				doffset += extent.width;
			}
		}

		return std::make_unique<StoredDataWork>(std::move(data));
	} else {
		const Queue* queue;
		auto qFam = transferQueueFamily(image.device(), &queue);
		auto cmdBuffer = image.device().commandProvider().get(qFam);
		auto downloadBuffer = image.device().transferManager().buffer(image.memorySize());

		vk::beginCommandBuffer(cmdBuffer, {});

		//change layout if needed
		if(layout != vk::ImageLayout::transferSrcOptimal && layout != vk::ImageLayout::general) {
			changeLayoutCommand(cmdBuffer, image, layout, vk::ImageLayout::transferSrcOptimal,
				{subres.aspectMask, subres.mipLevel, 1, subres.arrayLayer, 1});
			layout = vk::ImageLayout::transferSrcOptimal;
		}

		vk::BufferImageCopy region;
		region.imageOffset = offset;
		region.imageExtent = extent;
		region.imageSubresource = {subres.aspectMask, subres.mipLevel, subres.arrayLayer, 1};

		vk::cmdCopyImageToBuffer(cmdBuffer, image, layout, downloadBuffer.buffer(), {region});
		vk::endCommandBuffer(cmdBuffer);

		return std::make_unique<DownloadWork>(std::move(cmdBuffer), *queue,
			std::move(downloadBuffer));
	}
}

//free utility functions
void changeLayoutCommand(vk::CommandBuffer cmdBuffer, vk::Image img, vk::ImageLayout ol,
	vk::ImageLayout nl, const vk::ImageSubresourceRange& range)
{
	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = ol;
	barrier.newLayout = nl;
	barrier.image = img;
	barrier.subresourceRange = range;

	switch(ol) {
		case vk::ImageLayout::undefined:
			barrier.srcAccessMask = {}; break;
		case vk::ImageLayout::preinitialized:
			barrier.srcAccessMask = vk::AccessBits::hostWrite; break;
		case vk::ImageLayout::colorAttachmentOptimal:
			barrier.srcAccessMask = vk::AccessBits::colorAttachmentWrite; break;
		case vk::ImageLayout::depthStencilAttachmentOptimal:
			barrier.srcAccessMask = vk::AccessBits::depthStencilAttachmentWrite; break;
		case vk::ImageLayout::transferSrcOptimal:
			barrier.srcAccessMask = vk::AccessBits::transferRead; break;
		case vk::ImageLayout::transferDstOptimal:
			barrier.srcAccessMask = vk::AccessBits::transferWrite; break;
		case vk::ImageLayout::shaderReadOnlyOptimal:
			barrier.srcAccessMask = vk::AccessBits::shaderRead; break;
		default:
			break;
	}

	switch(nl) {
		case vk::ImageLayout::transferDstOptimal:
			barrier.dstAccessMask = vk::AccessBits::transferWrite; break;
			break;
		case vk::ImageLayout::transferSrcOptimal:
			barrier.srcAccessMask |= vk::AccessBits::transferRead;
			barrier.dstAccessMask = vk::AccessBits::transferRead;
			break;
		case vk::ImageLayout::colorAttachmentOptimal:
			barrier.srcAccessMask = vk::AccessBits::transferRead;
			barrier.dstAccessMask = vk::AccessBits::colorAttachmentWrite;
			break;
		case vk::ImageLayout::depthStencilAttachmentOptimal:
			barrier.dstAccessMask |= vk::AccessBits::depthStencilAttachmentWrite;
			break;
		case vk::ImageLayout::shaderReadOnlyOptimal:
			if(!barrier.srcAccessMask)
				barrier.srcAccessMask = vk::AccessBits::hostWrite | vk::AccessBits::transferWrite;
			barrier.dstAccessMask = vk::AccessBits::shaderRead;
			break;
		default:
			break;
	}

	const auto stage = vk::PipelineStageBits::topOfPipe;
	vk::cmdPipelineBarrier(cmdBuffer, stage, stage, {}, {}, {}, {barrier});
}

WorkPtr changeLayout(const Device& dev, vk::Image img, vk::ImageLayout ol, vk::ImageLayout nl,
	const vk::ImageSubresourceRange& range)
{
	const Queue* queue;
	auto qFam = transferQueueFamily(dev, &queue);

	auto cmdBuffer = dev.commandProvider().get(qFam);
	vk::beginCommandBuffer(cmdBuffer, {});
	changeLayoutCommand(cmdBuffer, img, ol, nl, range);
	vk::endCommandBuffer(cmdBuffer);

	return std::make_unique<CommandWork<void>>(std::move(cmdBuffer), *queue);
}

// static infos
// TODO: better defaults!
ViewableImage::CreateInfo ViewableImage::defaultColor2D()
{
	return {
		{
			{},
			vk::ImageType::e2d,
			vk::Format::b8g8r8a8Unorm,
			{},
			1, 1,
			vk::SampleCountBits::e1,
			vk::ImageTiling::optimal,
			vk::ImageUsageBits::colorAttachment | vk::ImageUsageBits::inputAttachment,
			vk::SharingMode::exclusive,
			0, nullptr, vk::ImageLayout::undefined
		},
		{
			{}, {},
			vk::ImageViewType::e2d,
			vk::Format::b8g8r8a8Unorm,
			{},
			{
				vk::ImageAspectBits::color,
				0, 1, 0, 1
			}
		}
	};
}

ViewableImage::CreateInfo ViewableImage::defaultDepth2D()
{
	return {
		{
			{},
			vk::ImageType::e2d,
			vk::Format::d32Sfloat,
			{},
			1, 1,
			vk::SampleCountBits::e1,
			vk::ImageTiling::optimal,
			vk::ImageUsageBits::depthStencilAttachment,
			vk::SharingMode::exclusive,
			0, nullptr, vk::ImageLayout::undefined
		},
		{
			{}, {},
			vk::ImageViewType::e2d,
			vk::Format::d32Sfloat,
			{},
			{
				vk::ImageAspectBits::depth,
				0, 1, 0, 1
			}
		}
	};
}

ViewableImage::CreateInfo ViewableImage::defaultDepthStencil2D()
{
	return {
		{
			{},
			vk::ImageType::e2d,
			vk::Format::d16UnormS8Uint,
			{},
			1, 1,
			vk::SampleCountBits::e1,
			vk::ImageTiling::optimal,
			vk::ImageUsageBits::depthStencilAttachment,
			vk::SharingMode::exclusive,
			0, nullptr, vk::ImageLayout::undefined
		},
		{
			{}, {},
			vk::ImageViewType::e2d,
			vk::Format::d16UnormS8Uint,
			{},
			{
				vk::ImageAspectBits::depth | vk::ImageAspectBits::stencil,
				0, 1, 0, 1
			}
		}
	};
}

//attachment
ViewableImage::ViewableImage(const Device& dev, const CreateInfo& info)
{
	create(dev, info.imgInfo, info.memoryTypeBits);
	init(info.viewInfo);
}

ViewableImage::~ViewableImage()
{
	if(vkImageView()) vk::destroyImageView(vkDevice(), vkImageView(), nullptr);
}

void ViewableImage::swap(ViewableImage& lhs) noexcept
{
	using std::swap;
	swap(resourceBase(), lhs.resourceBase());
	swap(image_, lhs.image_);
	swap(imageView_, lhs.imageView_);
}

void ViewableImage::create(const Device& dev, const vk::ImageCreateInfo& info,
	unsigned int memoryTypeBits)
{
	image_ = Image(dev, info, memoryTypeBits);
}

void ViewableImage::init(const vk::ImageViewCreateInfo& info)
{
	auto cpy = info;

	image_.assureMemory();
	cpy.image = vkImage();
	imageView_ = vk::createImageView(vkDevice(), cpy);
}

// Sampler
Sampler::Sampler(const Device& dev, const vk::SamplerCreateInfo& info) : ResourceHandle(dev)
{
	handle_ = vk::createSampler(dev, info);
}

Sampler::Sampler(const Device& dev, vk::Sampler sampler) : ResourceHandle(dev, sampler)
{
}

Sampler::~Sampler()
{
	if(vkHandle()) vk::destroySampler(device(), vkHandle());
}

// ImageView
ImageView::ImageView(const Device& dev, const vk::ImageViewCreateInfo& info) : ResourceHandle(dev)
{
	handle_ = vk::createImageView(dev, info);
}

ImageView::ImageView(const Device& dev, vk::ImageView view) : ResourceHandle(dev, view)
{
}

ImageView::~ImageView()
{
	if(vkHandle()) vk::destroyImageView(device(), vkHandle());
}

// Utility functions
unsigned int formatSizeBits(vk::Format format)
{
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

unsigned int formatSize(vk::Format format)
{
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
