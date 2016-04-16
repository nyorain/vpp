#include <vpp/renderer.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/surface.hpp>

#include <stdexcept>

namespace vpp
{

//SwapChainRenderer
SwapChainRenderer::SwapChainRenderer(const SwapChain& swapChain, const RendererBuilder& builder,
	const CreateInfo& info)
{
	{
		DeviceMemoryAllocator allocator(swapChain.device());
		initMemoryLess(allocator, swapChain, info);
	}

	initMemoryResources(builder);
}

SwapChainRenderer::SwapChainRenderer(DeviceMemoryAllocator& allocator, const SwapChain& swapChain,
	const RendererBuilder& builder, const CreateInfo& info)
{
	initMemoryLess(allocator, swapChain, info);
	initMemoryResources(builder);
}

SwapChainRenderer::SwapChainRenderer(SwapChainRenderer&& other) noexcept
{
	this->swap(other);
}

SwapChainRenderer& SwapChainRenderer::operator=(SwapChainRenderer&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

SwapChainRenderer::~SwapChainRenderer()
{
	destroy();
}

void SwapChainRenderer::swap(SwapChainRenderer& other) noexcept
{
	using std::swap;

	swap(device_, other.device_);
	swap(swapChain_, other.swapChain_);
	swap(info_, other.info_);
	swap(renderBuffers_, other.renderBuffers_);
	swap(staticAttachments_, other.staticAttachments_);
	swap(commandPool_, other.commandPool_);
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

void SwapChainRenderer::initMemoryLess(DeviceMemoryAllocator& allocator, const SwapChain& swapChain,
	const CreateInfo& info)
{
	if(info.renderPass == nullptr)
	{
		throw std::runtime_error("SwapChainRenderer: nullptr renderPass");
	}

	Resource::init(allocator.device());
	swapChain_ = &swapChain;
	info_ = info;

	//command pool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex(info_.queue.family);
	cmdPoolInfo.flags(vk::CommandPoolCreateFlagBits::ResetCommandBuffer);

	vk::createCommandPool(vkDevice(), &cmdPoolInfo, nullptr, &commandPool_);

	//static attachments
	staticAttachments_.reserve(info_.staticAttachments.size());
	for(auto& attachInfo : info_.staticAttachments)
	{
		attachInfo.size = swapChain.size();

		staticAttachments_.emplace_back();
		staticAttachments_.back().initMemoryLess(allocator, attachInfo);
	}

	//RenderBuffers
	//CommandBuffers
	renderBuffers_.reserve(swapChain.buffers().size());

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool(vkCommandPool());
	allocInfo.level(vk::CommandBufferLevel::Primary);
	allocInfo.commandBufferCount(swapChain.buffers().size());

	std::vector<vk::CommandBuffer> cmdBuffers(swapChain.buffers().size());
	vk::allocateCommandBuffers(vkDevice(), allocInfo, cmdBuffers); //store them later in for loop

	//frame buffers
	Framebuffer::CreateInfo fbInfo {vkRenderPass(), swapChain.size()};
	fbInfo.attachments = info_.dynamicAttachments;

	for(auto& cmdBuffer : cmdBuffers)
	{
		renderBuffers_.emplace_back();
		renderBuffers_.back().commandBuffer = cmdBuffer;
		renderBuffers_.back().framebuffer.initMemoryLess(allocator, fbInfo);
	}
}

void SwapChainRenderer::initMemoryResources(const RendererBuilder& builder)
{
	device().deviceMemoryAllocator().allocate();

	const std::size_t dynAttachSize = info().dynamicAttachments.size() + 1;
	std::map<unsigned int, vk::ImageView> attachmentMap;

	//static attachments
	for(std::size_t i(0); i < staticAttachments_.size(); i++)
	{
		staticAttachments_[i].initMemoryResources(info_.staticAttachments[i]);
		attachmentMap[dynAttachSize + i] = staticAttachments_[i].vkImageView();
	}

	//frameBufferAttachment resources
	for(std::size_t i(0); i < renderBuffers_.size(); i++)
	{
		attachmentMap[dynAttachSize - 1] = swapChain().buffers()[i].imageView;
		renderBuffers_[i].framebuffer.initMemoryResources(attachmentMap);
	}

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

void SwapChainRenderer::buildCommandBuffers(const RendererBuilder& builder)
{
	auto clearValues = builder.clearValues();
	auto width = swapChain().size().width();
	auto height = swapChain().size().height();

	for(std::size_t i(0); i < renderBuffers_.size(); ++i)
	{
		auto& renderer = renderBuffers_[i];
		vk::CommandBufferBeginInfo cmdBufInfo;

		vk::RenderPassBeginInfo beginInfo;
		beginInfo.renderPass(vkRenderPass());
		beginInfo.renderArea({{0, 0}, {width - 1, height - 1}}); //why +1 needed to show sth?
		beginInfo.clearValueCount(clearValues.size());
		beginInfo.pClearValues(clearValues.data());
		beginInfo.framebuffer(renderer.framebuffer.vkFramebuffer());

		vk::beginCommandBuffer(renderer.commandBuffer, cmdBufInfo);

		builder.beforeRender(renderer.commandBuffer);

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

		RenderPassInstance ini(renderer.commandBuffer, renderPass(),
			renderer.framebuffer.vkFramebuffer());
		builder.build(ini);

		vkCmdEndRenderPass(renderer.commandBuffer);

		builder.afterRender(renderer.commandBuffer);

		vk::ImageMemoryBarrier prePresentBarrier;
		prePresentBarrier.srcAccessMask(vk::AccessFlagBits::ColorAttachmentWrite);
		prePresentBarrier.dstAccessMask(vk::AccessFlags());
		prePresentBarrier.oldLayout(vk::ImageLayout::ColorAttachmentOptimal);
		prePresentBarrier.newLayout(vk::ImageLayout::PresentSrcKHR);
		prePresentBarrier.srcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
		prePresentBarrier.dstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
		prePresentBarrier.subresourceRange({vk::ImageAspectFlagBits::Color, 0, 1, 0, 1});
		prePresentBarrier.image(swapChain().buffers()[i].image);

		vk::cmdPipelineBarrier(renderer.commandBuffer, vk::PipelineStageFlagBits::AllCommands,
			vk::PipelineStageFlagBits::TopOfPipe, vk::DependencyFlags(), 0,
			nullptr, 0, nullptr, 1, &prePresentBarrier);

		vk::endCommandBuffer(renderer.commandBuffer);
	}
}

void SwapChainRenderer::render()
{
	vk::Semaphore presentComplete;
    vk::SemaphoreCreateInfo semaphoreCI;
	vk::createSemaphore(vkDevice(), &semaphoreCI, nullptr, &presentComplete);

    auto currentBuffer = swapChain().acquireNextImage(presentComplete);

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount(1);
	submitInfo.pWaitSemaphores(&presentComplete);
	submitInfo.commandBufferCount(1);
	submitInfo.pCommandBuffers(&renderBuffers_[currentBuffer].commandBuffer);

	vk::queueSubmit(vkQueue(), 1, &submitInfo, 0);
    swapChain().present(vkQueue(), currentBuffer);

    vk::destroySemaphore(vkDevice(), presentComplete, nullptr);
	vk::queueWaitIdle(vkQueue());
}

/*
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
*/

}
