#include <vpp/computePipeline.hpp>
#include <vpp/vk.hpp>

#include <utility>

namespace vpp
{

ComputePipeline::ComputePipeline(const Device& dev, const CreateInfo& createInfo) : Pipeline(dev)
{
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

	vk::createComputePipelines(vkDevice(), {}, 1, info, nullptr, pipeline_);
	//std::cout << vkCreateComputePipelines(reinterpret_cast<VkDevice>(vkDevice()), {}, 1, reinterpret_cast<const VkComputePipelineCreateInfo*>(&info), nullptr, reinterpret_cast<VkPipeline*>(&pipeline_)) << " mm\n";
}

ComputePipeline::ComputePipeline(ComputePipeline&& other) noexcept : Pipeline(std::move(other))
{
}

ComputePipeline& ComputePipeline::operator=(ComputePipeline&& other) noexcept
{
	Pipeline::operator=(std::move(other));
	return *this;
}

}
