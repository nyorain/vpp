#include <vpp/utility/file.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

#include <sys/stat.h>

namespace vpp
{

std::vector<std::uint8_t> readFile(const char* filename, bool binary)
{
	auto openmode = std::ios::ate;
	if(binary) openmode |= std::ios::binary;

	std::ifstream ifs(filename, openmode);
	if(!ifs.is_open()) throw std::runtime_error(std::string("couldnt open file ") + filename);

	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<std::uint8_t> buffer(size);
	auto data = reinterpret_cast<char*>(buffer.data());
	if(!ifs.read(data, size)) throw std::runtime_error(std::string("failed to read") + filename);

	return buffer;
}

void writeFile(const char* filename, const std::vector<std::uint8_t>& buffer, bool binary)
{
	auto openmode = std::ios::openmode{};
	if(binary) openmode = std::ios::binary;

	std::ofstream ofs(filename, openmode);
	if(!ofs.is_open()) throw std::runtime_error(std::string("couldnt open file ") + filename);

	auto data = reinterpret_cast<const char*>(buffer.data());
	ofs.write(data, buffer.size());
}

bool fileExists(const char* filename)
{
	struct stat s;
	return !stat(filename, &s);
}

}
