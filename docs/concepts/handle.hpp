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
	
//Handle
template<typename T>
class Handle
{
private:
	std::uint64_t data_ {};

public:
	Handle() = default;
	Handle(const T& n) : data_((std::uint64_t)n) {}
	
	operator T() const { return (T)data_; }
	operator bool() const { return data_ != 0; }

	T native() const { return (T)data_; }
	std::uint64_t data() cosnt { return data_; }
};

template<typename T>
bool operator==(const Handle<T>& a, const Handle<T>& b) { return a.data() == b.data(); }

template<typename T>
bool operator!=(const Handle<T>& a, const Handle<T>& b) { return a.data() != b.data(); }

///NullHandle
struct NullHandle {
	template<typename T> operator Handle<T>() const { return {}; }
} nullHandle;

}











namespace vk
{
	
//Handle
template<typename T>
struct Handle {
	std::uint64_t data_ {};
	operator bool() const { return data_; }
	operator std::uint64_t() const { return data_; }
	template<typename O> explicit operator O*() const { return (O*)data_; }
};


template<typename T>
bool operator==(const Handle<T>& a, const Handle<T>& b) { return a.data() == b.data(); }

template<typename T>
bool operator!=(const Handle<T>& a, const Handle<T>& b) { return a.data() != b.data(); }

///NullHandle
struct NullHandle {
	template<typename T> operator Handle<T>() const { return {}; }
} nullHandle;

}

#define VPP_DEFINE_HANDLE(name)\
	struct name##_T; \
	using name = vk::Handle<name##_T>;
