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
		ShaderProgram shader; //todo: diffentaite between graphics and compute shader programs
		vk::PipelineCreateFlags flags {};
	};

protected:
	ComputePipeline() = default;

	void create(const Device& context, const CreateInfo& createInfo);
	void destroy();

public:
	ComputePipeline(const Device& device, const CreateInfo& createInfo);
	~ComputePipeline();
};

}
