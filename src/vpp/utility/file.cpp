#include <vpp/utility/file.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

#include <sys/stat.h>

namespace vpp
{

std::vector<std::uint8_t> readFile(const StringParam& filename, bool binary)
{
	const static std::string errorMsg1 = "vpp::readFile: couldnt open file ";
	const static std::string errorMsg2 = "vpp::readFile: failed to read file ";

	auto openmode = std::ios::ate;
	if(binary) openmode |= std::ios::binary;

	std::ifstream ifs(filename, openmode);
	if(!ifs.is_open()) throw std::runtime_error(errorMsg1 + filename.data());

	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<std::uint8_t> buffer(size);
	auto data = reinterpret_cast<char*>(buffer.data());
	if(!ifs.read(data, size)) throw std::runtime_error(errorMsg2 + filename.data());

	return buffer;
}

void writeFile(const StringParam& filename, const std::vector<std::uint8_t>& buffer, bool binary)
{
	const static std::string errorMsg = "vpp::writeFile: couldnt open file ";

	auto openmode = std::ios::openmode{};
	if(binary) openmode = std::ios::binary;

	std::ofstream ofs(filename, openmode);
	if(!ofs.is_open()) throw std::runtime_error(errorMsg + filename.data());

	auto data = reinterpret_cast<const char*>(buffer.data());
	ofs.write(data, buffer.size());
}

bool fileExists(const StringParam& filename)
{
	struct stat s;
	return !stat(filename, &s);
}

}
