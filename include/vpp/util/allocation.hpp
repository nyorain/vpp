// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <cstddef> // std::size_t
#include <cmath> // std::ceil
#include <algorithm> // std::clamp
#include <type_traits> // std::is_unsigned

namespace vpp {

/// Utility struct that represents an allocated range (offset + size).
template<typename Size>
struct BasicAllocation {
	Size offset {0};
	Size size {0};
};

/// Returns the end of the given allocation (i.e. one-past-end address)
template<typename Size>
constexpr Size end(const BasicAllocation<Size>& a) {
	return a.offset + a.size;
}

template<typename S> constexpr bool
operator==(const BasicAllocation<S>& a, const BasicAllocation<S>& b) {
	return a.offset == b.offset && a.size == b.size;
}

template<typename S> constexpr bool
operator!=(const BasicAllocation<S>& a, const BasicAllocation<S>& b) {
	return a.offset != b.offset || a.size != b.size;
}

/// Aligns an offset to the given alignment.
/// An alignment of 0 zero will not change the offset.
/// An offset of 0 is treated as aligned with every possible alignment.
/// Undefined if either value is negative.
template<typename A, typename B>
constexpr auto align(A offset, B alignment) {
	if(offset == 0 || alignment == 0) {
		return offset;
	}

	auto rest = offset % alignment;
	return rest ? A(offset + (alignment - rest)) : A(offset);
}

/// Returns whether the first allocation fully contains the second one.
template<typename S> constexpr bool
contains(const BasicAllocation<S>& a, const BasicAllocation<S>& b) {
	return (b.offset == std::clamp(b.offset, a.offset, end(a)) &&
		   end(b) == std::clamp(end(b), a.offset, end(a)));
}

} // namespace vpp
