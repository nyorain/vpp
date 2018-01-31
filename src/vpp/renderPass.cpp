// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/renderPass.hpp>
#include <vpp/util/span.hpp>
#include <vpp/vk.hpp>

namespace vpp {

// RenderPass
RenderPass::RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info)
	: RenderPass(dev, vk::createRenderPass(dev, info))
{
}

RenderPass::RenderPass(const Device& dev, vk::RenderPass pass)
	: ResourceHandle(dev, pass)
{
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
