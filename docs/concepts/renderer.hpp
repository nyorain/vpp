// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp>
#include <vpp/queue.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/sync.hpp>
#include <functional>
#include <variant>

// TODO: interface where framebuffer is entirely owned by RendererInterface
//  makes probably more sense.
//  create/init then merges into one function.
// virtual void buildFramebuffers(const Swapchain&, nytl::Span<RenderBuffer>) = 0;

namespace vpp {

class Renderer;
struct RenderBuffer;

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
	virtual Framebuffer createFramebuffer(const Swapchain&, const RenderBuffer&) = 0;

	/// Should initialize all render buffers (could e.g. be used for two-step
	/// framebuffer initialization for fewer allocations).
	/// Will be called after all createFramebuffer calls and before
	/// the first record call on any of these framebuffers.
	/// Must only change the framebuffers of the given render buffers.
	virtual void initFramebuffers(nytl::Span<RenderBuffer>) = 0;
};

/// Simply default framebuffer handling RendererInterface implementation.
/// Creates the framebuffer with a number of static attachments (i.e.
/// one attachment per all framebuffers). Note that if you use a
/// static attachments, you can always render only one frame at a time.
/// You still have to implement the record method.
class DefaultRenderImpl : public Resource {
public:
	using AttachmentInfo = std::variant<
		std::monostate, // represents the swapchain attachment
		ViewableImage::CreateInfo, // creates a dynamic attachment
		// ViewableImage, // static, owned attachment, TODO
		vk::ImageView>; // static, non-owned attachment

	DefaultRenderImpl() = default;
	DefaultRenderImpl(const Device& dev, vk::RenderPass, 
		nytl::Span<AttachmentInfo> attachments);

	vpp::Framebuffer createFramebuffer(const vpp::Swapchain&, const RenderBuffer&);
	void initFramebuffers(nytl::Span<RenderBuffer>);

protected:
	vk::RenderPass renderPass_;
	std::vector<AttachmentInfo> attachments_;
	// std::vector<ViewableImage> ownedStaticAttachments_; // TODO

	struct Buffer {
		Framebuffer framebuffer;
		std::vector<ViewableImage> attachments;
	};

	std::vector<Buffer> buffers_;
};

struct RenderBuffer {
	bool valid {};
	unsigned int id {};
	vk::CommandBuffer commandBuffer {};
	Framebuffer framebuffer;
	vpp::Fence fence;
	vpp::Semaphore semaphore;
};

// TODO: record mode instead of rerecord bools

/// Simple default implementation for rendering onto a swapchain.
class Renderer : public ResourceReference<Renderer> {
public:
	/// When to record command buffers.
	enum class RecordMode {
		all, /// Re-records all command buffer when invalidated
		onDemand, /// Records command buffers when needed
		always /// Records the needed commandBuffer every frame (even if valid)
	};

public:
	Renderer() = default;

	/// Creates and initializes the Renderer.
	/// Only records command buffers if record is passed as true.
	Renderer(const Swapchain&, std::unique_ptr<RendererInterface>, 
		const Queue& present, const Queue& render, RecordMode = RecordMode::all);

	Renderer(Renderer&& lhs) = default;
	Renderer& operator=(Renderer&& lhs) = default;

	/// Resets the command buffer recorder.
	/// Invalidates all command buffers.
	/// If createBuffers is true, recreates all framebuffers.
	void impl(std::unique_ptr<RendererInterface>, bool createBuffers = false);

	/// Recreates all RenderBuffers.
	/// Must be called when the swapchain is recreated (e.g. changes
	/// its size), might also be used to just use another swapchain.
	/// Invalidates all command buffers.
	void recreateBuffers(const Swapchain* = nullptr);

	/// Invalidates all recorded command buffers.
	/// Only if the record mode is RecordMode::all all command buffers
	/// are re-recorded.
	void invalidate();

	/// Renders one frame. Sets the fence parameter to the fence
	/// signaling the end of the frame if vk::Result is success.
	/// Otherwise returns the ocurred error (which might be non-critical
	/// e.g. swapchainOutOfDate).
	/// It is allowed to call this function again before the fence completes
	/// (at least if your frame- and commandbuffers allow it). Until the
	/// fence is signaled, no rendering resources must be invalidated.
	/// This means calls to e.g. recreateBuffers in this time will trigger
	/// undefined behaviour.
	vk::Result render(vk::Fence&);

	/// Renders on frame and waits for all frame operations to finish.
	/// Returns any ocurred error (which might be non-critical
	/// e.g. swapchainOutOfDate).
	vk::Result renderBlock();

	/// Changes the record mode.
	/// Does not invalidate any currently recorded command buffers
	/// but if records all invalid command buffers RecordMode is set to all.
	void recordMode(RecordMode nm);

	const auto& swapchain() const { return *swapchain_; }
	const auto& resourceRef() const { return *swapchain_; }
	auto recordMode() const { return mode_; }

protected:
	const Swapchain* swapchain_ {};
	std::unique_ptr<RendererInterface> impl_;
	const Queue* present_ {};
	const Queue* render_ {};
	CommandPool commandPool_;
	std::vector<RenderBuffer> renderBuffers_;
	std::vector<vk::CommandBuffer> unusedCmdBufs_;
	std::vector<std::pair<vpp::Semaphore, vpp::Fence>> acquireSyncs_;
	RecordMode mode_ {RecordMode::all};
};


} // namespace vpp

// TODO: not sure if it might be a good idea to split interface

/// Function that records into the given RenderBuffer.
/// The framebuffer can be used only with the the renderPass
/// associated with this renderer.
// using Recorder = std::function<void(unsigned int id, const RenderBuffer&)>;

/// Function that (re-)creates the framebuffer of the given RenderBuffer.
/// Might e.g. add its own attachments according to the renderPass.
// using FramebufferBuilder = std::function<void(RenderBuffer&)>;

#include <vpp/vk.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/submit.hpp>
#include <dlg/dlg.hpp>

// NOTE (implementation): the current implementation should (almost) never
//  create any semaphores or fences. But this costs extra calls
//  to getFenceStats/resetFences. Not sure if worth it

namespace vpp {

Renderer::Renderer(const Swapchain& sc, std::unique_ptr<RendererInterface> impl,
	const Queue& present, const Queue& render, RecordMode mode) :
		swapchain_(&sc), impl_(std::move(impl)), present_(&present), 
		render_(&render), commandPool_(device(), render.family()), mode_(mode)
{
	dlg_assert(impl_);
	recreateBuffers(nullptr);
	acquireSyncs_.push_back({device(), device()});
}

void Renderer::impl(std::unique_ptr<RendererInterface> impl, bool createBuffers)
{
	dlg_assert(swapchain_);
	dlg_assert(impl);

	impl_ = std::move(impl);

	if(createBuffers) {
		for(auto& buf : renderBuffers_) {
			buf.framebuffer = impl_->createFramebuffer(swapchain(), buf);
		}

		impl_->initFramebuffers(renderBuffers_);
	}

	invalidate();
}

void Renderer::invalidate()
{
	dlg_assert(swapchain_);
	dlg_assert(impl_);
	dlg_assert(commandPool_);

	vk::resetCommandPool(device(), commandPool_, {});
	for(auto& buf : renderBuffers_) {
		if(mode_ == RecordMode::all) {
			impl_->record(buf);
			buf.valid = true;	
		} else {
			buf.valid = false;
		}
	}
}

void Renderer::recreateBuffers(const Swapchain* sc)
{
	dlg_assert(sc || swapchain_);
	dlg_assert(impl_);
	dlg_assert(commandPool_);

	if(sc) {
		swapchain_ = sc;
	}

	auto& scbufs = swapchain().renderBuffers();
	auto newCount = scbufs.size();

	// NOTE: we could also cache old fences/semaphores, not 
	// sure if worth it. We do it for command buffers since
	// otherwise they would leak or we would have to create the pool
	// for individually freeable command buffers
	if(newCount != renderBuffers_.size()) {
		auto cmdBufCount = newCount - renderBuffers_.size();
		if(cmdBufCount > unusedCmdBufs_.size()) {
			std::uint32_t allocCount = cmdBufCount - unusedCmdBufs_.size();
			vk::CommandBufferAllocateInfo allocInfo {
				commandPool_, 
				vk::CommandBufferLevel::primary,
				allocCount};
			auto old = unusedCmdBufs_.size();
			unusedCmdBufs_.resize(cmdBufCount);
			vk::allocateCommandBuffers(device(), allocInfo, unusedCmdBufs_[old]);
		} else if(renderBuffers_.size() > newCount) {
			for(auto i = newCount; i < renderBuffers_.size(); ++i) {
				unusedCmdBufs_.push_back(renderBuffers_[i].commandBuffer);
			}
		}

		renderBuffers_.resize(newCount);
	}

	for(auto i = 0u; i < newCount; ++i) {
		auto& buf = renderBuffers_[i];
		if(!buf.commandBuffer) {
			dlg_assert(!unusedCmdBufs_.empty());
			dlg_assert(!buf.fence && !buf.framebuffer);
			dlg_assert(!buf.valid && !buf.id);

			buf.commandBuffer = unusedCmdBufs_.back();
			unusedCmdBufs_.pop_back();
			buf.fence = {device()};
			buf.semaphore = {device()};
			buf.id = i;
		}

		buf.valid = false;
		buf.framebuffer = impl_->createFramebuffer(swapchain(), buf);
	}

	impl_->initFramebuffers(renderBuffers_);
	invalidate();
}

vk::Result Renderer::render(vk::Fence& fence)
{
	dlg_assert(swapchain_ && commandPool_ && impl_);

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
		impl_->record(buf);
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

	// NOTE: we should probably integrate this with submit manager
	//  if we don't choose to remove it
	vk::queueSubmit(*render_,  {submitInfo}, buf.fence);
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
	dlg_assert(swapchain_ && impl_);

	mode_ = nm;
	if(mode_ == RecordMode::all) {
		for(auto& buf : renderBuffers_) {
			if(!buf.valid) {
				impl_->record(buf);
				buf.valid = true;
			}
		}
	}
}

// DefaultRenderImpl
DefaultRenderImpl::DefaultRenderImpl(const Device& dev, 
	vk::RenderPass renderPass, nytl::Span<AttachmentInfo> attachments) : 
		Resource(dev), renderPass_(renderPass), 
		attachments_(attachments.begin(), attachments.end())
{
}

vpp::Framebuffer DefaultRenderImpl::createFramebuffer(const vpp::Swapchain& sc,
	const RenderBuffer& buf)
{
	// TODO: framebuffer rework
	Framebuffer::ExtAttachments ext;
	std::vector<const ViewableImage::CreateInfo> infos;
	int swapchain = -1;

/*
	for(auto i = 0u; i < attachments_.size(); ++i) {
		auto& a = attachments_[i];
		if(a.index() == 0) {
			dlg_assert(swapchain == -1);
			swapchain = i;
			// don't beak here, for assertion
		}
	}
*/

	for(auto i = 0u; i < attachments_.size(); ++i) {
		auto& a = attachments_[i];
		if(a.index() == 0) {
			dlg_assert(swapchain == -1);
			swapchain = i;
			ext[i] = sc.renderBuffers()[buf.id].imageView;
		} else if(a.index() == 1) {
			infos.push_back(std::get<1>(a));
		} else if(a.index() == 2) {
			ext[i] = std::get<2>(a);
		}
	}

	if(swapchain == -1) {
		ext[attachments_.size()] = sc.renderBuffers()[buf.id].imageView;
	}

	return {device(), renderPass_, sc.size(), infos, ext};
}

void DefaultRenderImpl::initFramebuffers(nytl::Span<RenderBuffer> bufs)
{
	// TODO
	for(auto& buf : bufs) {
		buf.framebuffer.init(renderPass_);
	}
}

} // namespace vpp