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

namespace nytl
{

namespace detail
{

template<typename T, typename = void>
struct Loader
{
	static bool call(std::istream& is, T& obj)
	{
		is >> obj;
		return 1;
	}
};

template<typename T>
struct Loader<T, void_t<decltype(T{}.load(std::cin))>>
{
	static bool call(std::istream& is, T& obj)
	{
		return obj.load(is);
	}
};

} //detail

///\ingroup utility
///\brief Load an object from an std::istream via operator/member function/custom overload.
///If your type does neither overload the >> operator nor have a load member function,
///specialize this template function with your own type.
template<typename T>
bool load(std::istream& is, T& obj)
{
	return detail::Loader<T>::call(is, obj);
}


namespace detail
{

//TODO: perfect forwarding needed here? Args/CArgs are explicitly specified...
//make_unique_wrapper, returns a unique_ptr for usual types and a void* for void type
template<typename Base, typename T, typename... Args> struct CreateWrapper
{
    static std::unique_ptr<Base> call(Args... args) { return std::make_unique<T>(args...); }
};

template<typename T, typename... Args> struct CreateWrapper<void, T, Args...>
{
    static void* call(Args... args) { return new T(args...); };
};

template<typename T, typename... Args> struct CreateWrapper<Any, T, Args...>
{
    static Any call(Args... args) { return Any(T(args...)); };
};

//createLoad
template<typename Base, typename T, typename... Args> struct CreateLoadWrapper
{
    static std::unique_ptr<Base> call(std::istream& is, Args... args)
	{
		auto ret = std::make_unique<T>(args...);
		if(!load(is, *ret)) return nullptr;
		return ret;
	}
};

template<typename T, typename... Args> struct CreateLoadWrapper<void, T, Args...>
{
    static void* call(std::istream& is, Args... args)
	{
		auto ret = new T(args...);
		if(!load(is, *ret)) return nullptr;
		return ret;
	};
};

template<typename T, typename... Args> struct CreateLoadWrapper<Any, T, Args...>
{
    static Any call(std::istream& is, Args... args)
	{
		auto ret = T(args...);
		if(!load(is, ret)) return Any();
		return Any(ret);
	};
};

//wrapper for checking if an object is valid
template<typename T>
struct CheckValidWrapper
{
	static bool call(const T& obj){ return obj != nullptr; }
};

template<>
struct CheckValidWrapper<Any>
{
	static bool call(const Any& obj){ return !obj.empty(); }
};

} //namespace detail

} //namespace nytl
