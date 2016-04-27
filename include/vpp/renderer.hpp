#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/renderPass.hpp>

#include <memory>
#include <vector>

namespace vpp
{

///Abstract Interface for building renderers.
class RendererBuilder
{
public:
	virtual void build(const RenderPassInstance& renderini) const = 0;
	virtual std::vector<vk::ClearValue> clearValues() const = 0;

	virtual void beforeRender(vk::CommandBuffer) const {};
	virtual void afterRender(vk::CommandBuffer) const {};
};

///Capable of rendering on a SwapChain.
//TODO: possibility for external (imageView) attachments
class SwapChainRenderer : public Resource
{
public:
	struct CreateInfo
	{
		const RenderPass* renderPass = nullptr;
		Device::Queue queue {};
		std::vector<ViewableImage::CreateInfo> staticAttachments;
		std::vector<ViewableImage::CreateInfo> dynamicAttachments;
	};

	struct RenderBuffer
	{
		Framebuffer framebuffer;
		vk::CommandBuffer commandBuffer;
	};

public:
	SwapChainRenderer() = default;
	SwapChainRenderer(const SwapChain& swapChain, const RendererBuilder& builder,
		const CreateInfo& info);
	~SwapChainRenderer();

	SwapChainRenderer(SwapChainRenderer&& other) noexcept;
	SwapChainRenderer& operator=(SwapChainRenderer&& other) noexcept;

	void initMemoryLess(const SwapChain& swapChain, const CreateInfo& info);
	void initMemoryResources(const RendererBuilder& builder);

	///Renders one frame and presents the swapChain buffer.
	void render();

	const RenderPass& renderPass() const { return *info_.renderPass; }
	const SwapChain& swapChain() const { return *swapChain_; }
	const CreateInfo& info() const { return info_; }
	const std::vector<RenderBuffer>& renderBuffers() const { return renderBuffers_; }
	const std::vector<ViewableImage>& staticAttachments() const { return staticAttachments_; }

	vk::CommandPool vkCommandPool() const { return commandPool_; }
	vk::RenderPass vkRenderPass() const { return renderPass().vkRenderPass(); }
	vk::Queue vkQueue()	const { return info().queue.queue; }

	void destroy();
	friend void swap(SwapChainRenderer& a, SwapChainRenderer& b) noexcept;

protected:
	void destroyRenderBuffers();
	void buildCommandBuffers(const RendererBuilder& builder);

protected:
	const SwapChain* swapChain_ = nullptr;

	CreateInfo info_ {};
	std::vector<RenderBuffer> renderBuffers_;
	std::vector<ViewableImage> staticAttachments_;
	vk::CommandPool commandPool_ {};
};

}
