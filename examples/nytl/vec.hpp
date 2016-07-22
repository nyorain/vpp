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

///\file
///\brief Includes the Vec template class as well as Vec typedefs and utility functions.

#pragma once

#include <nytl/tmp.hpp>
#include <nytl/scalar.hpp>
#include <nytl/misc.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <limits>
#include <stdexcept>

namespace nytl
{


template<std::size_t D, typename T> class Vec;

//typedefs
template<typename T = float> using Vec2 = Vec<2, T>;
template<typename T = float> using Vec3 = Vec<3, T>;
template<typename T = float> using Vec4 = Vec<4, T>;

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2ui = Vec2<unsigned int>;
using Vec2d = Vec2<double>;
using Vec2c = Vec2<char>;
using Vec2uc = Vec2<unsigned char>;
using Vec2l = Vec2<long>;
using Vec2ul = Vec2<unsigned long>;
using Vec2b = Vec2<bool>;

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3ui = Vec3<unsigned int>;
using Vec3d = Vec3<double>;
using Vec3c = Vec3<char>;
using Vec3uc = Vec3<unsigned char>;
using Vec3l = Vec3<long>;
using Vec3ul = Vec3<unsigned long>;
using Vec3b = Vec3<bool>;

using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;
using Vec4ui = Vec4<unsigned int>;
using Vec4d = Vec4<double>;
using Vec4c = Vec4<char>;
using Vec4uc = Vec4<unsigned char>;
using Vec4l = Vec4<long>;
using Vec4ul = Vec4<unsigned long>;
using Vec4b = Vec4<bool>;

//better use dynmiacsize_type = 0? 
//error messages with this one are kinda disgusting

///Constant that can be used as dimension parameter for the Vector to make its
///size variable (like std::vector), while still oferring all mathematical
///operators.
///You have to include <nytl/dynVec.hpp> to make it work.
constexpr std::size_t dynamicSize = std::numeric_limits<std::size_t>::max();


//Can be used if a parameter should be a Vec for more than one value or a scalar for exactly one.
namespace detail
{
	template<std::size_t D, typename P> struct VecScalar { using type = Vec<D, P>; };
	template<typename P> struct VecScalar<1, P> { using type = P; };
	template<typename P> struct VecScalar<0, P> { };
}

template<std::size_t D, typename P> using VecScalar = typename detail::VecScalar<D, P>::type;

//For serveral operations with/for Vec and its specializations, parameters are passed by by-copy 
//instead of by-reference. This have to be done because otherwise you could not perform 
//certain operations of the Vector in realtion to its current state.
//Example: v /= v[0]. If v[0] is passed as reference here, the components of the Vector are
//devided by difference values, since v[0] changes during the operation. If we pass it as
//copy instead, it might be more expensive, but we get at least the expected results.

///\brief The Vec class represents a sized group of \c D values of type \c T.
///\ingroup math
template<std::size_t D, typename T> class Vec
{
public:
	static constexpr std::size_t dim = D;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = Pointer;
    using ConstIterator = ConstPointer;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = std::ptrdiff_t;

	using RangeError = std::out_of_range;

	//stl
	using size_type = Size;
    using value_type = Value;
	using reference = Reference;
	using const_reference = ConstReference;
	using pointer = Pointer;
	using const_pointer = ConstPointer;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
	using difference_type = Difference;

public:
    constexpr Size size() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    Value data_[dim] {};

public:
    template<typename... Args, typename = typename
		std::enable_if_t<std::is_convertible< std::tuple<Args...>, TypeTuple<Value, dim>>::value>>
    Vec(Args&&... args) noexcept : data_{Value(args)...} {}

	explicit Vec(Size) noexcept {}

    Vec() noexcept = default;
    ~Vec() noexcept = default;

    Vec(const VecType& lhs) noexcept = default;
    VecType& operator=(const VecType& lhs) noexcept = default;

    Vec(VecType&& lhs) noexcept = default;
    VecType& operator=(VecType&& lhs) noexcept = default;

    //operator
    template <std::size_t OD, typename ot> VecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> VecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> VecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> VecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> VecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> VecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> VecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> VecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> VecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> VecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> VecType& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> VecType& operator <<=(OT lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

    VecType operator-() const 
		{ VecType ret{}; for(size_t i(0); i < size(); i++) ret[i] -= (*this)[i]; return ret; }

    template <Size OD, typename OT, typename = 
		typename std::enable_if<!std::is_reference<OT>::value>::type>
	operator Vec<OD, OT>() const 
	{ 
		auto ret = Vec<OD, OT> (size()); 
		for(size_t i(0); i < min(size(), ret.size()); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<Size S>
	Vec<S, T> subVec(Size position = 0) const
	{
		auto ret = Vec<S, T> {};
		for(std::size_t i(0); i < min(S, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(Size position = 0, Size psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    ConstPointer data() const noexcept { return data_; }
    Pointer data() noexcept { return data_; }

    void fill(const Value& val) { for(auto& v : data_) v = val; }

    Iterator begin() noexcept { return data_; }
    ConstIterator begin() const noexcept { return data_; }
    ConstIterator cbegin() const noexcept { return data_; }

    Iterator end() noexcept { return begin() + dim; }
    ConstIterator end() const noexcept { return begin() + dim; }
    ConstIterator cend() const noexcept { return begin() + dim; }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i){ return data_[i]; }
    ConstReference operator[](Size i) const { return data_[i]; }

    Reference at(Size i)
		{ if(i >= dim) throw RangeError("nytl::Vec::at: out of range"); return data_[i]; }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw RangeError("nytl::Vec::at: out of range"); return data_[i]; }

    Reference front() noexcept { return data_[0]; }
    ConstReference front() const noexcept { return data_[0]; }

    Reference back() noexcept { return data_[dim - 1]; }
    ConstReference back() const noexcept { return data_[dim - 1]; }

	void swap(VecType& other){ std::swap(data_, other.data_); }
};

template<typename T> class Vec<2, T>
{
public:
	static constexpr std::size_t dim = 2;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = Pointer;
    using ConstIterator = ConstPointer;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = std::ptrdiff_t;

	using RangeError = std::out_of_range;

	//stl
	using size_type = Size;
    using value_type = Value;
	using reference = Reference;
	using const_reference = ConstReference;
	using pointer = Pointer;
	using const_pointer = ConstPointer;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
	using difference_type = Difference;

public:
    constexpr Size size() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
	Value x {};
 	Value y {};

public:
	Vec(Value a, Value b) noexcept : x(a), y(b) {}
	explicit Vec(Size) noexcept {}

	Vec() noexcept = default;
	~Vec() noexcept = default;

	Vec(const VecType& lhs) = default;
	VecType& operator=(const VecType& lhs) = default;

    Vec(VecType&& lhs) noexcept = default;
    VecType& operator=(VecType&& lhs) noexcept = default;

    //operator
    template <Size OD, typename ot> VecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OValue> VecType& operator +=(OValue lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OValue> VecType& operator -=(OValue lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OValue> VecType& operator *=(OValue lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OValue> VecType& operator /=(OValue lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OValue> VecType& operator %=(OValue lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OValue> VecType& operator |=(OValue lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OValue> VecType& operator ^=(OValue lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OValue> VecType& operator &=(OValue lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    VecType operator-() const { return VecType(-x, -y); }

    template <Size OD, typename OT, typename = 
		typename std::enable_if<!std::is_reference<OT>::value>::type>
	operator Vec<OD, OT>() const 
	{ 
		auto ret = Vec<OD, OT> (size()); 
		for(size_t i(0); i < min(size(), ret.size()); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<Size S>
	Vec<S, Value> subVec(Size position = 0) const
	{
		auto ret = Vec<S, Value>{};
		for(Size i(0); i < min(S, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, Value> subVec(Size position = 0, Size psize = -1) const
	{
		auto ret = Vec<dynamicSize, Value>(size);
		for(Size i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    ConstPointer data() const noexcept { return &x; }
    Pointer data() noexcept { return &x; }

    void fill(const Value& val) { x = val; y = val; }

    Iterator begin() noexcept { return &x; }
    ConstIterator begin() const noexcept { return &x; }
    ConstIterator cbegin() const noexcept { return &x; }

    Iterator end() noexcept { return &y + 1; }
    ConstIterator end() const noexcept { return &y + 1; }
    ConstIterator cend() const noexcept { return &y + 1; }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i){ return *(data() + i); }
    ConstReference operator[](Size i) const { return *(data() + i); }

    Reference at(Size i)
		{ if(i >= dim) throw RangeError("nytl::Vec::at"); return *(data() + i); }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw RangeError("nytl::Vec::at"); return *(data() + i); }

    Reference front() noexcept { return x; }
    ConstReference front() const noexcept { return x; }

    Reference back() noexcept { return y; }
    ConstReference back() const noexcept { return y; }

	void swap(VecType& other){ std::swap(x, other.x); std::swap(y, other.y); }
};


template<typename T> class Vec<3, T>
{
public:
	static constexpr std::size_t dim = 3;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = Pointer;
    using ConstIterator = ConstPointer;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = std::ptrdiff_t;

	using RangeError = std::out_of_range;

	//stl
	using size_type = Size;
    using value_type = Value;
	using reference = Reference;
	using const_reference = ConstReference;
	using pointer = Pointer;
	using const_pointer = ConstPointer;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
	using difference_type = Difference;

public:
    constexpr Size size() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    Value x {};
    Value y {};
    Value z {};

public:
	Vec(Value a, Value b, Value c) noexcept : x(a), y(b), z(c) {}
	explicit Vec(Size) noexcept {}

    Vec() noexcept = default;
	~Vec() noexcept = default;

	Vec(const VecType& lhs) noexcept = default;
	VecType& operator=(const VecType& lhs) noexcept = default;

    Vec(VecType&& lhs) noexcept = default;
    VecType& operator=(VecType&& lhs) noexcept = default;

    //operator
    template <Size OD, typename ot> VecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> VecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> VecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> VecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> VecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> VecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> VecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> VecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> VecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    VecType operator-() const { return VecType(-x, -y, -z); }

    template <Size OD, typename OT, typename = 
		typename std::enable_if<!std::is_reference<OT>::value>::type>
	operator Vec<OD, OT>() const 
	{ 
		auto ret = Vec<OD, OT> (size()); 
		for(size_t i(0); i < min(size(), ret.size()); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<Size S>
	Vec<S, Value> subVec(Size position = 0) const
	{
		auto ret = Vec<S, Value>{};
		for(Size i(0); i < min(S, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(Size position = 0, Size psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(Size i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    ConstPointer data() const noexcept { return &x; }
    Pointer data() noexcept { return &x; }

    void fill(const value_type& val) { x = val; y = val; z = val; }

    Iterator begin() noexcept { return &x; }
    ConstIterator begin() const noexcept { return &x; }
    ConstIterator cbegin() const noexcept { return &x; }

    Iterator end() noexcept { return &z + 1; }
    ConstIterator end() const noexcept { return &z + 1; }
    ConstIterator cend() const noexcept { return &z + 1; }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i){ return *(data() + i); }
    ConstReference operator[](Size i) const { return *(data() + i); }

    Reference at(Size i)
		{ if(i >= dim) throw RangeError("nytl::Vec::at"); return data() + i; }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw std::out_of_range("nytl::Vec::at"); return data() + i; }

    Reference front() noexcept { return x; }
    ConstReference front() const noexcept { return x; }

    Reference back() noexcept { return z; }
    ConstReference back() const noexcept { return z; }

	void swap(VecType& other)
		{ std::swap(x, other.x); std::swap(y, other.y); std::swap(z, other.z); }

    //custom
    Vec2<value_type> xy() const noexcept { return Vec2<value_type>(x,y); }
    Vec2<value_type> yz() const noexcept { return Vec2<value_type>(y,z); }
    Vec2<value_type> xz() const noexcept { return Vec2<value_type>(x,z); }
};


template<typename T> class Vec<4, T>
{
public:
	static constexpr std::size_t dim = 4;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = Pointer;
    using ConstIterator = ConstPointer;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = std::ptrdiff_t;

	using RangeError = std::out_of_range;

	//stl
	using size_type = Size;
    using value_type = Value;
	using reference = Reference;
	using const_reference = ConstReference;
	using pointer = Pointer;
	using const_pointer = ConstPointer;
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using reverse_iterator = ReverseIterator;
	using const_reverse_iterator = ConstReverseIterator;
	using difference_type = Difference;

public:
    constexpr Size size() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    Value x {};
    Value y {};
    Value z {};
    Value w {};

public:
	Vec(value_type a, value_type b, value_type c, value_type d) noexcept : x(a), y(b), z(c), w(d) {}
	explicit Vec(size_type) noexcept {}

    Vec() noexcept = default;
	~Vec() noexcept = default;

	Vec(const VecType& lhs) noexcept = default;
	VecType& operator=(const VecType& lhs) noexcept = default;

    Vec(VecType&& lhs) noexcept = default;
    VecType& operator=(VecType&& lhs) noexcept = default;

    //operator
    template <Size OD, typename ot> VecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i];  return *this; }
    template <Size OD, typename ot> VecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i];  return *this; }

    template<typename OT> VecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs;  return *this; }
    template<typename OT> VecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs;  return *this; }
    template<typename OT> VecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs;  return *this; }
    template<typename OT> VecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs;  return *this; }
    template<typename OT> VecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs;  return *this; }
    template<typename OT> VecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs;  return *this; }
    template<typename OT> VecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs;  return *this; }
    template<typename OT> VecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs;  return *this; }

    VecType operator-() const { return  VecType(-x, -y, -z, -w); }

    template <Size OD, typename OT, typename = 
		typename std::enable_if<!std::is_reference<OT>::value>::type>
	operator Vec<OD, OT>() const
	{ 
		auto ret = Vec<OD, OT> (size()); 
		for(size_t i(0); i < min(size(), ret.size()); i++) 
			ret[i] = (*this)[i]; 
		return ret; 
	}

	template<Size S>
	Vec<S, Value> subVec(Size position = 0) const
	{
		auto ret = Vec<S, Value>{};
		for(Size i(0); i < min(S, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}	

	Vec<dynamicSize, T> subVec(Size position = 0, Size psize = -1) const
	{
		auto ret = Vec<dynamicSize, T>(size);
		for(std::size_t i(0); i < min(psize, size() - position); ++i)
			ret[i] = (*this)[position + i];

		return ret;
	}

    //stl norm stuff, std::array
    ConstPointer data() const noexcept { return &x; }
    Pointer data() noexcept { return &x; }

    void fill(const Value& val) { x = val; y = val; z = val; w = val; }

    Iterator begin() noexcept { return &x; }
    ConstIterator begin() const noexcept { return &x; }
    ConstIterator cbegin() const noexcept { return &x; }

    Iterator end() noexcept { return &w + 1; }
    ConstIterator end() const noexcept { return &w + 1; }
    ConstIterator cend() const noexcept { return &w + 1; }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i){ return *(data() + i); }
    ConstReference operator[](Size i) const { return *(data() + i); }

    Reference at(Size i)
		{ if(i >= dim) throw RangeError("nytl::Vec::at"); return data() + i; }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw std::out_of_range("nytl::Vec::at"); return data() + i; }

    Reference front() noexcept { return x; }
    ConstReference front() const noexcept { return x; }

    Reference back() noexcept { return w; }
    ConstReference back() const noexcept { return w; }

	void swap(VecType& other) { std::swap(x, other.x); std::swap(y, other.y); 
		std::swap(z, other.z); std::swap(w, other.w); }

    //custom
    Vec2<T> xy() const noexcept { return Vec2<T>(x,y); }
    Vec2<T> xz() const noexcept { return Vec2<T>(x,z); }
    Vec2<T> xw() const noexcept { return Vec2<T>(x,w); }
    Vec2<T> yz() const noexcept { return Vec2<T>(y,z); }
    Vec2<T> yw() const noexcept { return Vec2<T>(y,w); }
    Vec2<T> zw() const noexcept { return Vec2<T>(z,w); }

    Vec3<T> xyz() const noexcept { return Vec3<T>(x,y,z); }
    Vec3<T> xyw() const noexcept { return Vec3<T>(x,y,w); }
    Vec3<T> xzw() const noexcept { return Vec3<T>(x,z,w); }
    Vec3<T> yzw() const noexcept { return Vec3<T>(y,z,w); }
};

template<std::size_t D, typename T> constexpr std::size_t Vec<D, T>::dim;
template<typename T> constexpr std::size_t Vec<2, T>::dim;
template<typename T> constexpr std::size_t Vec<3, T>::dim;
template<typename T> constexpr std::size_t Vec<4, T>::dim;

//invalid specialization - therefore not specified
template<typename T> class Vec<0, T>;

//reference Vec, include <nytl/refVec.hpp> to make those work!
template<std::size_t D, typename T> class Vec<D, T&>;
template<typename T> class Vec<2, T&>;
template<typename T> class Vec<3, T&>;
template<typename T> class Vec<4, T&>;

//Dynamic storage Vec, include <nytl/dynVec.hpp> to make this work!
template<typename T> class Vec<dynamicSize, T>;

//Invalid for now.
//If someone finds a legit use case for this one post an issue on github nyorain/nytl
//Something like dynRefVec. Technically possible but not that useful.
template<typename T> class Vec<dynamicSize, T&>;

//operators/utility
#include <nytl/bits/vec.inl>

}


//////////////////////////////////////////////////////////////////////////////////////////////
/// \class nytl::Vec
/// \ingroup math
///
/// The template parameter D defines the dimension of the Vector.
/// The template parameter T defines the type of the Vectors components.
///
/// In most of the cases you do not have to care about the template parameters because there are
/// typedefs for the common used types, e.g.:
/// \li nytl::Vec2f, nytl::Vec3f and nytl::Vec4f for float Vecs
/// \li nytl::Vec2i, nytl::Vec3i and nytl::Vec4i for int Vecs
///
/// There are much more Vec typedefs. All of them are named after a common pattern:
/// \c "nytl::Vec" + \c D + \c T with \c D = {2, 3, 4} and \c T of: 
/// \li f for float
/// \li i for int
/// \li ui for unsigned int 
/// \li d for double
/// \li b for bool
/// \li c for char
/// \li uc for unsigned char
/// \li l for long
/// \li ul for unsigned long
///
/// Additionally there are nytl::Vec2<T>, nytl::Vec3<T> and nytl::Vec4<T> typedefs.
/// There are Vector specializations for 2;3 and 4 dimensional Vectors, as well as for a dynamic
/// dimension (indicated by nytl::dynamicsize_type as D) or a reference type.
/// All specializations behave like normal Vecs and provide the same operations. To make
/// the extra specializations work you have to include the corresponding headers,
/// <nytl/refVec.hpp> for reference-typed Vectors or <nytl/dynVec.hpp> for dynamic-sized Vectors.
///
/// Except for the special dimension dynamicsize_type which make the Vec object more like a
/// std::vector with the possibility to add and remove components, Vec objects are usually
/// fixed-size and therefore entirely allocated on the stack. Fixed-size Vec specializations
/// are always pod classes which means they can be safely casted to other pod types (one can
/// e.g. safely cast a Vec3<Vec2f> into a Mat32f).
/// 
/// In difference to std::array it is meant like a representation of a matheMatical Vector, it
/// also has specializations for Vec2, Vec3 and Vec4 which mache dealing with them easier.
/// The available functions for dealing with Vec and the design of Vec itself is closely
/// related to the design of Vec in glsl (free functional operators, matheMatical operators)
/// while still oferring modern c++ features that make using them easier (e.g. iterators, tmp 
/// constructors or conversion operators).
/// This is the most important class for all further matheMatical classes and operations in nytl
/// since it can be used for storing position, size or as a general fixed- (or even dynamic-) sized
/// container.
////////////////////////////////////////////////////////////////////////////////////////////////// 

