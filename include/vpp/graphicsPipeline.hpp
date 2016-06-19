#pragma once

#include <vpp/fwd.hpp>
#include <vpp/pipeline.hpp>

namespace vpp
{

class GraphicsPipeline : public Pipeline
{
public:
	struct CreateInfo; //defs.hpp
	struct States; //defs.hpp

public:
	GraphicsPipeline() = default;
	GraphicsPipeline(const Device& device, const CreateInfo& createInfo);
	~GraphicsPipeline() = default;

	GraphicsPipeline(GraphicsPipeline&& other) noexcept;
	GraphicsPipeline& operator=(GraphicsPipeline&& other) noexcept;
};

}
