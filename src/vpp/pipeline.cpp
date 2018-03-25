// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/pipeline.hpp>
#include <vpp/pipelineInfo.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// PipelineLayout
PipelineLayout::PipelineLayout(const Device& dev,
		const vk::PipelineLayoutCreateInfo& info) : ResourceHandle(dev) {
	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::PipelineLayout(const Device& dev,
		nytl::Span<const vk::DescriptorSetLayout> layouts,
		nytl::Span<const vk::PushConstantRange> ranges) : ResourceHandle(dev) {
	vk::PipelineLayoutCreateInfo info;
	info.setLayoutCount = layouts.size();
	info.pSetLayouts = layouts.data();
	info.pushConstantRangeCount = ranges.size();
	info.pPushConstantRanges = ranges.data();

	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::~PipelineLayout() {
	if(vkHandle()) {
		vk::destroyPipelineLayout(device(), vkHandle());
	}
}

// PipelineCache
PipelineCache::PipelineCache(const Device& dev) : ResourceHandle(dev) {
	handle_ = vk::createPipelineCache(dev, {});
}

PipelineCache::PipelineCache(const Device& dev,
		nytl::Span<const std::uint8_t> data) : ResourceHandle(dev) {
	handle_ = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::PipelineCache(const Device& dev, std::string_view filename,
		bool except) : ResourceHandle(dev) {

	std::vector<std::byte> data;
	try {
		data = readFile(filename);
	} catch(const std::exception& err) {
		dlg_info("PipelineCache: {}", err.what());
		if(except) {
			throw;
		}
	}

	handle_ = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::~PipelineCache() {
	if(vkHandle()) {
		vk::destroyPipelineCache(device(), vkHandle());
	}
}

bool save(vk::Device dev, vk::PipelineCache cache, std::string_view file) {
	auto data = vk::getPipelineCacheData(dev, cache);
	auto ptr = reinterpret_cast<const std::byte*>(data.data());

	try {
		writeFile(file, {ptr, data.size()});
	} catch(const std::exception& err) {
		dlg_warn("vpp::save(PipelineCache): {}", err.what());
		return false;
	}

	return true;
}

// Pipeline
Pipeline::~Pipeline()
{
	if(vkHandle()) {
		vk::destroyPipeline(device(), vkHandle());
	}
}

//
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
