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

/// Simple default implementation for rendering onto a surface.
/// Abstract class, one has to implement the framebuffer initialization
/// as well as the command buffer recording.
/// See the DefaultRenderer class for a simple framebuffer helper.
/// Internally uses a swapchain.
class Renderer : public Resource {
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
		Fence fence;
		Semaphore semaphore;
	};

public:
	virtual ~Renderer() = default;

	/// Resizes the internal swapchain and recreates the render
	/// buffers. Should be called when the underlaying surface was
	/// resized (or e.g. when render returned outOfDate or suboptimal).
	/// Will set the size parameter to the actually used size (but
	/// must be filled with an expected size which might be used).
	void resize(vk::Extent2D& size, const vpp::SwapchainSettings& = {});

	/// Invalidates all recorded command buffers.
	/// Only if the record mode is RecordMode::all all command buffers
	/// are re-recorded.
	void invalidate();

	/// Renders one frame. Sets the fence parameter to the fence
	/// signaling the end of the frame if vk::Result is success.
	/// Otherwise returns the ocurred redner error (which might be non-critical
	/// e.g. a suboptimal swapchain, will not render then nontheless).
	/// It is allowed to call this function again before the fence completes
	/// (at least if your frame- and commandbuffers allow it). Until the
	/// fence is signaled, no rendering resources must be invalidated.
	/// This means calls to e.g. resize in this time will trigger
	/// undefined behaviour.
	vk::Result render(vk::Fence&);

	/// Renders on frame and waits for all frame operations to finish.
	/// Returns any ocurred rendering error (which might be non-critical
	/// e.g. a suboptimal swapchain, will not render then nontheless).
	vk::Result renderBlock();

	/// Changes the record mode.
	/// Does not invalidate any currently recorded command buffers
	/// but if records all invalid command buffers RecordMode is set to all.
	void recordMode(RecordMode);

	const auto& swapchain() const { return swapchain_; }
	auto recordMode() const { return mode_; }

protected:
	/// Initializes the Renderer into invalid state.
	/// This is the same state it will have after being moved from.
	/// No methods must be called on it.
	Renderer() = default;

	/// Creates and initializes the Renderer.
	/// Only records command buffers if record is passed as true.
	/// If no rendering queue is given, will choose any queue from
	/// the associated device with graphics bit set.
	/// The real size of the swapchain will be returned in size.
	Renderer(vk::SurfaceKHR, vk::Extent2D& size, const Queue& present, 
		const Queue* render = {}, const vpp::SwapchainSettings& = {},
		RecordMode = RecordMode::all);

	Renderer(Renderer&&) noexcept = default;
	Renderer& operator=(Renderer&&) noexcept = default;

	// - abstract interface -
	/// Records into the given RenderBuffer.
	/// Should use the render buffers framebuffer which is guaranteed
	/// to be a framebuffer returned by createFramebuffer. It is 
	/// also guaranteed that initFramebuffers was called since then.
	virtual void record(const RenderBuffer&) = 0;

	/// Should initialize all internal information required for rendering, 
	/// such as framebuffers. Will be called everytime the swapchain
	/// was recreated and therefore size and/or image views have changed.
	virtual void init(const vk::Extent2D&, nytl::Span<RenderBuffer>) = 0;

protected:
	vk::SurfaceKHR surface_ {};
	Swapchain swapchain_ {};
	const Queue* present_ {};
	const Queue* render_ {};
	CommandPool commandPool_;
	std::vector<RenderBuffer> renderBuffers_;
	std::vector<std::pair<Semaphore, Fence>> acquireSyncs_;
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
	/// Like the Renderer constructor but additionally needs the
	/// renderPass to create the framebuffers for.
	DefaultRenderer(vk::RenderPass, vk::SurfaceKHR, vk::Extent2D& size, 
		const Queue& present, const Queue* render = {}, 
		const vpp::SwapchainSettings& = {}, RecordMode = RecordMode::all);

	/// Overrides the default init method with a call to the init function
	/// below (without additional attachments). You have to override in
	/// when you want to use additional attachments and then assure in 
	/// every call that the attachments have the correct dimensions.
	void init(const vk::Extent2D& sz, nytl::Span<RenderBuffer> bufs) override {
		init(sz, bufs, {});
	}

	/// Initializes the framebuffers in the given renderbuffers with
	/// the given attachments and the swapchain attachment.
	/// If all given attachments are not nullHandle, will add the swapchain
	/// image view as last framebuffer attachment, otherwise if there is
	/// exactly one nullHandle image view will replace it with the
	/// renderBuffer's swapchain imageView.
	/// Passing more than one nullHandle imageView is undefined behvaiour.
	void init(const vk::Extent2D&, nytl::Span<RenderBuffer>,
		std::vector<vk::ImageView> attachments);

	// TODO: don't use a vector above. Maybe there MUST be a nullHandle
	// imageView, we use a span and guarantee to leave it unchanged (
	// after the call finishes?)

protected:
	vk::RenderPass renderPass_ {};
};

} // namespace vpp

#include <vpp/vk.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/submit.hpp>
#include <dlg/dlg.hpp>

// NOTE (implementation): the current implementation should (almost) never
//  create any semaphores or fences. But this costs extra calls
//  to getFenceStats/resetFences. Not sure if worth it

namespace vpp {

Renderer::Renderer(vk::SurfaceKHR surface, vk::Extent2D& size, 
	const Queue& present, const Queue* render, 
	const vpp::SwapchainSettings& scs, RecordMode mode) :
		Resource(present.device()), surface_(surface), present_(&present), 
		render_(render), mode_(mode)
{
	if(!render_) {
		render_ = device().queue(vk::QueueBits::graphics);
		if(!render_) {
			throw std::runtime_error("Renderer: device has no graphcis queue");
		}
	} else {
		dlg_assert(&device() == &render_->device());
	}

	dlg_assert(&device() == &present_->device());
	commandPool_ = {device(), render_->family()};
	acquireSyncs_.push_back({device(), device()});
	resize(size, scs);
}

void Renderer::invalidate()
{
	dlg_assert(swapchain_);
	dlg_assert(commandPool_);

	vk::resetCommandPool(device(), commandPool_, {});
	for(auto& buf : renderBuffers_) {
		if(mode_ == RecordMode::all) {
			record(buf);
			buf.valid = true;	
		} else {
			buf.valid = false;
		}
	}
}

void Renderer::resize(vk::Extent2D& size, 
	const vpp::SwapchainSettings& settings)
{
	dlg_assert(swapchain_);
	dlg_assert(commandPool_);

	// invalidate old image views
	for(auto& buf : renderBuffers_) {
		buf.imageView = {};
		buf.valid = false;
	}

	std::vector<std::pair<vk::Image, ImageView>> views;
	swapchain_.resize(surface_, size, settings, &views);
	auto newCount = views.size();

	std::vector<vk::CommandBuffer> cmdBufs;
	if(newCount > renderBuffers_.size()) {
		std::uint32_t cmdBufCount = newCount - renderBuffers_.size();
		vk::CommandBufferAllocateInfo allocInfo {
			commandPool_, 
			vk::CommandBufferLevel::primary,
			cmdBufCount};
		cmdBufs.resize(cmdBufCount);
		vk::allocateCommandBuffers(device(), allocInfo, *cmdBufs.data());
	} else if(newCount < renderBuffers_.size()) {			
		cmdBufs.resize(renderBuffers_.size() - newCount);
		for(auto i = newCount; i < renderBuffers_.size(); ++i) {
			cmdBufs.push_back(renderBuffers_[i].commandBuffer);
		}
		vk::freeCommandBuffers(device(), commandPool_, cmdBufs);
		cmdBufs.clear();
	}

	renderBuffers_.resize(newCount);
	for(auto i = 0u; i < newCount; ++i) {
		auto& buf = renderBuffers_[i];
		if(!buf.commandBuffer) {
			dlg_assert(!cmdBufs.empty());
			dlg_assert(!buf.fence && !buf.framebuffer);
			dlg_assert(!buf.valid && !buf.id);

			buf.commandBuffer = cmdBufs.back();
			cmdBufs.pop_back();
			buf.fence = {device()};
			buf.semaphore = {device()};
			buf.id = i;
		}

		buf.valid = false;
	}

	init(size, renderBuffers_);
	invalidate();
}

vk::Result Renderer::render(vk::Fence& fence)
{
	dlg_assert(swapchain_ && commandPool_);

	std::pair<vpp::Semaphore, vpp::Fence>* acquireSync {};
	for(auto& sync : acquireSyncs_) {
		auto state = vk::getFenceStatus(device(), sync.second);
		if(state == vk::Result::success) {
			vk::resetFences(device(), {sync.second});
			acquireSync = &sync;
			break;
		}
	}

	if(!acquireSync) {
		acquireSyncs_.push_back({device(), device()});
		acquireSync = &acquireSyncs_.back();
	}

	std::uint32_t id;
	auto res = swapchain().acquire(id, acquireSync->first, acquireSync->second);
	if(res != vk::Result::success) {
		return res;
	}

	dlg_assert(id < renderBuffers_.size());
	auto& buf = renderBuffers_[id];
	if(!buf.valid || mode_ == RecordMode::always) {
		dlg_assert(mode_ != RecordMode::all);
		record(buf);
		buf.valid = true;
	}

	auto semaphores = {acquireSync->first.vkHandle()};
	vk::PipelineStageFlags flags = vk::PipelineStageBits::colorAttachmentOutput;

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = semaphores.size();
	submitInfo.pWaitSemaphores = semaphores.begin();
	submitInfo.pWaitDstStageMask = &flags;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &buf.semaphore.vkHandle();
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buf.commandBuffer;

	// NOTE: integrate with submit manager?
	{
		QueueLock lock(device());
		vk::queueSubmit(*render_,  {submitInfo}, buf.fence);
	}

	fence = buf.fence;	

	return swapchain().present(*present_, id, buf.semaphore);
}

vk::Result Renderer::renderBlock()
{
	vk::Fence fence;
	auto res = render(fence);
	if(res != vk::Result::success) {
		return res;
	}

	vk::waitForFences(device(), {fence}, true, UINT64_MAX);
	return vk::Result::success;
}

void Renderer::recordMode(RecordMode nm)
{
	dlg_assert(swapchain_);

	mode_ = nm;
	if(mode_ == RecordMode::all) {
		for(auto& buf : renderBuffers_) {
			if(!buf.valid) {
				record(buf);
				buf.valid = true;
			}
		}
	}
}

// DefaultRenderer
DefaultRenderer::DefaultRenderer(vk::RenderPass rp, vk::SurfaceKHR s, 
	vk::Extent2D& size, const Queue& present, const Queue* render, 
	const vpp::SwapchainSettings& scs, RecordMode mode)
		: Renderer(s, size, present, render, scs, mode), renderPass_(rp)
{
}

void DefaultRenderer::init(const vk::Extent2D& size, 
	nytl::Span<RenderBuffer> bufs, std::vector<vk::ImageView> attachments)
{
	auto scPos = -1;
	for(auto i = 0u; i < attachments.size(); ++i) {
		if(!attachments[i]) {
			dlg_assert(scPos == -1);
			scPos = i;
		}
	}

	if(scPos == -1) {
		scPos = attachments.size();
		attachments.emplace_back();	
	}

	for(auto& buf : bufs) {
		attachments[scPos] = buf.imageView;
		vk::FramebufferCreateInfo info ({}, 
			renderPass_, 
			attachments.size(), 
			attachments.data(),
			size.width, 
			size.height, 
			1);
		buf.framebuffer = {device(), info};
	}
}

} // namespace vpp