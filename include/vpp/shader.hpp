#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <string>
#include <vector>

namespace vpp
{

///Wrapper around a single shader stage module.
class ShaderStage : public Resource
{
public:
	struct CreateInfo
	{
		std::string filename;
		vk::ShaderStageBits stage;
		const vk::SpecializationInfo* specializationInfo {nullptr};
		std::string entry { u8"main"};
	};

	static vk::ShaderModule loadModule(vk::Device dev, const std::string& file);

public:
	ShaderStage() = default;
	ShaderStage(const Device& device, const CreateInfo& info);
	~ShaderStage();

	ShaderStage(ShaderStage&& other) noexcept;
	ShaderStage& operator=(ShaderStage&& other) noexcept;

	const vk::PipelineShaderStageCreateInfo& vkStageInfo() const { return stageInfo_; }
	friend void swap(ShaderStage& a, ShaderStage& b) noexcept;

protected:
	vk::PipelineShaderStageCreateInfo stageInfo_;
};

//TODO: at the moment every single ShaderStage holds an extra word for the device pointer
//		just store the vk stage info instead of the ShaderStage to remove the overhead
//		Will also make it possible to use the same shadr module for multiple shader
//		programs which is important

///ShaderProgram with multiple stages for graphic pipelines.
class ShaderProgram : public Resource
{
public:
	ShaderProgram() = default;
	ShaderProgram(const Device& device, const std::vector<ShaderStage::CreateInfo>& infos = {});
	~ShaderProgram() = default;

	ShaderProgram(ShaderProgram&& other) noexcept = default;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept = default;

	void addStage(const ShaderStage::CreateInfo& createInfo);
	void addStages(const std::vector<ShaderStage::CreateInfo>& createInfo);

	const std::vector<ShaderStage>& stages() const { return stages_; }
	std::vector<vk::PipelineShaderStageCreateInfo> vkStageInfos() const;

protected:
	std::vector<ShaderStage> stages_;
};

}
