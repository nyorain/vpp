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

struct GraphicsPipeline::States
{
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	vk::PipelineTessellationStateCreateInfo tessellation;
	vk::PipelineViewportStateCreateInfo viewport;
	vk::PipelineRasterizationStateCreateInfo rasterization;
	vk::PipelineMultisampleStateCreateInfo multisample;
	vk::PipelineDepthStencilStateCreateInfo depthStencil;
	vk::PipelineColorBlendStateCreateInfo colorBlend;

	std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
	std::vector<vk::Viewport> viewports;
	std::vector<vk::Rect2D> scissors;

	States() = default;
	States(const vk::Viewport& viewport);
};

struct GraphicsPipeline::CreateInfo
{
	ShaderProgram shader;
	vk::RenderPass renderPass {};
	vk::PipelineCache cache {};

	std::vector<std::reference_wrapper<DescriptorSetLayout>> descriptorSetLayouts;
	std::vector<std::reference_wrapper<VertexBufferLayout>> vertexBufferLayouts;

	vk::PipelineCreateFlags flags {};
	GraphicsPipeline::States states {};

	std::vector<vk::DynamicState> dynamicStates;
	std::vector<vk::PushConstantRange> pushConstantRanges;
};

}
