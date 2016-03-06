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
	vk::Extents2D size_;
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

class FrameBufferRenderer : public Resource
{
protected:
	vk::CommandBuffer commandBuffer_;

protected:
	FrameBufferRenderer() = default;

	void create(std::vector<vk::ClearValue>& clearValues = {});
	void create(std::vector<vk::ClearValue>& clearValues, const vk::Extent2D& size);

	virtual void build(const RenderInstance& renderini) = 0;
};
*/
class RendererBase
{
public:
	virtual void build(const RenderInstance& renderini) const = 0;
	virtual std::vector<RendererRequirements> requirements() const = 0;
};


class RenderInstance : public Resource
{
protected:
	vk::RenderPass renderPass_;
	vk::CommandBuffer commandBuffer_;
	vK::FrameBuffer framebuffer_;

	unsigned int currentSubpass_ {0};

public:
	RenderInstance(vk::CommandBuffer cmdbuffer, vk::RenderPass pass, vk::Framebuffer framebuffer);
	~RenderInstance();

	vk::CommandBuffer vkCommandBuffer() const { return commandBuffer_; }
	//void nextSubpass();
};

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

/*
struct RendererRequirements
{
	std::vector<AttachmentInfo> attachmentsInfos;
	std::vector<SubpassInfo> subpassInfos;
};

class SwapChainRenderer : public Resource
{
public:
	struct Buffer
	{
		unsigned int id;
		FrameBuffer frameBuffer;
		vk::CommandBuffer commandBuffer;
	};

protected:
	const Renderer* renderer_;
	const SwapChain* swapChain_;

	std::vector<FrameBufferAttachment> staticAttachments_;
	std::vector<FrameBuffer> framebuffers_;

public:
	SwapChainRenderer(const Renderer& renderer, const SwapChain& swapChain);
};

class SurfaceRenderer : public Resource
{
protected:
	const Surface* surface_;

	SwapChain swapChain_;
	SwapChainRenderer swapChainRenderer_;

public:
	SurfaceRenderer(const Renderer& renderer, const SwapChain& swapChain);
};
*/

}
