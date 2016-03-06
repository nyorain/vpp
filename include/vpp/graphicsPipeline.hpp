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
	struct StatesCreateInfo
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
		//StatesCreateInfo(const std::vector<vk::DynamicState>& dynamic = {});
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

protected:
	GraphicsPipeline() = default;

	void create(const Device& context, const CreateInfo& createInfo);
	void destroy();

public:
	GraphicsPipeline(const Device& device, const CreateInfo& createInfo);
	~GraphicsPipeline();
};

}
