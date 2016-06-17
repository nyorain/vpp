#include <vpp/renderer.hpp>
#include <vpp/vk.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>
#include <vpp/queue.hpp>
#include <vpp/provider.hpp>

#include <stdexcept>

namespace vpp
{

//SwapChainRenderer
SwapChainRenderer::SwapChainRenderer(const SwapChain& sc, const CreateInfo& inf, RendererBuilder& bld)
{
	create(sc, inf);
	init(bld);
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
	cmdPoolInfo.queueFamilyIndex = info.queue->family();
	cmdPoolInfo.flags = vk::CommandPoolCreateBits::resetCommandBuffer;

	//attachments
	//TODO: more efficient: reserve/resize prediction?
	std::vector<vk::ImageCreateInfo> dynamic;
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
			dynamic.push_back(attachInfo.info.first);
		}
		else
		{
			auto imgInfo = attachInfo.info.first;
			imgInfo.extent = {size.width, size.height, 1};
			staticAttachments_.emplace_back();
			staticAttachments_.back().create(device(), imgInfo);
		}

		++i;
	}

	//RenderBuffers
	//CommandBuffers
	renderBuffers_.reserve(swapChain.renderBuffers().size());

	auto qFam = info.queue->family();
	auto cmdBuffers = device().commandProvider().get(qFam, renderBuffers_.size());

	//frame buffers
	for(auto& cmdBuffer : cmdBuffers)
	{
		renderBuffers_.emplace_back();
		renderBuffers_.back().commandBuffer = std::move(cmdBuffer);
		renderBuffers_.back().framebuffer.create(device(), swapChain.size(), dynamic);
	}
}

void SwapChainRenderer::init(RendererBuilder& builder)
{
	Framebuffer::ExtAttachments attachmentMap;
	std::vector<vk::ImageViewCreateInfo> viewInfos;

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
			statAttach.init(ainfo.info.second);
			attachmentMap[attachmentMapID] = statAttach.vkImageView();

			attachmentMapID++;
			staticAttachmentID++;
		}
		else if(!ainfo.external && ainfo.dynamic)
		{
			viewInfos.push_back(ainfo.info.second);
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

	builder.init(*this);
	buildCommandBuffers(builder);
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

void SwapChainRenderer::buildCommandBuffers(RendererBuilder& builder)
{
	auto clearValues = builder.clearValues();
	auto width = swapChain().size().width;
	auto height = swapChain().size().height;

	for(std::size_t i(0); i < renderBuffers_.size(); ++i)
	{
		auto& renderer = renderBuffers_[i];
		auto vkbuf = renderer.commandBuffer.vkCommandBuffer();
		vk::CommandBufferBeginInfo cmdBufInfo;

		vk::RenderPassBeginInfo beginInfo;
		beginInfo.renderPass = info_.renderPass;
		beginInfo.renderArea = {{0, 0}, {width, height}};
		beginInfo.clearValueCount = clearValues.size();
		beginInfo.pClearValues = clearValues.data();
		beginInfo.framebuffer = renderer.framebuffer.vkFramebuffer();

		vk::beginCommandBuffer(vkbuf, cmdBufInfo);

		builder.beforeRender(vkbuf);

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
		builder.build(ini);

		vk::cmdEndRenderPass(vkbuf);

		builder.afterRender(vkbuf);

		vk::ImageMemoryBarrier prePresentBarrier;
		prePresentBarrier.srcAccessMask = vk::AccessBits::colorAttachmentWrite;
		prePresentBarrier.dstAccessMask = vk::AccessFlags();
		prePresentBarrier.oldLayout = vk::ImageLayout::colorAttachmentOptimal;
		prePresentBarrier.newLayout = vk::ImageLayout::presentSrcKHR;
		prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.subresourceRange = {vk::ImageAspectBits::color, 0, 1, 0, 1};
		prePresentBarrier.image = swapChain().renderBuffers()[i].image;

		vk::cmdPipelineBarrier(vkbuf, vk::PipelineStageBits::allCommands,
			vk::PipelineStageBits::topOfPipe, vk::DependencyFlags(), {}, {}, {prePresentBarrier});

		vk::endCommandBuffer(vkbuf);
	}
}

void SwapChainRenderer::render()
{
    vk::SemaphoreCreateInfo semaphoreCI;
	auto presentComplete = vk::createSemaphore(vkDevice(), semaphoreCI);

    auto currentBuffer = swapChain().acquireNextImage(presentComplete);

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentComplete;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &renderBuffers_[currentBuffer].commandBuffer.vkCommandBuffer();

	vk::queueSubmit(info_.queue->vkQueue(), 1, submitInfo, 0);
    swapChain().present(info_.queue->vkQueue(), currentBuffer);

    vk::destroySemaphore(vkDevice(), presentComplete, nullptr);
	vk::queueWaitIdle(info_.queue->vkQueue());
}

}
