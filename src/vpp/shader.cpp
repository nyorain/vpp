#include <vpp/shader.hpp>
#include <vpp/vk.hpp>

#include <string>
#include <fstream>
#include <cmath>
#include <cstring>
#include <iostream>

namespace vpp
{

std::vector<std::uint32_t> readFile(const std::string& filename, bool binary)
{
	 auto openmode = std::ios::ate;
	 if(binary) openmode |= std::ios::binary;

	std::ifstream ifs(filename, openmode);
	if(!ifs.is_open())
	{
		std::cerr << "vpp::readFile: failed to open " << filename << ", " << strerror(errno) << "\n";
		return {};
	}

	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<std::uint32_t> buffer(std::ceil(size / 4));
	auto data = reinterpret_cast<char*>(buffer.data());
	if(!ifs.read(data, size))
	{
		std::cerr << "vpp::readFile: failed to read " << filename << ", " << strerror(errno) << "\n";
		return {};
	}

	return buffer;
}

vk::ShaderModule loadShaderModule(vk::Device dev, const std::string& filename)
{
	auto code = readFile(filename, 1);
	if(code.empty())
	{
		std::cerr << "vpp::ShaderStage::loadModule: failed to load " << filename << "\n";
		return {};
	}

	return loadShaderModule(dev, code);
}

vk::ShaderModule loadShaderModule(vk::Device dev, const std::vector<std::uint32_t>& code)
{
	vk::ShaderModuleCreateInfo info;
	info.codeSize = code.size();
	info.pCode = code.data();

	return vk::createShaderModule(dev, info);
}

//ShaderModule
ShaderModule::ShaderModule(const Device& dev, const std::string& filename) : Resource(dev)
{
	module_ = loadShaderModule(dev, filename);
	if(!module_) throw std::runtime_error("vpp::ShaderModule: failed to create from " + filename);
}

ShaderModule::ShaderModule(const Device& dev, const std::vector<std::uint32_t>& code) : Resource(dev)
{
	module_ = loadShaderModule(dev, code);
	if(!module_) throw std::runtime_error("vpp::ShaderModule: failed to create from given code");
}

ShaderModule::~ShaderModule()
{
	if(module_ && device()) vk::destroyShaderModule(device(), module_);
}

ShaderModule::ShaderModule(ShaderModule&& other) noexcept
{
	swap(*this, other);
}

ShaderModule& ShaderModule::operator=(ShaderModule&& other) noexcept
{
	this->~ShaderModule();
	swap(*this, other);
	return *this;
}

void swap(ShaderModule& a, ShaderModule& b) noexcept
{
	using std::swap;
	swap(a.module_, b.module_);
	swap(a.device_, b.device_);
}

//ShaderStage
ShaderStage::ShaderStage(const Device& dev, const std::string& filename, const CreateInfo& info)
	: Resource(dev), owned_(true)
{
	module_ = loadShaderModule(dev, filename);
	if(!module_) throw std::runtime_error("vpp::ShaderModule: failed to create from " + filename);
}

ShaderStage::ShaderStage(const Device& dev, const std::vector<std::uint32_t>& code,
	const CreateInfo& info) : Resource(dev), owned_(true)
{
	module_ = loadShaderModule(dev, code);
	if(!module_) throw std::runtime_error("vpp::ShaderModule: failed to create from given code");
}

ShaderStage::ShaderStage(const Device& dev, vk::ShaderModule module, const CreateInfo& info)
	: Resource(dev), owned_(false), module_(module)
{
}

ShaderStage::~ShaderStage()
{
	if(owned_ && module_) vk::destroyShaderModule(vkDevice(), module_, nullptr);
	info_ = {};
}

ShaderStage::ShaderStage(ShaderStage&& other) noexcept
{
	swap(*this, other);
}

ShaderStage& ShaderStage::operator=(ShaderStage&& other) noexcept
{
	this->~ShaderStage();
	swap(*this, other);
	return *this;
}

void swap(ShaderStage& a, ShaderStage& b) noexcept
{
	using std::swap;
	swap(a.device_, b.device_);
	swap(a.info_, b.info_);
	swap(a.owned_, b.owned_);
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

void ShaderProgram::stage(const std::string& filename, const ShaderStage::CreateInfo& createInfo)
{
	auto s = stage(createInfo.stage);
	if(s) *s = {device(), filename, createInfo};
	else stages_.emplace_back(device(), filename, createInfo);
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

}
