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
		ShaderStage shader;
		vk::PipelineCreateFlags flags {};
	};

public:
	ComputePipeline() = default;
	ComputePipeline(const Device& dev, const CreateInfo& createInfo);
	~ComputePipeline() = default;

	ComputePipeline(ComputePipeline&& other) noexcept : Pipeline(std::move(other)) {}
	ComputePipeline& operator=(ComputePipeline&& other) noexcept
		{ Pipeline::operator=(std::move(other)); return *this; }

	void init(const Device& dev, const CreateInfo& createInfo);
};

}
