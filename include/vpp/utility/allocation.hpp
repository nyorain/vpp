#pragma once

#include <memory>
#include <vector>

namespace vpp
{

///Utility struct that represents an allocated range (offset + size).
struct Allocation
{
	std::size_t offset {0};
	std::size_t size {0};

	std::size_t end() const { return offset + size; }
};

}
