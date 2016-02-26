#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <memory>
#include <vector>

namespace vpp
{

///Capable of rendering on a SwapChain.
class Renderer : public Resource
{
protected:
	struct FrameRenderer
	{
		unsigned int id;
		vk::Framebuffer frameBuffer;
		vk::CommandBuffer commandBuffer;
	};

protected:
	const SwapChain* swapChain_;

	std::vector<FrameRenderer> frameRenderers_;
	vk::RenderPass renderPass_;
	vk::CommandPool commandPool_;

protected:
	Renderer() = default;

	void create(const SwapChain& swapChain);

	void initCommandPool();
	void initRenderPass();
	void initRenderers();

	void destroy();
	void destroyRenderers();
	void destroyRenderPass();
	void destroyCommandPool();

	virtual void buildCommandBuffer(const FrameRenderer& renderer) const;
	virtual void buildRenderer(vk::CommandBuffer buffer) const;

public:
	Renderer(const SwapChain& swapChain);
	virtual ~Renderer();

	void render(vk::Queue queue);
	void reset(const SwapChain& swapChain, bool complete = 0);

	vk::CommandPool vkCommandPool() const { return commandPool_; }
	vk::RenderPass vkRenderPass() const { return renderPass_; }

	const SwapChain& swapChain() const { return *swapChain_; }
	const std::vector<FrameRenderer>& frameRenderers() const { return frameRenderers_; }
};

}
