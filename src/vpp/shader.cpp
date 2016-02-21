#include <vpp/shader.hpp>
#include <vpp/call.hpp>

#include <string>
#include <fstream>
#include <vector>

namespace vpp
{

namespace
{

/*
std::vector<char> readTextFile(const std::string& filename)
{
	std::string content;
	std::ifstream ifs(filename);

	if (!ifs.is_open())
	{
		std::cerr << "Failed to open file " << fileName << "\n";
		return "";
	}

	std::string line;
	while(!ifs.eof())
	{
		getline(ifs, line);
		content.append(line + "\n");
	}

	ifs.close();
	return content;
}
*/

std::vector<char> readFile(const std::string& filename, bool binary)
{
	 auto openmode = std::ios::in | std::ios::ate;
	 if(binary) openmode |= std::ios::binary

	std::ifstream ifs(filename, openmode);
	auto size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if(!file.read(buffer.data(), size)) return {};

	return buffer;
}

vk::ShaderModule loadShader(const std::string& fileName, vk::Device device,
	vk::ShaderStageFlagBits stage)
{
	vk::ShaderModule module;

	auto code = readFile(filename, 1);
	if(code.empty()) return module;

	VkShaderModuleCreateInfo moduleCreateInfo;
	moduleCreateInfo.codeSize(code.size());
	moduleCreateInfo.pCode(static_cast<std::uint32_t*>(code.data()));

	VPP_CALL(vk::createShaderModule(device, &moduleCreateInfo, NULL, &module));
	return module;
}

vk::ShaderModule loadShaderGLSL(const char *fileName, vk::Device device,
	vk::ShaderStageFlagBits stage)
{
	vk::ShaderModule module;

	auto code = read(filename, 0);
	if(code.empty()) return module;

	auto stageInt = static_cast<std::uint32_t>(stage);
	std::vector<std::uint32_t> header = {0x07230203, 0, stage};

	code.insert(code.cbegin(), header.cbegin(), header.cend());

	VkShaderModuleCreateInfo moduleCreateInfo;
	moduleCreateInfo.codeSize(3 * sizeof(std::uint32_t) + code.size() + 1);
	moduleCreateInfo.pCode(static_cast<std::uint32_t*>(code.data()));

	VPP_CALL(vk::createShaderModule(device, &moduleCreateInfo, NULL, &module));
	return module;
}

}
