//This file is taken from nyorain/nytl nytl/stringParam
#pragma once

#include <string>
#include <cstring>
#include <vpp/utility/range.hpp>

namespace vpp
{

///This Overload class can be used to effeciently accept string parameters.
///Usually, there are 3 possibilities to take string params:
///- Take a const std::string& param (may introduce a huge overhead)
///- Take a const char* param (inconvinient to always call .c_str() on strings)
///- Overload the function for both versions (you have to declare 2 functions)
///Instead, one can simply take a const StringParam& parameter, which will introduce no
///additional overhead and easily constructs from const char* or a std::string.
///It can then (for usage) be converted into a const char*, but note that it will not be possible
///to get the length of the string later on. If you later need the size in any way, use
///SizedStringParam instead, which might me more efficient than later querying the length
///with std::strlen.
///\sa SizedStringParam
class StringParam
{
public:
	constexpr StringParam() = default;
	constexpr StringParam(const char* chars) : data_(chars) {}
	StringParam(const std::string& string) : data_(string.c_str())
		{ if(string.empty()) data_ = nullptr; } ///XXX: is this check needed/useful?

	constexpr bool empty() const { return data_ == nullptr; }
	constexpr const char* data() const { return data_; }

	constexpr operator const char*() const { return data_; }

public:
	const char* data_ = nullptr;
};

///Class dervied from StringParam that also holds the length of the stored string.
///Can be used as parameter type for strings which size is needed in performance-critical
///functions.
///\sa StringParam
class SizedStringParam : public StringParam
{
public:
	constexpr SizedStringParam() = default;
	SizedStringParam(const std::string& string) : StringParam(string), length_(string.size()) {}
	SizedStringParam(const char* chars) : StringParam(chars), length_(std::strlen(chars)) {}
	constexpr SizedStringParam(const Range<char>& range) : StringParam(range.data()),
		length_(range.size()) {}

	constexpr std::size_t length() const { return length_; }
	constexpr std::size_t size() const { return length_; }

	constexpr const char* begin() const { return data_; }
	constexpr const char* end() const { return data_ + length_; }

	constexpr const char* cbegin() const { return data_; }
	constexpr const char* cend() const { return data_ + length_; }

	constexpr operator const char*() const { return data_; }
	constexpr operator Range<char>() const { return {data_, length_}; }

public:
	std::size_t length_ = 0;
};

}
