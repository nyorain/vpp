#pragma once

#include <vpp/fwd.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

///\{
///Create multiple vulkan graphic pipelines at once.
///Might be more efficient than constructing them individually.
std::vector<Pipeline> createGraphicsPipelines(const Device& dev,
	const Range<vk::GraphicsPipelineCreateInfo>& infos, vk::PipelineCache cache = {});

std::vector<Pipeline> createGraphicsPipelines(
	const Range<std::reference_wrapper<GraphicsPipelineBuilder>>& builder,
	vk::PipelineCache cache = {});
///\}

}
