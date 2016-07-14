#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

#include <string>
#include <vector>

namespace vpp
{

///Utility function that can be used to read binary shader files.
vk::ShaderModule loadShaderModule(vk::Device dev, const char* filename);
vk::ShaderModule loadShaderModule(vk::Device dev, const std::vector<std::uint8_t>& code);

class ShaderModule : public ResourceHandle<vk::ShaderModule>
{
public:
	ShaderModule() = default;
	ShaderModule(const Device& dev, const char* file);
	ShaderModule(const Device& dev, const std::vector<std::uint8_t>& bytes);
	~ShaderModule();

	ShaderModule(ShaderModule&& other) noexcept = default;
	ShaderModule& operator=(ShaderModule&& other) noexcept = default;
};

///Vulkan Shader Stage wrapper around owned or non-owned shader modules.
class ShaderStage : public Resource
{
public:
	struct CreateInfo
	{
		vk::ShaderStageBits stage;
		const vk::SpecializationInfo* specialization = nullptr;
		const char* entry = u8"main";
	};

public:
	ShaderStage() = default;
	ShaderStage(const Device& dev, vk::ShaderModule module, const CreateInfo& info);
	ShaderStage(const Device& dev, const char* name, const CreateInfo& info);
	ShaderStage(const Device& dev, const std::vector<std::uint8_t>& code, const CreateInfo& info);
	~ShaderStage();

	ShaderStage(ShaderStage&& other) noexcept;
	ShaderStage& operator=(ShaderStage other) noexcept;

	vk::PipelineShaderStageCreateInfo vkStageInfo() const;
	vk::ShaderModule vkShaderModule() const { return module_; }
	const CreateInfo& info() const { return info_; }
	bool owned() const { return owned_; }

	operator vk::PipelineShaderStageCreateInfo() const;
	friend void swap(ShaderStage& a, ShaderStage& b) noexcept;

protected:
	vk::ShaderModule module_ {};
	CreateInfo info_;
	bool owned_ = false; //whether it should be destructed or was external retrieved
};

///ShaderProgram with multiple stages for graphic pipelines.
class ShaderProgram : public Resource
{
public:
	ShaderProgram() = default;
	ShaderProgram(const Device& device);
	~ShaderProgram() = default;

	ShaderProgram(ShaderProgram&& other) noexcept = default;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept = default;

	///\{
	///Returns the given shader stage if there is any, nullptr otherwise.
	ShaderStage* stage(vk::ShaderStageBits stage);
	const ShaderStage* stage(vk::ShaderStageBits stage) const;
	///\}

	///\{
	///Changes or adds a new shader stage (depending on info::stage).
	void stage(const char* filename, const ShaderStage::CreateInfo& info);
	void stage(vk::ShaderModule module, const ShaderStage::CreateInfo& info);
	///\}

	std::vector<vk::PipelineShaderStageCreateInfo> vkStageInfos() const;
	const std::vector<ShaderStage>& stages() const { return stages_; }

protected:
	std::vector<ShaderStage> stages_;
};

///\{
///Duplicates a given ShaderProgram/ShaderModule. Note that this is implemented as extra
///function instead of a copy operator since it does not perform a deep copy. The new
///instance will still reference the same vk::ShaderModules as the old original one,
///so it must be assured that they remain when the new objects is used.
///It was chosen to implement a copy function for both of these types, since they
///are able to hold non-owned objects.
ShaderProgram copy(const ShaderProgram& other);
ShaderStage copy(const ShaderStage& other);
///\}

}
