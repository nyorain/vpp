// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <cstddef> // std::size_t
#include <cmath> // std::ceil

namespace vpp {

/// Utility struct that represents an allocated range (offset + size).
struct Allocation {
	std::size_t offset {0};
	std::size_t size {0};

	std::size_t end() const { return offset + size; }
};

/// Aligns an offset to the given alignment.
/// An alignment of 0 zero will not change the offset.
/// An offset of 0 is treated as aligned with every possible alignment.
template<typename A, typename B> constexpr auto align(A offset, B alignment)
{
	if(offset == 0 || alignment == 0)
		return offset;

	auto rest = offset % alignment;
	return rest ? offset + (alignment - rest) : offset;
}

} // namespace vpp
