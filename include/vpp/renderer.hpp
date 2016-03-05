#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

#include <memory>
#include <vector>

namespace vpp
{

/*
//higher abstraction needed!
class FrameBuffer : public Resource
{
public:
	enum class AttachmentType
	{
		depth,
		color,
		input,
		resolve,
		preserve
	};

	struct Attachment
	{
		std::unique_ptr<Image> image {}; //optional, may be provides externally (swapchain e.g.)
		vk::ImageView imageView {};
		vk::Format format;
	};

	struct AttachmentInfo
	{
		AttachmentType type;
		vk::Image image_ {}; //external image to use, if {} a new one will be created
	};

	struct CreateInfo
	{
		std::vector<AttachmentInfo> attachements;
	};

protected:
	nytl::Vec2f size_;
	std::vector<Attachement> attachments_;
	vk::Framebuffer frameBuffer_;

public:
	FrameBuffer(const Device& dev, vk::RenderPass renderpass, const CreateInfo& info);
	FrameBuffer(const RenderPass& renderpass, const CreateInfo& info);
	FrameBuffer(const RenderPass& renderpass);

	const std::vector<Attachment>& attachments() const { return attachments_; }
	const nytl::Vec2f size() const { return size_; }
	vk::Framebuffer vkFrameBuffer() const { return frameBuffer_; }
};

class RenderPass : public Resource
{
public:
	struct CreateInfo
	{
		std::vector<FrameBuffer::AttachmentInfo> attachments;
	};

public:
	RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info);
};

class RenderInstance : public Resource
{
protected:
	vk::RenderPass renderPass_;
	vk::CommandBuffer commandBuffer_;
	const Framebuffer& framebuffer_;
};

class FrameBufferRenderer : public Resource
{
protected:
	FrameBuffer frameBuffer_;
	vk::CommandBuffer commandBuffer_;

public:
	RenderInstance render();
};
*/

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

	struct DepthStencil
	{
		std::unique_ptr<Image> image {nullptr};
		vk::ImageView imageView {};
		vk::Format format;
	};

protected:
	const SwapChain* swapChain_;
	DepthStencil depthStencil_;

	std::vector<FrameRenderer> frameRenderers_;
	vk::RenderPass renderPass_;
	vk::CommandPool commandPool_;

protected:
	Renderer() = default;

	void create(const SwapChain& swapChain);

	void initCommandPool();
	void initRenderPass();
	void initDepthStencil();
	void initRenderers();

	void destroy();
	void destroyRenderers();
	void destroyRenderPass();
	void destroyCommandPool();
	void destroyDepthStencil();

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
