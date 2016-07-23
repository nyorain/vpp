// Copyright © 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <cstdint>

namespace vk
{

template<typename T, typename U = std::uint32_t>
class Flags
{
public:
	constexpr Flags() = default;
	constexpr Flags(T bit) : value_(static_cast<U>(bit)) {}
	constexpr Flags(bool, T bit) : value_(~static_cast<U>(bit)) {}

	constexpr Flags& operator=(const Flags& rhs) { value_ = rhs.value(); return *this; }
	constexpr Flags& operator|=(const Flags& rhs) { value_ |= rhs.value(); return *this; }
    constexpr Flags& operator&=(const Flags& rhs) { value_ &= rhs.value_; return *this; }
    constexpr Flags& operator^=(const Flags& rhs) { value_ ^= rhs.value(); return *this; }
    constexpr Flags operator|(const Flags& rhs) const { return Flags(rhs) |= *this; }
    constexpr Flags operator&(const Flags& rhs) const { return Flags(rhs) &= *this; }
    constexpr Flags operator^(const Flags& rhs) const { return Flags(rhs) ^= *this; }
    constexpr operator bool() const { return (value()); }
    constexpr bool operator!() const { return !(value()); }
    constexpr bool operator==(const Flags& rhs) const { return value_ == rhs.value(); }
    constexpr bool operator!=(const Flags& rhs) const { return value_ != rhs.value(); }

    constexpr explicit operator U() const { return value_; }
	constexpr const U& value() const { return value_; }

protected:
	U value_ {};
};

template <typename T> auto constexpr operator|(T bit, const Flags<T>& flags) { return flags | bit; }
template <typename T> auto constexpr operator&(T bit, const Flags<T>& flags) { return flags & bit; }
template <typename T> auto constexpr operator^(T bit, const Flags<T>& flags) { return flags ^ bit; }

}

#define VPP_BITMASK_OPS(T) \
	constexpr vk::Flags<T> operator|(T a, T b) { return vk::Flags<T>(a) | b; } \
	constexpr vk::Flags<T> operator&(T a, T b) { return vk::Flags<T>(a) & b; } \
	constexpr vk::Flags<T> operator^(T a, T b) { return vk::Flags<T>(a) ^ b; } \
	constexpr vk::Flags<T> operator~(T bit) { return vk::Flags<T>(false, bit); }
