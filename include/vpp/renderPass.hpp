// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <vector>

namespace vpp {

/// RAII Vulkan Renderpass wrapper.
/// Stores its description information.
class RenderPass : public ResourceHandle<vk::RenderPass> {
public:
	RenderPass() = default;
	RenderPass(const Device& dev, const vk::RenderPassCreateInfo& info);
	RenderPass(const Device& dev, vk::RenderPass pass);
	~RenderPass();

	RenderPass(RenderPass&& rhs) noexcept { swap(*this, rhs); }
	RenderPass& operator=(RenderPass&& rhs) noexcept { swap(*this, rhs); return *this; }
};

/// Vulkan RenderPass Instance, i.e. a commandbuffer recording session during a render pass.
class RenderPassInstance : public nytl::NonCopyable {
protected:
	vk::RenderPass renderPass_ {};
	vk::CommandBuffer commandBuffer_ {};
	vk::Framebuffer framebuffer_ {};

public:
	RenderPassInstance(vk::CommandBuffer cmdbuf, vk::RenderPass pass, vk::Framebuffer framebuffer);
	~RenderPassInstance();

	const vk::RenderPass& renderPass() const { return renderPass_; }
	const vk::CommandBuffer& vkCommandBuffer() const { return commandBuffer_; }
	const vk::Framebuffer& vkFramebuffer() const { return framebuffer_; }
};

} // namespace vpp
