#include <vpp/renderer.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/submit.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

void Renderer::init(const vk::SwapchainCreateInfoKHR& scInfo,
	const Queue& present, const Queue* render, const RecordMode mode)
{
	swapchain_ = {present.device(), scInfo};
	present_ = &present;
	mode_ = mode;

	if(!render) {
		render_ = device().queue(vk::QueueBits::graphics);
		if(!render_) {
			throw std::runtime_error("Renderer: device has no graphcis queue");
		}
	} else {
		render_ = render;
		dlg_assert(&device() == &render_->device());
	}

	dlg_assert(&device() == &present_->device());
	commandPool_ = {device(), render_->family()};

	acquireSemaphore_ = {device()};
	createBuffers(scInfo.imageExtent, scInfo.imageFormat);
	invalidate();
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

void Renderer::createBuffers(const vk::Extent2D& size, vk::Format format)
{
	auto images = swapchain().images();
	auto newCount = images.size();

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

	static const vk::ComponentMapping components{
		vk::ComponentSwizzle::r,
		vk::ComponentSwizzle::g,
		vk::ComponentSwizzle::b,
		vk::ComponentSwizzle::a
	};

	static const vk::ImageSubresourceRange range {
		vk::ImageAspectBits::color, 0, 1, 0, 1
	};

	vk::ImageViewCreateInfo viewInfo {};
	viewInfo.format = format;
	viewInfo.subresourceRange = range;
	viewInfo.viewType = vk::ImageViewType::e2d;
	viewInfo.components = components;

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

		buf.image = viewInfo.image = images[i];
		buf.imageView = {device(), viewInfo};
		buf.valid = false;
	}

	initBuffers(size, renderBuffers_);
}

void Renderer::resize(const vk::Extent2D& size, 
	vk::SwapchainCreateInfoKHR& info)
{
	dlg_assert(swapchain_);
	dlg_assert(commandPool_);

	// invalidate old image views
	for(auto& buf : renderBuffers_) {
		buf.imageView = {};
		buf.valid = false;
	}

	swapchain_.resize(size, info);
	createBuffers(info.imageExtent, info.imageFormat);
	invalidate();
}

vk::Result Renderer::render(vk::Fence* fence)
{
	dlg_assert(swapchain_ && commandPool_);

	// we use acquireSemaphore_ to acquire the image
	// this semaphore is always unsignaled
	std::uint32_t id;
	auto res = swapchain().acquire(id, acquireSemaphore_);
	if(res != vk::Result::success) {
		return res;
	}

	dlg_assert(id < renderBuffers_.size());
	auto& buf = renderBuffers_[id];

	// now we got the buffer id, we swap the buffers semaphore
	// with acquireSemaphore_. This guaranteed that acquireSemaphore_
	// is unsignaled again since the old semaphore in the buffer
	// must have been unsignaled (otherwise we could not have
	// acquired this image)
	using std::swap;
	swap(acquireSemaphore_, buf.semaphore);

	// make sure the command buffer is recorded
	if(!buf.valid || mode_ == RecordMode::always) {
		dlg_assertl(dlg_level_debug, mode_ != RecordMode::all);
		record(buf);
		buf.valid = true;
	}

	vk::PipelineStageFlags flags = vk::PipelineStageBits::colorAttachmentOutput;

	// note how we use the same semaphore for wait and signal
	// signal will occur always after wait has finished which
	// makes this valid
	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &buf.semaphore.vkHandle();
	submitInfo.pWaitDstStageMask = &flags;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &buf.semaphore.vkHandle();
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buf.commandBuffer;

	if(buf.fence.used) {
		vk::waitForFences(device(), {buf.fence}, true, UINT64_MAX);
		vk::resetFences(device(), {buf.fence});
	}

	// NOTE: integrate with submit manager?
	{
		QueueLock lock(device());
		vk::queueSubmit(*render_,  {submitInfo}, buf.fence);
	}

	buf.fence.used = true;
	if(fence) {
		*fence = buf.fence;
	}

	return swapchain().present(*present_, id, buf.semaphore);
}

vk::Result Renderer::renderBlock()
{
	vk::Fence fence {};
	auto res = render(&fence);
	if(fence) {
		vk::waitForFences(device(), {fence}, true, UINT64_MAX);
	}

	return res;
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

void Renderer::wait()
{
	for(auto& buf : renderBuffers_)	{
		if(buf.fence.used) {
			vk::waitForFences(device(), {buf.fence}, true, UINT64_MAX);
			buf.fence.used = false;
		}
	}
}

// DefaultRenderer
void DefaultRenderer::init(vk::RenderPass rp, 
	const vk::SwapchainCreateInfoKHR& scInfo, const Queue& present, 
	const Queue* render, RecordMode mode)
{
	renderPass_ = rp;
	Renderer::init(scInfo, present, render, mode);
}

void DefaultRenderer::initBuffers(const vk::Extent2D& size, 
	nytl::Span<RenderBuffer> buffers)
{
	initBuffers(size, buffers, {});
}

void DefaultRenderer::initBuffers(const vk::Extent2D& size, 
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

// TrackedFence
TrackedFence::~TrackedFence()
{
	if(used && vkHandle()) {
		vk::waitForFences(device(), {vkHandle()}, true, UINT64_MAX);
	}
}

} // namespace vpp