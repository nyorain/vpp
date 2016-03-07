#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>

namespace vpp
{

class ComputePipeline : public Pipeline
{
public:
	struct CreateInfo
	{
		std::vector<DescriptorSetLayout*> descriptorSetLayouts;
		std::vector<VertexBufferLayout*> vertexBufferLayouts;
		ShaderStage shader;
		vk::PipelineCreateFlags flags {};
	};

public:
	ComputePipeline() = default;
	ComputePipeline(const Device& device, const CreateInfo& createInfo);
	~ComputePipeline();

	void init(const Device& context, const CreateInfo& createInfo);
};

}
