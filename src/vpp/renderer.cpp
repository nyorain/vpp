#include <vpp/renderer.hpp>
#include <vpp/defs.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>
#include <vpp/queue.hpp>
#include <vpp/provider.hpp>
#include <vpp/submit.hpp>

#include <stdexcept>

namespace vpp
{

//Builder
std::vector<vk::SubmitInfo>
RendererBuilder::submit(vk::CommandBuffer cmdBuf, vk::Semaphore wait, vk::Semaphore complete)
{
	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &wait;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuf;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &complete;

	return {submitInfo};
}

//SwapChainRenderer
SwapChainRenderer::SwapChainRenderer(const SwapChain& sc, const CreateInfo& inf, RenderImpl bld)
{
	create(sc, inf);
	init(std::move(bld));
}

SwapChainRenderer::SwapChainRenderer(SwapChainRenderer&& other) noexcept
{
	swap(*this, other);
}

SwapChainRenderer& SwapChainRenderer::operator=(SwapChainRenderer&& other) noexcept
{
	this->~SwapChainRenderer();
	swap(*this, other);
	return *this;
}

SwapChainRenderer::~SwapChainRenderer()
{
	destroyRenderBuffers();
	staticAttachments_.clear();

	swapChain_ = nullptr;
}

void swap(SwapChainRenderer& a, SwapChainRenderer& b) noexcept
{
	using std::swap;

	swap(a.swapChain_, b.swapChain_);
	swap(a.renderBuffers_, b.renderBuffers_);
	swap(a.staticAttachments_, b.staticAttachments_);
	swap(a.renderImpl_, b.renderImpl_);
	swap(a.info_, b.info_);
}

void SwapChainRenderer::create(const SwapChain& swapChain, const CreateInfo& info)
{
	if(info.renderPass == nullptr)
	{
		throw std::runtime_error("SwapChainRenderer: nullptr renderPass");
	}

	swapChain_ = &swapChain;
	info_ = info;

	//command pool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex = info.queueFamily;
	cmdPoolInfo.flags = vk::CommandPoolCreateBits::resetCommandBuffer; //XXX: needed?

	//attachments
	//TODO: more efficient: reserve/resize prediction?
	std::vector<ViewableImage::CreateInfo> dynamic;
	Framebuffer::ExtAttachments ext;
	auto size = swapChain.size();
	auto i = 0u;

	for(auto& attachInfo : info.attachments)
	{
		if(i == info.swapChainAttachment) ++i;
		if(attachInfo.external)
		{
			ext[i] = attachInfo.external;
		}
		else if(attachInfo.dynamic)
		{
			auto imgInfo = attachInfo.createInfo.imgInfo;
			imgInfo.extent = {size.width, size.height, 1};
			dynamic.push_back(attachInfo.createInfo);
		}
		else
		{
			auto imgInfo = attachInfo.createInfo.imgInfo;
			imgInfo.extent = {size.width, size.height, 1};
			staticAttachments_.emplace_back();
			staticAttachments_.back().create(device(), imgInfo, attachInfo.createInfo.memoryFlags);
		}

		++i;
	}

	//RenderBuffers
	//CommandBuffers
	renderBuffers_.reserve(swapChain.renderBuffers().size());

	auto qFam = info.queueFamily;
	auto cmdBuffers = device().commandProvider().get(qFam, renderBuffers_.size());

	//frame buffers
	for(auto& cmdBuffer : cmdBuffers)
	{
		renderBuffers_.emplace_back();
		renderBuffers_.back().commandBuffer = std::move(cmdBuffer);
		renderBuffers_.back().framebuffer.create(device(), swapChain.size(), dynamic);
	}
}

void SwapChainRenderer::init(RenderImpl builder)
{
	Framebuffer::ExtAttachments attachmentMap;
	std::vector<vk::ImageViewCreateInfo> viewInfos;

	renderImpl_ = std::move(builder);

	//attachments
	auto staticAttachmentID = 0u;
	auto attachmentMapID = 0u;
	auto attachInfoID = 0u;

	for(auto i = 0u; i < info_.attachments.size(); ++i)
	{
		if(i == info_.swapChainAttachment) continue;

		auto& ainfo = info_.attachments[attachInfoID];
		if(!ainfo.external && !ainfo.dynamic)
		{
			auto& statAttach = staticAttachments_[staticAttachmentID];
			statAttach.init(ainfo.createInfo.viewInfo);
			attachmentMap[attachmentMapID] = statAttach.vkImageView();

			attachmentMapID++;
			staticAttachmentID++;
		}
		else if(!ainfo.external && ainfo.dynamic)
		{
			viewInfos.push_back(ainfo.createInfo.viewInfo);
		}
		else if(ainfo.external)
		{
			attachmentMap[attachmentMapID] = ainfo.external;
			attachmentMapID++;
		}

		++attachInfoID;
	}

	//frameBufferAttachment resources
	for(std::size_t i(0); i < renderBuffers_.size(); i++)
	{
		attachmentMap[info_.swapChainAttachment] = swapChain().renderBuffers()[i].imageView;
		renderBuffers_[i].framebuffer.init(info_.renderPass, viewInfos, attachmentMap);
	}

	builder->init(*this);
}

void SwapChainRenderer::destroyRenderBuffers()
{
	std::vector<vk::CommandBuffer> cmdBuffers;
	cmdBuffers.reserve(renderBuffers_.size());

	for(auto& renderer : renderBuffers_)
	{
		auto vkbuf = renderer.commandBuffer.vkCommandBuffer();
		if(vkbuf) cmdBuffers.push_back(vkbuf);
	}

	if(!cmdBuffers.empty())
	{
		auto vkpool = renderBuffers_[0].commandBuffer.commandPool().vkCommandPool();
		vk::freeCommandBuffers(vkDevice(), vkpool, cmdBuffers);
	}

	renderBuffers_.clear();
}

void SwapChainRenderer::record(int id)
{
	if(id == -1)
	{
		for(std::size_t i(0); i < renderBuffers_.size(); ++i) record(id);
		return;
	}

	auto clearValues = renderImpl_->clearValues(id);
	auto width = swapChain().size().width;
	auto height = swapChain().size().height;

	vk::ImageMemoryBarrier barrier;
	barrier.srcAccessMask = vk::AccessBits::colorAttachmentWrite;
	barrier.dstAccessMask = vk::AccessFlags();
	barrier.oldLayout = vk::ImageLayout::presentSrcKHR;
	barrier.newLayout = vk::ImageLayout::colorAttachmentOptimal;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange = {vk::ImageAspectBits::color, 0, 1, 0, 1};
	barrier.image = swapChain().renderBuffers()[id].image;

	auto& renderer = renderBuffers_[id];
	auto vkbuf = renderer.commandBuffer.vkCommandBuffer();
	vk::CommandBufferBeginInfo cmdBufInfo;

	vk::RenderPassBeginInfo beginInfo;
	beginInfo.renderPass = info_.renderPass;
	beginInfo.renderArea = {{0, 0}, {width, height}};
	beginInfo.clearValueCount = clearValues.size();
	beginInfo.pClearValues = clearValues.data();
	beginInfo.framebuffer = renderer.framebuffer.vkFramebuffer();

	vk::beginCommandBuffer(vkbuf, cmdBufInfo);

	//present to attachment layout
	vk::cmdPipelineBarrier(vkbuf, vk::PipelineStageBits::allCommands,
		vk::PipelineStageBits::topOfPipe, vk::DependencyFlags(), {}, {}, {barrier});

	renderImpl_->beforeRender(vkbuf);

	vk::cmdBeginRenderPass(vkbuf, beginInfo, vk::SubpassContents::eInline);

	//Update dynamic viewport state
	vk::Viewport viewport;
	viewport.width = width;
	viewport.height = height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;
	vk::cmdSetViewport(vkbuf, 0, 1, viewport);

	//Update dynamic scissor state
	vk::Rect2D scissor;
	scissor.extent = {width, height};
	scissor.offset = {0, 0};
	vk::cmdSetScissor(vkbuf, 0, 1, scissor);

	RenderPassInstance ini(vkbuf, info_.renderPass, renderer.framebuffer.vkFramebuffer());
	renderImpl_->build(*this, id, ini);

	vk::cmdEndRenderPass(vkbuf);

	renderImpl_->afterRender(vkbuf);

	barrier.oldLayout = vk::ImageLayout::colorAttachmentOptimal;
	barrier.newLayout = vk::ImageLayout::presentSrcKHR;

	//attachment to present layout
	vk::cmdPipelineBarrier(vkbuf, vk::PipelineStageBits::allCommands,
		vk::PipelineStageBits::topOfPipe, vk::DependencyFlags(), {}, {}, {barrier});

	vk::endCommandBuffer(vkbuf);
}

std::unique_ptr<Work<void>> SwapChainRenderer::render(Queue* present, Queue* gfx)
{
    vk::SemaphoreCreateInfo semaphoreCI;
	auto acquireComplete = vk::createSemaphore(vkDevice(), semaphoreCI);
	auto renderComplete = vk::createSemaphore(vkDevice(), semaphoreCI);

    auto currentBuffer = swapChain().acquireNextImage(acquireComplete);

	auto& cmdBuf = renderBuffers_[currentBuffer].commandBuffer;
	auto submits = renderImpl_->submit(cmdBuf, acquireComplete, renderComplete);

	auto execState = device().submitManager().add(*gfx, submits[0]);

	//TODO: which kind of submit makes sence here? submit ALL queued commands?
	//execState.submit();
	device().submitManager().submit();

	//TODO: some kind of queue lock? must be synchronized.
	//need some submitmanager improvements as general queue sync manager.
    swapChain().present(*present, currentBuffer, renderComplete);

	class WorkImpl : public Work<void>
	{
	public:
		vk::Semaphore acquire_;
		vk::Semaphore render_;
		CommandExecutionState executionState_;
		WorkBase::State state_ = WorkBase::State::submitted;

		WorkImpl(vk::Semaphore acquire, vk::Semaphore render, CommandExecutionState state)
			: acquire_(acquire), render_(render), executionState_(std::move(state)) {}

		virtual void finish() override
		{
			wait();
			auto& dev = executionState_.device();

    		if(acquire_) vk::destroySemaphore(dev, acquire_, nullptr);
    		if(render_) vk::destroySemaphore(dev, render_, nullptr);

			acquire_ = {};
			render_ = {};

			state_ = WorkBase::State::finished;
		}
		virtual WorkBase::State state() override
		{
			return state_;
		}
		virtual void submit() override
		{
			std::cerr << "vpp::SwapChainRenderer::WorkImpl::submit, was already submitted\n";
		}
		virtual void wait() override
		{
			executionState_.wait();
			state_ = WorkBase::State::executed;
		}
	};

	return std::make_unique<WorkImpl>(acquireComplete, renderComplete, std::move(execState));
}

}
