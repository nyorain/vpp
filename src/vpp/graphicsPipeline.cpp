#include <vpp/graphicsPipeline.hpp>
#include <vpp/defs.hpp>
#include <vpp/context.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>

#include <utility>

namespace vpp
{

//info default ctor
// GraphicsPipeline::States::States(const vk::Viewport& viewportinfo)
// {
// 	//needed data
// 	blendAttachments.emplace_back();
// 	blendAttachments.back().blendEnable = false;
// 	blendAttachments.back().colorWriteMask =
// 		vk::ColorComponentBits::r |
// 		vk::ColorComponentBits::g |
// 		vk::ColorComponentBits::b |
// 		vk::ColorComponentBits::a;
//
// 	viewports.emplace_back(viewportinfo);
//
// 	vk::Extent2D extent {std::uint32_t(viewportinfo.width), std::uint32_t(viewportinfo.height)};
// 	scissors.push_back({{0, 0}, extent});
//
// 	//structs
// 	vk::StencilOpState stencil;
// 	stencil.failOp = vk::StencilOp::keep;
// 	stencil.passOp = vk::StencilOp::keep;
// 	stencil.compareOp = vk::CompareOp::always;
//
// 	//fill
// 	inputAssembly.topology = vk::PrimitiveTopology::triangleList;
//
// 	rasterization.polygonMode = vk::PolygonMode::fill;
// 	rasterization.cullMode = vk::CullModeBits::back;
// 	rasterization.frontFace = vk::FrontFace::counterClockwise;
// 	rasterization.depthClampEnable = true;
// 	rasterization.rasterizerDiscardEnable = false;
// 	rasterization.depthBiasEnable = false;
// 	rasterization.lineWidth = 1.f;
//
// 	colorBlend.attachmentCount = blendAttachments.size();
// 	colorBlend.pAttachments = blendAttachments.data();
//
// 	// viewport.viewportCount = viewports.size();
// 	// viewport.pViewports = viewports.data();
// 	// viewport.scissorCount = scissors.size();
// 	// viewport.pScissors = scissors.data();
//
// 	viewport.viewportCount = 1;
// 	viewport.scissorCount = 1;
//
// 	depthStencil.depthTestEnable = true;
// 	depthStencil.depthWriteEnable = true;
// 	depthStencil.depthCompareOp = vk::CompareOp::lessOrEqual;
// 	depthStencil.depthBoundsTestEnable = false;
// 	depthStencil.stencilTestEnable = false;
// 	depthStencil.back = stencil;
// 	depthStencil.front = stencil;
//
// 	multisample.pSampleMask = nullptr;
// 	multisample.rasterizationSamples = vk::SampleCountBits::e1;
// }
//
// //pipeline
// GraphicsPipeline::GraphicsPipeline(const Device& dev, const CreateInfo& createInfo) : Pipeline(dev)
// {
// 	//vertexInfo
// 	vk::PipelineVertexInputStateCreateInfo vertexInfo {};
//
// 	//Binding description
// 	std::size_t attributeCount = 0;
// 	for(auto& layout : createInfo.vertexBufferLayouts)
// 		attributeCount += layout.get().attributes.size();
//
// 	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
// 	bindingDescriptions.reserve(createInfo.vertexBufferLayouts.size());
//
// 	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
// 	attributeDescriptions.reserve(attributeCount);
//
// 	for(auto& layout : createInfo.vertexBufferLayouts)
// 	{
// 		std::size_t location = 0;
// 		std::size_t offset = 0;
//
// 		for(auto& attribute : layout.get().attributes)
// 		{
// 			attributeDescriptions.emplace_back();
// 			attributeDescriptions.back().location = location++;
// 			attributeDescriptions.back().binding = layout.get().binding;
// 			attributeDescriptions.back().format = attribute;
// 			attributeDescriptions.back().offset = offset;
// 			offset += formatSize(attribute) / 8;
// 		}
//
// 		bindingDescriptions.emplace_back();
// 		bindingDescriptions.back().binding = layout.get().binding;
// 		bindingDescriptions.back().stride = offset;
// 		bindingDescriptions.back().inputRate = vk::VertexInputRate::vertex;
// 	}
//
// 	vertexInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
// 	vertexInfo.pVertexBindingDescriptions = bindingDescriptions.data();
// 	vertexInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
// 	vertexInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
//
// 	//pipeline layout
// 	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
// 	descriptorSetLayouts.reserve(createInfo.descriptorSetLayouts.size());
//
// 	for(auto& layout : createInfo.descriptorSetLayouts) descriptorSetLayouts.push_back(layout.get());
//
// 	vk::PipelineLayoutCreateInfo pipelineLayoutInfo {};
// 	pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
// 	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
//
// 	pipelineLayout_ = vk::createPipelineLayout(vkDevice(), pipelineLayoutInfo);
//
// 	//create it
// 	vk::GraphicsPipelineCreateInfo pipelineInfo;
//
// 	//dynamic state
// 	vk::PipelineDynamicStateCreateInfo dynamicState;
// 	if(!createInfo.dynamicStates.empty())
// 	{
// 		dynamicState.pDynamicStates = createInfo.dynamicStates.data();
// 		dynamicState.dynamicStateCount = createInfo.dynamicStates.size();
// 		pipelineInfo.pDynamicState = &dynamicState;
// 	}
//
// 	auto infos = createInfo.shader.vkStageInfos();
// 	pipelineInfo.stageCount = infos.size();
// 	pipelineInfo.pStages = infos.data();
//
// 	pipelineInfo.pViewportState = &createInfo.states.viewport;
// 	pipelineInfo.layout = pipelineLayout_;
// 	pipelineInfo.pVertexInputState = &vertexInfo;
// 	pipelineInfo.renderPass = createInfo.renderPass;
// 	pipelineInfo.subpass = createInfo.subpass;
// 	pipelineInfo.pInputAssemblyState = &createInfo.states.inputAssembly;
// 	pipelineInfo.pRasterizationState = &createInfo.states.rasterization;
// 	pipelineInfo.pColorBlendState = &createInfo.states.colorBlend;
// 	pipelineInfo.pMultisampleState = &createInfo.states.multisample;
// 	pipelineInfo.pDepthStencilState = &createInfo.states.depthStencil;
// 	pipelineInfo.pTessellationState = nullptr;
//
// 	vk::createGraphicsPipelines(vkDevice(), createInfo.cache, 1, pipelineInfo, nullptr, vkHandle());
// }
//
// GraphicsPipeline::GraphicsPipeline(const Device& dev, vk::Pipeline pipeline)
// {
//
// }
//
// std::vector<vk::GraphicsPipelineCreateInfo>
// GraphicsPipeline::parseInfos(const Range<CreateInfo>& infos)
// {
// 	std::vector<vk::GraphicsPipelineCreateInfo> ret;
// 	return ret;
// }
//
// std::vector<GraphicsPipeline> GraphicsPipeline::create(const Device& dev,
// 	const Range<CreateInfo>& infos)
// {
// 	auto vkInfos = parseInfos(infos);
// 	return vk::createGraphicsPipelines(dev, {}, vkInfos);
// }



GraphicsPipelineBuilder::GraphicsPipelineBuilder(const Device& dev, vk::RenderPass rp,
	unsigned int subpass) : shader(dev), renderPass(rp), subpass(xsubpass)
{
	dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};

	//fill
	states.inputAssembly.topology = vk::PrimitiveTopology::triangleList;

	states.rasterization.polygonMode = vk::PolygonMode::fill;
	states.rasterization.cullMode = vk::CullModeBits::none;
	states.rasterization.frontFace = vk::FrontFace::counterClockwise;
	states.rasterization.depthClampEnable = true;
	states.rasterization.rasterizerDiscardEnable = false;
	states.rasterization.depthBiasEnable = false;
	states.rasterization.lineWidth = 1.f;

	//blendAttachments
	states.blendAttachments.emplace_back();
	states.blendAttachments.back().blendEnable = false;
	states.blendAttachments.back().colorWriteMask =
		vk::ColorComponentBits::r |
		vk::ColorComponentBits::g |
		vk::ColorComponentBits::b |
		vk::ColorComponentBits::a;

	//stencil
	vk::StencilOpState stencil;
	states.stencil.failOp = vk::StencilOp::keep;
	states.stencil.passOp = vk::StencilOp::keep;
	states.stencil.compareOp = vk::CompareOp::always;

	states.depthStencil.depthTestEnable = true;
	states.depthStencil.depthWriteEnable = true;
	states.depthStencil.depthCompareOp = vk::CompareOp::lessOrEqual;
	states.depthStencil.depthBoundsTestEnable = false;
	states.depthStencil.stencilTestEnable = false;
	states.depthStencil.back = stencil;
	states.depthStencil.front = stencil;

	//multisample
	states.multisample.pSampleMask = nullptr;
	states.multisample.rasterizationSamples = vk::SampleCountBits::e1;
}

Pipeline GraphicsPipelineBuilder::build(vk::PipelineCache cache) const
{
	vk::GraphicsPipelineCreateInfo info;
	auto layout = parse(info);
	auto pipeline = vk::createGraphicsPipelines(shader.device(), cache, {info});
	return {shader.device(), pipeline, layout, true};
}

vk::PipelineLayout GraphicsPipelineBuilder::parse(vk::GraphicsPIpelineCreateInfo& ret) const
{
	vk::GraphicsPipelineCreateInfo ret;

	//Binding description
	std::size_t attributeCount = 0;
	for(auto& layout : vertexBufferLayouts)
		attributeCount += layout.get().attributes.size();

	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
	bindingDescriptions.reserve(vertexBufferLayouts.size());

	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.reserve(attributeCount);

	for(auto& layout : vertexBufferLayouts)
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
		bindingDescriptions.back().inputRate = vk::VertexInputRate::vertex; ///XXX: paramterize this
	}

	vk::PipelineVertexInputStateCreateInfo vertexInfo {};
	vertexInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
	vertexInfo.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//dynamic state
	vk::PipelineDynamicStateCreateInfo dynamicState;
	if(!dynamicStates.empty())
	{
		dynamicState.pDynamicStates = dynamicStates.data();
		dynamicState.dynamicStateCount = dynamicStates.size();
		ret.pDynamicState = &dynamicState;
	}

	auto infos = shader.vkStageInfos();
	ret.stageCount = infos.size();
	ret.pStages = infos.data();

	ret.pViewportState = &createInfo.states.viewport;
	ret.layout = pipelineLayout;
	ret.pVertexInputState = &vertexInfo;
	ret.renderPass = renderPass;
	ret.subpass = subpass;
	ret.pInputAssemblyState = &states.inputAssembly;
	ret.pRasterizationState = &states.rasterization;
	ret.pColorBlendState = &states.colorBlend;
	ret.pMultisampleState = &states.multisample;
	ret.pDepthStencilState = &states.depthStencil;
	ret.pTessellationState = nullptr;

	return pipelineLayout;
}

std::vector<Pipeline> createGraphicsPipelines(const Device& dev,
	const Range<vk::GraphicsPipelineCreateInfo>& infos, vk::PipelineCache cache)
{
	auto pipelines = vk::createGraphicsPipelines(dev, cache, infos);
	std::vector<Pipeline> ret;
	ret.reserve(pipelines.size())
	for(auto& p : pipelines) ret.push_back(p);
	return ret;
}

std::vector<Pipeline> createGraphicsPipelines(const Range<GraphicsPipelineBuilder>& builder,
	vk::PipelineCache cache)
{
	if(builder.empty()) return {};

	std::vector<vk::GraphicsPipelineCreateInfo> infos;
	infos.reserve(builder.size());
	for(auto& b : builder) infos.push_back(b.parse());

	return createGraphicsPipelines(builder.front().shader.device(), infos, cache);
}

}
