// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/util/file.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>

namespace vpp {

std::vector<std::uint8_t> readFile(nytl::StringParam filename, bool binary)
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

void writeFile(nytl::StringParam filename, nytl::Span<const std::uint8_t> buffer, bool binary)
{
	const static std::string errorMsg = "vpp::writeFile: couldnt open file ";

	auto openmode = std::ios::openmode{};
	if(binary) openmode = std::ios::binary;

	std::ofstream ofs(filename, openmode);
	if(!ofs.is_open()) throw std::runtime_error(errorMsg + filename.data());

	auto data = reinterpret_cast<const char*>(buffer.data());
	ofs.write(data, buffer.size());
}

bool fileExists(nytl::StringParam filename)
{
	struct stat s;
	return !stat(filename, &s);
}

} // namespace vpp
