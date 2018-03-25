#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>
#include <vpp/framebuffer.hpp>
#include <variant>

namespace vpp {

/// Idea (was previously in vpp): Framebuffer that owns its attachments.
/// Easier to create and use.
class ManagedFramebuffer : public vpp::Framebuffer {
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

