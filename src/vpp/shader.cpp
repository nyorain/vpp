#include <vpp/shader.hpp>

#include <string>
#include <fstream>
#include <vector>

namespace vpp
{

//utility
namespace
{

std::vector<char> readFile(const std::string& filename, bool binary)
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

	std::vector<char> buffer(size);
	if(!ifs.read(buffer.data(), size))
	{
		std::cerr << "vpp::readFile: failed to read " << filename << ", " << strerror(errno) << "\n";
		return {};
	}

	return buffer;
}

}

//loadShader
vk::ShaderModule loadShader(vk::Device device, const std::string& filename)
{
	vk::ShaderModule module;

	auto code = readFile(filename, 1);
	if(code.empty())
	{
		std::cerr << "vpp::loadShader: failed to load code from " << filename << "\n";
		return module;
	}

	vk::ShaderModuleCreateInfo moduleCreateInfo;
	moduleCreateInfo.codeSize(code.size());
	moduleCreateInfo.pCode(reinterpret_cast<std::uint32_t*>(code.data()));

	vk::createShaderModule(device, &moduleCreateInfo, nullptr, &module);
	return module;
}

//stage
ShaderStage::ShaderStage(const Device& device, const CreateInfo& info)
{
	init(device, info);
}

ShaderStage::~ShaderStage()
{
	destroy();
}

ShaderStage::ShaderStage(ShaderStage&& other) noexcept
{
	using std::swap;

	device_ = other.device_;
	swap(stageInfo_, other.stageInfo_);
}

ShaderStage& ShaderStage::operator=(ShaderStage&& other) noexcept
{
	destroy();

	stageInfo_ = std::move(other.stageInfo_);
	other.stageInfo_ = vk::PipelineShaderStageCreateInfo {};

	return *this;
}

void ShaderStage::init(const Device& device, const CreateInfo& info)
{
	destroy();
	Resource::init(device);

	stageInfo_.stage(info.stage);
	stageInfo_.pSpecializationInfo(info.specializationInfo);
	stageInfo_.pName(info.entry.c_str());
	stageInfo_.module(loadShader(vkDevice(), info.filename));
}

void ShaderStage::destroy()
{
	if(stageInfo_.module())
	{
		vk::destroyShaderModule(vkDevice(), stageInfo_.module(), nullptr);
	}

	stageInfo_ = vk::PipelineShaderStageCreateInfo {};
}

//shader class
ShaderProgram::ShaderProgram(const Device& device, const std::vector<ShaderStage::CreateInfo>& infos)
	: Resource(device)
{
	addStages(infos);
}

void ShaderProgram::init(const Device& device, const std::vector<ShaderStage::CreateInfo>& infos)
{
	Resource::init(device);
	addStages(infos);
}

void ShaderProgram::destroy()
{
	stages_.clear();
}

void ShaderProgram::addStage(const ShaderStage::CreateInfo& createInfo)
{
	stages_.emplace_back(device(), createInfo);
}

void ShaderProgram::addStages(const std::vector<ShaderStage::CreateInfo>& createInfo)
{
	for(auto& s : createInfo) addStage(s);
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
