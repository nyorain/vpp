#include <vpp/renderer.hpp>
#include <vpp/vk.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>
#include <vpp/queue.hpp>

#include <stdexcept>

namespace vpp
{

//SwapChainRenderer
SwapChainRenderer::SwapChainRenderer(const SwapChain& swapChain, RendererBuilder& builder,
	const CreateInfo& info)
{
	initMemoryLess(swapChain, info);
	initMemoryResources(builder);
}

SwapChainRenderer::SwapChainRenderer(SwapChainRenderer&& other) noexcept
{
	swap(*this, other);
}

SwapChainRenderer& SwapChainRenderer::operator=(SwapChainRenderer&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

SwapChainRenderer::~SwapChainRenderer()
{
	destroy();
}

void swap(SwapChainRenderer& a, SwapChainRenderer& b) noexcept
{
	using std::swap;

	swap(a.device_, b.device_);
	swap(a.swapChain_, b.swapChain_);
	swap(a.info_, b.info_);
	swap(a.renderBuffers_, b.renderBuffers_);
	swap(a.staticAttachments_, b.staticAttachments_);
	swap(a.commandPool_, b.commandPool_);
}

void SwapChainRenderer::destroy()
{
	destroyRenderBuffers();
	staticAttachments_.clear();

	if(vkCommandPool()) vk::destroyCommandPool(vkDevice(), vkCommandPool(), nullptr);

	//reset
	commandPool_ = {};
	swapChain_ = nullptr;
	info_ = {};

	Resource::destroy();
}

void SwapChainRenderer::initMemoryLess(const SwapChain& swapChain, const CreateInfo& info)
{
	if(info.renderPass == nullptr)
	{
		throw std::runtime_error("SwapChainRenderer: nullptr renderPass");
	}

	Resource::init(swapChain.device());
	swapChain_ = &swapChain;
	info_ = info;

	//command pool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex = info_.queue->family();
	cmdPoolInfo.flags = vk::CommandPoolCreateBits::resetCommandBuffer;

	commandPool_ = vk::createCommandPool(vkDevice(), cmdPoolInfo);

	//static attachments
	auto size = swapChain.size();
	staticAttachments_.reserve(info_.staticAttachments.size());
	for(auto& attachInfo : info_.staticAttachments)
	{
		attachInfo.imageInfo.extent = {size.width, size.height, 1};
		staticAttachments_.emplace_back();
		staticAttachments_.back().initMemoryLess(device(), attachInfo.imageInfo);
	}

	//RenderBuffers
	//CommandBuffers
	renderBuffers_.reserve(swapChain.buffers().size());

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = vkCommandPool();
	allocInfo.level = vk::CommandBufferLevel::primary;
	allocInfo.commandBufferCount = swapChain.buffers().size();

	std::vector<vk::CommandBuffer> cmdBuffers(swapChain.buffers().size());
	vk::allocateCommandBuffers(vkDevice(), allocInfo, *cmdBuffers.data());

	//frame buffers
	Framebuffer::CreateInfo fbInfo {vkRenderPass(), swapChain.size()};
	//fbInfo.attachments = info_.dynamicAttachments;

	for(auto& cmdBuffer : cmdBuffers)
	{
		renderBuffers_.emplace_back();
		renderBuffers_.back().commandBuffer = cmdBuffer;
		renderBuffers_.back().framebuffer.initMemoryLess(device(), fbInfo);
	}
}

void SwapChainRenderer::initMemoryResources(RendererBuilder& builder)
{
	//const std::size_t dynAttachSize = info().dynamicAttachments.size() + 1;
	std::map<unsigned int, vk::ImageView> attachmentMap;

	//static attachments
	for(std::size_t i(0); i < staticAttachments_.size(); i++)
	{
		staticAttachments_[i].image().assureMemory();
		staticAttachments_[i].initMemoryResources(info_.staticAttachments[i].viewInfo);
		attachmentMap[i + 1] = staticAttachments_[i].vkImageView();
	}

	//frameBufferAttachment resources
	for(std::size_t i(0); i < renderBuffers_.size(); i++)
	{
		attachmentMap[0] = swapChain().buffers()[i].imageView;
		renderBuffers_[i].framebuffer.initMemoryResources(attachmentMap);
	}

	builder.init(*this);

	//record command buffers
	buildCommandBuffers(builder);
}

void SwapChainRenderer::destroyRenderBuffers()
{
	std::vector<vk::CommandBuffer> cmdBuffers;
	cmdBuffers.reserve(renderBuffers_.size());

	for(auto& renderer : renderBuffers_)
	{
		if(renderer.commandBuffer)
			cmdBuffers.push_back(renderer.commandBuffer);
	}

	if(!cmdBuffers.empty())
	{
		vk::freeCommandBuffers(vkDevice(), vkCommandPool(), cmdBuffers);
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
		vk::CommandBufferBeginInfo cmdBufInfo;

		vk::RenderPassBeginInfo beginInfo;
		beginInfo.renderPass = vkRenderPass();
		beginInfo.renderArea = {{0, 0}, {width, height}};
		beginInfo.clearValueCount = clearValues.size();
		beginInfo.pClearValues = clearValues.data();
		beginInfo.framebuffer = renderer.framebuffer.vkFramebuffer();

		vk::beginCommandBuffer(renderer.commandBuffer, cmdBufInfo);

		builder.beforeRender(renderer.commandBuffer);

		vk::cmdBeginRenderPass(renderer.commandBuffer, beginInfo, vk::SubpassContents::eInline);

		//Update dynamic viewport state
		vk::Viewport viewport;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		vk::cmdSetViewport(renderer.commandBuffer, 0, 1, viewport);

		//Update dynamic scissor state
		vk::Rect2D scissor;
		scissor.extent = {width, height};
		scissor.offset = {0, 0};
		vk::cmdSetScissor(renderer.commandBuffer, 0, 1, scissor);

		RenderPassInstance ini(renderer.commandBuffer, renderPass(),
			renderer.framebuffer.vkFramebuffer());
		builder.build(ini);

		vkCmdEndRenderPass(renderer.commandBuffer);

		builder.afterRender(renderer.commandBuffer);

		vk::ImageMemoryBarrier prePresentBarrier;
		prePresentBarrier.srcAccessMask = vk::AccessBits::colorAttachmentWrite;
		prePresentBarrier.dstAccessMask = vk::AccessFlags();
		prePresentBarrier.oldLayout = vk::ImageLayout::colorAttachmentOptimal;
		prePresentBarrier.newLayout = vk::ImageLayout::presentSrcKHR;
		prePresentBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		prePresentBarrier.subresourceRange = {vk::ImageAspectBits::color, 0, 1, 0, 1};
		prePresentBarrier.image = swapChain().buffers()[i].image;

		vk::cmdPipelineBarrier(renderer.commandBuffer, vk::PipelineStageBits::allCommands,
			vk::PipelineStageBits::topOfPipe, vk::DependencyFlags(), {}, {}, {prePresentBarrier});

		vk::endCommandBuffer(renderer.commandBuffer);
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
	submitInfo.pCommandBuffers = &renderBuffers_[currentBuffer].commandBuffer;

	vk::queueSubmit(info_.queue->vkQueue(), 1, submitInfo, 0);
    swapChain().present(info_.queue->vkQueue(), currentBuffer);

    vk::destroySemaphore(vkDevice(), presentComplete, nullptr);
	vk::queueWaitIdle(info_.queue->vkQueue());
}

}
