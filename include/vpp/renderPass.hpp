#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <vector>

namespace vpp
{

///Vulkan Renderpass. Can be created just from a vulkan device.
///Stores its description information.
class RenderPass : public Resource
{
public:
	RenderPass() = default;
	RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info);
	RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info);
	~RenderPass();

	RenderPass(RenderPass&& other) noexcept;
	RenderPass& operator=(RenderPass&& other) noexcept;

	const std::vector<vk::AttachmentDescription>& attachments() const { return attachments_; }
	const std::vector<vk::SubpassDependency>& dependencies() const { return dependencies_; }
	const std::vector<vk::SubpassDescription>& subpasses() const { return subpasses_; }
	const std::vector<vk::AttachmentReference>& references() const { return references_; }

	vk::RenderPass vkRenderPass() const { return renderPass_; }

	friend void swap(RenderPass& a, RenderPass& b) noexcept;

protected:
	void initInfos(const vk::RenderPassCreateInfo& info);

protected:
	vk::RenderPass renderPass_ {};

	std::vector<vk::AttachmentDescription> attachments_;
	std::vector<vk::SubpassDescription> subpasses_;
	std::vector<vk::SubpassDependency> dependencies_;
	std::vector<vk::AttachmentReference> references_;
};

///Vulkan RenderPass Instance, i.e. a commandbuffer recording session during a render pass.
class RenderPassInstance : public Resource
{
protected:
	const RenderPass& renderPass_ {};
	vk::CommandBuffer commandBuffer_ {};
	vk::Framebuffer framebuffer_ {};

	unsigned int currentSubpass_ {0};

public:
	RenderPassInstance(vk::CommandBuffer cmdbuffer, const RenderPass& pass,
		vk::Framebuffer framebuffer);
	~RenderPassInstance();

	const RenderPass& renderPass() const { return renderPass_; }
	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_; }
	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }

	unsigned int currentSubpass() const { return currentSubpass_; }
	void nextSubpass();
};

}
