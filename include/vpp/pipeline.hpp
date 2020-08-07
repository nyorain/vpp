// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/shader.hpp>
#include <vpp/handles.hpp>
#include <vkpp/structs.hpp>
#include <string_view>

namespace vpp {

/// Utility for easier pipeline creation. Sets some useful defaults,
/// provides everything needed and connects it.
/// Checklist for things you probably have to set yourself:
/// - vertex input state stuff (all of vertexInfo), empty by default
/// - [assembly info] default is triangle fan
/// - [blend attachment] one attachment by default, enabled with add ops
/// - optional: depthStencil, tesselation, multisample, other dynamic states
class GraphicsPipelineInfo {
public:
	vk::PipelineVertexInputStateCreateInfo vertex;
	vk::PipelineInputAssemblyStateCreateInfo assembly;
	vk::PipelineRasterizationStateCreateInfo rasterization;
	vk::PipelineColorBlendStateCreateInfo blend;
	vk::PipelineViewportStateCreateInfo viewport;
	vk::PipelineDynamicStateCreateInfo dynamic;
	vk::PipelineMultisampleStateCreateInfo multisample;
	vk::PipelineTessellationStateCreateInfo tesselation;
	vk::PipelineDepthStencilStateCreateInfo depthStencil;

	vpp::ShaderProgram program;

public:
	GraphicsPipelineInfo();
	GraphicsPipelineInfo(vk::RenderPass, vk::PipelineLayout,
		vpp::ShaderProgram&& program,
		unsigned subpass = 0,
		vk::SampleCountBits = vk::SampleCountBits::e1);

	const vk::GraphicsPipelineCreateInfo& info();

	void pNext(void*);
	void flags(vk::PipelineCreateFlags);
	void layout(vk::PipelineLayout);
	void renderPass(vk::RenderPass rp);
	void subpass(unsigned);
	void base(vk::Pipeline = {});
	void base(int);

private:
	vk::GraphicsPipelineCreateInfo info_;
};

/// Saves a pipeline cache to the given filename.
/// Returns whether saving was succesful. Will not throw on failure.
bool save(vk::Device dev, vk::PipelineCache cache, std::string_view filename);
inline bool save(const PipelineCache& cache, std::string_view file) {
	return save(cache.device(), cache, file);
}

} // namespace vpp
