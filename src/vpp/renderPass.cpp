#include <vpp/renderPass.hpp>

namespace vpp
{

//RenderPass
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
	swap(references_, other.references_);
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
	references_.clear();

	Resource::destroy();
}

void RenderPass::initInfos(const vk::RenderPassCreateInfo& info)
{
	attachments_.reserve(info.attachmentCount());
	for(std::size_t i(0); i < info.attachmentCount(); ++i)
		attachments_.push_back(info.pAttachments()[i]);

	subpasses_.reserve(info.subpassCount());
	for(std::size_t i(0); i < info.subpassCount(); ++i)
	{
		auto& sub = info.pSubpasses()[i];
		subpasses_.push_back(sub);

		if(sub.pDepthStencilAttachment())
		{
			references_.push_back(*sub.pDepthStencilAttachment());
		}

		for(auto& ref : makeRange(sub.pColorAttachments(), sub.colorAttachmentCount()))
		{
			references_.push_back(ref);
		}

		for(auto& ref : makeRange(sub.pInputAttachments(), sub.inputAttachmentCount()))
		{
			references_.push_back(ref);
		}
	}

	dependencies_.reserve(info.dependencyCount());
	for(std::size_t i(0); i < info.dependencyCount(); ++i)
		dependencies_.push_back(info.pDependencies()[i]);

}

//XXX: could make this RAII wrapper for render pass instances later on
//RenderPassInstance
RenderPassInstance::RenderPassInstance(vk::CommandBuffer cmdbuffer, const RenderPass& pass,
	vk::Framebuffer framebuffer) : Resource(pass.device()), renderPass_(pass)
{
	commandBuffer_ = cmdbuffer;
	framebuffer_ = framebuffer;
}

RenderPassInstance::~RenderPassInstance()
{

}

void RenderPassInstance::nextSubpass()
{
	//TODO: check renderpass for left subpasses.

	currentSubpass_ += 1;
	vk::cmdNextSubpass(vkCommandBuffer(), vk::SubpassContents::Inline);
}


}
