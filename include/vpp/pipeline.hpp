#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>

namespace vpp
{

class Pipeline : public Resource
{
protected:
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_ {};

public:
	vk::Pipeline vkPipeline() const { return pipeline_; }
	vk::PipelineLayout vkPipelineLayout() const { return pipelineLayout_; }

	void drawCommands(vk::CommandBuffer cmdBuffer, const std::vector<Buffer*>& vertices,
		const std::vector<DescriptorSet*>& descriptors) const;
};

}
