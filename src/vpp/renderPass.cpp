#include <vpp/renderPass.hpp>
#include <vpp/utility/range.hpp>
#include <vpp/vk.hpp>

namespace vpp
{

//RenderPass
RenderPass::RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info)
	: RenderPass(dev, vk::createRenderPass(dev, info), info)
{
}

RenderPass::RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info)
	: ResourceHandle(dev, pass)
{
	attachments_.reserve(info.attachmentCount);
	for(std::size_t i(0); i < info.attachmentCount; ++i)
		attachments_.push_back(info.pAttachments[i]);

	subpasses_.reserve(info.subpassCount);
	for(std::size_t i(0); i < info.subpassCount; ++i)
	{
		auto& sub = info.pSubpasses[i];
		subpasses_.push_back(sub);

		if(sub.pDepthStencilAttachment) references_.push_back(*sub.pDepthStencilAttachment);

		for(auto& ref : makeRange(*sub.pColorAttachments, sub.colorAttachmentCount))
			references_.push_back(ref);

		for(auto& ref : makeRange(*sub.pInputAttachments, sub.inputAttachmentCount))
			references_.push_back(ref);
	}

	dependencies_.reserve(info.dependencyCount);
	for(std::size_t i(0); i < info.dependencyCount; ++i)
		dependencies_.push_back(info.pDependencies[i]);
}

RenderPass::~RenderPass()
{
	if(vkHandle()) vk::destroyRenderPass(vkDevice(), vkHandle());
}

//XXX: could make this RAII wrapper for render pass instances later on
//RenderPassInstance
RenderPassInstance::RenderPassInstance(vk::CommandBuffer cb, vk::RenderPass rp, vk::Framebuffer fb)
	: renderPass_(rp), commandBuffer_(cb), framebuffer_(fb)
{
}

RenderPassInstance::~RenderPassInstance()
{
}

///
vk::RenderPassCreateInfo& defaultRenderPassCreateInfo(vk::Format colorFormat)
{
	static bool init = false;
	static vk::AttachmentDescription attachments[2] {};
	static vk::AttachmentReference colorReference;
	static vk::AttachmentReference depthReference;
	static vk::SubpassDescription subpass;
	static vk::RenderPassCreateInfo renderPassInfo;

	if(!init)
	{
		init = true;

		//color from swapchain
		attachments[0].samples = vk::SampleCountBits::e1;
		attachments[0].loadOp = vk::AttachmentLoadOp::clear;
		attachments[0].storeOp = vk::AttachmentStoreOp::store;
		attachments[0].stencilLoadOp = vk::AttachmentLoadOp::dontCare;
		attachments[0].stencilStoreOp = vk::AttachmentStoreOp::dontCare;
		attachments[0].initialLayout = vk::ImageLayout::undefined;
		attachments[0].finalLayout = vk::ImageLayout::presentSrcKHR;

		colorReference.attachment = 0;
		colorReference.layout = vk::ImageLayout::colorAttachmentOptimal;

		//depth from own depth stencil
		attachments[1].format = vk::Format::d32Sfloat;
		attachments[1].samples = vk::SampleCountBits::e1;
		attachments[1].loadOp = vk::AttachmentLoadOp::clear;
		attachments[1].storeOp = vk::AttachmentStoreOp::store;
		attachments[1].stencilLoadOp = vk::AttachmentLoadOp::dontCare;
		attachments[1].stencilStoreOp = vk::AttachmentStoreOp::dontCare;
		attachments[1].initialLayout = vk::ImageLayout::undefined;
		attachments[1].finalLayout = vk::ImageLayout::undefined;

		depthReference.attachment = 1;
		depthReference.layout = vk::ImageLayout::depthStencilAttachmentOptimal;

		//only subpass
		subpass.pipelineBindPoint = vk::PipelineBindPoint::graphics;
		subpass.flags = {};
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorReference;
		subpass.pResolveAttachments = nullptr;
		subpass.pDepthStencilAttachment = &depthReference;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

		renderPassInfo.attachmentCount = 2;
		renderPassInfo.pAttachments = attachments;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 0;
		renderPassInfo.pDependencies = nullptr;
	}

	attachments[0].format = colorFormat;
	return renderPassInfo;
}

}
