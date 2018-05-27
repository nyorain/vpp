// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp>
#include <vpp/queue.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/sync.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/commandBuffer.hpp>
#include <functional>
#include <variant>

namespace vpp {

/// Additional (optional) rendering synchronization.
struct RenderInfo {
	nytl::Span<const StageSemaphore> wait {};
	nytl::Span<const vk::Semaphore> signal {};
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
	/// Can also be used to change the swapchain settings (e.g.
	/// when vsync was toggled).
	void recreate(const vk::Extent2D& size, vk::SwapchainCreateInfoKHR&);

	/// Invalidates all recorded command buffers.
	/// Only if the record mode is RecordMode::all all command buffers
	/// are re-recorded before this function returns, so with this record
	/// mode the function must not be called while a render call
	/// is left unfinished.
	void invalidate();

	/// Renders one frame.
	/// If submitID is not zero, will set it to the id received from
	/// the submitter that can be used to track its state and e.g. wait for it
	/// to complete.
	/// If acquiring the image fails, returns the error and does not render.
	/// In this case the submitID (if not null) will be empty.
	/// If presenting the image fails, returns the error. But since the
	/// command buffer was then already submitted, will set the submit id
	/// (if not null). That means that even when it returns a error, you
	/// have to check (and eventually) wait upon the submit id (or otherwise
	/// assure that rendering is finished e.g. via deviceIdle).
	/// Otherwise returns vk::Result::Success.
	/// Until the rendering completes (which can be tracked via submitID or
	/// by e.g. calling vulkans waitIdle functions), no rendering resources must
	/// be invalidated. This means calls to e.g. resize in this time will
	/// trigger undefined behavior.
	/// Submitting the rendering command buffer will add the given semaphores
	/// and stages from RenderInfo.
	/// It is allowed to call this function again before the fence completes
	/// (at least if your frame- and commandbuffers allow it).
	vk::Result render(std::optional<uint64_t>* submitID = {},
		const RenderInfo& = {});

	/// Renders one frame and waits for all frame operations to finish.
	/// Internally just calls render and waits for the submit id to finish.
	/// Returns any error that ocurred in render.
	vk::Result renderSync(const RenderInfo& = {});

	/// Changes the record mode.
	/// Does not invalidate any currently recorded command buffers
	/// but if records all invalid command buffers RecordMode is set to all,
	/// so when there are invalid command buffers and this is called with
	/// RecordMode::all, there must be no unfinished render calls.
	void recordMode(RecordMode);

	const auto& swapchain() const { return swapchain_; }
	auto recordMode() const { return mode_; }
	const auto& resourceRef() const { return *present_; }
	auto& submitter() const { return *submitter_; }

protected:
	/// Initializes the Renderer into invalid state.
	/// The deriving class can initialize its own resources, but then
	/// has to call init before any methods can be used.
	/// Has no initializing constructor since the init
	/// function requires the virtual functions.
	Renderer(const Queue& present, QueueSubmitter* render = {},
		RecordMode = RecordMode::all);

	Renderer(Renderer&&) noexcept = default;
	Renderer& operator=(Renderer&&) noexcept = default;

	/// Creates and initializes the Renderer.
	/// Only records command buffers if record is passed as true.
	/// If no rendering queue is given, will choose any queue from
	/// the associated device with graphics bit set.
	/// The real size of the swapchain will be returned in size.
	/// Will also record all command buffers (if the record mode
	/// is RecrodMode::all).
	void init(const vk::SwapchainCreateInfoKHR&);

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
	QueueSubmitter* submitter_ {};
	CommandPool commandPool_;
	std::vector<RenderBuffer> renderBuffers_;
	vpp::Semaphore acquireSemaphore_;
	RecordMode mode_ {RecordMode::all};

	std::vector<vk::Semaphore> waitCache_;
	std::vector<vk::PipelineStageFlags> waitStageCache_;
	std::vector<vk::Semaphore> signalCache_;
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
	using Renderer::Renderer;

	/// Like the Renderer::init but additionally stores the
	/// renderPass to create the framebuffers for.
	void init(vk::RenderPass, const vk::SwapchainCreateInfoKHR&);

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
	/// Passing more than one nullHandle imageView is undefined behavior.
	void initBuffers(const vk::Extent2D&, nytl::Span<RenderBuffer>,
		std::vector<vk::ImageView> attachments);

	const auto& renderPass() const { return renderPass_; }

protected:
	vk::RenderPass renderPass_ {};
};

} // namespace vpp
