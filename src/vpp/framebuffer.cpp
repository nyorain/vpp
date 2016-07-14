#include <vpp/framebuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

//Framebuffer
Framebuffer::Framebuffer(const Device& dev, vk::RenderPass rp, const vk::Extent2D& size,
	const AttachmentsInfo& attachments, const ExtAttachments& ext)
{
	create(dev, size, attachments);
	init(rp, attachments, ext);
}

Framebuffer::~Framebuffer()
{
	if(vkHandle()) vk::destroyFramebuffer(vkDevice(), vkHandle(), nullptr);
}

void Framebuffer::create(const Device& dev, const vk::Extent2D& size,
	const AttachmentsInfo& attachments)
{
	std::vector<vk::ImageCreateInfo> info;
	info.resize(attachments.size());
	for(auto& at : attachments) info.push_back(at.imgInfo);

	create(dev, size, info);
}

void Framebuffer::create(const Device& dev, const vk::Extent2D& size,
	const std::vector<vk::ImageCreateInfo>& imgInfo)
{
	Resource::init(dev);
	width_ = size.width;
	height_ = size.height;

	attachments_.reserve(imgInfo.size());
	for(auto& attinfo : imgInfo)
	{
		auto imageInfo = attinfo;
		imageInfo.extent = {size.width, size.height, 1};

		attachments_.emplace_back();
		attachments_.back().create(device(), imageInfo);
	}
}

void Framebuffer::init(vk::RenderPass rp, const AttachmentsInfo& attachments,
	const ExtAttachments& extAttachments)
{
	std::vector<vk::ImageViewCreateInfo> info;
	info.resize(attachments.size());
	for(auto& at : attachments) info.push_back(at.viewInfo);

	init(rp, info, extAttachments);
}

void Framebuffer::init(vk::RenderPass rp, const std::vector<vk::ImageViewCreateInfo>& viewInfo,
	const ExtAttachments& extAttachments)
{
	if(viewInfo.size() < attachments_.size())
		throw std::logic_error("vpp::Framebuffer::init: to few viewInfos");

	for(std::size_t i(0); i < attachments_.size(); ++i) attachments_[i].init(viewInfo[i]);

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
	createInfo.renderPass = rp;
	createInfo.attachmentCount = attachments.size();
	createInfo.pAttachments = attachments.data();
	createInfo.width = width_;
	createInfo.height = height_;
	createInfo.layers = 1; ///XXX: should be paramterized?

	vkHandle() = vk::createFramebuffer(vkDevice(), createInfo);
}

vk::Extent2D Framebuffer::size() const
{
	return {width_, height_};
}

//static utility
Framebuffer::AttachmentsInfo Framebuffer::parseRenderPass(const RenderPass& rp, const vk::Extent2D& s)
{
	AttachmentsInfo ret;
	ret.reserve(rp.attachments().size());

	for(std::size_t i(0); i < rp.attachments().size(); ++i)
	{
		ViewableImage::CreateInfo fbaInfo;
		fbaInfo.imgInfo.format = rp.attachments()[i].format;
		fbaInfo.imgInfo.extent = {s.width, s.height, 1};
		fbaInfo.imgInfo.format = rp.attachments()[i].format;

		vk::ImageUsageFlags usage {};
		vk::ImageAspectFlags aspect {};

		for(auto& sub : rp.subpasses())
		{
			if(sub.pDepthStencilAttachment && sub.pDepthStencilAttachment->attachment == i)
			{
				usage |= vk::ImageUsageBits::depthStencilAttachment;
				aspect |= vk::ImageAspectBits::depth | vk::ImageAspectBits::stencil;
			}

			for(auto& ref : makeRange(sub.pInputAttachments, sub.inputAttachmentCount))
			{
				if(ref.attachment == i)
				{
					usage |= vk::ImageUsageBits::inputAttachment;
					aspect |= vk::ImageAspectBits::depth;
				}
			}

			for(auto& ref : makeRange(sub.pColorAttachments, sub.colorAttachmentCount))
			{
				if(ref.attachment == i)
				{
					usage |= vk::ImageUsageBits::colorAttachment;
					aspect |= vk::ImageAspectBits::color;
				}
			}
		}

		fbaInfo.imgInfo.usage = usage;
		fbaInfo.viewInfo.vkHandle().subresourceRange.aspectMask = aspect;

		ret.push_back(fbaInfo);
	}

	return ret;
}

}
