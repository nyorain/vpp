#pragma once

#include <vpp/fwd.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/utility/range.hpp>
#include <vpp/vulkan/structs.hpp>

#include <vector>

namespace vpp
{

//TODO: something about derivates when copying builder.

///Class to easier create vulkan graphics pipelines.
///Note that objects of this class can be copied, but this will not duplicate the
///undeerlaying shader modules. So if a pipelineBuilder owns a shader module, its copies
///(which still reference those modules) shall not be used to create a pipeline after
///the original goes out of scope.
class GraphicsPipelineBuilder
{
public:
	GraphicsPipelineBuilder() = default;
	GraphicsPipelineBuilder(const Device& dev, vk::RenderPass rp, unsigned int xsubpass = 0);
	~GraphicsPipelineBuilder() = default;

	GraphicsPipelineBuilder(const GraphicsPipelineBuilder& other);
	GraphicsPipelineBuilder& operator=(const GraphicsPipelineBuilder& other);

	Pipeline build(vk::PipelineCache cache = {});
	vk::GraphicsPipelineCreateInfo parse();

public:
	ShaderProgram shader;
	vk::RenderPass renderPass {};
	unsigned int subpass {};

	vk::PipelineLayout layout;
	std::vector<VertexBufferLayout> vertexBufferLayouts;

	vk::PipelineCreateFlags flags {};
	std::vector<vk::DynamicState> dynamicStates;

	struct
	{
		std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
		std::vector<vk::Viewport> viewports;
		std::vector<vk::Rect2D> scissors;

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
		vk::PipelineTessellationStateCreateInfo tessellation;
		vk::PipelineViewportStateCreateInfo viewport;
		vk::PipelineRasterizationStateCreateInfo rasterization;
		vk::PipelineMultisampleStateCreateInfo multisample;
		vk::PipelineDepthStencilStateCreateInfo depthStencil;
		vk::PipelineColorBlendStateCreateInfo colorBlend;
	} states;

protected:
	vk::PipelineVertexInputStateCreateInfo vertexInfo_;
	vk::PipelineDynamicStateCreateInfo dynamicState_;
	std::vector<vk::PipelineShaderStageCreateInfo> stageInfos_;
	std::vector<vk::VertexInputBindingDescription> bindingDescriptions_;
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions_;
};

///\{
///Create multiple vulkan graphic pipelines at once.
///Might be more efficient than constructing them individually.
std::vector<Pipeline> createGraphicsPipelines(const Device& dev,
	const Range<vk::GraphicsPipelineCreateInfo>& infos, vk::PipelineCache cache = {});

std::vector<Pipeline> createGraphicsPipelines(
	const Range<std::reference_wrapper<GraphicsPipelineBuilder>>& builder,
	vk::PipelineCache cache = {});
///\}

}
