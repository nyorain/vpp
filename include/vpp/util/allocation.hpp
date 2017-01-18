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

/// Aligns an offset to the given alignment
template<typename A, typename B> constexpr auto align(A offset, B alignment)
	{ return std::ceil(offset / double(alignment)) * alignment; }

} // namespace vpp
