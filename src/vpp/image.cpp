#include <vpp/image.hpp>
#include <vpp/defs.hpp>
#include <vpp/provider.hpp>
#include <vpp/transfer.hpp>
#include <utility>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	create(dev, info, mflags);
}

Image::Image(Image&& other) noexcept
{
	swap(*this, other);
}

Image& Image::operator=(Image&& other) noexcept
{
	this->~Image();
	swap(*this, other);
	return *this;
}

Image::~Image()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage());

	memoryEntry_ = {};
	image_ = {};
}

void swap(Image& a, Image& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.memoryEntry_, b.memoryEntry_);
}

void Image::create(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags flags)
{
	image_ = vk::createImage(dev.vkDevice(), info);
	auto reqs = vk::getImageMemoryRequirements(dev.vkDevice(), image_);

	reqs.memoryTypeBits = dev.memoryTypeBits(flags, reqs.memoryTypeBits);
	dev.memoryAllocator().request(image_, reqs, info.tiling, memoryEntry_);
}

WorkPtr Image::fill(const std::uint8_t& data, std::size_t size,
	vk::Format format, const vk::Extent3D& extent) const
{
	// assureMemory();
	//
	// if(mappable())
	// {
	// 	auto map = memoryEntry().map();
	// 	std::memcpy(map.ptr(), &data, size);
	// 	if(!map.coherent()) map.flushRanges();
	// 	return std::make_unique<FinishedWork<void>>();
	// }
	// else
	// {
	// 	auto cmdBuffer = device().commandProvider().get(0);
	// 	auto vkcmdb = cmdBuffer.vkCommandBuffer();
	// 	auto uploadBuffer = device().transferManager().buffer(size);
	// 	uploadBuffer.buffer().fill(data);
	//
	// 	vk::BufferImageCopy region;
	//
	// 	if(layout != vk::ImageLayout::TransferDstOptimal && layout != vk::ImageLayout::General)
	// 	{
	// 		changeLayout(vkcmdb, layout, vk::ImageLayout::TransferDstOptimal);
	// 	}
	//
	// 	vk::cmdCopyBufferToImage(vkcmb, vkbuf, vkImage(), layout, 1, &region);
	//
	// 	//custom work implementation
	// 	struct WorkImpl : public CommandWork<void>
	// 	{
	// 		TransferRange uploadRange_;
	//
	// 		WorkImpl(CommandBuffer&& buffer, TransferRange&& range)
	// 			: CommandWork(std::move(buffer)), uploadRange_(std::move(range)) {}
	//
	// 		virtual void finish() override { CommandWork::finish(); uploadRange_ = {}; }
	// 	};
	//
	// 	return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(uploadBuffer));
	// }
}

//free utility functions
WorkPtr changeLayout(const Device& dev, vk::Image img, vk::ImageLayout ol, vk::ImageLayout nl,
	vk::ImageAspectFlags aspect)
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

	auto cmdBuffer = dev.commandProvider().get(0);
	vk::beginCommandBuffer(cmdBuffer, {});

	const auto stage = vk::PipelineStageBits::topOfPipe;
	vk::cmdPipelineBarrier(cmdBuffer, stage, stage, {}, {}, {}, {barrier});
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
	image_ = {};
}

ViewableImage::ViewableImage(ViewableImage&& other) noexcept
{
	swap(*this, other);
}

ViewableImage& ViewableImage::operator=(ViewableImage&& other) noexcept
{
	this->~ViewableImage();
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
