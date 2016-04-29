#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp
{
	
///Vulkan Framebuffer.
class Framebuffer : public Resource
{
public:
	struct CreateInfo
	{
		vk::RenderPass renderPass {};
		vk::Extent2D size {};
		std::vector<ViewableImage::CreateInfo> attachments;
	};

public:
	static std::vector<ViewableImage::CreateInfo> parseRenderPass(const RenderPass& renderPass);

public:
	Framebuffer() = default;
	Framebuffer(const Device& dev, const CreateInfo& info);
	~Framebuffer();

	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	void initMemoryLess(const Device& dev, const CreateInfo& info);
	void initMemoryResources(const std::map<unsigned int, vk::ImageView>& extAttachments = {});

	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }
	const vk::Extent2D& size() const { return info_.size; }
	const std::vector<ViewableImage>& attachments() const { return attachments_; }
	const CreateInfo& info() const { return info_; }

	void destroy();
	friend void swap(Framebuffer& a, Framebuffer& b) noexcept;

protected:
	vk::Framebuffer framebuffer_ {};
	std::vector<ViewableImage> attachments_;
	CreateInfo info_ {};
};

}
