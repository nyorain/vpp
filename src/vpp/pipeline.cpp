// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/pipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// GraphicsPipelineInfo
GraphicsPipelineInfo::GraphicsPipelineInfo(vk::RenderPass renderPass,
		vk::PipelineLayout layout, ShaderProgram&& program, unsigned subpass,
		vk::SampleCountBits samples) : program(std::move(program)) {

	static const auto dynStates = {
		vk::DynamicState::viewport,
		vk::DynamicState::scissor};

	static const vk::PipelineColorBlendAttachmentState blendAttachment = {
		true,

		vk::BlendFactor::srcAlpha,
		vk::BlendFactor::oneMinusSrcAlpha,
		vk::BlendOp::add,

		vk::BlendFactor::srcAlpha,
		vk::BlendFactor::oneMinusSrcAlpha,
		vk::BlendOp::add,

		vk::ColorComponentBits::r |
			vk::ColorComponentBits::g |
			vk::ColorComponentBits::b |
			vk::ColorComponentBits::a,
	};

	info_.stageCount = program.vkStageInfos().size();
	info_.pStages = program.vkStageInfos().data();
	info_.layout = layout;
	info_.renderPass = renderPass;
	info_.subpass = subpass;

	assembly.topology = vk::PrimitiveTopology::triangleFan;

	rasterization.polygonMode = vk::PolygonMode::fill;
	rasterization.cullMode = vk::CullModeBits::none;
	rasterization.frontFace = vk::FrontFace::counterClockwise;
	rasterization.depthClampEnable = false;
	rasterization.rasterizerDiscardEnable = false;
	rasterization.depthBiasEnable = false;
	rasterization.lineWidth = 1.f;

	multisample.rasterizationSamples = samples;

	blend.attachmentCount = 1;
	blend.pAttachments = &blendAttachment;

	viewport.scissorCount = 1;
	viewport.viewportCount = 1;

	dynamic.dynamicStateCount = dynStates.size();
	dynamic.pDynamicStates = dynStates.begin();
}

const vk::GraphicsPipelineCreateInfo& GraphicsPipelineInfo::info() {
	info_.pStages = program.vkStageInfos().data();
	info_.stageCount = program.vkStageInfos().size();
	info_.pDynamicState = &dynamic;
	info_.pMultisampleState = &multisample;
	info_.pViewportState = &viewport;
	info_.pColorBlendState = &blend;
	info_.pRasterizationState = &rasterization;
	info_.pVertexInputState = &vertex;
	info_.pInputAssemblyState = &assembly;
	info_.pDepthStencilState = &depthStencil;
	info_.pTessellationState = &tesselation;
	return info_;
}

void GraphicsPipelineInfo::pNext(void* p) {
	info_.pNext = p;
}
void GraphicsPipelineInfo::flags(vk::PipelineCreateFlags flags) {
	info_.flags = flags;
}
void GraphicsPipelineInfo::layout(vk::PipelineLayout layout) {
	info_.layout = layout;
}
void GraphicsPipelineInfo::renderPass(vk::RenderPass rp) {
	info_.renderPass = rp;
}
void GraphicsPipelineInfo::subpass(unsigned s) {
	info_.subpass = s;
}
void GraphicsPipelineInfo::base(vk::Pipeline b) {
	info_.basePipelineIndex = -1;
	info_.basePipelineHandle = b;
	if(b) {
		info_.flags |= vk::PipelineCreateBits::derivative;
	}
}
void GraphicsPipelineInfo::base(int i) {
	info_.basePipelineIndex = i;
	info_.basePipelineHandle = {};
	if(i != -1) {
		info_.flags |= vk::PipelineCreateBits::derivative;
	}
}

} // namespace vpp
