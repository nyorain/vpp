#include <vpp/computePipeline.hpp>
#include <vpp/vk.hpp>

#include <utility>

namespace vpp
{

ComputePipeline::ComputePipeline(const Device& dev, const CreateInfo& createInfo)
{
	init(dev, createInfo);
}

ComputePipeline::ComputePipeline(ComputePipeline&& other) noexcept : Pipeline(std::move(other))
{
}

ComputePipeline& ComputePipeline::operator=(ComputePipeline&& other) noexcept
{
	Pipeline::operator=(std::move(other));
	return *this;
}

void ComputePipeline::init(const Device& dev, const CreateInfo& createInfo)
{
	Resource::init(dev);

	//pipeline layout
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	descriptorSetLayouts.reserve(createInfo.descriptorSetLayouts.size());

	for(auto& layout : createInfo.descriptorSetLayouts)
		descriptorSetLayouts.push_back(layout->vkDescriptorSetLayout());

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

	pipelineLayout_ = vk::createPipelineLayout(vkDevice(), pipelineLayoutInfo);

	//pipeline
	vk::ComputePipelineCreateInfo info;
	info.stage = createInfo.shader.vkStageInfo();
	info.layout = pipelineLayout_;

	vk::createComputePipelines(vkDevice(), 0, 1, info, nullptr, pipeline_);
}

}
