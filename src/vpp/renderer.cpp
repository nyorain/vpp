#include <vpp/renderer.hpp>
#include <vpp/call.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>

namespace vpp
{

Renderer::Renderer(const SwapChain& swapChain) : swapChain_(&swapChain)
{
	initCommandPool();
	initRenderPass();
	initRenderers();
}

Renderer::~Renderer()
{
	destroy();
}

void Renderer::destroy()
{
	destroyRenderers();
	destroyRenderPass();
	destroyCommandPool();
}

void Renderer::destroyRenderers()
{
	std::vector<vk::CommandBuffer> cmdBuffers;
	cmdBuffers.reserve(frameRenderers_.size());

	for(auto& renderer : frameRenderers_)
	{
		vk::destroyFramebuffer(vkDevice(), renderer.frameBuffer, nullptr);
		cmdBuffers.push_back(renderer.commandBuffer);
	}

	if(!cmdBuffers.empty())
	{
		vk::freeCommandBuffers(vkDevice(), vkCommandPool(), cmdBuffers);
	}

	frameRenderers_.clear();
}

void Renderer::destroyRenderPass()
{
	if(vkRenderPass()) vk::destroyRenderPass(vkDevice(), vkRenderPass(), nullptr);
}

void Renderer::destroyCommandPool()
{
	if(vkCommandPool()) vk::destroyCommandPool(vkDevice(), vkCommandPool(), nullptr);
}

void Renderer::reset(const SwapChain& swapChain, bool complete)
{
	swapChain_ = &swapChain;

	destroyRenderers();

	//re-init
	if(complete)
	{
		destroyRenderPass();
		initRenderPass();
	}
	initRenderers();
}

void Renderer::initCommandPool()
{
	auto queues = swapChain().surface().supportedGraphicalQueueFamilies(vkPhysicalDevice());
	if(queues.empty()) throw std::runtime_error("Renderer: no supported queue");

	//commandPool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex(0);
	cmdPoolInfo.flags(vk::CommandPoolCreateFlagBits::ResetCommandBuffer);

	VPP_CALL(vk::createCommandPool(vkDevice(), &cmdPoolInfo, nullptr, &commandPool_));
}

void Renderer::initRenderPass()
{
	vk::AttachmentDescription attachments[1] {};
	attachments[0].format(swapChain().format());
	attachments[0].samples(vk::SampleCountFlagBits::e1);
	attachments[0].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[0].storeOp(vk::AttachmentStoreOp::Store);
	attachments[0].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[0].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[0].initialLayout(vk::ImageLayout::ColorAttachmentOptimal);
	attachments[0].finalLayout(vk::ImageLayout::ColorAttachmentOptimal);

	vk::AttachmentReference colorReference;
	colorReference.attachment(0);
	colorReference.layout(vk::ImageLayout::ColorAttachmentOptimal);

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint(vk::PipelineBindPoint::Graphics);
	subpass.flags(vk::SubpassDescriptionFlags());
	subpass.inputAttachmentCount(0);
	subpass.pInputAttachments(nullptr);
	subpass.colorAttachmentCount(1);
	subpass.pColorAttachments(&colorReference);
	subpass.pResolveAttachments(nullptr);
	subpass.pDepthStencilAttachment(nullptr);
	subpass.preserveAttachmentCount(0);
	subpass.pPreserveAttachments(nullptr);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount(1);
	renderPassInfo.pAttachments(attachments);
	renderPassInfo.subpassCount(1);
	renderPassInfo.pSubpasses(&subpass);
	renderPassInfo.dependencyCount(0);
	renderPassInfo.pDependencies(nullptr);

	VPP_CALL(vk::createRenderPass(vkDevice(), &renderPassInfo, nullptr, &renderPass_));
}

void Renderer::initRenderers()
{
	frameRenderers_.resize(swapChain().buffers().size());

	//cmdBuffers
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool(vkCommandPool());
	allocInfo.level(vk::CommandBufferLevel::Primary);
	allocInfo.commandBufferCount(frameRenderers_.size());

	std::vector<vk::CommandBuffer> cmdBuffers(frameRenderers_.size());
	VPP_CALL(vk::allocateCommandBuffers(vkDevice(), allocInfo, cmdBuffers));

	//frameBuffer
	vk::ImageView attachments[1] {};

	vk::FramebufferCreateInfo createInfo;
	createInfo.renderPass(vkRenderPass());
	createInfo.attachmentCount(1);
	createInfo.pAttachments(attachments);
	createInfo.width(swapChain().extent().width());
	createInfo.height(swapChain().extent().height());
	createInfo.layers(1);

	for(std::size_t i(0); i < frameRenderers_.size(); ++i)
	{
        attachments[0] = swapChain().buffers()[i].imageView;

		vk::Framebuffer frameBuffer;
        VPP_CALL(vk::createFramebuffer(vkDevice(), &createInfo, nullptr, &frameBuffer));

		frameRenderers_[i].commandBuffer = cmdBuffers[i];
		frameRenderers_[i].frameBuffer = frameBuffer;
		frameRenderers_[i].id = i;

		buildCommandBuffer(frameRenderers_[i]);
	}
}

void Renderer::buildCommandBuffer(const FrameRenderer& renderer)
{
	vk::CommandBufferBeginInfo cmdBufInfo;

	vk::ClearValue clearValues[1];
	clearValues[0].vkHandle().color = {{0.55f, 0.025f, 0.025f, 1.0f}};

	//WHY
	auto width = swapChain().extent().width();
	auto height = swapChain().extent().height();

	vk::RenderPassBeginInfo beginInfo;
	beginInfo.renderPass(vkRenderPass());
	beginInfo.renderArea({{0, 0}, {width + 1, height + 1}});
	beginInfo.clearValueCount(1);
	beginInfo.pClearValues(clearValues);
	beginInfo.framebuffer(renderer.frameBuffer);

	VPP_CALL(vk::beginCommandBuffer(renderer.commandBuffer, cmdBufInfo));
	vk::cmdBeginRenderPass(renderer.commandBuffer, beginInfo, vk::SubpassContents::Inline);

	//Update dynamic viewport state
	vk::Viewport viewport;
	viewport.width(width);
	viewport.height(height);
	viewport.minDepth(0.0f);
	viewport.maxDepth(1.0f);
	vk::cmdSetViewport(renderer.commandBuffer, 0, 1, &viewport);

	//Update dynamic scissor state
	vk::Rect2D scissor;
	scissor.extent({width, height});
	scissor.offset({0, 0});
	vk::cmdSetScissor(renderer.commandBuffer, 0, 1, &scissor);

	buildRenderer(renderer.commandBuffer);
	vkCmdEndRenderPass(renderer.commandBuffer);

	vk::ImageMemoryBarrier prePresentBarrier;
	prePresentBarrier.srcAccessMask(vk::AccessFlagBits::ColorAttachmentWrite);
	prePresentBarrier.dstAccessMask(vk::AccessFlags());
	prePresentBarrier.oldLayout(vk::ImageLayout::ColorAttachmentOptimal);
	prePresentBarrier.newLayout(vk::ImageLayout::PresentSrcKHR);
	prePresentBarrier.srcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
	prePresentBarrier.dstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
	prePresentBarrier.subresourceRange({vk::ImageAspectFlagBits::Color, 0, 1, 0, 1});
	prePresentBarrier.image(swapChain().buffers()[renderer.id].image);

	vk::cmdPipelineBarrier(renderer.commandBuffer, vk::PipelineStageFlagBits::AllCommands,
		vk::PipelineStageFlagBits::TopOfPipe, vk::DependencyFlags(), 0,
		nullptr, 0, nullptr, 0, nullptr);

	VPP_CALL(vk::endCommandBuffer(renderer.commandBuffer));
}

void Renderer::buildRenderer(vk::CommandBuffer buffer)
{
}

void Renderer::render(vk::Queue queue)
{
	vk::Semaphore presentComplete;
    vk::SemaphoreCreateInfo semaphoreCI;
	VPP_CALL(vk::createSemaphore(vkDevice(), &semaphoreCI, nullptr, &presentComplete));

    auto currentBuffer = swapChain().acquireNextImage(presentComplete);

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount(1);
	submitInfo.pWaitSemaphores(&presentComplete);
	submitInfo.commandBufferCount(1);
	submitInfo.pCommandBuffers(&frameRenderers_[currentBuffer].commandBuffer);

	VPP_CALL(vk::queueSubmit(queue, 1, &submitInfo, 0));
    swapChain().present(queue, currentBuffer);

    vk::destroySemaphore(vkDevice(), presentComplete, nullptr);
    VPP_CALL(vk::queueWaitIdle(queue));
}

vk::Instance Renderer::vkInstance() const { return swapChain().vkInstance(); }
vk::PhysicalDevice Renderer::vkPhysicalDevice() const { return swapChain().vkPhysicalDevice(); }
vk::Device Renderer::vkDevice() const { return swapChain().vkDevice(); }

}
