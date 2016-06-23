#pragma once

#include <vector>
#include <cstdint>

namespace vpp
{

std::vector<uint8_t> readFile(const char* filename, bool binary = true);
void writeFile(const char* filename, const std::vector<uint8_t>& buffer, bool binary = true);
bool fileExists(const char* filename);

}
