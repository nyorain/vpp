#include <vpp/image.hpp>
#include <vpp/provider.hpp>
#include <vpp/transfer.hpp>
#include <utility>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	vk::MemoryRequirements reqs;
	vk::createImage(dev.vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(dev.vkDevice(), image_, &reqs);

	reqs.memoryTypeBits(dev.memoryTypeBits(mflags, reqs.memoryTypeBits()));
	dev.memoryAllocator().request(image_, reqs, info.tiling(), memoryEntry_);
}

Image::Image(Image&& other) noexcept
{
	swap(*this, other);
}

Image& Image::operator=(Image&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

Image::~Image()
{
	destroy();
}

void swap(Image& a, Image& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.memoryEntry_, b.memoryEntry_);
}

void Image::destroy()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage(), nullptr);

	memoryEntry_ = {};
	image_ = {};
}

MemoryMapView Image::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
}

void Image::assureMemory() const
{
	memoryEntry_.allocate();
}

void Image::changeLayoutCommand(vk::CommandBuffer buffer, vk::ImageLayout oldlayout, vk::ImageLayout
	newlayout) const
{
	// vk::ImageMemoryBarrier barrier;
	// barrier.oldLayout(oldlayout);
	// barrier.newLayout(newlayout);
	// barrier.image(vkImage());
	//
	// vkCmdPipelineMemory(cmdBuffer, {}, {}, {}, 0, nullptr, 0, nullptr, 1, &barrier);
}

std::unique_ptr<Work<void>> Image::changeLayout(vk::ImageLayout oldlayout, vk::ImageLayout newlayout) const
{
	// auto cmdBuffer = device().commandProvider().get(0);
	// changeLayoutCommand(cmdBuffer.vkCommandBuffer(), oldlayout, newlayout);
	// return std::make_unique<CommandWork<void>>(std::move(cmdBuffer));
}

std::unique_ptr<Work<void>> Image::fill(const std::uint8_t& data, std::size_t size,
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

//static convinience attachment instances
ViewableImage::CreateInfo ViewableImage::defaultDepth2D {
	{
		{},
		vk::ImageType::e2D,
		vk::Format::D16UnormS8Uint,
		{},
		1, 1,
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::Optimal,
		vk::ImageUsageFlagBits::DepthStencilAttachment | vk::ImageUsageFlagBits::Sampled,
		vk::SharingMode::Exclusive,
		0, nullptr, vk::ImageLayout::Undefined
	},
	{
		{}, {},
		vk::ImageViewType::e2D,
		vk::Format::D16UnormS8Uint,
		{},
		{
			vk::ImageAspectFlagBits::Depth | vk::ImageAspectFlagBits::Stencil,
			0, 1, 0, 1
		}
	}
};

ViewableImage::CreateInfo ViewableImage::defaultColor2D {
	{
		{},
		vk::ImageType::e2D,
		vk::Format::B8G8R8A8Unorm,
		{},
		1, 1,
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::Optimal,
		vk::ImageUsageFlagBits::ColorAttachment | vk::ImageUsageFlagBits::InputAttachment,
		vk::SharingMode::Exclusive,
		0, nullptr, vk::ImageLayout::Undefined
	},
	{
		{}, {},
		vk::ImageViewType::e2D,
		vk::Format::B8G8R8A8Unorm,
		{},
		{
			vk::ImageAspectFlagBits::Color,
			0, 1, 0, 1
		}
	}
};

//attachment
ViewableImage::ViewableImage(const Device& dev, const CreateInfo& info)
{
	initMemoryLess(dev, info.imageInfo, info.imageMemoryFlags);
	initMemoryResources(info.viewInfo);
}

ViewableImage::~ViewableImage()
{
	destroy();
}

ViewableImage::ViewableImage(ViewableImage&& other) noexcept
{
	swap(*this, other);
}

ViewableImage& ViewableImage::operator=(ViewableImage&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

void swap(ViewableImage& a, ViewableImage& b) noexcept
{
	using std::swap;

	swap(a.image_, b.image_);
	swap(a.imageView_, b.imageView_);
}

void ViewableImage::destroy()
{
	if(vkImageView()) vk::destroyImageView(vkDevice(), vkImageView(), nullptr);

	image_ = {};
}

void ViewableImage::initMemoryLess(const Device& dev, const vk::ImageCreateInfo& info,
	vk::MemoryPropertyFlags flags)
{
	image_ = Image(dev, info, flags);
}

void ViewableImage::initMemoryResources(vk::ImageViewCreateInfo info)
{
	image_.assureMemory();
	info.image(vkImage());
	vk::createImageView(vkDevice(), &info, nullptr, &imageView_);
}

}
