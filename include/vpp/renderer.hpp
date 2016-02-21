#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>

namespace vpp
{

///Capable of rendering on a SwapChain.
class Renderer
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
	virtual void buildCommandBuffer(const FrameRenderer& renderer);
	virtual void buildRenderer(vk::CommandBuffer buffer);

	void initCommandPool();
	void initRenderPass();
	void initRenderers();

	void destroy();
	void destroyRenderers();
	void destroyRenderPass();
	void destroyCommandPool();

public:
	Renderer(const SwapChain& swapChain);
	~Renderer();

	void render(vk::Queue queue);
	void reset(const SwapChain& swapChain, bool complete = 0);

	vk::Instance vkInstance() const;
	vk::PhysicalDevice vkPhysicalDevice() const;
	vk::Device vkDevice() const;

	vk::CommandPool vkCommandPool() const { return commandPool_; }
	vk::RenderPass vkRenderPass() const { return renderPass_; }

	const SwapChain& swapChain() const { return *swapChain_; }
	const std::vector<FrameRenderer>& frameRenderers() const { return frameRenderers_; }
};

}
