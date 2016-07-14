#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/image.hpp>

#include <memory>
#include <vector>

namespace vpp
{

///Abstract Interface for building renderers.
///Will be passed to a SwapChainRenderer constructor to record the prebaked commandBuffers.
class RendererBuilder
{
public:
	using AdditionalSemaphores = std::vector<std::pair<vk::Semaphore, vk::PipelineStageBits>>;

public:
	///This function is called to record the render commands into the given renderpass instance.
	virtual void build(unsigned int id, const RenderPassInstance& ini) = 0;

	///Should return the clearValues for the given render buffer id.
	virtual std::vector<vk::ClearValue> clearValues(unsigned int id) = 0;

	///This function will be called once by the SwapChainRenderer, after it was constructed
	///and before it will use any other builder functions.
	virtual void init(SwapChainRenderer&) {};

	///Will be called to record additional command buffer commands before rendering.
	virtual void beforeRender(vk::CommandBuffer) {};

	///Will be called to record additional command buffer commands after rendering.
	virtual void afterRender(vk::CommandBuffer) {};

	///Will be called everytime the commands for a frame are submitted to the gpu.
	///The the RendererBuilder then has the possibilty to queue e.g. additional command buffers
	///and return a semaphores for that should be waited before beginning
	///with the rendering commands.

	///XXX: alternative function.
	///+: greater flexibility, some things maybe not possible with 2nd version (?)
	///-: way more complexity, renderer builder has to assure that command buffer array and
	///		semaphore and stages mask remain valid. Worse interface.
	//virtual CommandExecutionState submit(unsigned int, vk::CommandBuffer gfx, vk::Semaphore render,
	//	vk::Semaphore& present);

	///XXX: this function is bad at all... it allocates something on the heap but is one
	///	of the functions called every frame. Can be done better in some way?
	/// Some SubmitManager functions as well. Then at least use some kind of pmr.

	virtual AdditionalSemaphores submit(unsigned int id) { return {}; }

	///This function is called before every frame and allows the builder to execute/queue
	///additional operations or to re-record the command buffer for the given id.
	///It is called exactly before the command buffer for the given id is queued for submission.
	virtual void frame(unsigned int id) {};
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
///for the given render pass, the class itself wont (and cannnot) perform any checking.
class SwapChainRenderer : public ResourceReference<SwapChainRenderer>
{
public:
	struct AttachmentInfo
	{
		//will be used to create static/dynamic attachments
		ViewableImage::CreateInfo createInfo;

		//specifies whether it is a dynamic or static attachment.
		//Each framebuffer (for the different swapChain attachments) has its own dyanmic attachments
		//while static attachments are shared among them
		bool dynamic = false;

		//if valid all other members will be ignored and this will be used as some additional
		//external attachment. Note that it must have at least the size of the swapChain
		vk::ImageView external = {};
	};

	///The CreateInfo struct holds all information that is needed for construction a
	///SwapChainRenderer. It allows to define additional attachments of different types.
	struct CreateInfo
	{
		vk::RenderPass renderPass; //the render pass to use for the rendering
		unsigned int queueFamily; //the queue family for graphical operations
		std::vector<AttachmentInfo> attachments; //additional attachments
		unsigned int swapChainAttachment = 0; //id of the swapChain attachment in the fbs

		//Those parameters are used to make resizing (i.e. recreating) more efficient in
		//terms of memory allocation
		unsigned int maxWidth = 1920;
		unsigned int maxHeight = 1080;
	};

	///The RenderBuffer class hold a framebuffer for each swapChain image as well a
	///commandBuffer with the last recoreded commands for this framebuffer.
	///It is necessary to create a commandBuffer for each framebuffer since at the
	///beginning of a render pass one has to specify the framebuffer he wants to
	///draw into and one has to create multiple framebuffers for the different
	///swapChain color images.
	struct RenderBuffer
	{
		Framebuffer framebuffer;
		CommandBuffer commandBuffer;
	};

	///Convinience typedef for the rendering work and presentation work.
	using RenderWork = std::unique_ptr<Work<void>>;

	///Typedef for the renderer builder implementation.
	using RenderImpl = std::unique_ptr<RendererBuilder>;

public:
	SwapChainRenderer() = default;
	SwapChainRenderer(const SwapChain& swapChain, const CreateInfo& info, RenderImpl builder);
	~SwapChainRenderer();

	SwapChainRenderer(SwapChainRenderer&& other) noexcept;
	SwapChainRenderer& operator=(SwapChainRenderer other) noexcept;

	///Creates all static attachments and all framebuffers.
	void create(const SwapChain& swapChain, const CreateInfo& info);

	///Initialized all attachments and creates the vulkan framebuffers.
	void init(RenderImpl builder);

	///Renders one frame and returns after that. May block (waiting for the next present image)
	///depending on the settings of the given swapChain.
	///The queue paramters are optional. If they are nullptr, a queue will automatically be selected.
	///\param presentQueue The queue to submit the present commands to.
	///\param graphicsQueue The queue to submit the graphics commands to.
	///\exception std::logic_error If a valid present or graphics queue cannot be found or if
	///the family of the grahpics queue is not compatible with the recorded command buffers.
	///\return A Work pointer that can be used to track the state or wait for the rendering
	///to finish.
	RenderWork render(const Queue* present = nullptr, const Queue* graphics = nullptr);

	///Renders one frame and waits until all rendering operations are finished.
	///The queue paramters are optional. If they are nullptr, a queue will automatically be selected.
	///\param presentQueue The queue to submit the present commands to.
	///\param graphicsQueue The queue to submit the graphics commands to.
	///\exception std::logic_error If a valid present or graphics queue cannot be found or if
	///the family of the grahpics queue is not compatible with the recorded command buffers.
	void renderBlock(const Queue* present = nullptr, const Queue* graphics = nullptr);

	///Calls the builder to build the commandBuffer with the given id.
	///\param id The id of the render buffer to (re)record. If it is -1, all buffers will be recorded.
	void record(int id = -1);

	const SwapChain& swapChain() const { return *swapChain_; }
	const std::vector<RenderBuffer>& renderBuffers() const { return renderBuffers_; }
	const std::vector<ViewableImage>& staticAttachments() const { return staticAttachments_; }

	vk::RenderPass vkRenderPass() const { return info_.renderPass; }
	unsigned int renderQueueFamily() const { return info_.queueFamily; }
	unsigned int swapChainAttachment() const { return info_.swapChainAttachment; }
	const std::vector<AttachmentInfo>& attachmentInfos() const { return info_.attachments; }

	const SwapChain& resourceRef() const { return *swapChain_; }
	friend void swap(SwapChainRenderer& a, SwapChainRenderer& b) noexcept;

protected:
	const SwapChain* swapChain_ = nullptr;
	RenderImpl renderImpl_ = nullptr;
	std::vector<RenderBuffer> renderBuffers_;
	std::vector<ViewableImage> staticAttachments_;
	CreateInfo info_;
};

}
