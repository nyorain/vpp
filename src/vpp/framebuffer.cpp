// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/framebuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/span.hpp>

namespace vpp {

// Framebuffer
Framebuffer::Framebuffer(const Device& dev, vk::RenderPass rp, const vk::Extent2D& size,
	const AttachmentsInfo& attachments, const ExtAttachments& ext)
{
	create(dev, size, attachments);
	init(rp, attachments, ext);
}

Framebuffer::Framebuffer(const Device& dev, const vk::Extent2D& size, vk::Framebuffer framebuffer)
	: ResourceHandle(dev, framebuffer), width_(size.width), height_(size.height)
{

}

Framebuffer::~Framebuffer()
{
	if(vkHandle()) vk::destroyFramebuffer(vkDevice(), vkHandle(), nullptr);
}

void Framebuffer::swap(Framebuffer& lhs) noexcept
{
	using std::swap;
	swap(resourceBase(), lhs.resourceBase());
	swap(attachments_, lhs.attachments_);
	swap(width_, lhs.width_);
	swap(height_, lhs.height_);
}

void Framebuffer::create(const Device& dev, const vk::Extent2D& size,
	const AttachmentsInfo& attachments)
{
	Resource::init(dev);
	width_ = size.width;
	height_ = size.height;

	attachments_.reserve(attachments.size());
	for(auto& attinfo : attachments) {
		auto imgInfo = attinfo.imgInfo;
		imgInfo.extent = {size.width, size.height, 1};

		attachments_.emplace_back();
		attachments_.back().create(device(), imgInfo, attinfo.memoryTypeBits);
	}
}

void Framebuffer::init(vk::RenderPass rp, const AttachmentsInfo& attachments,
	const ExtAttachments& extAttachments)
{
	std::vector<vk::ImageViewCreateInfo> info;
	info.reserve(attachments.size());
	for(auto& at : attachments) info.push_back(at.viewInfo);

	init(rp, info, extAttachments);
}

void Framebuffer::init(vk::RenderPass rp, const std::vector<vk::ImageViewCreateInfo>& viewInfo,
	const ExtAttachments& extAttachments)
{
	if(viewInfo.size() < attachments_.size())
		throw std::logic_error("vpp::Framebuffer::init: to few viewInfos");

	for(std::size_t i(0); i < attachments_.size(); ++i) attachments_[i].init(viewInfo[i]);

	// framebuffer
	// parse attachments
	std::vector<vk::ImageView> attachments;
	attachments.resize(attachments_.size() + extAttachments.size());

	for(auto& extView : extAttachments) {
		if(extView.first > attachments.size())
			throw std::logic_error("vpp::Framebuffer: invalid external Attachment id given");

		attachments[extView.first] = extView.second;
	}

	std::size_t currentID = 0;
	for(auto& buf : attachments_) {
		while(attachments[currentID])
			++currentID;
		attachments[currentID] = buf.vkImageView();
		++currentID;
	}

	// createinfo
	vk::FramebufferCreateInfo createInfo;
	createInfo.renderPass = rp;
	createInfo.attachmentCount = attachments.size();
	createInfo.pAttachments = attachments.data();
	createInfo.width = width_;
	createInfo.height = height_;
	createInfo.layers = 1; // TODO: should this be paramterized somehow?

	handle_ = vk::createFramebuffer(vkDevice(), createInfo);
}

vk::Extent2D Framebuffer::size() const
{
	return {width_, height_};
}

//static utility
std::vector<ViewableImage::CreateInfo>
Framebuffer::parseRenderPass(const RenderPass& rp, const vk::Extent2D& size)
{
	std::vector<ViewableImage::CreateInfo> ret;
	ret.reserve(rp.attachments().size());

	for(std::size_t i(0); i < rp.attachments().size(); ++i) {
		ViewableImage::CreateInfo fbaInfo;
		fbaInfo.imgInfo.format = rp.attachments()[i].format;
		fbaInfo.imgInfo.extent = {size.width, size.height, 1};
		fbaInfo.imgInfo.format = rp.attachments()[i].format;

		vk::ImageUsageFlags usage {};
		vk::ImageAspectFlags aspect {};

		for(auto& sub : rp.subpasses()) {
			if(sub.pDepthStencilAttachment && sub.pDepthStencilAttachment->attachment == i) {
				usage |= vk::ImageUsageBits::depthStencilAttachment;
				aspect |= vk::ImageAspectBits::depth | vk::ImageAspectBits::stencil;
			}

			using SpanAR = nytl::Span<const vk::AttachmentReference>;
			for(auto& ref : SpanAR(*sub.pInputAttachments, sub.inputAttachmentCount)) {
				if(ref.attachment == i) {
					usage |= vk::ImageUsageBits::inputAttachment;
					aspect |= vk::ImageAspectBits::depth;
				}
			}

			for(auto& ref : SpanAR(*sub.pColorAttachments, sub.colorAttachmentCount)) {
				if(ref.attachment == i) {
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

} // namespace vpp
