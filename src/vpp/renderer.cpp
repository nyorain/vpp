#include <vpp/renderer.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>

#include <stdexcept>

namespace vpp
{

//Renderpass
RenderPass::RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info)
{
	init(dev, info);
}

RenderPass::RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info)
{
	Resource::init(dev);
	renderPass_ = pass;

	initInfos(info);
}

RenderPass::~RenderPass()
{
	destroy();
}

RenderPass::RenderPass(RenderPass&& other) noexcept
{
	this->swap(other);
}

RenderPass& RenderPass::operator=(RenderPass&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

void RenderPass::swap(RenderPass& other) noexcept
{
	using std::swap;

	swap(renderPass_, other.renderPass_);
	swap(attachments_, other.attachments_);
	swap(subpasses_, other.subpasses_);
	swap(dependencies_, other.dependencies_);
	swap(device_, other.device_);
}

void RenderPass::init(const Device& dev, const vk::RenderPassCreateInfo& info)
{
	Resource::init(dev);
	vk::createRenderPass(vkDevice(), &info, nullptr, &renderPass_);

	initInfos(info);
}

void RenderPass::destroy()
{
	if(vkRenderPass()) vk::destroyRenderPass(vkDevice(), vkRenderPass(), nullptr);

	renderPass_ = {};
	attachments_.clear();
	subpasses_.clear();
	dependencies_.clear();

	Resource::destroy();
}

void RenderPass::initInfos(const vk::RenderPassCreateInfo& info)
{
	attachments_.reserve(info.attachmentCount());
	for(std::size_t i(0); i < info.attachmentCount(); ++i)
		attachments_.push_back(info.pAttachments()[i]);

	subpasses_.reserve(info.pSubpasses());
	for(std::size_t i(0); i < info.subpassCount(); ++i)
		subpasses_.push_back(info.pSubpasses()[i]);

	dependencies_.reserve(info.pDependencies());
	for(std::size_t i(0); i < info.dependencyCount(); ++i)
		dependencies_.push_back(info.pDependencies()[i]);

}

//XXX: could make this RAII wrapper for render pass instances later on
//RenderInstance
RenderPassInstance::RenderPassInstance(vk::CommandBuffer cmdbuf, vk::RenderPass pass,
	vk::Framebuffer fbuf) : commandBuffer_(cmdbuf), renderPass_(pass), framebuffer_(fbuf)
{
}

RenderPassInstance::~RenderPassInstance()
{
}

//SwapChainRenderer
SwapChainRenderer::SwapChainRenderer(const SwapChain& swapChain, RendererBuilder& builder)
{
	init(swapChain, builder);
}

SwapChainRenderer::~SwapChainRenderer()
{
	destroy();
}

void SwapChainRenderer::init(const SwapChain& swapChain, RendererBuilder& builder)
{
	Resource::init(swapChain.device());
	swapChain_ = &swapChain;
	builder_ = &builder;

	initCommandPool();
	initDepthStencil();
	initRenderPass();
	initRenderers();
}

void SwapChainRenderer::destroy()
{
	destroyRenderers();
	destroyRenderPass();
	destroyDepthStencil();
	destroyCommandPool();

	swapChain_ = nullptr;
	builder_ = nullptr;

	Resource::destroy();
}

void SwapChainRenderer::destroyRenderers()
{
	std::vector<vk::CommandBuffer> cmdBuffers;
	cmdBuffers.reserve(frameRenderers_.size());

	for(auto& renderer : frameRenderers_)
	{
		vk::destroyFramebuffer(vkDevice(), renderer.framebuffer, nullptr);
		cmdBuffers.push_back(renderer.commandBuffer);
	}

	if(!cmdBuffers.empty())
	{
		vk::freeCommandBuffers(vkDevice(), vkCommandPool(), cmdBuffers);
	}

	frameRenderers_.clear();
}

void SwapChainRenderer::destroyRenderPass()
{
	if(vkRenderPass()) vk::destroyRenderPass(vkDevice(), vkRenderPass(), nullptr);
}

void SwapChainRenderer::destroyCommandPool()
{
	if(vkCommandPool()) vk::destroyCommandPool(vkDevice(), vkCommandPool(), nullptr);
}

void SwapChainRenderer::destroyDepthStencil()
{
	if(depthStencil_.imageView) vk::destroyImageView(vkDevice(), depthStencil_.imageView, nullptr);

	depthStencil_.imageView = {};
	depthStencil_.image.reset();
}

void SwapChainRenderer::reset(const SwapChain& swapChain, bool complete)
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

void SwapChainRenderer::initCommandPool()
{
	//commandPool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex(0);
	cmdPoolInfo.flags(vk::CommandPoolCreateFlagBits::ResetCommandBuffer);

	vk::createCommandPool(vkDevice(), &cmdPoolInfo, nullptr, &commandPool_);
}

void SwapChainRenderer::initRenderPass()
{
	vk::AttachmentDescription attachments[2] {};

	//color from swapchain
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

	//depth from own depth stencil
	attachments[1].format(depthStencil_.format);
	attachments[1].samples(vk::SampleCountFlagBits::e1);
	attachments[1].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[1].storeOp(vk::AttachmentStoreOp::Store);
	attachments[1].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[1].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[1].initialLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);
	attachments[1].finalLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	vk::AttachmentReference depthReference;
	depthReference.attachment(1);
	depthReference.layout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	//only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint(vk::PipelineBindPoint::Graphics);
	subpass.flags(vk::SubpassDescriptionFlags());
	subpass.inputAttachmentCount(0);
	subpass.pInputAttachments(nullptr);
	subpass.colorAttachmentCount(1);
	subpass.pColorAttachments(&colorReference);
	subpass.pResolveAttachments(nullptr);
	subpass.pDepthStencilAttachment(&depthReference); //XXX
	subpass.preserveAttachmentCount(0);
	subpass.pPreserveAttachments(nullptr);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount(2); //XXX
	renderPassInfo.pAttachments(attachments);
	renderPassInfo.subpassCount(1);
	renderPassInfo.pSubpasses(&subpass);
	renderPassInfo.dependencyCount(0);
	renderPassInfo.pDependencies(nullptr);

	vk::createRenderPass(vkDevice(), &renderPassInfo, nullptr, &renderPass_);
}

void SwapChainRenderer::initRenderers()
{
	frameRenderers_.resize(swapChain().buffers().size());

	//cmdBuffers
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool(vkCommandPool());
	allocInfo.level(vk::CommandBufferLevel::Primary);
	allocInfo.commandBufferCount(frameRenderers_.size());

	std::vector<vk::CommandBuffer> cmdBuffers(frameRenderers_.size());
	vk::allocateCommandBuffers(vkDevice(), allocInfo, cmdBuffers);

	//framebuffer
	vk::ImageView attachments[2] {};

	//depth - every framebuffer uses the same image
	attachments[1] = depthStencil_.imageView;

	vk::FramebufferCreateInfo createInfo;
	createInfo.renderPass(vkRenderPass());
	createInfo.attachmentCount(2); //XXX
	createInfo.pAttachments(attachments);
	createInfo.width(swapChain().extent().width());
	createInfo.height(swapChain().extent().height());
	createInfo.layers(1);

	for(std::size_t i(0); i < frameRenderers_.size(); ++i)
	{
		//color - different for every framebuffer (swapcahin)
        attachments[0] = swapChain().buffers()[i].imageView;

		vk::Framebuffer framebuffer;
        vk::createFramebuffer(vkDevice(), &createInfo, nullptr, &framebuffer);

		frameRenderers_[i].commandBuffer = cmdBuffers[i];
		frameRenderers_[i].framebuffer = framebuffer;
		frameRenderers_[i].id = i;

		buildCommandBuffer(frameRenderers_[i]);
	}
}

void SwapChainRenderer::initDepthStencil()
{
	//query depth format
	std::vector<vk::Format> formats =
	{
		vk::Format::D32SfloatS8Uint,
		vk::Format::D32Sfloat,
		vk::Format::D24UnormS8Uint,
		vk::Format::D16UnormS8Uint,
		vk::Format::D16Unorm
	};

	bool found = 0;
	for (auto& format : formats)
	{
		vk::FormatProperties formatProps;
		vk::getPhysicalDeviceFormatProperties(vkPhysicalDevice(), format, &formatProps);
		if(formatProps.optimalTilingFeatures() & vk::FormatFeatureFlagBits::DepthStencilAttachment)
		{
			depthStencil_.format = format;
			found = 1;
			break;
		}
	}

	if(!found)
	{
		throw std::runtime_error("vpp::Renderer: found no valid depth stencil format\n");
	}

	//image
	vk::ImageCreateInfo info;
	info.imageType(vk::ImageType::e2D);
	info.format(depthStencil_.format);
	info.extent({swapChain().extent().width(), swapChain().extent().height(), 1});
	info.mipLevels(1);
	info.arrayLayers(1);
	info.samples(vk::SampleCountFlagBits::e1);
	info.tiling(vk::ImageTiling::Optimal);
	info.usage(vk::ImageUsageFlagBits::DepthStencilAttachment | vk::ImageUsageFlagBits::TransferSrc);
	info.flags({});

	depthStencil_.image.reset(new Image(device(), info, vk::MemoryPropertyFlagBits::DeviceLocal));

	//view
	auto aspects = vk::ImageAspectFlagBits::Depth | vk::ImageAspectFlagBits::Stencil;

	vk::ImageSubresourceRange subrange;
	subrange.aspectMask(aspects);
	subrange.baseMipLevel(0);
	subrange.levelCount(1);
	subrange.baseArrayLayer(0);
	subrange.layerCount(1);

	vk::ImageViewCreateInfo viewInfo = {};
	viewInfo.viewType(vk::ImageViewType::e2D);
	viewInfo.format(depthStencil_.format);
	viewInfo.image(depthStencil_.image->vkImage());
	viewInfo.flags({});
	viewInfo.subresourceRange(subrange);

	vk::createImageView(vkDevice(), &viewInfo, nullptr, &depthStencil_.imageView);
}

void SwapChainRenderer::buildCommandBuffer(const FrameRenderer& renderer) const
{
	auto clearValues = builder_->clearValues();
	auto width = swapChain().extent().width();
	auto height = swapChain().extent().height();

	vk::CommandBufferBeginInfo cmdBufInfo;

	vk::RenderPassBeginInfo beginInfo;
	beginInfo.renderPass(vkRenderPass());
	beginInfo.renderArea({{0, 0}, {width - 1, height - 1}}); //why +1 needed to show sth?
	beginInfo.clearValueCount(clearValues.size()); //XXX
	beginInfo.pClearValues(clearValues.data());
	beginInfo.framebuffer(renderer.framebuffer);

	vk::beginCommandBuffer(renderer.commandBuffer, cmdBufInfo);
	vk::cmdBeginRenderPass(renderer.commandBuffer, &beginInfo, vk::SubpassContents::Inline);

	//Update dynamic viewport state
	vk::Viewport viewport;
	viewport.width(width);
	viewport.height(height);
	viewport.minDepth(0.f);
	viewport.maxDepth(1.f);
	vk::cmdSetViewport(renderer.commandBuffer, 0, 1, &viewport);

	//Update dynamic scissor state
	vk::Rect2D scissor;
	scissor.extent({width, height});
	scissor.offset({0, 0});
	vk::cmdSetScissor(renderer.commandBuffer, 0, 1, &scissor);

	builder_->build({renderer.commandBuffer, vkRenderPass(), renderer.framebuffer});
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

	vk::endCommandBuffer(renderer.commandBuffer);
}

void SwapChainRenderer::render(vk::Queue queue)
{
	vk::Semaphore presentComplete;
    vk::SemaphoreCreateInfo semaphoreCI;
	vk::createSemaphore(vkDevice(), &semaphoreCI, nullptr, &presentComplete);

    auto currentBuffer = swapChain().acquireNextImage(presentComplete);

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount(1);
	submitInfo.pWaitSemaphores(&presentComplete);
	submitInfo.commandBufferCount(1);
	submitInfo.pCommandBuffers(&frameRenderers_[currentBuffer].commandBuffer);

	vk::queueSubmit(queue, 1, &submitInfo, 0);
    swapChain().present(queue, currentBuffer);

    vk::destroySemaphore(vkDevice(), presentComplete, nullptr);
	vk::queueWaitIdle(queue);
}

}
