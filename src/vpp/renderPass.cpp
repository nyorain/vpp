#include <vpp/renderPass.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

//RenderPass
RenderPass::RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info)
	: Resource(dev)
{
	renderPass_ = vk::createRenderPass(vkDevice(), info);
	initInfos(info);
}

RenderPass::RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info)
	: Resource(dev), renderPass_(pass)
{
	initInfos(info);
}

RenderPass::~RenderPass()
{
	if(vkRenderPass()) vk::destroyRenderPass(vkDevice(), vkRenderPass());

	renderPass_ = {};
	attachments_.clear();
	subpasses_.clear();
	dependencies_.clear();
	references_.clear();
}

RenderPass::RenderPass(RenderPass&& other) noexcept
{
	swap(*this, other);
}

RenderPass& RenderPass::operator=(RenderPass&& other) noexcept
{
	this->~RenderPass();
	swap(*this, other);
	return *this;
}

void swap(RenderPass& a, RenderPass& b) noexcept
{
	using std::swap;

	swap(a.renderPass_, b.renderPass_);
	swap(a.attachments_, b.attachments_);
	swap(a.subpasses_, b.subpasses_);
	swap(a.dependencies_, b.dependencies_);
	swap(a.references_, b.references_);
	swap(a.device_, b.device_);
}

void RenderPass::initInfos(const vk::RenderPassCreateInfo& info)
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

		for(auto& ref : makeRange(sub.pColorAttachments, sub.colorAttachmentCount))
			references_.push_back(ref);

		for(auto& ref : makeRange(sub.pInputAttachments, sub.inputAttachmentCount))
			references_.push_back(ref);
	}

	dependencies_.reserve(info.dependencyCount);
	for(std::size_t i(0); i < info.dependencyCount; ++i)
		dependencies_.push_back(info.pDependencies[i]);
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
	currentSubpass_++;
	vk::cmdNextSubpass(vkCommandBuffer(), vk::SubpassContents::eInline);
}


}
