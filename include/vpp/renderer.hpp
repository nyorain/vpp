// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp>
#include <vpp/queue.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/sync.hpp>
#include <vpp/swapchain.hpp>
#include <functional>
#include <variant>

namespace vpp {

/// Fence that knows if it has been used.
/// Will wait on destruction if it has been used.
class TrackedFence : public Fence {
public:	
	bool used {};

public:
	using Fence::Fence;
	TrackedFence(TrackedFence&&) noexcept = default;
	TrackedFence& operator=(TrackedFence&&) noexcept = default;
	~TrackedFence();
};

/// Simple default implementation for rendering onto a surface.
/// Abstract class, one has to implement the framebuffer initialization
/// as well as the command buffer recording.
/// See the DefaultRenderer class for a simple framebuffer helper.
/// Internally uses a swapchain.
/// Remember that the Renderer destructor will destroy all commandbuffers
/// and fences so it must be assured that no submission is left unfinished.
class Renderer : public ResourceReference<Renderer> {
public:
	/// When to record command buffers.
	enum class RecordMode {
		all, /// Re-records all command buffer when invalidated, default
		onDemand, /// Records command buffers when needed
		always /// Records the needed commandBuffer every frame (even if valid)
	};

	/// All required information to rendering onto one buffer of
	/// the swapchain.
	struct RenderBuffer {
		bool valid {};
		unsigned int id {};
		vk::Image image {};
		vk::CommandBuffer commandBuffer {};
		ImageView imageView;
		Framebuffer framebuffer;
		Semaphore semaphore;
		TrackedFence fence;
	};

public:
	/// This will automatically finish all pending render calls.
	/// But derived should ensure that they (e.g. in their destructor)
	/// are not destroying any rendering resources.
	virtual ~Renderer() = default;

	/// Resizes the internal swapchain and recreates the render
	/// buffers. Should be called when the underlaying surface was
	/// resized (or e.g. when render returned outOfDate or suboptimal).
	/// Will set the size parameter to the actually used size (but
	/// must be filled with an expected size which might be used).
	/// Must not be called while a render call is left unfinished.
	void resize(const vk::Extent2D& size, vk::SwapchainCreateInfoKHR&);

	/// Invalidates all recorded command buffers.
	/// Only if the record mode is RecordMode::all all command buffers
	/// are re-recorded before this function returns, so with this record
	/// mode the function must not be called while a render call
	/// is left unfinished.
	void invalidate();

	/// Renders one frame. If fence is not nullptr, sets the fence parameter to 
	/// the fence signaling the end of the frame if vk::Result is success.
	/// Otherwise returns the ocurred redner error (which might be non-critical
	/// e.g. a suboptimal swapchain, will not render then nontheless).
	/// It is allowed to call this function again before the fence completes
	/// (at least if your frame- and commandbuffers allow it). Until the
	/// fence is signaled, no rendering resources must be invalidated.
	/// This means calls to e.g. resize in this time will trigger
	/// undefined behaviour.
	/// Unless you will assure submission finish in some other way (e.g.
	/// calling this->wait(), vk{Device,Queue}WaitIdle) you want to
	/// receive the fence and make sure all submissions are finished
	/// before destroying rendering resources (used buffer/pipelines or
	/// calling resize).
	/// This function might block for quite some time nontheless when acquiring
	/// the next image or waiting for the last execution on this
	/// render buffer to finish.
	vk::Result render(vk::Fence* = {});

	/// Renders one frame and waits for all frame operations to finish.
	/// Returns any ocurred rendering error (which might be non-critical
	/// e.g. a suboptimal swapchain, will not render then nontheless).
	vk::Result renderBlock();

	/// Changes the record mode.
	/// Does not invalidate any currently recorded command buffers
	/// but if records all invalid command buffers RecordMode is set to all,
	/// so when there are invalid command buffers and this is called with 
	/// RecordMode::all, there must be no unfinished render calls.
	void recordMode(RecordMode);

	/// Waits for all pending rendering operations from this
	/// renderer to finish.
	void wait();

	const auto& swapchain() const { return swapchain_; }
	auto recordMode() const { return mode_; }
	const auto& resourceRef() const { return swapchain_; }

protected:
	/// Initializes the Renderer into invalid state.
	/// One must call init before any other function can be called.
	/// Has no initializing constructor since the init
	/// function requires the virtual functions.
	Renderer() = default;

	Renderer(Renderer&&) noexcept = default;
	Renderer& operator=(Renderer&&) noexcept = default;

	/// Creates and initializes the Renderer.
	/// Only records command buffers if record is passed as true.
	/// If no rendering queue is given, will choose any queue from
	/// the associated device with graphics bit set.
	/// The real size of the swapchain will be returned in size.
	/// Will also record all command buffers (if the record mode
	/// is RecrodMode::all).
	void init(const vk::SwapchainCreateInfoKHR&,
		const Queue& present, const Queue* render = {},
		RecordMode = RecordMode::all);

	/// (Re-)creates the render buffers.
	void createBuffers(const vk::Extent2D& size, vk::Format swapchainFormat);

	// - abstract interface -
	/// Records into the given RenderBuffer.
	/// Should use the render buffers framebuffer which is guaranteed
	/// to be a framebuffer returned by createFramebuffer. It is 
	/// also guaranteed that initFramebuffers was called since then.
	virtual void record(const RenderBuffer&) = 0;

	/// Should initialize all internal information required for rendering, 
	/// such as framebuffers. Will be called everytime the swapchain
	/// was recreated and therefore size and/or image views have changed.
	virtual void initBuffers(const vk::Extent2D&, nytl::Span<RenderBuffer>) = 0;

protected:
	Swapchain swapchain_ {};
	const Queue* present_ {};
	const Queue* render_ {};
	CommandPool commandPool_;
	std::vector<RenderBuffer> renderBuffers_;
	vpp::Semaphore acquireSemaphore_;
	RecordMode mode_ {RecordMode::all};
};

/// Simple default framebuffer handling Renderer implementation.
/// Just initializes the framebuffers with the swapchain color attachments.
/// Derived classes can also add additional attachments that will be added
/// to the framebuffer (but then should also make sure they have the right
/// size every time init is called - usually by overriding it and calling
/// the Base default implementation).
/// The record method is still abstract.
class DefaultRenderer : public Renderer {
protected:
	/// Like the Renderer::init but additionally stores the
	/// renderPass to create the framebuffers for.
	void init(vk::RenderPass, const vk::SwapchainCreateInfoKHR&,
		const Queue& present, const Queue* render = {},
		RecordMode = RecordMode::all);

	/// Overrides the default init method with a call to the init function
	/// below (without additional attachments). You have to override in
	/// when you want to use additional attachments and then assure in 
	/// every call that the attachments have the correct dimensions.
	void initBuffers(const vk::Extent2D&, nytl::Span<RenderBuffer>) override;

	/// Initializes the framebuffers in the given renderbuffers with
	/// the given attachments and the swapchain attachment.
	/// If all given attachments are not nullHandle, will add the swapchain
	/// image view as last framebuffer attachment, otherwise if there is
	/// exactly one nullHandle image view will replace it with the
	/// renderBuffer's swapchain imageView.
	/// Passing more than one nullHandle imageView is undefined behvaiour.
	void initBuffers(const vk::Extent2D&, nytl::Span<RenderBuffer>,
		std::vector<vk::ImageView> attachments);

	const auto& renderPass() const { return renderPass_; }

protected:
	vk::RenderPass renderPass_ {};
};

} // namespace vpp