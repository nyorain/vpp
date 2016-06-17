#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/renderPass.hpp>

#include <memory>
#include <vector>

namespace vpp
{

//TODO XXX: re-record command buffers? some flag on creation?
///Abstract Interface for building renderers.
///Will be passed to a SwapChainRenderer constructor to record the prebaked commandBuffers.
class RendererBuilder
{
public:
	virtual void build(const RenderPassInstance& renderini) const = 0;
	virtual std::vector<vk::ClearValue> clearValues() const = 0;

	virtual void init(const SwapChainRenderer&) {};
	virtual void beforeRender(vk::CommandBuffer) const {};
	virtual void afterRender(vk::CommandBuffer) const {};
};

///Capable of rendering on a SwapChain.
///CreateInfo::swapChainAttachment defines the id of the attachment the swap chain
///color image should be used for.
///Additional attachments can either be chosen to be static, dynamic or external.
///Static attachments mean that there is only one viewable image for all framebuffers, that is
///what one would usually use for things like e.g. depth buffers.
///For dynamic attachments, every framebuffer gets its own viewable image as attachment.
///External attachments are just imageViews provided by the user of this class.
///The given additional attachments will be attached around the swapChain attachments,
///i.e. if the swapChainAttachment id is 1 and there are additionally 2 static attachments
///given the static attachments will have the ids 0 and 2.
///One must assure that the given attachments will create a framebuffer that is compatible
///for the given render pass, the class itself will perform no checking.
//TODO XXX: one big pile of shit needs a (complete) rework. another day.
//TODO XXX: rework queues. seperate gfx and present. Possibility for automatic selection
class SwapChainRenderer : public ResourceReference<SwapChainRenderer>
{
public:
	//todo
	struct AttachmentInfo
	{
		ViewableImage::CreateInfo info; //create info for static/dynamic attachments
		vk::MemoryPropertyFlags memoryFlags; //memory flags for static/dynamic attachments
		bool dynamic = false; //dynamic or static attachment?
		vk::ImageView external = {}; //if != VK_NULL_HANDLE all other members will be ignored
	};

	struct CreateInfo
	{
		vk::RenderPass renderPass;
		Queue* queue {}; //the graphical and present queue to use. XXX
		std::vector<AttachmentInfo> attachments {};
		unsigned int swapChainAttachment = 0; //id of the swapChain attachment
	};

	struct RenderBuffer
	{
		Framebuffer framebuffer;
		CommandBuffer commandBuffer;
	};

public:
	SwapChainRenderer() = default;
	SwapChainRenderer(const SwapChain& swapChain, const CreateInfo& info, RendererBuilder& builder);
	~SwapChainRenderer();

	SwapChainRenderer(SwapChainRenderer&& other) noexcept;
	SwapChainRenderer& operator=(SwapChainRenderer&& other) noexcept;

	void create(const SwapChain& swapChain, const CreateInfo& info);
	void init(RendererBuilder& builder);

	///Renders one frame and presents the swapChain buffer.
	///Depending on the swapChain settings this call might block (vsync).
	void render();

	const SwapChain& swapChain() const { return *swapChain_; }
	const std::vector<RenderBuffer>& renderBuffers() const { return renderBuffers_; }
	const std::vector<ViewableImage>& staticAttachments() const { return staticAttachments_; }

	const SwapChain& resourceRef() const { return *swapChain_; }
	friend void swap(SwapChainRenderer& a, SwapChainRenderer& b) noexcept;

protected:
	const SwapChain* swapChain_ = nullptr;
	const Queue* queue_ = nullptr;

	std::vector<RenderBuffer> renderBuffers_;
	std::vector<ViewableImage> staticAttachments_;
	CreateInfo info_;

protected:
	void destroyRenderBuffers();
	void buildCommandBuffers(RendererBuilder& builder);
};

}
