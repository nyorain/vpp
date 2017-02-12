// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/renderer.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/surface.hpp>
#include <vpp/queue.hpp>
#include <vpp/submit.hpp>
#include <vpp/sync.hpp>
#include <vpp/vk.hpp>

#include <stdexcept>

namespace vpp {

// SwapchainRenderer
SwapchainRenderer::SwapchainRenderer(const Swapchain& sc, const CreateInfo& inf, RenderImpl bld)
{
	create(sc, inf);
	init(std::move(bld));
}

SwapchainRenderer::~SwapchainRenderer()
{
}

void SwapchainRenderer::swap(SwapchainRenderer& lhs) noexcept
{
	using std::swap;
	swap(resourceBase(), lhs.resourceBase());
	swap(swapChain_, lhs.swapChain_);
	swap(renderBuffers_, lhs.renderBuffers_);
	swap(staticAttachments_, lhs.staticAttachments_);
	swap(renderImpl_, lhs.renderImpl_);
	swap(info_, lhs.info_);
}

void SwapchainRenderer::create(const Swapchain& swapChain, const CreateInfo& info)
{
	if(!info.renderPass)
		throw std::runtime_error("vpp::SwapchainRenderer: invalid renderPass");

	swapChain_ = &swapChain;
	info_ = info;

	// attachments
	std::vector<ViewableImage::CreateInfo> dynamic;
	Framebuffer::ExtAttachments ext;
	auto size = swapChain.size();
	auto i = 0u;

	// parse attachments
	for(auto& attachInfo : info_.attachments) {
		if(i == info.swapChainAttachment) ++i;
		if(attachInfo.external) {
			ext[i] = attachInfo.external;
		} else if(attachInfo.dynamic) {
			auto& imgInfo = attachInfo.createInfo.imgInfo;
			imgInfo.extent.width = std::max(size.width, info.maxWidth);
			imgInfo.extent.height = std::max(size.height, info.maxHeight);
			imgInfo.extent.depth = 1;
			dynamic.push_back(attachInfo.createInfo);
		} else {
			auto& imgInfo = attachInfo.createInfo.imgInfo;
			imgInfo.extent.width = std::max(size.width, info.maxWidth);
			imgInfo.extent.height = std::max(size.height, info.maxHeight);
			imgInfo.extent.depth = 1;
			staticAttachments_.emplace_back();
			staticAttachments_.back().create(device(), imgInfo,
				attachInfo.createInfo.memoryTypeBits);
		}

		++i;
	}

	// create render buffers
	// command buffers
	renderBuffers_.reserve(swapChain.renderBuffers().size());

	auto qFam = info.queueFamily;
	auto cmdBuffers = device().commandProvider().get(qFam, swapChain.renderBuffers().size(),
		vk::CommandPoolCreateBits::resetCommandBuffer);

	// frame buffers
	for(auto& cmdBuffer : cmdBuffers) {
		renderBuffers_.emplace_back();
		renderBuffers_.back().commandBuffer = std::move(cmdBuffer);
		renderBuffers_.back().framebuffer.create(device(), swapChain.size(), dynamic);
	}
}

void SwapchainRenderer::init(RenderImpl builder)
{
	Framebuffer::ExtAttachments attachmentMap;
	std::vector<vk::ImageViewCreateInfo> viewInfos;

	renderImpl_ = std::move(builder);

	//attachments
	auto staticAttachmentID = 0u; //staticAttachments_ id
	auto attachmentMapID = 0u; //attachmentMap[] id
	auto attachInfoID = 0u; //info_.attachments id

	for(auto i = 0u; i < info_.attachments.size(); ++i) {
		if(attachmentMapID == info_.swapChainAttachment) attachmentMapID++;

		auto& ainfo = info_.attachments[attachInfoID];
		if(!ainfo.external && !ainfo.dynamic) {
			auto& statAttach = staticAttachments_[staticAttachmentID];
			statAttach.init(ainfo.createInfo.viewInfo);
			attachmentMap[attachmentMapID] = statAttach.vkImageView();

			attachmentMapID++;
			staticAttachmentID++;
		} else if(!ainfo.external && ainfo.dynamic) {
			viewInfos.push_back(ainfo.createInfo.viewInfo);
		} else if(ainfo.external) {
			attachmentMap[attachmentMapID] = ainfo.external;
			attachmentMapID++;
		}

		++attachInfoID;
	}

	// frameBufferAttachment resources
	for(std::size_t i(0); i < renderBuffers_.size(); i++) {
		attachmentMap[info_.swapChainAttachment] = swapChain().renderBuffers()[i].imageView;
		renderBuffers_[i].framebuffer.init(info_.renderPass, viewInfos, attachmentMap);
	}

	renderImpl_->init(*this);
}

void SwapchainRenderer::recreate()
{
	// TODO: reuse command buffers?
	renderBuffers_.clear();
	staticAttachments_.clear();

	create(*swapChain_, info_);
	init(std::move(renderImpl_));
}

void SwapchainRenderer::record(int id)
{
	if(id == -1) {
		for(std::size_t i(0); i < renderBuffers_.size(); ++i) record(i);
		return;
	}

	auto clearValues = renderImpl_->clearValues(id);
	auto width = swapChain().size().width;
	auto height = swapChain().size().height;

	auto& renderer = renderBuffers_[id];
	auto vkbuf = renderer.commandBuffer.vkHandle();
	vk::CommandBufferBeginInfo cmdBufInfo;

	vk::RenderPassBeginInfo beginInfo;
	beginInfo.renderPass = info_.renderPass;
	beginInfo.renderArea = {{0, 0}, {width, height}};
	beginInfo.clearValueCount = clearValues.size();
	beginInfo.pClearValues = clearValues.data();
	beginInfo.framebuffer = renderer.framebuffer;

	vk::beginCommandBuffer(vkbuf, cmdBufInfo);
	renderImpl_->beforeRender(vkbuf);
	vk::cmdBeginRenderPass(vkbuf, beginInfo, vk::SubpassContents::eInline);

	// Update dynamic viewport state
	vk::Viewport viewport;
	viewport.width = width;
	viewport.height = height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;
	vk::cmdSetViewport(vkbuf, 0, 1, viewport);

	// Update dynamic scissor state
	vk::Rect2D scissor;
	scissor.extent = {width, height};
	scissor.offset = {0, 0};
	vk::cmdSetScissor(vkbuf, 0, 1, scissor);

	RenderPassInstance ini(vkbuf, info_.renderPass, renderer.framebuffer);
	renderImpl_->build(id, ini);

	vk::cmdEndRenderPass(vkbuf);
	renderImpl_->afterRender(vkbuf);
	vk::endCommandBuffer(vkbuf);
}

std::unique_ptr<Work<void>> SwapchainRenderer::render(const Queue& present, const Queue* gfx)
{
	if(gfx == nullptr) gfx = device().queue(vk::QueueBits::graphics);
	if(!gfx) throw std::runtime_error("vpp::SwapchainRenderer::render: no graphics queue");

	vk::SemaphoreCreateInfo semaphoreCI;
	auto acquireComplete = Semaphore(device());
	auto renderComplete = Semaphore(device());

	unsigned int currentBuffer;
	swapChain().acquire(currentBuffer, acquireComplete);

	renderImpl_->frame(currentBuffer);

	auto& cmdBuf = renderBuffers_[currentBuffer].commandBuffer;
	auto additionals = renderImpl_->submit(currentBuffer);

	std::vector<vk::Semaphore> semaphores {acquireComplete};
	std::vector<vk::PipelineStageFlags> flags {vk::PipelineStageBits::colorAttachmentOutput};
	semaphores.reserve(additionals.size() + 1);
	flags.reserve(additionals.size() + 1);

	for(auto& sem : additionals) {
		semaphores.push_back(sem.first);
		flags.push_back(sem.second);
	}

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = semaphores.size();
	submitInfo.pWaitSemaphores = semaphores.data();
	submitInfo.pWaitDstStageMask = flags.data();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderComplete.vkHandle();

	CommandExecutionState execState;
	device().submitManager().add(*gfx, {cmdBuf}, submitInfo, &execState);
	device().submitManager().submit(*gfx);

	swapChain().present(present, currentBuffer, renderComplete);

	class WorkImpl : public Work<void> {
	public:
		Semaphore acquire_;
		Semaphore render_;
		CommandExecutionState executionState_;
		WorkBase::State state_ = WorkBase::State::submitted;

		~WorkImpl()
		{
			try {
				finish();
			} catch(const std::exception& err) {
				warn("vpp::SwapchainRenderer::render: Work::finish: ", err.what());
			}
		}

		virtual void finish() override
		{
			wait();
			acquire_ = {};
			render_ = {};
			state_ = WorkBase::State::finished;
		}

		virtual WorkBase::State state() override { return state_; }
		virtual void submit() override {}

		virtual void wait() override
		{
			executionState_.wait();
			state_ = WorkBase::State::executed;
		}
	};

	auto work = std::make_unique<WorkImpl>();
	work->acquire_ = std::move(acquireComplete);
	work->render_ = std::move(renderComplete);
	work->executionState_ = std::move(execState);

	return std::move(work);
}

void SwapchainRenderer::renderBlock(const Queue& present, const Queue* gfx)
{
	if(gfx == nullptr) gfx = device().queue(vk::QueueBits::graphics);
	if(!gfx) throw std::runtime_error("vpp::SwapchainRenderer::renderBlock: no graphics queue");

	auto acquireComplete = Semaphore(device());
	auto renderComplete = Semaphore(device());

	unsigned int currentBuffer;
	swapChain().acquire(currentBuffer, acquireComplete);

	renderImpl_->frame(currentBuffer);

	auto& cmdBuf = renderBuffers_[currentBuffer].commandBuffer;
	auto additionals = renderImpl_->submit(currentBuffer);

	std::vector<vk::Semaphore> semaphores {acquireComplete};
	std::vector<vk::PipelineStageFlags> flags {vk::PipelineStageBits::colorAttachmentOutput};
	semaphores.reserve(additionals.size() + 1);
	flags.reserve(additionals.size() + 1);

	for(auto& sem : additionals) {
		semaphores.push_back(sem.first);
		flags.push_back(sem.second);
	}

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = semaphores.size();
	submitInfo.pWaitSemaphores = semaphores.data();
	submitInfo.pWaitDstStageMask = flags.data();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderComplete.vkHandle();

	CommandExecutionState execState;
	device().submitManager().add(*gfx, {cmdBuf}, submitInfo, &execState);
	device().submitManager().submit(*gfx);

	swapChain().present(present, currentBuffer, renderComplete);

	execState.wait();
}

} // namespace vpp
