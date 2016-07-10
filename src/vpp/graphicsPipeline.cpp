#include <vpp/graphicsPipeline.hpp>
#include <vpp/defs.hpp>
#include <vpp/context.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>

#include <utility>

namespace vpp
{

//info default ctor
GraphicsPipeline::States::States(const vk::Viewport& viewportinfo)
{
	//needed data
	blendAttachments.emplace_back();
	blendAttachments.back().blendEnable = false;
	blendAttachments.back().colorWriteMask =
		vk::ColorComponentBits::r |
		vk::ColorComponentBits::g |
		vk::ColorComponentBits::b |
		vk::ColorComponentBits::a;

	viewports.emplace_back(viewportinfo);

	vk::Extent2D extent {std::uint32_t(viewportinfo.width), std::uint32_t(viewportinfo.height)};
	scissors.push_back({{0, 0}, extent});

	//structs
	vk::StencilOpState stencil;
	stencil.failOp = vk::StencilOp::keep;
	stencil.passOp = vk::StencilOp::keep;
	stencil.compareOp = vk::CompareOp::always;

	//fill
	inputAssembly.topology = vk::PrimitiveTopology::triangleList;

	rasterization.polygonMode = vk::PolygonMode::fill;
	rasterization.cullMode = vk::CullModeBits::back;
	rasterization.frontFace = vk::FrontFace::counterClockwise;
	rasterization.depthClampEnable = true;
	rasterization.rasterizerDiscardEnable = false;
	rasterization.depthBiasEnable = false;
	rasterization.lineWidth = 1.f;

	colorBlend.attachmentCount = blendAttachments.size();
	colorBlend.pAttachments = blendAttachments.data();

	viewport.viewportCount = viewports.size();
	viewport.pViewports = viewports.data();
	viewport.scissorCount = scissors.size();
	viewport.pScissors = scissors.data();

	depthStencil.depthTestEnable = true;
	depthStencil.depthWriteEnable = true;
	depthStencil.depthCompareOp = vk::CompareOp::lessOrEqual;
	depthStencil.depthBoundsTestEnable = false;
	depthStencil.stencilTestEnable = false;
	depthStencil.back = stencil;
	depthStencil.front = stencil;

	multisample.pSampleMask = nullptr;
	multisample.rasterizationSamples = vk::SampleCountBits::e1;
}

//pipeline
GraphicsPipeline::GraphicsPipeline(const Device& device, const CreateInfo& createInfo)
{
	Pipeline::init(device);

	//vertexInfo
	vk::PipelineVertexInputStateCreateInfo vertexInfo {};

	//Binding description
	std::size_t attributeCount = 0;
	for(auto& layout : createInfo.vertexBufferLayouts)
		attributeCount += layout.get().attributes.size();

	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
	bindingDescriptions.reserve(createInfo.vertexBufferLayouts.size());

	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.reserve(attributeCount);

	for(auto& layout : createInfo.vertexBufferLayouts)
	{
		std::size_t location = 0;
		std::size_t offset = 0;

		for(auto& attribute : layout.get().attributes)
		{
			attributeDescriptions.emplace_back();
			attributeDescriptions.back().location = location++;
			attributeDescriptions.back().binding = layout.get().binding;
			attributeDescriptions.back().format = attribute;
			attributeDescriptions.back().offset = offset;
			offset += formatSize(attribute) / 8;
		}

		bindingDescriptions.emplace_back();
		bindingDescriptions.back().binding = layout.get().binding;
		bindingDescriptions.back().stride = offset;
		bindingDescriptions.back().inputRate = vk::VertexInputRate::vertex;
	}

	vertexInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
	vertexInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//pipeline layout
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	descriptorSetLayouts.reserve(createInfo.descriptorSetLayouts.size());

	for(auto& layout : createInfo.descriptorSetLayouts)
		descriptorSetLayouts.push_back(layout.get().vkDescriptorSetLayout());

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo {};
	pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

	pipelineLayout_ = vk::createPipelineLayout(vkDevice(), pipelineLayoutInfo);

	//create it
	vk::GraphicsPipelineCreateInfo pipelineInfo;

	//dynamic state
	if(createInfo.dynamicStates.size() > 0)
	{
		vk::PipelineDynamicStateCreateInfo dynamicState;
		dynamicState.pDynamicStates = createInfo.dynamicStates.data();
		dynamicState.dynamicStateCount = createInfo.dynamicStates.size();
		pipelineInfo.pDynamicState = &dynamicState;
	}

	auto infos = createInfo.shader.vkStageInfos();

	pipelineInfo.layout = pipelineLayout_;
	pipelineInfo.pVertexInputState = &vertexInfo;
	pipelineInfo.renderPass = createInfo.renderPass;
	pipelineInfo.stageCount = infos.size();
	pipelineInfo.pStages = infos.data();
	pipelineInfo.pInputAssemblyState = &createInfo.states.inputAssembly;
	pipelineInfo.pRasterizationState = &createInfo.states.rasterization;
	pipelineInfo.pColorBlendState = &createInfo.states.colorBlend;
	pipelineInfo.pMultisampleState = &createInfo.states.multisample;
	pipelineInfo.pViewportState = &createInfo.states.viewport;
	pipelineInfo.pDepthStencilState = &createInfo.states.depthStencil;
	pipelineInfo.pTessellationState = nullptr;

	vk::createGraphicsPipelines(vkDevice(), createInfo.cache, 1, pipelineInfo, nullptr, pipeline_);
}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) noexcept : Pipeline(std::move(other))
{
}

GraphicsPipeline& GraphicsPipeline::operator=(GraphicsPipeline&& other) noexcept
{
	Pipeline::operator=(std::move(other));
	return *this;
}

}
