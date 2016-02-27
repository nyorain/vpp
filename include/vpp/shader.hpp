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
		std::string filename;
		vk::ShaderStageFlagBits stage;
		const vk::SpecializationInfo* specializationInfo {nullptr};
		std::string entry { u8"main"};
	};

protected:
	std::vector<vk::PipelineShaderStageCreateInfo> stages_;

protected:
	void destroy();

public:
	ShaderProgram() = default;
	ShaderProgram(const Device& device, const std::vector<StageCreateInfo>& infos = {});
	~ShaderProgram();

	void create(const Device& device);

	void addStage(const vk::PipelineShaderStageCreateInfo& createInfo);
	void addStage(const StageCreateInfo& createInfo);

	void addStages(const std::vector<vk::PipelineShaderStageCreateInfo>& createInfo);
	void addStages(const std::vector<StageCreateInfo>& crrateInfo);

	const std::vector<vk::PipelineShaderStageCreateInfo>& stages() const { return stages_; }
};

}
