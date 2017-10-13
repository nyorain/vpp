#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>
#include <variant>

namespace vpp {

class Framebuffer : public ResourceHandle<vk::Framebuffer> {
public:
	Framebuffer() = default;
	Framebuffer(const Device&, const vk::FramebufferCreateInfo&);
	~Framebuffer();

	Framebuffer(Framebuffer&& rhs) noexcept { swap(*this, rhs); }
	Framebuffer& operator=(Framebuffer rhs) noexcept { swap(*this, rhs); return *this; }
};


class ManagedFramebuffer : public Framebuffer {
public:
	using AttachmentInfo = std::variant<
		// Both create an owned viewable image
		ViewableImage::CreateInfo, 
		std::pair<vk::ImageCreateInfo, unsigned int>,
		vk::ImageView>; // static, non-owned attachment

public:
	ManagedFramebuffer() = default;
	~ManagedFramebuffer();

	ManagedFramebuffer(ManagedFramebuffer&& rhs) noexcept;
	ManagedFramebuffer& operator=(ManagedFramebuffer rhs) noexcept;

	void create(const Device&, const vk::Extent2D& size, 
		nytl::Span<AttachmentInfo> attachments);
	void init(vk::RenderPass);

	friend void swap(Framebuffer& a, Framebuffer& b) noexcept;

protected:
	std::vector<ViewableImage> attachments_;
};

} // namespace vpp

