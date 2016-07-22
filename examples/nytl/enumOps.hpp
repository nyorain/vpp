/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <type_traits>

///\file
///\brief Defines binary operators for enums.

namespace nytl
{

template<typename T, typename U = std::underlying_type_t<T>>
class Flags
{
public:
	Flags() = default;
	Flags(T bit) : value_(static_cast<U>(bit)) {}
	Flags(bool, T bit) : value_(~static_cast<U>(bit)) {}
	Flags(const Flags& rhs) : value_(rhs.value()) {}
	Flags& operator=(const Flags& rhs) { value_ = rhs.value(); return *this; }
	Flags& operator|=(const Flags& rhs) { value_ |= rhs.value(); return *this; }
    Flags& operator&=(const Flags& rhs) { value_ &= rhs.value_; return *this; }
    Flags& operator^=(const Flags& rhs) { value_ ^= rhs.value(); return *this; }
    Flags operator|(const Flags& rhs) const { return Flags(rhs) |= *this; }
    Flags operator&(const Flags& rhs) const { return Flags(rhs) &= *this; }
    Flags operator^(const Flags& rhs) const { return Flags(rhs) ^= *this; }
    operator bool() const { return (value()); }
    bool operator!() const { return !(value()); }
    bool operator==(const Flags& rhs) const { return value_ == rhs.value(); }
    bool operator!=(const Flags& rhs) const { return value_ != rhs.value(); }

    explicit operator U() const { return value_; }
	const U& value() const { return value_; }

protected:
	U value_ {};
};

template <typename T> Flags<T> operator|(T bit, const Flags<T>& flags) { return flags | bit; }
template <typename T> Flags<T> operator&(T bit, const Flags<T>& flags) { return flags & bit; }
template <typename T> Flags<T> operator^(T bit, const Flags<T>& flags) { return flags ^ bit; }

}

#define NYTL_ENABLE_ENUM_OPS(T) \
	inline nytl::Flags<T> operator|(T a, T b) { return nytl::Flags<T>(a) | b; } \
	inline nytl::Flags<T> operator&(T a, T b) { return nytl::Flags<T>(a) & b; } \
	inline nytl::Flags<T> operator^(T a, T b) { return nytl::Flags<T>(a) ^ b; } \
	inline nytl::Flags<T> operator~(T bit) { return nytl::Flags<T>(false, bit); }
