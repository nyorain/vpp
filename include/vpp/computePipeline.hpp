// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/util/span.hpp>

#include <vector>

namespace vpp {

//T ODO: something about derivates when copying builder.

/// Create multiple vulkan compute pipelines at once.
/// Might be more efficient than constructing them individually.
std::vector<Pipeline> createComputePipelines(const Device& dev,
	nytl::Span<const vk::ComputePipelineCreateInfo>& infos, vk::PipelineCache cache = {});

std::vector<Pipeline> createComputePipelines(
	nytl::Span<const std::reference_wrapper<ComputePipelineBuilder>>& builder,
	vk::PipelineCache cache = {});

/// Utililty class for easier compute pipeline construction.
class ComputePipelineBuilder {
public:
	ComputePipelineBuilder() = default;

	ComputePipelineBuilder(const ComputePipelineBuilder& other);
	ComputePipelineBuilder& operator=(const ComputePipelineBuilder& other);

	Pipeline build(vk::PipelineCache cache = {});
	vk::ComputePipelineCreateInfo parse();

public:
	ShaderStage shaderStage;
	vk::PipelineLayout layout {};
	vk::PipelineCreateFlags flags {};
	vk::Pipeline baseHandle {};
};

} // namespace vpp
