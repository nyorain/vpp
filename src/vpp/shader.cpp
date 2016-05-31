#include <vpp/shader.hpp>

#include <string>
#include <fstream>
#include <cmath>
#include <cstring>

namespace vpp
{

//utility
namespace
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

}

//loadShader
vk::ShaderModule ShaderStage::loadModule(vk::Device device, const std::string& filename)
{
	vk::ShaderModule module {};

	auto code = readFile(filename, 1);
	if(code.empty())
	{
		std::cerr << "vpp::ShaderStage::loadModule: failed to load code from " << filename << "\n";
		return module;
	}

	vk::ShaderModuleCreateInfo moduleCreateInfo;
	moduleCreateInfo.codeSize = code.size();
	moduleCreateInfo.pCode = code.data();

	return vk::createShaderModule(device, moduleCreateInfo);
}

//stage
ShaderStage::ShaderStage(const Device& device, const CreateInfo& info) : Resource(device)
{
	stageInfo_.stage = info.stage;
	stageInfo_.pSpecializationInfo = info.specializationInfo;
	stageInfo_.pName = info.entry.c_str();
	stageInfo_.module = loadModule(vkDevice(), info.filename);
}

ShaderStage::~ShaderStage()
{
	if(stageInfo_.module) vk::destroyShaderModule(vkDevice(), stageInfo_.module, nullptr);
	stageInfo_ = {};
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
	swap(a.stageInfo_, b.stageInfo_);
}

//shader class
ShaderProgram::ShaderProgram(const Device& device, const std::vector<ShaderStage::CreateInfo>& infos)
	: Resource(device)
{
	addStages(infos);
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
