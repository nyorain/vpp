#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/shader.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/image.hpp>
#include <vpp/renderer.hpp>

#include <vector>
#include <functional>

namespace vpp
{

struct ViewableImage::CreateInfo
{
	vk::ImageCreateInfo imgInfo;
	vk::ImageViewCreateInfo viewInfo;
	vk::MemoryPropertyFlags memoryFlags {}; //memory flags for image
};

struct SwapChainRenderer::AttachmentInfo
{
	//will be used to create static/dynamic attachments
	ViewableImage::CreateInfo createInfo;

	//specifies whether it is a dynamic or static attachment.
	//Each framebuffer (for the different swapChain attachments) has its own dyanmic attachments
	//while static attachments are shared among them
	bool dynamic = false;

	//if valid all other members will be ignored and this will be used as some additional
	//external attachment. Note that it must have at least the size of the swapChain
	vk::ImageView external = {};
};

// struct GraphicsPipeline::States
// {
// 	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
// 	vk::PipelineTessellationStateCreateInfo tessellation;
// 	vk::PipelineViewportStateCreateInfo viewport;
// 	vk::PipelineRasterizationStateCreateInfo rasterization;
// 	vk::PipelineMultisampleStateCreateInfo multisample;
// 	vk::PipelineDepthStencilStateCreateInfo depthStencil;
// 	vk::PipelineColorBlendStateCreateInfo colorBlend;
//
// 	std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
// 	std::vector<vk::Viewport> viewports;
// 	std::vector<vk::Rect2D> scissors;
//
// 	States() = default;
// 	States(const vk::Viewport& viewport);
// };
//
// struct GraphicsPipeline::CreateInfo
// {
// 	CreateInfo() = default;
// 	//CreateInfo(const Device& dev, const Range<vk::DynamicState>& dyanmic = {});
//
// 	ShaderProgram shader;
// 	vk::PipelineCache cache {};
//
// 	vk::RenderPass renderPass {};
// 	unsigned int subpass {};
//
// 	std::vector<std::reference_wrapper<DescriptorSetLayout>> descriptorSetLayouts;
// 	std::vector<std::reference_wrapper<VertexBufferLayout>> vertexBufferLayouts;
//
// 	vk::PipelineCreateFlags flags {};
// 	GraphicsPipeline::States states {};
//
// 	std::vector<vk::DynamicState> dynamicStates;
// 	std::vector<vk::PushConstantRange> pushConstantRanges;
// };


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

	Pipeline build(vk::PipelineCache cache = {}) const;
	vk::PipelineLayout parse(vk::GraphicsPipelineCreateInfo& createInfo) const;

public:
	ShaderProgram shader;
	vk::RenderPass renderPass {};
	unsigned int subpass {};

	vk::PipelineLayout layout;
	std::vector<VertexBufferLayout> vertexBufferLayouts;

	vk::PipelineCreateFlags flags {};
	std::vector<vk::DynamicState> dynamicStates;
	std::vector<vk::PushConstantRange> pushConstantRanges;

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
};

}
