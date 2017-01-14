// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

///\file Defines StringParam classes for convinient and efficient string paramters.

#pragma once

#ifndef NYTL_INCLUDE_STRING_PARAM
#define NYTL_INCLUDE_STRING_PARAM

#include <string> // std::string
#include <cstring> // std:strcmp TODO: replace with constexpr function
#include "span.hpp" // nytl::Span

namespace nytl {

/// \brief This class can be used to effeciently accept string parameters of different types.
/// \details Usually, there are multiple possibilities to take string params:
/// - Take a std::string param (may introduce a huge overhead for raw string literals)
/// - Take a const char* param (inconvinient to call .c_str() on strings and rather C than C++)
/// - Overload the function for both versions or make it a template
/// Instead, one can simply take a StringParam parameter, which will introduce no
/// additional overhead and can be easily constructed from a const char* or a std::string.
/// It can then (for usage) be converted into a const char*, but note that it will not be possible
/// to get the length of the string later on. If you later need the size in any way, use
/// [nytl::SizedStringParam]() instead, which might me more efficient than later
/// querying the length with std::strlen.
/// \note This class only makes sense for functions that take non-mutable string parameters.
class StringParam {
public:
	constexpr StringParam() noexcept = default;
	constexpr StringParam(const char* chars) noexcept : data_(chars) {}
	StringParam(const std::string& string) noexcept : data_(string.c_str()) {}

	constexpr bool empty() const noexcept { return data_ == nullptr; }
	constexpr const char* data() const noexcept { return data_; }
	std::string string() const { return data_; }

	constexpr operator const char*() const noexcept { return data_; }

protected:
	const char* data_ = nullptr;
};

inline bool operator==(const StringParam& param, const char* other)
	{ return std::strcmp(param.data(), other) == 0; }
inline bool operator!=(const StringParam& param, const char* other)
	{ return std::strcmp(param.data(), other) != 0; }

inline bool operator==(const StringParam& param, const std::string& other)
	{ return std::strcmp(param.data(), other.c_str()) == 0; }
inline bool operator!=(const StringParam& param, const std::string& other)
	{ return std::strcmp(param.data(), other.c_str()) != 0; }

/// Computes the length of a null-terminated string at runtime.
/// The returned length does not include the first null-terminator.
/// Does basically the same as std::strlen but can do it at compile time.
constexpr std::size_t strlen(const char* str)
{
	std::size_t ret = 0u;
	while(*(str + ret) != '\0') ++ret;
	return ret;
}

/// \brief Class dervied from [nytl::StringParam]() that also holds the length of the stored string.
/// \details Can be used as parameter type for strings which size is needed in performance-critical
/// functions. Can be more efficient than querying the length with std::strlen since if an
/// object is constructed from a std::string it can simply store the length.
/// \note This class only makes sense for functions that take non-mutable string parameters.
class SizedStringParam : public StringParam {
public:
	constexpr SizedStringParam() noexcept = default;
	constexpr SizedStringParam(const char* chars, std::size_t length) noexcept
		: StringParam(chars), length_(length) {}
	constexpr SizedStringParam(const Span<char>& span) noexcept
		: StringParam(span.data()), length_(span.size()) {}
	constexpr SizedStringParam(const char* chars) noexcept
		: StringParam(chars), length_(strlen(chars)) {}
	constexpr SizedStringParam(const StringParam& other) noexcept
		: StringParam(other), length_(strlen(other.data())) {}
	SizedStringParam(const std::string& string) noexcept
		: StringParam(string), length_(string.size()) {}

	constexpr std::size_t length() const noexcept { return length_; }
	constexpr std::size_t size() const noexcept { return length_; }

	constexpr const char* begin() const noexcept { return data_; }
	constexpr const char* end() const noexcept { return data_ + length_; }

	constexpr const char* cbegin() const noexcept { return data_; }
	constexpr const char* cend() const noexcept { return data_ + length_; }

	constexpr operator const char*() const noexcept { return data_; }
	constexpr operator Span<const char>() const noexcept { return {data_, length_}; }

protected:
	std::size_t length_ = 0;
};

} // namespace nytl

#endif //header guard
