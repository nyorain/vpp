// The MIT License (MIT)
//
// Copyright (c) 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#pragma once

#ifndef NYTL_INCLUDE_FLAGS_HPP
#define NYTL_INCLUDE_FLAGS_HPP

#include <type_traits>

///\file
///\brief Defines binary operators for enums.

namespace nytl
{

template<typename T, typename U = std::underlying_type_t<T>>
class Flags
{
public:
	constexpr Flags() noexcept = default;
	constexpr Flags(T bit) noexcept : value_(static_cast<U>(bit)) {}
	constexpr Flags(bool, T bit) noexcept : value_(~static_cast<U>(bit)) {}
	~Flags() noexcept = default;

	constexpr Flags& operator=(const Flags& r) noexcept { value_ = r.value(); return *this; }
	constexpr Flags& operator|=(const Flags& r) noexcept { value_ |= r.value(); return *this; }
    constexpr Flags& operator&=(const Flags& r) noexcept { value_ &= r.value_; return *this; }
    constexpr Flags& operator^=(const Flags& r) noexcept { value_ ^= r.value(); return *this; }
    constexpr Flags operator|(const Flags& r) const noexcept { return Flags(r) |= *this; }
    constexpr Flags operator&(const Flags& r) const noexcept { return Flags(r) &= *this; }
    constexpr Flags operator^(const Flags& r) const noexcept { return Flags(r) ^= *this; }
    constexpr operator bool() const noexcept { return (value()); }
    constexpr bool operator!() const noexcept { return !(value()); }
    constexpr bool operator==(const Flags& rhs) const noexcept { return value_ == rhs.value(); }
    constexpr bool operator!=(const Flags& rhs) const noexcept { return value_ != rhs.value(); }

    constexpr explicit operator U() const noexcept { return value_; }
	constexpr const U& value() const noexcept { return value_; }

protected:
	U value_ {};
};

template <typename T> Flags<T> constexpr operator|(T bit, const Flags<T>& flags) noexcept
	{ return flags | bit; }
template <typename T> Flags<T> constexpr operator&(T bit, const Flags<T>& flags) noexcept
	{ return flags & bit; }
template <typename T> Flags<T> constexpr operator^(T bit, const Flags<T>& flags) noexcept
	{ return flags ^ bit; }

}

#define NYTL_FLAG_OPS(T) \
	constexpr nytl::Flags<T> operator|(T a, T b) noexcept { return nytl::Flags<T>(a) | b; } \
	constexpr nytl::Flags<T> operator&(T a, T b) noexcept { return nytl::Flags<T>(a) & b; } \
	constexpr nytl::Flags<T> operator^(T a, T b) noexcept { return nytl::Flags<T>(a) ^ b; } \
	constexpr nytl::Flags<T> operator~(T bit) noexcept { return nytl::Flags<T>(false, bit); }

#endif //header guard

namespace vk
{
	using nytl::Flags;
}
