#pragma once

#include <vpp/fwd.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/utility/range.hpp>

#include <vector>

namespace vpp
{

//TODO: something about derivates when copying builder.
//really ugly that the extra file is needed here...
class GraphicsPipelineBuilder; //defs.hpp

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
