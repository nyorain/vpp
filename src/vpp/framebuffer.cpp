#include <vpp/framebuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

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
		vk::ImageUsageFlagBits::DepthStencilAttachment | vk::ImageUsageFlagBits::TransferSrc,
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
		vk::ImageUsageFlagBits::ColorAttachment,
		vk::SharingMode::Exclusive,
		0, nullptr, vk::ImageLayout::Undefined
	},
	{
		{}, {},
		vk::ImageViewType::e2D,
		vk::Format::D16UnormS8Uint,
		{
			vk::ComponentSwizzle::R,
			vk::ComponentSwizzle::G,
			vk::ComponentSwizzle::B,
			vk::ComponentSwizzle::A
		},
		{
			vk::ImageAspectFlagBits::Color,
			0, 1, 0, 1
		}
	}
};

//attachment
ViewableImage::ViewableImage(const Device& dev, const CreateInfo& info)
	: Resource(dev)
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
	swap(a.device_, b.device_);
}

void ViewableImage::destroy()
{
	if(vkImageView()) vk::destroyImageView(vkDevice(), vkImageView(), nullptr);

	image_ = {};
	Resource::destroy();
}

void ViewableImage::initMemoryLess(const Device& dev, const vk::ImageCreateInfo& info,
	vk::MemoryPropertyFlags flags)
{
	Resource::init(dev);
	image_ = Image(device(), info, flags);
}

void ViewableImage::initMemoryResources(vk::ImageViewCreateInfo info)
{
	image_.assureMemory();
	info.image(vkImage());
	vk::createImageView(vkDevice(), &info, nullptr, &imageView_);
}

//Framebuffer
Framebuffer::Framebuffer(const Device& dev, const CreateInfo& info)
{
	initMemoryLess(dev, info);
	initMemoryResources();
}

Framebuffer::~Framebuffer()
{
	destroy();
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
	swap(*this, other);
}
Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

void swap(Framebuffer& a, Framebuffer& b) noexcept
{
	using std::swap;

	swap(a.device_, b.device_);
	swap(a.framebuffer_, b.framebuffer_);
	swap(a.attachments_, b.attachments_);
	swap(a.info_, b.info_);
}

void Framebuffer::destroy()
{
	if(vkFramebuffer()) vk::destroyFramebuffer(vkDevice(), vkFramebuffer(), nullptr);

	attachments_.clear();
	framebuffer_ = {};
	info_ = {};

	Resource::destroy();
}

void Framebuffer::initMemoryLess(const Device& dev, const CreateInfo& info)
{
	Resource::init(dev);
	info_ = info;

	attachments_.reserve(info.attachments.size());
	for(auto& attachmentinfo : info_.attachments) {
		attachmentinfo.imageInfo.extent({size().width(), size().height(), 1});
		attachments_.emplace_back();
		attachments_.back().initMemoryLess(device(), attachmentinfo.imageInfo);
	}
}

void Framebuffer::initMemoryResources(const std::map<unsigned int, vk::ImageView>& extAttachments)
{
	for(std::size_t i(0); i < attachments_.size(); ++i)
		attachments_[i].initMemoryResources(info_.attachments[i].viewInfo);

	//framebuffer
	//attachments
	std::vector<vk::ImageView> attachments;
	attachments.resize(attachments_.size() + extAttachments.size());

	for(auto& extView : extAttachments)
	{
		if(extView.first > attachments.size())
			throw std::logic_error("bpp::Framebuffer: invalid external Attachment id given");

		attachments[extView.first] = extView.second;
	}

	std::size_t currentID = 0;
	for(auto& buf : attachments_)
	{
		while(attachments[currentID]) ++currentID;

		attachments[currentID] = buf.vkImageView();
		++currentID;
	}

	//createinfo
	vk::FramebufferCreateInfo createInfo;
	createInfo.renderPass(info_.renderPass);
	createInfo.attachmentCount(attachments.size());
	createInfo.pAttachments(attachments.data());
	createInfo.width(size().width());
	createInfo.height(size().height());
	createInfo.layers(1);

	vk::createFramebuffer(vkDevice(), &createInfo, nullptr, &framebuffer_);
}

//static utility
std::vector<ViewableImage::CreateInfo> Framebuffer::parseRenderPass(const RenderPass& rp)
{
	std::vector<ViewableImage::CreateInfo> ret;
	ret.reserve(rp.attachments().size());

	for(std::size_t i(0); i < rp.attachments().size(); ++i)
	{
		ViewableImage::CreateInfo fbaInfo;
		fbaInfo.imageInfo.format(rp.attachments()[i].format());
		fbaInfo.viewInfo.format(rp.attachments()[i].format());

		vk::ImageUsageFlags usage {};
		vk::ImageAspectFlags aspect {};

		for(auto& sub : rp.subpasses())
		{
			if(sub.pDepthStencilAttachment() && sub.pDepthStencilAttachment()->attachment() == i)
			{
				usage |= vk::ImageUsageFlagBits::DepthStencilAttachment;
				aspect |= vk::ImageAspectFlagBits::Depth | vk::ImageAspectFlagBits::Stencil;
			}

			for(auto& ref : makeRange(sub.pInputAttachments(), sub.inputAttachmentCount()))
			{
				if(ref.attachment() == i)
				{
					usage |= vk::ImageUsageFlagBits::InputAttachment;
					aspect |= vk::ImageAspectFlagBits::Depth;
				}
			}

			for(auto& ref : makeRange(sub.pColorAttachments(), sub.colorAttachmentCount()))
			{
				if(ref.attachment() == i)
				{
					usage |= vk::ImageUsageFlagBits::ColorAttachment;
					aspect |= vk::ImageAspectFlagBits::Color;
				}
			}
		}

		fbaInfo.imageInfo.usage(usage);
		fbaInfo.viewInfo.vkHandle().subresourceRange.aspectMask = aspect;

		ret.push_back(fbaInfo);
	}

	return ret;
}


}
