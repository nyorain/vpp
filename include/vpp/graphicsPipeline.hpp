#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>
#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>

#include <memory>

namespace vpp
{

class GraphicsPipeline : public Pipeline
{
public:
	class StatesCreateInfo
	{
	public:
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
		vk::PipelineTessellationStateCreateInfo tessellation;
		vk::PipelineViewportStateCreateInfo viewport;
		vk::PipelineRasterizationStateCreateInfo rasterization;
		vk::PipelineMultisampleStateCreateInfo multisample;
		vk::PipelineDepthStencilStateCreateInfo depthStencil;
		vk::PipelineColorBlendStateCreateInfo colorBlend;

	public:
		std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments_;
		std::vector<vk::Viewport> viewports_;
		std::vector<vk::Rect2D> scissors_;

	public:
		StatesCreateInfo() = default;
		StatesCreateInfo(const vk::Viewport& viewport); //default initialized states
	};

	struct CreateInfo
	{
		std::vector<DescriptorSetLayout*> descriptorSetLayouts;
		std::vector<VertexBufferLayout*> vertexBufferLayouts;
		std::vector<vk::DynamicState> dynamicStates;
		std::vector<vk::PushConstantRange> pushConstantRanges;

		vk::RenderPass renderPass;
		ShaderProgram shader;

		vk::PipelineCreateFlags flags {};
		StatesCreateInfo states {};
	};

public:
	GraphicsPipeline() = default;
	GraphicsPipeline(const Device& device, const CreateInfo& createInfo);
	~GraphicsPipeline() = default;

	GraphicsPipeline(GraphicsPipeline&& other) noexcept : Pipeline(std::move(other)) {}
	GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept
		{ Pipeline::operator=(std::move(other)); return *this; }

	void init(const Device& context, const CreateInfo& createInfo);
};

}
