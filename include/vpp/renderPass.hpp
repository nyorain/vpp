// Copyright (c) 2017 nyorain
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
	RenderPass(const Device& dev, vk::RenderPass pass, const vk::RenderPassCreateInfo& info);
	~RenderPass();

	RenderPass(RenderPass&& lhs) noexcept { swap(lhs); }
	RenderPass& operator=(RenderPass&& lhs) noexcept { swap(lhs); return *this; }

	const std::vector<vk::AttachmentDescription>& attachments() const { return attachments_; }
	const std::vector<vk::SubpassDependency>& dependencies() const { return dependencies_; }
	const std::vector<vk::SubpassDescription>& subpasses() const { return subpasses_; }
	const std::vector<vk::AttachmentReference>& references() const { return references_; }

protected:
	std::vector<vk::AttachmentDescription> attachments_;
	std::vector<vk::SubpassDescription> subpasses_;
	std::vector<vk::SubpassDependency> dependencies_;
	std::vector<vk::AttachmentReference> references_;
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
