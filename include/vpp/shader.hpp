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
public:
	struct StageCreateInfo
	{
		std::string file;
		vk::ShaderStageFlagBits stage;
		const vk::SpecializationInfo* {nullptr};
		std::string entry {"main"};
	};

protected:
	std::vector<vk::PipelineShaderStageCreateInfo> stages_;

protected:
	ShaderProgram() = default;

	void create(const Device& device);
	void destroy();

public:
	ShaderProgram(const Device& device, const std::vector<);
	~ShaderProgram();

	void addStage(const vk::PipelineShaderStageCreateInfo& createInfo);
	void addStage(const StageCreateInfo& crrateInfo);

	void addStages(const std::vector<vk::PipelineShaderStageCreateInfo>& createInfo);
	void addStages(const std::vector<StageCreateInfo>& crrateInfo);

	const std::vector<vk::PipelineShaderStageCreateInfo>& stages() const { return stages_; }
};

}
