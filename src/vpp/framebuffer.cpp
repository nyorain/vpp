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

void swap(Framebuffer& a, Framebuffer& b) noexcept
{
	using std::swap;
	using RH = ResourceHandle<vk::Framebuffer>;

	swap(static_cast<RH&>(a), static_cast<RH&>(b));
	swap(a.attachments_, b.attachments_);
	swap(a.width_, b.width_);
	swap(a.height_, b.height_);
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

} // namespace vpp
