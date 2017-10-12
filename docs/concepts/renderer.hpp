#pragma once

#include <vpp/fwd.hpp>
#include <functional>

class Renderer;

/// Interface for all required operations to use a Renderer.
class RendererInterface {
public:
	/// Function that records into the given RenderBuffer.
	/// Should use the render buffers framebuffer which is guaranteed
	/// to be a framebuffer returned by createFramebuffer. It is 
	/// also guaranteed that initFramebuffers was called since then.
	virtual void record(const RenderBuffer&) = 0;

	/// Function that (re-)creates the framebuffer of the given RenderBuffer.
	/// Returning an empty framebuffer object is treated as error.
	/// The returned framebuffer might be not initialized yet, see 
	/// initFramebuffers below.
	virtual vpp::Framebuffer createFramebuffer(vk::Extent2D, const RenderBuffer&) = 0;

	/// Should initialize all render buffers (could e.g. be used for two-step
	/// framebuffer initialization for fewer allocations).
	/// Will be called after all createFramebuffer calls and before
	/// the first record call on any of these framebuffers.
	virtual void initFramebuffers() = 0;
};

/// Simply default framebuffer handling Renderer implementation.
/// Creates the framebuffer with a number of static attachments (i.e.
/// one attachment per all framebuffers). Note that if you use a
/// static attachments, you can always render only one frame at a time.
/// You still have to implement the record method.
class DefaultRenderer {
public:
	using AttachmentInfo = std::variant<
		std::monostate, // represents the swapchain attachment
		ViewableImage::CreateInfo, // creates a dynamic attachment
		vpp::ViewableImage, // static, owned attachment
		vk::ImageView> // static, non-owned attachment

	std::vector<AttachmentInfo> attachments;
	std::vector<ViewableImage> ownedStaticAttachments;
	vk::RenderPass renderPass;

	virtual vpp::Framebuffer createFramebuffer(vk::Extent2D, const RenderBuffer&) = 0;
	virtual void initFramebuffers() = 0;
};

/// Simple default implementation for rendering onto a swapchain.
class Renderer {
public:
	struct RenderBuffer {
		bool valid;
		unsigned int id;
		vk::CommandBuffer;
		vpp::Framebuffer;
		vpp::ImageView;
		vk::Image;

		// own semaphore and fence? e.g. when rendering multiple buffers?
	};


public:
	Renderer(const Swapchain&, const Queue& present);
	~Renderer();

	/// Resets the command buffer recorder.
	/// Calls invalidate with the givne rerecord paramter.
	void impl(RendererInterface, bool rerecord = false);

	/// Invalidates all recorded command buffers.
	/// If rerecord is true, immediately records all command buffers,
	/// otherwise only if needed.
	void invalidate(bool rerecord);

	/// Recreates all RenderBuffers.
	/// Must be called when the swapchain is recreated (e.g. changes
	/// its size), might also be used to just use another swapchain.
	void recreateBuffers(const Swapchain* = nullptr);

	/// Renders one frame. Sets the fence parameter to the fence
	/// signaling the end of the frame if vk::Result is success.
	/// Otherwise returns the ocurred error (which might be non-critical
	/// e.g. swapchainOutOfDate).
	/// It is allowed to call this function again before the fence completes
	/// (at least if your created framebuffers allow it, i.e. share no
	/// attachments) but until then no rendering resources must be invalidated.
	/// This means calls to e.g. recreateBuffers in this time will trigger
	/// undefined behvaiours.
	vk::Result render(const vpp::Fence*&);

	/// Renders on frame and waits for all frame operations to finish.
	/// Returns any ocurred error (which might be non-critical
	/// e.g. swapchainOutOfDate).
	vk::Result renderBlock();

protected:
	const Swapchain* swapchain_ = nullptr;
	const Queue* present_ = nullptr;
	RendererInterface impl_;
	std::vector<RenderBuffer> renderBuffers_;
};


// TODO: not sure if it might be a good idea to split interface

/// Function that records into the given RenderBuffer.
/// The framebuffer can be used only with the the renderPass
/// associated with this renderer.
// using Recorder = std::function<void(unsigned int id, const RenderBuffer&)>;

/// Function that (re-)creates the framebuffer of the given RenderBuffer.
/// Might e.g. add its own attachments according to the renderPass.
// using FramebufferBuilder = std::function<void(RenderBuffer&)>;