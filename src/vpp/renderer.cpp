// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/renderer.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/submit.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

Renderer::Renderer(const Queue& present, QueueSubmitter* submitter,
	RecordMode mode) : present_(&present), mode_(mode) {

	submitter_ = submitter ? submitter : &device().queueSubmitter();
	commandPool_ = {device(), submitter_->queue().family()};
	acquireSemaphore_ = {device()};
}

void Renderer::init(const vk::SwapchainCreateInfoKHR& scInfo) {
	dlg_assert(present_ && commandPool_ && acquireSemaphore_);
	dlg_assert(!swapchain_);

	swapchain_ = {present_->device(), scInfo};
	createBuffers(scInfo.imageExtent, scInfo.imageFormat);
	invalidate();
}

void Renderer::invalidate() {
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

void Renderer::createBuffers(const vk::Extent2D& size, vk::Format format) {
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
			dlg_assert(!buf.valid && !buf.id);

			buf.commandBuffer = cmdBufs.back();
			cmdBufs.pop_back();
			buf.semaphore = {device()};
			buf.id = i;
		}

		buf.image = viewInfo.image = images[i];
		buf.imageView = {device(), viewInfo};
		buf.valid = false;
	}

	initBuffers(size, renderBuffers_);
}

void Renderer::recreate(const vk::Extent2D& size,
		vk::SwapchainCreateInfoKHR& info) {

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

vk::Result Renderer::render(std::optional<uint64_t>* sid,
		const RenderInfo& info) {

	dlg_assert(swapchain_ && commandPool_);
	if(sid) {
		sid->reset();
	}

	// we use acquireSemaphore_ to acquire the image
	// this semaphore is always unsignaled
	std::uint32_t id;
	auto res = swapchain().acquire(id, acquireSemaphore_, {}, UINT64_MAX - 1);
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
	if(!buf.valid) {
		record(buf);
		buf.valid = true;
	} else if(mode_ == RecordMode::always) {
		// NOTE: there is currently no better way to do this,
		// we cannot reset the commandPool since there might be
		// command buffers that are currently in use
		// Maybe create commandPool with the freeable flag in this case
		buf.commandBuffer = {};
		buf.commandBuffer = commandPool_.allocate();
		record(buf);
	}

	// note how we use the same semaphore for wait and signal
	// signal will occur always after wait has finished which
	// makes this valid
	waitCache_.clear();
	waitStageCache_.clear();

	waitCache_.push_back(buf.semaphore.vkHandle());
	waitStageCache_.push_back(vk::PipelineStageBits::colorAttachmentOutput);

	for(auto w : info.wait) {
		waitCache_.push_back(w.semaphore);
		waitStageCache_.push_back(w.stage);
	}

	signalCache_.clear();
	signalCache_.push_back(buf.semaphore);
	signalCache_.insert(signalCache_.end(), info.signal.begin(),
		info.signal.end());

	vk::SubmitInfo submitInfo;
	submitInfo.pWaitSemaphores = waitCache_.data();
	submitInfo.pWaitDstStageMask = waitStageCache_.data();
	submitInfo.waitSemaphoreCount = waitCache_.size();
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &buf.commandBuffer;
	submitInfo.pSignalSemaphores = signalCache_.data();
	submitInfo.signalSemaphoreCount = signalCache_.size();

	auto submitID = submitter().add(submitInfo);
	submitter().submit();

	if(sid) {
		*sid = submitID;
	}

	return swapchain().present(*present_, id, buf.semaphore);
}

vk::Result Renderer::renderSync(const RenderInfo& info) {
	std::optional<uint64_t> id;
	auto res = render(&id, info);
	if(id) {
		submitter().wait(*id);
	}

	return res;
}

void Renderer::recordMode(RecordMode nm) {
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
void DefaultRenderer::init(vk::RenderPass rp,
		const vk::SwapchainCreateInfoKHR& scInfo) {

	renderPass_ = rp;
	Renderer::init(scInfo);
}

void DefaultRenderer::initBuffers(const vk::Extent2D& size,
		nytl::Span<RenderBuffer> buffers) {

	initBuffers(size, buffers, {});
}

void DefaultRenderer::initBuffers(const vk::Extent2D& size,
		nytl::Span<RenderBuffer> bufs, std::vector<vk::ImageView> attachments) {

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
