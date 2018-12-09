// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/util/file.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

namespace vpp {

std::vector<std::byte> readFile(std::string_view filename, bool binary) {
	auto openmode = std::ios::ate;
	if(binary) {
		openmode |= std::ios::binary;
	}

	std::ifstream ifs(std::string{filename}, openmode);
	ifs.exceptions(std::ostream::failbit | std::ostream::badbit);

	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	std::vector<std::byte> buffer(size);
	auto data = reinterpret_cast<char*>(buffer.data());
	ifs.read(data, size);

	return buffer;
}

void writeFile(std::string_view filename, nytl::Span<const std::byte> buffer,
		bool binary) {

	auto openmode = binary ? std::ios::binary : std::ios::openmode{};
	std::ofstream ofs(std::string{filename}, openmode);
	ofs.exceptions(std::ostream::failbit | std::ostream::badbit);
	auto data = reinterpret_cast<const char*>(buffer.data());
	ofs.write(data, buffer.size());
}

} // namespace vpp
