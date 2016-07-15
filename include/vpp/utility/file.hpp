#pragma once

#include <vpp/utility/stringParam.hpp>

#include <vector>
#include <cstdint>

namespace vpp
{

std::vector<uint8_t> readFile(const StringParam& filename, bool binary = true);
void writeFile(const StringParam& filename, const std::vector<uint8_t>& buffer, bool binary = true);
bool fileExists(const StringParam& filename);

}
