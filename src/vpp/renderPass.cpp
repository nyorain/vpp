#include <vpp/renderPass.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

//RenderPass
RenderPass::RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info)
	: RenderPass(dev, vk::createRenderPass(dev, info), info)
{
}

RenderPass::RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info)
	: Resource(dev), renderPass_(pass)
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

RenderPass::~RenderPass()
{
	if(vkRenderPass()) vk::destroyRenderPass(vkDevice(), vkRenderPass());
}

RenderPass::RenderPass(RenderPass&& other) noexcept : Resource(other)
{
	attachments_ = std::move(other.attachments_);
	references_ = std::move(other.references_);
	subpasses_ = std::move(other.subpasses_);
	dependencies_ = std::move(other.dependencies_);

	std::swap(renderPass_, other.renderPass_);
}

RenderPass& RenderPass::operator=(RenderPass other) noexcept
{
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

//XXX: could make this RAII wrapper for render pass instances later on
//RenderPassInstance
RenderPassInstance::RenderPassInstance(vk::CommandBuffer cb, vk::RenderPass rp, vk::Framebuffer fb)
	: renderPass_(rp), commandBuffer_(cb), framebuffer_(fb)
{
}

RenderPassInstance::~RenderPassInstance()
{
}

}
