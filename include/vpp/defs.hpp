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
	ViewableImage::CreateInfo createInfo; //static/dynamic attachments
	bool dynamic = false; //dynamic or static attachment?
	vk::ImageView external = {}; //if != VK_NULL_HANDLE all other members will be ignored
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

	States(vk::Viewport viewport = {});
};

GraphicsPipeline::States defaultGraphicsPipelineStates();

struct GraphicsPipeline::CreateInfo
{
	ShaderProgram shader;
	vk::RenderPass renderPass;

	std::vector<std::reference_wrapper<DescriptorSetLayout>> descriptorSetLayouts;
	std::vector<std::reference_wrapper<VertexBufferLayout>> vertexBufferLayouts;

	vk::PipelineCreateFlags flags {};
	GraphicsPipeline::States states {};

	std::vector<vk::DynamicState> dynamicStates;
	std::vector<vk::PushConstantRange> pushConstantRanges;
};

}
