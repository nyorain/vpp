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

//Inspried from https://github.com/irrequietus/typestring

#pragma once

#include <cstddef>
#include <tuple>

namespace nytl
{

namespace detail
{
	template<std::size_t N>
	constexpr std::size_t stringSize(const char (&)[N])
	{
		return N - 1;
	}

	template<typename T>
	constexpr std::size_t stringSize(const T& str)
	{
		return str.size();
	}

	//
	template<std::size_t i>
	struct StringTupleSize
	{
		template<typename... T>
		static constexpr std::size_t size(const std::tuple<T...>& tup)
		{
			return StringTupleSize<i - 1>::size(tup) + detail::stringSize(std::get<i>(tup));
		}
	};
	
	template<>
	struct StringTupleSize<0>
	{
		template<typename... T>
		static constexpr std::size_t size(const std::tuple<T...>& tup)
		{
			return detail::stringSize(std::get<0>(tup));
		}
	};
	
	
	template <std::size_t sub> 
	struct MultStringCharAt
	{
		template<typename... T>
		static constexpr char charAt(std::size_t i, const std::tuple<T...>& tup)
		{
			using detail::stringSize;
			return i < stringSize(std::get<sub>(tup)) 
				? std::get<sub>(tup)[stringSize(std::get<sub>(tup)) - i - 1] 
				: MultStringCharAt<sub - 1>::charAt(i - stringSize(std::get<sub>(tup)), tup);
		}
	};
	
	template <> 
	struct MultStringCharAt<0>
	{
		template<typename... T>
		static constexpr char charAt(std::size_t i, const std::tuple<T...>& tup)
		{
			using detail::stringSize;
			return i < stringSize(std::get<0>(tup))
				? std::get<0>(tup)[stringSize(std::get<0>(tup)) - i - 1] 
				: 'c';
		}
	};

}

class ConstString
{
public:
	const char* content_ {nullptr};
	const std::size_t size_ {0};

public:
	template<std::size_t N>
	constexpr ConstString(const char (&str)[N]) : content_(str), size_(N - 1) {} 

	constexpr ConstString(const char* content, std::size_t size) 
		: content_(content), size_(size) {}

	constexpr std::size_t size() const { return size_; }
	constexpr char operator[](std::size_t i) const { return content_[i]; }

	constexpr operator const char*() const { return content_; }
};

template<typename... T>
class MultipleConstString
{
public:
	using Size = std::size_t;

	static constexpr Size count_ = sizeof...(T);
	std::tuple<T...> strings_;

public:
	constexpr MultipleConstString(const T&... str) : strings_(str...) {}

	template<Size i>
	constexpr std::size_t subSize() const { return stringSize(std::get<i>(strings_)); }

	constexpr char operator[](std::size_t i) const
	{ return detail::MultStringCharAt<count_ - 1>::charAt(size() - i - 1, strings_); }

	constexpr std::size_t size() const 
	{ return detail::StringTupleSize<count_ - 1>::size(strings_); }
};

template<char... C>
class TemplateString
{
private:
	using Size = std::size_t;

    static constexpr char const content_[sizeof...(C) + 1] = { C...,'\0' };
    static constexpr Size size_ = sizeof...(C);

public:
    static constexpr char const* data() noexcept { return content_; }
    static constexpr Size size() noexcept { return size_; };

    static constexpr char const* cbegin() noexcept { return content_; }
    static constexpr char const* cend() noexcept { return size_; }

	static constexpr ConstString constString() noexcept { return ConstString(content_); }
};

template<char... C> constexpr char const TemplateString<C...>::content_[sizeof...(C) + 1];

//utility
//makeConstString
template<typename T> constexpr
const T& makeCSTR(const T& str)
{
	return str;
}

template<std::size_t N> constexpr
ConstString makeCSTR(const char (&a)[N])
{
	return ConstString(a);
}

template<typename... T> constexpr
auto makeMultipleConstString(const T&... str)
	-> MultipleConstString<decltype(makeCSTR(str))...>
{ 
	return MultipleConstString<decltype(makeCSTR(str))...>(makeCSTR(str) ...); 
}

template<std::size_t N, std::size_t M>
constexpr char tsgrab(char const(&c)[M]) noexcept
{ 
	return c[N < M ? N : M-1]; 
}

template<std::size_t N, typename T>
constexpr char tsgrab(const T& str) noexcept
{ 
	return str[N < str.size() ? N : str.size()]; 
}

template<char... X>
auto tspoke(TemplateString<X...>) 
	-> TemplateString<X...>;

template<char... X, char... Y>
auto tspoke(TemplateString<X...>, TemplateString<'\0'>, TemplateString<Y>...) 
	-> TemplateString<X...>;

template<char A, char... X, char... Y>
auto tspoke(TemplateString<X...>, TemplateString<A>, TemplateString<Y>...) 
	-> decltype(tspoke(TemplateString<X...,A>(), TemplateString<Y>()...));

template<char... C>
auto tspeek(TemplateString<C...>)
	-> decltype(tspoke(TemplateString<C>()...));
}

/* 2^0 = 1 */
#define NYTL_TS1(n,x) nytl::tsgrab<0x##n##0>(x)

/* 2^4 = 16 */
#define NYTL_TS16(n,x) \
        nytl::tsgrab<0x##n##0>(x), nytl::tsgrab<0x##n##1>(x) \
      , nytl::tsgrab<0x##n##2>(x), nytl::tsgrab<0x##n##3>(x) \
      , nytl::tsgrab<0x##n##4>(x), nytl::tsgrab<0x##n##5>(x) \
      , nytl::tsgrab<0x##n##6>(x), nytl::tsgrab<0x##n##7>(x) \
      , nytl::tsgrab<0x##n##8>(x), nytl::tsgrab<0x##n##9>(x) \
      , nytl::tsgrab<0x##n##A>(x), nytl::tsgrab<0x##n##B>(x) \
      , nytl::tsgrab<0x##n##C>(x), nytl::tsgrab<0x##n##D>(x) \
      , nytl::tsgrab<0x##n##E>(x), nytl::tsgrab<0x##n##F>(x)

/* 2^6 = 64 */
#define NYTL_TS64(n,x) \
        NYTL_TS16(n##0,x), NYTL_TS16(n##1,x), NYTL_TS16(n##2,x) \
      , NYTL_TS16(n##3,x)

//TemplateStringMacro
#define TS_(...) \
    decltype(nytl::tspeek(nytl::TemplateString<NYTL_TS64(,makeMultipleConstString(__VA_ARGS__, "\0"))>()))

