#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>

namespace vpp
{

class Pipeline : public Resource
{
public:
	enum class Type
	{
		graphic,
		compute
	};

protected:
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_ {};

protected:
	Pipeline() = default;
	using Resource::create;

public:
	Pipeline(const Device& dev);
	~Pipeline() = default;

	vk::Pipeline vkPipeline() const { return pipeline_; }
	vk::PipelineLayout vkPipelineLayout() const { return pipelineLayout_; }

	void drawCommands(vk::CommandBuffer cmdBuffer, const std::vector<Buffer*>& vertices,
		const std::vector<DescriptorSet*>& descriptors) const;
};

}
