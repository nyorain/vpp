// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/renderPass.hpp>
#include <vpp/util/span.hpp>
#include <vpp/vk.hpp>

namespace vpp {

// RenderPass
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
	for(std::size_t i(0); i < info.subpassCount; ++i) {
		auto& sub = info.pSubpasses[i];
		subpasses_.push_back(sub);

		if(sub.pDepthStencilAttachment) references_.push_back(*sub.pDepthStencilAttachment);

		using SpanAR = nytl::Span<const vk::AttachmentReference>;
		for(auto& ref : SpanAR(*sub.pColorAttachments, sub.colorAttachmentCount))
			references_.push_back(ref);

		for(auto& ref : SpanAR(*sub.pInputAttachments, sub.inputAttachmentCount))
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

// XXX: could make this RAII wrapper for render pass instances later on
// RenderPassInstance
RenderPassInstance::RenderPassInstance(vk::CommandBuffer cb, vk::RenderPass rp, vk::Framebuffer fb)
	: renderPass_(rp), commandBuffer_(cb), framebuffer_(fb)
{
}

RenderPassInstance::~RenderPassInstance()
{
}

}
