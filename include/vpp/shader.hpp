#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

namespace vpp
{

vk::ShaderModule
loadShader(vk::Device dev, const std::string& file, vk::ShaderStageFlagBits stage);


class ShaderProgram : public Resource
{
protected:
	std::vector<vk::PipelineShaderStageCreateInfo> stages_;

protected:
	ShaderProgram() = default;

	void create(const Device& device);
	void destroy();

public:
	ShaderProgram(const Device& device);
	~ShaderProgram();

	void addStage(vk::PipelineShaderStageCreateInfo createInfo);
	void addStage(const std::string& file, vk::ShaderStageFlagBits stage,
		const vk::SpecializationInfo* = nullptr, const std::string& name = "main");

	const std::vector<vk::PipelineShaderStageCreateInfo>& stages() const { return stages_; }
};

}
