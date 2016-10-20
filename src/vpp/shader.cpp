#include <vpp/shader.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/file.hpp>

#include <string>
#include <fstream>
#include <cmath>
#include <cstring>
#include <iostream>

namespace vpp
{

vk::ShaderModule loadShaderModule(vk::Device dev, const StringParam& filename)
{
	auto code = readFile(filename, true);
	if(code.size() % 4) throw std::runtime_error(filename.string() + " has an invalid size");
	auto ptr = reinterpret_cast<const std::uint32_t*>(code.data());
	return loadShaderModule(dev, {ptr, code.size() / 4});
}

vk::ShaderModule loadShaderModule(vk::Device dev, const Range<std::uint32_t>& code)
{
	vk::ShaderModuleCreateInfo info;
	info.codeSize = code.size() * 4;
	info.pCode = code.data();

	return vk::createShaderModule(dev, info);
}

//ShaderModule
ShaderModule::ShaderModule(const Device& dev, const StringParam& filename) : ResourceHandle(dev)
{
	const static std::string errorMsg = "vpp::ShaderMoudle: failed to create from ";

	vkHandle() = loadShaderModule(dev, filename);
	if(!vkHandle()) throw std::runtime_error(errorMsg + filename.data());
}

ShaderModule::ShaderModule(const Device& dev, const Range<std::uint32_t>& code)
	: ResourceHandle(dev)
{
	vkHandle() = loadShaderModule(dev, code);
	if(!vkHandle()) throw std::runtime_error("vpp::ShaderModule: failed to create from given code");
}

ShaderModule::~ShaderModule()
{
	if(vkHandle()) vk::destroyShaderModule(device(), vkHandle());
}

//ShaderStage
ShaderStage::ShaderStage(const Device& dev, const StringParam& filename, const CreateInfo& info)
	: Resource(dev), info_(info), owned_(true)
{
	const static std::string errorMsg = "vpp::ShaderMoudle: failed to create from ";

	module_ = loadShaderModule(dev, filename);
	if(!module_) throw std::runtime_error(errorMsg + filename.data());
}

ShaderStage::ShaderStage(const Device& dev, const Range<std::uint32_t>& code,
	const CreateInfo& info) : Resource(dev), info_(info), owned_(true)
{
	module_ = loadShaderModule(dev, code);
	if(!module_) throw std::runtime_error("vpp::ShaderModule: failed to create from given code");
}

ShaderStage::ShaderStage(const Device& dev, vk::ShaderModule module, const CreateInfo& info)
	: Resource(dev), module_(module), info_(info), owned_(false)
{
}

ShaderStage::~ShaderStage()
{
	if(owned_ && module_) vk::destroyShaderModule(vkDevice(), module_, nullptr);
}

void ShaderStage::swap(ShaderStage& lhs) noexcept
{
	using std::swap;
	swap(resourceBase(), lhs.resourceBase());
	swap(info_, lhs.info_);
	swap(owned_, lhs.owned_);
	swap(module_, lhs.module_);
}

vk::PipelineShaderStageCreateInfo ShaderStage::vkStageInfo() const
{
	vk::PipelineShaderStageCreateInfo ret;
	ret.stage = info_.stage;
	ret.pSpecializationInfo = info_.specialization;
	ret.pName = info_.entry;
	ret.module = module_;

	return ret;
}

//shader class
ShaderProgram::ShaderProgram(const Device& device) : Resource(device)
{
}

void ShaderProgram::stage(const StringParam& filename, const ShaderStage::CreateInfo& createInfo)
{
	auto s = stage(createInfo.stage);
	if(s) *s = {device(), filename, createInfo};
	else stages_.emplace_back(device(), filename, createInfo);
}

void ShaderProgram::stage(const Range<std::uint32_t>& bytes,
	const ShaderStage::CreateInfo& createInfo)
{
	auto s = stage(createInfo.stage);
	if(s) *s = {device(), bytes, createInfo};
	else stages_.emplace_back(device(), bytes, createInfo);
}

void ShaderProgram::stage(vk::ShaderModule module, const ShaderStage::CreateInfo& createInfo)
{
	auto s = stage(createInfo.stage);
	if(s) *s = {device(), module, createInfo};
	else stages_.emplace_back(device(), module, createInfo);
}

ShaderStage* ShaderProgram::stage(vk::ShaderStageBits stage)
{
	for(auto& s : stages_) if(s.info().stage == stage) return &s;
	return nullptr;
}

const ShaderStage* ShaderProgram::stage(vk::ShaderStageBits stage) const
{
	for(auto& s : stages_) if(s.info().stage == stage) return &s;
	return nullptr;
}

std::vector<vk::PipelineShaderStageCreateInfo> ShaderProgram::vkStageInfos() const
{
	std::vector<vk::PipelineShaderStageCreateInfo> ret;
	ret.reserve(stages_.size());

	for(auto& stage : stages_)
		ret.push_back(stage.vkStageInfo());

	return ret;
}

//copy
ShaderProgram copy(const ShaderProgram& other)
{
	ShaderProgram ret(other.device());
	for(auto& stage : other.stages()) ret.stage(stage.vkShaderModule(), stage.info());
	return ret;
}

ShaderStage copy(const ShaderStage& other)
{
	return {other.device(), other.vkShaderModule(), other.info()};
}

}
