#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

#include <memory>
#include <vector>

namespace vpp
{

///Vulkan Renderpass. Can be created just from a vulkan device.
///Stores its description information.
class RenderPass : public Resource
{
protected:
	vk::RenderPass renderPass_ {};

	std::vector<vk::AttachmentDescription> attachments_;
	std::vector<vk::SubpassDescription> subpasses_;
	std::vector<vk::SubpassDependency> dependencies_;

protected:
	void init(const Device& dev, const vk::RenderPassCreateInfo& info);
	void initInfos(const vk::RenderPassCreateInfo& info);

	void destroy();

public:
	RenderPass() = default;
	RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info);
	RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info);
	~RenderPass();

	RenderPass(RenderPass&& other) noexcept;
	RenderPass& operator=(RenderPass&& other) noexcept;

	void swap(RenderPass& other) noexcept;

	vk::RenderPass vkRenderPass() const { return renderPass_; }

	const std::vector<vk::AttachmentDescription>& attachments() const { return attachments_; }
	const std::vector<vk::SubpassDependency>& dependencies() const { return dependencies_; }
	const std::vector<vk::SubpassDescription>& subpasses() const { return subpasses_; }
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
	RenderPassInstance(vk::CommandBuffer cmdbuffer, vk::RenderPass pass, vk::Framebuffer framebuffer);
	~RenderPassInstance();

	const RenderPass& renderPass() const { return renderPass_; }
	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_; }
	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }

	unsigned int currentSubpass() const { return currentSubpass_; }
	void nextSubpass();
};

///Abstract Interface for building renderers.
class RendererBuilder
{
public:
	virtual void build(const RenderInstance& renderini) const = 0;
	virtual std::vector<vk::ClearValue> clearValues() const = 0;
};

///Capable of rendering on a SwapChain.
class SwapChainRenderer : public Resource
{
protected:
	struct FrameRenderer
	{
		Framebuffer framebuffer;
		vk::CommandBuffer commandBuffer;
	};

	struct CreateInfo
	{
		vk::Renderpass renderPass;
		std::vector<Framebuffer::AttachmentInfo> dynamicAttachments;
		std::vector<Framebuffer::AttachmentInfo> staticAttachments;
		std::vector<vk::ImageView> externalAttachments;
	};

	struct RenderPassCreateInfo
	{
		const RenderPass& renderPass; //MUST be specified!
		std::vector<vk::ImageView> externalAttachments;
	};

protected:
	const SwapChain* swapChain_;
	RendererBuilder* builder_;

	DepthStencil depthStencil_;

	std::vector<FrameRenderer> frameRenderers_;
	vk::RenderPass renderPass_;
	vk::CommandPool commandPool_;

protected:
	void init(const SwapChain& swapChain, RendererBuilder& builder);
	void initCommandPool();
	void initRenderPass();
	void initDepthStencil();
	void initRenderers();

	void destroy();
	void destroyRenderers();
	void destroyRenderPass();
	void destroyCommandPool();
	void destroyDepthStencil();

	void buildCommandBuffer(const FrameRenderer& renderer) const;

public:
	SwapChainRenderer() = default;
	SwapChainRenderer(const SwapChain& swapChain, RendererBuilder& builder, const CreateInfo& info);
	SwapChainRenderer(const SwapChain& swapChain, RendererBuilder& builder,
		const RenderPass& renderPass, std::vector<vk::ImageView> externalAttachments = {});
	SwapChainRenderer(DeviceMemoryAllocator& allocator, const SwapChain& swapChain,
		RendererBuilder& builder, const CreateInfo& info);
	SwapChainRenderer(DeviceMemoryAllocator& allocator, const SwapChain& swapChain,
		RendererBuilder& builder, const RenderPassCreateInfo& info);
	~SwapChainRenderer();

	void render(vk::Queue queue);
	void reset(const SwapChain& swapChain, bool complete = 0);

	void initMemoryLess(DeviceMemoryAllocator& allocator, const SwapChain& swapChain,
		RendererBuilder& builder, const CreateInfo& info);
	void initMemoryLess(DeviceMemoryAllocator& allocator, const SwapChain& swapChain,
		RendererBuilder& builder, const RenderPassCreateInfo& info);
	void initMemoryResources();

	vk::CommandPool vkCommandPool() const { return commandPool_; }
	vk::RenderPass vkRenderPass() const { return renderPass_; }

	const SwapChain& swapChain() const { return *swapChain_; }
	const std::vector<FrameRenderer>& frameRenderers() const { return frameRenderers_; }
};

}
