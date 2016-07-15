#pragma once

#include <memory>
#include <vector>
#include <cmath>

namespace vpp
{

///Utility struct that represents an allocated range (offset + size).
struct Allocation
{
	std::size_t offset {0};
	std::size_t size {0};

	std::size_t end() const { return offset + size; }
};

///Aligns an offset.
template<typename T> T align(T offset, T alignment)
	{ return std::ceil(offset / double(alignment)) * alignment; }

}
