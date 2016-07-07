#include <vpp/image.hpp>
#include <vpp/defs.hpp>
#include <vpp/provider.hpp>
#include <vpp/transfer.hpp>
#include <vpp/transferWork.hpp>
#include <utility>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	image_ = vk::createImage(dev.vkDevice(), info);
	auto reqs = vk::getImageMemoryRequirements(dev.vkDevice(), image_);

	reqs.memoryTypeBits = dev.memoryTypeBits(mflags, reqs.memoryTypeBits);
	dev.deviceAllocator().request(image_, reqs, info.tiling, memoryEntry_);
}

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, std::uint32_t memoryTypeBits)
{
	image_ = vk::createImage(dev.vkDevice(), info);
	auto reqs = vk::getImageMemoryRequirements(dev.vkDevice(), image_);

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(image_, reqs, info.tiling, memoryEntry_);
}

Image::Image(Image&& other) noexcept
{
	swap(*this, other);
}

Image& Image::operator=(Image other) noexcept
{
	swap(*this, other);
	return *this;
}

Image::~Image()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage());
}

void swap(Image& a, Image& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.memoryEntry_, b.memoryEntry_);
}

WorkPtr fill(const Image& image, const std::uint8_t& data, vk::ImageLayout layout,
	const vk::Extent3D& extent, const vk::ImageSubresourceLayers& subres, vk::DeviceSize size,
	vk::DeviceSize offset, bool allowMap)
{
	image.assureMemory();
	if(size = vk::wholeSize) size = image.size() - offset;

	if(image.mappable() && allowMap)
	{
		auto map = image.memoryEntry().map();
		std::memcpy(map.ptr(), &data, size);
		if(!map.coherent()) map.flush();
		return std::make_unique<FinishedWork<void>>();
	}
	else
	{
		auto cmdBuffer = image.device().commandProvider().get(0);
		auto uploadBuffer = image.device().transferManager().buffer(size);
		fill140(uploadBuffer.buffer(), raw(data, size));

		vk::BufferImageCopy region;
		region.imageExtent = extent;
		region.imageSubresource = subres;

		vk::beginCommandBuffer(cmdBuffer, {});

		//change layout if needed
		if(layout != vk::ImageLayout::transferDstOptimal && layout != vk::ImageLayout::general)
			changeLayoutCommand(cmdBuffer, image, layout, vk::ImageLayout::transferDstOptimal,
				subres.aspectMask);

		vk::cmdCopyBufferToImage(cmdBuffer, uploadBuffer.buffer(), image, layout, {region});
		vk::endCommandBuffer(cmdBuffer);

		return std::make_unique<UploadWork>(std::move(cmdBuffer), std::move(uploadBuffer));
	}
}

DataWorkPtr retrieve(const Image& image, vk::ImageLayout layout, const vk::Extent3D& extent,
	const vk::ImageSubresourceLayers& subres, bool allowMap)
{
#ifndef NDEBUG
	if(!image.memoryEntry().allocated())
	{
		std::cerr << "vpp::retrieve(image): image has no memory. Calling assureMemory()\n";
		image.assureMemory();
	}
#endif //NDEBUG

	if(image.mappable() && allowMap)
	{
		return std::make_unique<MappableDownloadWork>(image);
	}
	else
	{
		auto cmdBuffer = image.device().commandProvider().get(0);
		auto downloadBuffer = image.device().transferManager().buffer(image.size());

		vk::beginCommandBuffer(cmdBuffer, {});

		//change layout if needed
		if(layout != vk::ImageLayout::transferSrcOptimal && layout != vk::ImageLayout::general)
			changeLayoutCommand(cmdBuffer, image, layout, vk::ImageLayout::transferSrcOptimal,
				subres.aspectMask);

		vk::endCommandBuffer(cmdBuffer);
		return std::make_unique<DownloadWork>(std::move(cmdBuffer), std::move(downloadBuffer));
	}
}

//free utility functions
void changeLayoutCommand(vk::CommandBuffer cmdBuffer, vk::Image img, vk::ImageLayout ol,
	vk::ImageLayout nl, vk::ImageAspectFlags aspect)
{
	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = ol;
	barrier.newLayout = nl;
	barrier.image = img;
	barrier.subresourceRange = {aspect, 0, 1, 0, 1};

	switch(ol)
	{
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
	}

	switch(nl)
	{
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
	}

	const auto stage = vk::PipelineStageBits::topOfPipe;
	vk::cmdPipelineBarrier(cmdBuffer, stage, stage, {}, {}, {}, {barrier});
}

WorkPtr changeLayout(const Device& dev, vk::Image img, vk::ImageLayout ol, vk::ImageLayout nl,
	vk::ImageAspectFlags aspect)
{

	auto cmdBuffer = dev.commandProvider().get(0);
	vk::beginCommandBuffer(cmdBuffer, {});
	changeLayoutCommand(cmdBuffer, img, ol, nl, aspect);
	vk::endCommandBuffer(cmdBuffer);

	return std::make_unique<CommandWork<void>>(std::move(cmdBuffer));
}

//static infos
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
			vk::Format::d16UnormS8Uint,
			{},
			1, 1,
			vk::SampleCountBits::e1,
			vk::ImageTiling::optimal,
			vk::ImageUsageBits::depthStencilAttachment | vk::ImageUsageBits::sampled,
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
	create(dev, info.imgInfo, info.memoryFlags);
	init(info.viewInfo);
}

ViewableImage::~ViewableImage()
{
	if(vkImageView()) vk::destroyImageView(vkDevice(), vkImageView(), nullptr);
}

ViewableImage::ViewableImage(ViewableImage&& other) noexcept
{
	swap(*this, other);
}

ViewableImage& ViewableImage::operator=(ViewableImage other) noexcept
{
	swap(*this, other);
	return *this;
}

void swap(ViewableImage& a, ViewableImage& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.imageView_, b.imageView_);
}

void ViewableImage::create(const Device& dev, const vk::ImageCreateInfo& info,
	vk::MemoryPropertyFlags flags)
{
	image_ = Image(dev, info, flags);
}

void ViewableImage::init(const vk::ImageViewCreateInfo& info)
{
	auto cpy = info;

	image_.assureMemory();
	cpy.image = vkImage();
	imageView_ = vk::createImageView(vkDevice(), cpy);
}

}
