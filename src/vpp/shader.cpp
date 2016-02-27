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

vk::ShaderModule loadShader(vk::Device device, const std::string& filename,
	vk::ShaderStageFlagBits stage)
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

	vk::createShaderModule(device, &moduleCreateInfo, NULL, &module);
	return module;
}

//shader class
ShaderProgram::ShaderProgram(const Device& device, const std::vector<StageCreateInfo>& infos)
	: Resource(device)
{
	addStages(infos);
}
ShaderProgram::~ShaderProgram()
{
	destroy();
}

void ShaderProgram::create(const Device& device)
{
	Resource::create(device);
}

void ShaderProgram::destroy()
{
	//todo
}

void ShaderProgram::addStage(const vk::PipelineShaderStageCreateInfo& createInfo)
{
	stages_.push_back(createInfo);
}
void ShaderProgram::addStage(const StageCreateInfo& createInfo)
{
	vk::PipelineShaderStageCreateInfo info;
	info.stage(createInfo.stage);
	info.pSpecializationInfo(createInfo.specializationInfo);
	info.pName(createInfo.entry.c_str());
	info.module(loadShader(vkDevice(), createInfo.filename, createInfo.stage));

	addStage(info);
}

void ShaderProgram::addStages(const std::vector<vk::PipelineShaderStageCreateInfo>& createInfo)
{
	for(auto& s : createInfo) addStage(s);
}
void ShaderProgram::addStages(const std::vector<StageCreateInfo>& createInfo)
{
	for(auto& s : createInfo) addStage(s);
}

}
