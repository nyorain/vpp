#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>

#include <vector>
#include <functional>

namespace vpp
{

class ComputePipeline : public Pipeline
{
public:
	struct CreateInfo
	{
		std::vector<std::reference_wrapper<DescriptorSetLayout>> descriptorSetLayouts;
		ShaderStage shader;
		vk::PipelineCreateFlags flags {};
	};

public:
	ComputePipeline() = default;
	ComputePipeline(const Device& dev, const CreateInfo& createInfo);
	~ComputePipeline() = default;

	ComputePipeline(ComputePipeline&& other) noexcept;
	ComputePipeline& operator=(ComputePipeline&& other) noexcept;
};

}
