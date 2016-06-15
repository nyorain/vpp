#include <vpp/framebuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

//Framebuffer
Framebuffer::Framebuffer(const Device& dev, const CreateInfo& info, const ExtAttachments& ext)
{
	create(dev, info);
	init(info.renderPass, ext);
}

Framebuffer::~Framebuffer()
{
	if(vkFramebuffer()) vk::destroyFramebuffer(vkDevice(), vkFramebuffer(), nullptr);
	attachments_.clear();
	framebuffer_ = {};
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
	swap(*this, other);
}
Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
	this->~Framebuffer();
	swap(*this, other);
	return *this;
}

void swap(Framebuffer& a, Framebuffer& b) noexcept
{
	using std::swap;

	swap(a.framebuffer_, b.framebuffer_);
	swap(a.attachments_, b.attachments_);
	swap(a.device_, b.device_);
}

void Framebuffer::create(const Device& dev, const CreateInfo& info)
{
	create(dev, info.size, info.attachments);
}

void Framebuffer::create(const Device& dev, const vk::Extent2D& size, const AttachmentsInfo& info)
{
	Resource::init(dev);
	attachmentsInfo_ = info;
	size_ = size;

	attachments_.reserve(info.size());
	for(auto& attinfo : info) {
		auto imageInfo = attinfo.imageInfo;
		imageInfo.extent = {size.width, size.height, 1};

		attachments_.emplace_back();
		attachments_.back().create(device(), imageInfo);
	}
}

void Framebuffer::init(vk::RenderPass rp, const ExtAttachments& extAttachments)
{
	for(std::size_t i(0); i < attachments_.size(); ++i)
		attachments_[i].init(attachmentsInfo_[i].viewInfo);

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
	createInfo.width = size_.width;
	createInfo.height = size_.height;
	createInfo.layers = 1; ///XXX: should be paramterized?

	framebuffer_ = vk::createFramebuffer(vkDevice(), createInfo);
}

//static utility
std::vector<ViewableImage::CreateInfo> Framebuffer::parseRenderPass(const RenderPass& rp)
{
	std::vector<ViewableImage::CreateInfo> ret;
	ret.reserve(rp.attachments().size());

	for(std::size_t i(0); i < rp.attachments().size(); ++i)
	{
		ViewableImage::CreateInfo fbaInfo;
		fbaInfo.imageInfo.format = rp.attachments()[i].format;
		fbaInfo.viewInfo.format = rp.attachments()[i].format;

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

		fbaInfo.imageInfo.usage = usage;
		fbaInfo.viewInfo.vkHandle().subresourceRange.aspectMask = aspect;

		ret.push_back(fbaInfo);
	}

	return ret;
}


}
