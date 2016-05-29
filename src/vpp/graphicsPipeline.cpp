#include <vpp/graphicsPipeline.hpp>
#include <vpp/vk.hpp>
#include <vpp/context.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>

#include <utility>

namespace vpp
{

//info default ctor
GraphicsPipeline::StatesCreateInfo::StatesCreateInfo(const vk::Viewport& viewportinfo)
{
	//needed data
	blendAttachments_.emplace_back();
	blendAttachments_.back().blendEnable = false;
	blendAttachments_.back().colorWriteMask =
		vk::ColorComponentBits::r |
		vk::ColorComponentBits::g |
		vk::ColorComponentBits::b |
		vk::ColorComponentBits::a;

	viewports_.emplace_back(viewportinfo);

	vk::Extent2D extent(viewportinfo.width(), viewportinfo.height());
	scissors_.push_back({{0, 0}, extent});

	//structs
	vk::StencilOpState stencil;
	stencil.failOp(vk::StencilOp::Keep);
	stencil.passOp(vk::StencilOp::Keep);
	stencil.compareOp(vk::CompareOp::Always);

	//fill
	inputAssembly.topology(vk::PrimitiveTopology::TriangleList);

	rasterization.polygonMode(vk::PolygonMode::Fill);
	rasterization.cullMode(vk::CullModeFlagBits::Back);
	rasterization.frontFace(vk::FrontFace::CounterClockwise);
	rasterization.depthClampEnable(true);
	rasterization.rasterizerDiscardEnable(false);
	rasterization.depthBiasEnable(false);
	rasterization.lineWidth(1.f);

	colorBlend.attachmentCount(blendAttachments_.size());
	colorBlend.pAttachments(blendAttachments_.data());

	viewport.viewportCount(viewports_.size());
	viewport.pViewports(viewports_.data());
	viewport.scissorCount(scissors_.size());
	viewport.pScissors(scissors_.data());

	depthStencil.depthTestEnable(true);
	depthStencil.depthWriteEnable(true);
	depthStencil.depthCompareOp(vk::CompareOp::LessOrEqual);
	depthStencil.depthBoundsTestEnable(false);
	depthStencil.stencilTestEnable(false);
	depthStencil.back(stencil);
	depthStencil.front(stencil);

	multisample.pSampleMask(nullptr);
	multisample.rasterizationSamples(vk::SampleCountFlagBits::e1);
}

//pipeline
GraphicsPipeline::GraphicsPipeline(const Device& device, const CreateInfo& createInfo)
{
	Pipeline::init(device);

	//vertexInfo
	vk::PipelineVertexInputStateCreateInfo vertexInfo;

	//Binding description
	std::size_t attributeCount = 0;
	for(auto& layout : createInfo.vertexBufferLayouts)
		attributeCount += layout->attributes.size();

	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
	bindingDescriptions.reserve(createInfo.vertexBufferLayouts.size());

	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.reserve(attributeCount);

	for(auto& layout : createInfo.vertexBufferLayouts)
	{
		std::size_t location = 0;
		std::size_t offset = 0;

		for(auto& attribute : layout->attributes)
		{
			attributeDescriptions.emplace_back();
			attributeDescriptions.back().location(location++);
			attributeDescriptions.back().binding(layout->binding);
			attributeDescriptions.back().format(attribute);
			attributeDescriptions.back().offset(offset);
			offset += formatSize(attribute) / 8;
		}

		bindingDescriptions.emplace_back();
		bindingDescriptions.back().binding(layout->binding);
		bindingDescriptions.back().stride(offset);
		bindingDescriptions.back().inputRate(vk::VertexInputRate::Vertex);
	}

	vertexInfo.vertexBindingDescriptionCount(bindingDescriptions.size());
	vertexInfo.pVertexBindingDescriptions(bindingDescriptions.data());
	vertexInfo.vertexAttributeDescriptionCount(attributeDescriptions.size());
	vertexInfo.pVertexAttributeDescriptions(attributeDescriptions.data());

	//pipeline layout
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	descriptorSetLayouts.reserve(createInfo.descriptorSetLayouts.size());

	for(auto& layout : createInfo.descriptorSetLayouts)
		descriptorSetLayouts.push_back(layout->vkDescriptorSetLayout());

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts(descriptorSetLayouts.data());

	vk::createPipelineLayout(vkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_);

	//dynamic state
	vk::PipelineDynamicStateCreateInfo dynamicState;
	dynamicState.pDynamicStates(createInfo.dynamicStates.data());
	dynamicState.dynamicStateCount(createInfo.dynamicStates.size());

	//create it
	//why is this needed? app crashes without copying it
	auto infos = createInfo.shader.vkStageInfos();
	vk::GraphicsPipelineCreateInfo pipelineInfo;

	pipelineInfo.layout(pipelineLayout_);
	pipelineInfo.pVertexInputState(&vertexInfo);
	pipelineInfo.pDynamicState(&dynamicState);
	pipelineInfo.renderPass(createInfo.renderPass);
	pipelineInfo.stageCount(infos.size());
	pipelineInfo.pStages(infos.data());
	pipelineInfo.pInputAssemblyState(&createInfo.states.inputAssembly);
	pipelineInfo.pRasterizationState(&createInfo.states.rasterization);
	pipelineInfo.pColorBlendState(&createInfo.states.colorBlend);
	pipelineInfo.pMultisampleState(&createInfo.states.multisample);
	pipelineInfo.pViewportState(&createInfo.states.viewport);
	pipelineInfo.pDepthStencilState(&createInfo.states.depthStencil);
	pipelineInfo.pTessellationState(nullptr);

	vk::createGraphicsPipelines(vkDevice(), 0, 1, &pipelineInfo, nullptr, &pipeline_);
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
