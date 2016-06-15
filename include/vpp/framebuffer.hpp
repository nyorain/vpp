#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp
{

//must be resource since it might be created without internal manages attachments
///Vulkan Framebuffer.
///Can be initialized with two step initialization.
class Framebuffer : public Resource
{
public:
	using ExtAttachments = std::unordered_map<unsigned int, vk::ImageView>;
	using AttachmentsInfo = std::vector<ViewableImage::CreateInfo>;

	struct CreateInfo
	{
		vk::RenderPass renderPass {};
		vk::Extent2D size {};
		AttachmentsInfo attachments {};
	};

public:
	///Can be used to create required framebuffer attachments for a render pass.
	///The returned vecotr can be passed as the attachments vector of the CreateInfo
	///object that is given as create paramater.
	static std::vector<ViewableImage::CreateInfo> parseRenderPass(const RenderPass& renderPass);

public:
	Framebuffer() = default;
	Framebuffer(const Device& dev, const CreateInfo& info, const ExtAttachments& ext = {});
	~Framebuffer();

	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	void create(const Device& dev, const CreateInfo& info);
	void create(const Device& dev, const vk::Extent2D& size, const AttachmentsInfo& attchs);
	void init(vk::RenderPass rp, const ExtAttachments& extAttachments = {});

	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }
	friend void swap(Framebuffer& a, Framebuffer& b) noexcept;

protected:
	vk::Framebuffer framebuffer_ {};
	std::vector<ViewableImage> attachments_;
	AttachmentsInfo attachmentsInfo_;
	vk::Extent2D size_;
};

}
