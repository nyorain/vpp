#include <vpp/shader.hpp>
#include <vpp/call.hpp>

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

	VPP_CALL(vk::createShaderModule(device, &moduleCreateInfo, NULL, &module));
	return module;
}

//shader class
ShaderProgram::ShaderProgram(const Device& device, const std::vector<StageCreateInfo>& infos)
{

}
ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::addStage(const vk::PipelineShaderStageCreateInfo& createInfo)
{

}
void ShaderProgram::addStage(const StageCreateInfo& createInfo)
{

}

void ShaderProgram::addStages(const std::vector<vk::PipelineShaderStageCreateInfo>& createInfo)
{

}
void ShaderProgram::addStages(const std::vector<StageCreateInfo>& crrateInfo)
{
	
}

}
