// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#ifndef NYTL_INCLUDE_NON_COPYABLE
#define NYTL_INCLUDE_NON_COPYABLE

namespace nytl {

/// Derive from this class to make it impossible to copy objects of the derived class.
/// \module utility
class NonCopyable {
	constexpr NonCopyable(const NonCopyable&) = delete;
	constexpr NonCopyable& operator =(const NonCopyable&) = delete;

protected:
	constexpr NonCopyable() noexcept = default;
	constexpr NonCopyable(NonCopyable&&) noexcept = default;
	constexpr NonCopyable& operator=(NonCopyable&&) noexcept = default;
};

/// Derive from this class to make it impossible to copy or move objects of the derived class.
/// \module utility
class NonMovable {
	constexpr NonMovable(const NonMovable&) = delete;
	constexpr NonMovable& operator =(const NonMovable&) = delete;
	constexpr NonMovable(NonMovable&&) = delete;
	constexpr NonMovable& operator=(NonMovable&&) = delete;

protected:
	constexpr NonMovable() noexcept = default;
};

} // namespace nytl

#endif // header guard
