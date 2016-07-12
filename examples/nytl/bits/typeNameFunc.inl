/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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

#include <nytl/system.hpp>
#include <string>
#include <cstring>

namespace nytl
{

//detail typename helper
namespace detail
{

template<typename T>
struct TypeNameFuncHelper
{
	static const char* call()
	{
		#if defined(NYTL_COMPILER_MSC)
			constexpr auto prefix = "nytl::detail::TypeNameFuncHelper<";
			constexpr auto suffix1 = ">::call";
			constexpr auto suffix2 = "";
			constexpr auto typeRepeats = 1;
		#elif defined(NYTL_COMPILER_CLANG)
 			constexpr auto prefix = 
				"static nytl::detail::StringWrapper nytl::detail::TypeNameFuncHelper<";
 			constexpr auto suffix1 = ">::call() [T = ";
 			constexpr auto suffix2 = "]";
 			constexpr auto typeRepeats = 2;
		#elif defined(NYTL_COMPILER_GNU)
			constexpr auto prefix = "static nytl::detail::StringWrapper "
		"nytl::detail::TypeNameFuncHelper<T>::call() [with T = ";
 			constexpr auto suffix1 = "]";
 			constexpr auto suffix2 = "";
 			constexpr auto typeRepeats = 1;
		#else
 			//#warning "nytl::TypeNameFuncHelper does not support your compiler."
			constexpr auto prefix = "";
 			constexpr auto suffix1 = "";
 			constexpr auto suffix2 = "";
 			constexpr auto typeRepeats = 1;
			return typeid(T).name();
		#endif

 			constexpr auto fLength = sizeof(NYTL_PRETTY_FUNCTION) - 1u;
 			constexpr auto pLength = sizeof(prefix) - 1u;
 			constexpr auto sLength = sizeof(suffix1) - 1u + sizeof(suffix2) - 1u;
 			constexpr auto tLength = (fLength - (pLength + sLength)) / typeRepeats;

			static char name[tLength + 1u] = {};
			std::memcpy(name, NYTL_PRETTY_FUNCTION + pLength, tLength - 1u);
			return name;
	}
};

template<typename T>
std::string typeNameFunc()
{
	return std::string(TypeNameFuncHelper<T>::call());
}

} //detail

} //nytl
