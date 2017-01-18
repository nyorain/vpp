/*

#include <vpp/computePipeline.hpp>
#include <vpp/vk.hpp>

#include <utility>

namespace vpp {

Pipeline ComputePipelineBuilder::build(const Device& dev, vk::PipelineCache cache)
{
	vk::Pipeline pipeline;
	vk::ComputePipelineCreateInfo ret;
	ret.flags = flags;
	ret.stage = shaderStage;
	ret.layout = layout;
	vk::createComputePipelines(dev, cache, 1, ret, nullptr, pipeline);
	return {dev, pipeline};
}

vk::ComputePipelineCreateInfo ComputePipelineBuilder::parse()
{
	vk::ComputePipelineCreateInfo ret;
	ret.flags = flags;
	ret.stage = shaderStage.vkStageInfo();
	ret.layout = layout;
	return ret;
}

std::vector<Pipeline> createComputePipelines(const Device& dev,
	nytl::Span<const vk::ComputePipelineCreateInfo> infos, vk::PipelineCache cache)
{
	auto pipelines = vk::createComputePipelines(dev, cache, infos);
	std::vector<Pipeline> ret;
	ret.reserve(pipelines.size());
	for(auto& p : pipelines) ret.emplace_back(dev, p);
	return ret;
}

std::vector<Pipeline> createComputePipelines(
	nytl::Span<const std::reference_wrapper<ComputePipelineBuilder>> builder,
	vk::PipelineCache cache)
{
	if(builder.empty()) return {};

	std::vector<vk::ComputePipelineCreateInfo> infos;
	infos.reserve(builder.size());
	for(auto& b : builder) infos.push_back(b.get().parse());

	return createComputePipelines(builder.front().get().shaderStage.device(), infos, cache);
}

}

*/
