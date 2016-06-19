#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <string>
#include <vector>

namespace vpp
{

using ShaderModulePtr = std::shared_ptr<vk::ShaderModule>;
ShaderModulePtr loadShaderModule(vk::Device dev, const std::string& file);
ShaderModulePtr loadShaderModule(vk::Device dev, const std::vector<std::uint8_t>& data);

///Wrapper around a single shader stage module.
class ShaderStage
{
public:
	struct CreateInfo
	{
		vk::ShaderStageBits stage;
		const vk::SpecializationInfo* specializationInfo {}; //optional, if set, must stay valid
		const char* name = u8"main"; //utf-8 encoded entry point, must stay valid until destruction
	};

public:
	ShaderStage() = default;
	ShaderStage(const Device& device, const std::string& file, const CreateInfo& info);
	ShaderStage(const Device& device, ShaderModulePtr module, const CreateInfo& info);
	~ShaderStage();

	ShaderStage(ShaderStage&& other) noexcept;
	ShaderStage& operator=(ShaderStage&& other) noexcept;

	vk::PipelineShaderStageCreateInfo vkStageInfo() const;
	vk::ShaderModule vkShaderModule() const { return *module_; }
	ShaderModulePtr shaderModule() const { return module_; }

	operator vk::PipelineShaderStageCreateInfo() const;
	friend void swap(ShaderStage& a, ShaderStage& b) noexcept;

protected:
	CreateInfo info_;
	ShaderModulePtr module_;
};

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

	std::vector<vk::PipelineShaderStageCreateInfo> vkStageInfos() const;

protected:
	std::vector<ShaderStage> stages_;
};

}
