#include <vpp/vk.hpp>
#include <vpp/shader.hpp>

struct CompleteRenderPassInfo {
	vk::RenderPassCreateInfo renderPassInfo;
	std::vector<vk::AttachmentDescription> attachments;
	std::vector<vk::SubpassDescription> subpasses;
	std::vector<vk::SubpassDependency> dependencies;

	const vk::RenderPassCreateInfo& connect();
	operator vk::RenderPassCreateInfo() { return connect(); }
};

const vk::RenderPassCreateInfo& CompleteRenderPassInfo::connect()
{
	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = subpasses.size();
	renderPassInfo.pSubpasses = subpasses.data();
	renderPassInfo.dependencyCount = dependencies.size();
	renderPassInfo.pDependencies = dependencies.data();
	return renderPassInfo;
}


namespace init::rp {

CompleteRenderPassInfo defaultColorOnly(vk::Format colorFormat);
CompleteRenderPassInfo defaultColorDepth(vk::Format colorFormat, vk::Format depthFormat = {});

}


//
class GraphicsPipelineInfo {
public:
	const vk::GraphicsPipelineCreateInfo& connect();
	operator const vk::GraphicsPipelineCreateInfo&();

public:
	vk::GraphicsPipelineCreateInfo createInfo;
	vpp::ShaderProgram shaderStages;

	vk::PipelineVertexInputStateCreateInfo vertexInput {};
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly {};
	vk::PipelineTessellationStateCreateInfo tessellation {};
	vk::PipelineViewportStateCreateInfo viewport {};
	vk::PipelineRasterizationStateCreateInfo rasterization {};
	vk::PipelineMultisampleStateCreateInfo multisample {};
	vk::PipelineDepthStencilStateCreateInfo depthStencil {};
	vk::PipelineColorBlendStateCreateInfo colorBlend {};
	vk::PipelineDynamicStateCreateInfo dynamic {};

	std::vector<vk::DynamicState> dynamicStates;
	std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
	std::vector<vk::Viewport> viewports;
	std::vector<vk::Rect2D> scissors;
};

namespace init {

enum class GfxPipelineBits {
	depth,
	blend
};

using GfxPipelineFlags = nytl::Flags<GfxPipelineBits>;

GraphicsPipelineInfo gfxPipeline(vk::RenderPass, vk::PipelineLayout layout, GfxPipelineFlags = {});

} // namespace init

// - implementation
const vk::GraphicsPipelineCreateInfo& GraphicsPipelineInfo::connect()
{
	dynamic.dynamicStateCount = dynamicStates.size();
	dynamic.pDynamicStates = dynamicStates.data();

	colorBlend.attachmentCount = blendAttachments.size();
	colorBlend.pAttachments = blendAttachments.data();

	viewport.viewportCount = viewports.size();
	viewport.pViewports = viewports.data();

	viewport.scissorCount = scissors.size();
	viewport.pScissors = scissors.data();

	createInfo.pVertexInputState = &vertexInput;
	createInfo.pInputAssemblyState = &inputAssembly;
	createInfo.pTessellationState = &tessellation;
	createInfo.pViewportState = &viewport;
	createInfo.pRasterizationState = &rasterization;
	createInfo.pMultisampleState = &multisample;
	createInfo.pDepthStencilState = &depthStencil;
	createInfo.pColorBlendState = &colorBlend;
	createInfo.pDynamicState = &dynamic;

	return createInfo;
}

GraphicsPipelineInfo::operator const vk::GraphicsPipelineCreateInfo&()
{
	return connect();
}

namespace init {

GraphicsPipelineInfo gfxPipeline(vk::RenderPass rp, vk::PipelineLayout layout,
	GfxPipelineFlags flags)
{
	GraphicsPipelineInfo ret;
	ret.createInfo.renderPass = rp;
	ret.createInfo.layout = layout;

	ret.inputAssembly.topology = vk::PrimitiveTopology::triangleList;
	ret.rasterization.polygonMode = vk::PolygonMode::fill;
	ret.rasterization.cullMode = vk::CullModeBits::back;
	ret.rasterization.frontFace = vk::FrontFace::counterClockwise;
	ret.rasterization.depthClampEnable = true;
	ret.rasterization.rasterizerDiscardEnable = false;
	ret.rasterization.depthBiasEnable = false;
	ret.rasterization.lineWidth = 1.f;
	ret.multisample.rasterizationSamples = vk::SampleCountBits::e1;

	ret.viewports.emplace_back();
	ret.scissors.emplace_back();

	ret.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};

	ret.blendAttachments.emplace_back();

	if(flags & GfxPipelineBits::blend) {
		ret.blendAttachments.back().blendEnable = true;
		ret.blendAttachments.back().alphaBlendOp = vk::BlendOp::add;
		ret.blendAttachments.back().srcColorBlendFactor = vk::BlendFactor::srcAlpha;
		ret.blendAttachments.back().dstColorBlendFactor = vk::BlendFactor::oneMinusSrcAlpha;
		ret.blendAttachments.back().srcAlphaBlendFactor = vk::BlendFactor::one;
		ret.blendAttachments.back().dstAlphaBlendFactor = vk::BlendFactor::zero;
		ret.blendAttachments.back().colorWriteMask =
			vk::ColorComponentBits::r |
			vk::ColorComponentBits::g |
			vk::ColorComponentBits::b |
			vk::ColorComponentBits::a;
	}

	if(flags & GfxPipelineBits::depth) {
		ret.depthStencil.depthWriteEnable = true;
		ret.depthStencil.depthTestEnable = true;
		ret.depthStencil.depthCompareOp = vk::CompareOp::lessOrEqual;
		ret.depthStencil.depthBoundsTestEnable = true;

	}

	return ret;
}

} // namespace init
