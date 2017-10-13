// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp {

// TODO: tsi at its worst. Rework it.
// Maybe simple Framebuffer raii class and then an additional class
// that knows (and maybe owns) its attachments?
// use attachment variant like in renderer

/// Vulkan Framebuffer consisting of multiple image attachments.
/// Can be initialized with two step initialization.
/// Stores its own size and is able to create and manage its own attachments but does
/// not store any further information such as render pass compatibilty.
class Framebuffer : public ResourceHandle<vk::Framebuffer> {
public:
	using ExtAttachments = std::unordered_map<unsigned int, vk::ImageView>;
	using AttachmentsInfo = nytl::Span<const ViewableImage::CreateInfo>;

public:
	Framebuffer() = default;
	Framebuffer(const Device&, vk::RenderPass, const vk::Extent2D& size,
		const AttachmentsInfo& attachments, const ExtAttachments& externalAttachments = {});
	Framebuffer(const Device&, const vk::Extent2D& size, vk::Framebuffer);
	Framebuffer(const Device&, const vk::FramebufferCreateInfo& info);
	~Framebuffer();

	Framebuffer(Framebuffer&& rhs) noexcept { swap(*this, rhs); }
	Framebuffer& operator=(Framebuffer rhs) noexcept { swap(*this, rhs); return *this; }

	void create(const Device&, const vk::Extent2D& size, const AttachmentsInfo& info);

	/// \exception std::logic_error if there are less view infos than image infos passed to create.
	void init(vk::RenderPass rp, const AttachmentsInfo& info, const ExtAttachments& ext = {});
	void init(vk::RenderPass rp, const std::vector<vk::ImageViewCreateInfo>& info,
		const ExtAttachments& externalAttachments = {});

	const std::vector<ViewableImage>& attachments() const { return attachments_; }
	vk::Extent2D size() const;

	friend void swap(Framebuffer& a, Framebuffer& b) noexcept;

protected:
	std::vector<ViewableImage> attachments_;
	unsigned int width_ = 0;
	unsigned int height_ = 0;
};

} // namespace vpp
