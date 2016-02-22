#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

namespace vpp
{

class VertexBufferLayout
{
	std::vector<vk::Format> attributes_;
};

class VertexBuffer
{
	VertexBufferLayout layout_;
	vk::Buffer buffer_ {};
};

class DescriptorSet
{

};

class GraphicsPipeline : public Resource
{
protected:
	vk::RenderPass renderPass_;

	vk::DescriptorPool descriptorPool_ {};
	vk::DescriptorSet descriptorSet_ {};
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_;

	vk::DeviceMemory memory_ {};

	std::vector<vk::VertexInputBindingDescription> bindingDescriptions_;
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions_;
	vk::PipelineVertexInputStateCreateInfo pipelineVertexInfo_;

protected:
	GraphicsPipeline() = default;
	void create(const Device& context, const vk::GraphicsPipelineCreateInfo& info,
		vk::RenderPass renderPass);

	void initDescriptorPool();
	void initDescriptorSet();
	void initPipelineLayout();
	void initPipeline();
	void initBuffer();

	void destroy();

public:
	GraphicsPipeline(const Device& context, const vk::GraphicsPipelineCreateInfo&,
		vk::RenderPass renderPass);
	~GraphicsPipeline();

	void renderCommands(vk::CommandBuffer cmdBuffer) const;
};

}
