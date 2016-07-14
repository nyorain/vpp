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

}
