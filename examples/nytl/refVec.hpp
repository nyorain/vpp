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
///\brief Makes the Vec specialization for reference types work.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/bits/refVecIterator.inl>

namespace nytl
{

template<size_t D, typename T> using RefVec = Vec<D, T&>;

template<typename T> using RefVec2 = Vec<2, T&>;
template<typename T> using RefVec3 = Vec<3, T&>;
template<typename T> using RefVec4 = Vec<4, T&>;

using RefVec2f = RefVec2<float>;
using RefVec2i = RefVec2<int>;
using RefVec2ui = RefVec2<unsigned int>;
using RefVec2d = RefVec2<double>;
using RefVec2c = RefVec2<char>;
using RefVec2uc = RefVec2<unsigned char>;
using RefVec2l = RefVec2<long>;
using RefVec2ul = RefVec2<unsigned long>;

using RefVec3f = RefVec3<float>;
using RefVec3i = RefVec3<int>;
using RefVec3ui = RefVec3<unsigned int>;
using RefVec3d = RefVec3<double>;
using RefVec3c = RefVec3<char>;
using RefVec3uc = RefVec3<unsigned char>;
using RefVec3l = RefVec3<long>;
using RefVec3ul = RefVec3<unsigned long>;

using RefVec4f = RefVec4<float>;
using RefVec4i = RefVec4<int>;
using RefVec4ui = RefVec4<unsigned int>;
using RefVec4d = RefVec4<double>;
using RefVec4c = RefVec4<char>;
using RefVec4uc = RefVec4<unsigned char>;
using RefVec4l = RefVec4<long>;
using RefVec4ul = RefVec4<unsigned long>;

template<size_t D, typename T> 
class Vec<D, T&>
{
public:
	static constexpr std::size_t dim = D;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec<dim, T>;
    using RefVecType = Vec<D, T&>;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = RefVecIterator<dim, Value>;
    using ConstIterator = ConstRefVecIterator<dim, Value>;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = Size;

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
    constexpr Size length() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    pointer data_[dim];

public:
    template<typename... Args, typename = typename 
		std::enable_if_t<
			std::is_convertible<
				std::tuple<Args...>, 
				TypeTuple<value_type, dim>
			>::value
		>>
    Vec(Args&&... args) : data_{&args...} {}
    ~Vec() noexcept = default;

    Vec(const RefVecType& other) noexcept = default;
    Vec(const VecType& other) noexcept 
		{ for(Size i(0); i < dim; ++i) data_[i] = &other[i]; }

    RefVecType& operator=(const VecType& other) noexcept 
		{ for(Size i(0); i < dim; ++i) *data_[i] = other[i]; return *this; }

    //operator
    template <std::size_t OD, typename ot> RefVecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> RefVecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> RefVecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> RefVecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> RefVecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> RefVecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> RefVecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> RefVecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> RefVecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> RefVecType& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> RefVecType& operator <<=(OT lhs)
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

	//stl 
	//TODO: correct data implementations 
    //ConstPointer data() const noexcept { return data_; }
    //pointer data() noexcept { return data_; }

    void fill(const Value& val) { for(auto& v : data_) *v = val; }

    Iterator begin() noexcept { return Iterator(*this); }
    ConstIterator begin() const noexcept { return ConstIterator(*this); }
    ConstIterator cbegin() const noexcept { return ConstIterator(*this); }

    Iterator end() noexcept { return Iterator(*this, dim); }
    ConstIterator end() const noexcept { return ConstIterator(*this, dim); }
    ConstIterator cend() const noexcept { return ConstIterator(*this, dim); }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return constReverseIterator(cbegin()); }

    Reference operator[](Size i){ return *data_[i]; }
    ConstReference operator[](Size i) const { return *data_[i]; }

    Reference at(Size i)
		{ if(i >= dim || i < 0) throw RangeError("nytl::Vec::at"); return *data_[i]; }
    ConstReference at(Size i) const 
		{ if(i >= dim || i < 0) throw RangeError("nytl::Vec::at"); return *data_[i]; }

    Reference front() noexcept { return *data_[0]; }
    ConstReference front() const noexcept { return *data_[0]; }

    Reference back() noexcept { return *data_[dim - 1]; }
    ConstReference back() const noexcept { return *data_[dim - 1]; }

	void swap(VecType& other)
		{ for(Size i(0); i < size(); ++i) std::swap(*data_[i], other[i]); }

	friend void swap(RefVecType& a, RefVecType& b) { a.swap(b); }
};

template<typename T> class Vec<2, T&>
{
public:
	static constexpr std::size_t dim = 2;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec<dim, T>;
    using RefVecType = Vec<dim, T&>;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = RefVecIterator<dim, Value>;
    using ConstIterator = ConstRefVecIterator<dim, Value>;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = Size;

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
    constexpr Size length() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    Reference x;
    Reference y;

public:
    Vec(Reference a, Reference b) : x(a), y(b) {}
    ~Vec() noexcept = default;

    Vec(const RefVecType& other) noexcept = default;

    Vec(VecType& other) noexcept : x(other.x), y(other.y) {}
    RefVecType& operator=(const VecType& other) noexcept 
		{ x = other.x; y = other.y; return *this; }

    //operator
    template <std::size_t OD, typename ot> RefVecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> RefVecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> RefVecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> RefVecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> RefVecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> RefVecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> RefVecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> RefVecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> RefVecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> RefVecType& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> RefVecType& operator <<=(OT lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

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
    ConstPointer data() const noexcept { return &x; }
    Pointer data() noexcept { return &x; }

    void fill(const Value& val) { x = val; y = val; }

    Iterator begin() noexcept { return Iterator(*this); }
    ConstIterator begin() const noexcept { return ConstIterator(*this); }
    ConstIterator cbegin() const noexcept { return ConstIterator(*this); }

    Iterator end() noexcept { return Iterator(*this, dim); }
    ConstIterator end() const noexcept { return ConstIterator(*this, dim); }
    ConstIterator cend() const noexcept { return ConstIterator(*this, dim); }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i){ if(i == 0) return x; return y; }
    ConstReference operator[](Size i) const { if(i == 0) return x; return y; }

    Reference at(Size i)
		{ if(i >= dim)	throw RangeError("nytl::RefVec::at"); return (*this)[i]; }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw std::out_of_range("nytl::RefVec::at"); return (*this)[i]; }

    Reference front() noexcept { return x; }
    ConstReference front() const noexcept { return x; }

    Reference back() noexcept { return y; }
    ConstReference back() const noexcept { return y; }

	void swap(VecType& other){ std::swap(x, other.x); std::swap(y, other.y); }
	friend void swap(RefVecType& a, RefVecType& b) { a.swap(b); }
};

template<typename T> class Vec<3, T&>
{
public:
	static constexpr std::size_t dim = 3;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec<dim, T>;
    using RefVecType = Vec<dim, T&>;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = RefVecIterator<dim, Value>;
    using ConstIterator = ConstRefVecIterator<dim, Value>;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = Size;

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
    constexpr Size length() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    Reference x;
    Reference y;
    Reference z;

public:
    Vec(Reference a, Reference b, Reference c) noexcept : x(a), y(b), z(c) {}
    ~Vec() noexcept = default;

    Vec(const RefVecType& other) noexcept = default;

    Vec(const VecType& other) noexcept : x(other.x), y(other.y), z(other.z) {}
    RefVecType& operator=(const VecType& other) noexcept 
		{ x = other.x; y = other.y; z = other.z; return *this; } 

    //operator
    template <std::size_t OD, typename ot> RefVecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> RefVecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> RefVecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> RefVecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> RefVecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> RefVecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> RefVecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> RefVecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> RefVecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> RefVecType& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> RefVecType& operator <<=(OT lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

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
    ConstPointer data() const noexcept { return &x; }
    Pointer data() noexcept { return &x; }

    void fill(const Value& val) { x = val; y = val; z = val; }

    Iterator begin() noexcept { return Iterator(*this); }
    ConstIterator begin() const noexcept { return ConstIterator(*this); }
    ConstIterator cbegin() const noexcept { return ConstIterator(*this); }

    Iterator end() noexcept { return Iterator(*this, dim); }
    ConstIterator end() const noexcept { return ConstIterator(*this, dim); }
    ConstIterator cend() const noexcept { return ConstIterator(*this, dim); }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i){ if(i == 0) return x; if(i == 1) return y; return z; }
    ConstReference operator[](Size i) const { if(i == 0) return x; if(i == 1) return y; return z; }

    Reference at(Size i)
		{ if(i >= dim)	throw RangeError("nytl::RefVec::at"); return (*this)[i]; }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw RangeError("nytl::RefVec::at"); return (*this)[i]; }

    Reference front() noexcept { return x; }
    ConstReference front() const noexcept { return x; }

    Reference back() noexcept { return z; }
    ConstReference back() const noexcept { return z; }

	void swap(VecType& other)
		{ std::swap(x, other.x); std::swap(y, other.y); std::swap(z, other.z); }
	friend void swap(RefVecType& a, RefVecType& b) { a.swap(b); }

    //custom
    Vec2<Reference> xy() const noexcept { return Vec2<Reference>(x,y); }
    Vec2<Reference> yz() const noexcept { return Vec2<Reference>(y,z); }
    Vec2<Reference> xz() const noexcept { return Vec2<Reference>(x,z); }
};

template<typename T> class Vec<4, T&>
{
public:
	static constexpr std::size_t dim = 4;

	using Size = std::size_t;
	using Precision = T;
	using Value = T;
    using VecType = Vec<dim, T>;
    using RefVecType = Vec<dim, T&>;

    using Reference = Value&;
    using ConstReference = const Value&;
    using Pointer = Value*;
    using ConstPointer = const Value*;
    using Iterator = RefVecIterator<dim, Value>;
    using ConstIterator = ConstRefVecIterator<dim, Value>;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
    using Difference = Size;

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
    constexpr Size length() const noexcept { return dim; }
    constexpr Size max_size() const noexcept { return dim; }
    constexpr bool empty() const noexcept { return dim == 0; }

public:
    Reference x;
    Reference y;
    Reference z;
    Reference w;

public:
    Vec(Reference a, Reference b, Reference c, Reference d) noexcept : x(a), y(b), z(c), w(d) {}
    ~Vec() noexcept = default;

    Vec(const RefVecType& other) noexcept = default;

    Vec(const VecType& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w) {} 
    RefVecType& operator=(const VecType& other) noexcept 
		{ x = other.x; y = other.y; z = other.z; w = other.w; return *this; }

    //operator
    template <std::size_t OD, typename ot> RefVecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] += lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] -= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] *= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] /= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] %= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] |= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] &= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <std::size_t OD, typename ot> RefVecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(size_t i = 0; i < min(lhs.size(), dim); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename OT> RefVecType& operator +=(OT lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename OT> RefVecType& operator -=(OT lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename OT> RefVecType& operator *=(OT lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename OT> RefVecType& operator /=(OT lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename OT> RefVecType& operator %=(OT lhs)
		{ for(auto& val : *this) val %= lhs; return *this; }
    template<typename OT> RefVecType& operator |=(OT lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename OT> RefVecType& operator ^=(OT lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename OT> RefVecType& operator &=(OT lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename OT> RefVecType& operator >>=(OT lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename OT> RefVecType& operator <<=(OT lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

    VecType operator-() const { return VecType(x, y, z); }

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
    ConstPointer data() const noexcept { return &x; }
    Pointer data() noexcept { return &x; }

    void fill(const Value& val) { x = val; y = val; z = val; w = val; }

    Iterator begin() noexcept { return Iterator(*this); }
    ConstIterator begin() const noexcept { return ConstIterator(*this); }
    ConstIterator cbegin() const noexcept { return ConstIterator(*this); }

    Iterator end() noexcept { return Iterator(*this, dim); }
    ConstIterator end() const noexcept { return ConstIterator(*this, dim); }
    ConstIterator cend() const noexcept { return ConstIterator(*this, dim); }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }

    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(cbegin()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }

    Reference operator[](Size i)
		{ if(i == 0) return x; if(i == 1) return y; if(i == 2) return z; return w; }
    ConstReference operator[](Size i) const 
		{ if(i == 0) return x; if(i == 1) return y; if(i == 2) return z; return w; }

    Reference at(Size i)
		{ if(i >= dim)	throw RangeError("nytl::RefVec::at"); return (*this)[i]; }
    ConstReference at(Size i) const 
		{ if(i >= dim) throw RangeError("nytl::RefVec::at"); return (*this)[i]; }

    Reference front() noexcept { return x; }
    ConstReference front() const noexcept { return x; }

    Reference back() noexcept { return w; }
    ConstReference back() const noexcept { return w; }

	void swap(VecType& other)
		{ using std::swap; swap(x, other.x); swap(y, other.y); swap(z, other.z); swap(w, other.w);}

	friend void swap(RefVecType& a, RefVecType& b){ a.swap(b); }

    //custom
    Vec2<Reference> xy() const noexcept { return Vec2<Reference>(x,y); }
    Vec2<Reference> xz() const noexcept { return Vec2<Reference>(x,z); }
    Vec2<Reference> xw() const noexcept { return Vec2<Reference>(x,w); }
    Vec2<Reference> yz() const noexcept { return Vec2<Reference>(y,z); }
    Vec2<Reference> yw() const noexcept { return Vec2<Reference>(y,w); }
    Vec2<Reference> zw() const noexcept { return Vec2<Reference>(z,w); }

    Vec3<Reference> xyz() const noexcept { return Vec3<Reference>(x,y,z); }
    Vec3<Reference> xyw() const noexcept { return Vec3<Reference>(x,y,w); }
    Vec3<Reference> xzw() const noexcept { return Vec3<Reference>(x,z,w); }
    Vec3<Reference> yzw() const noexcept { return Vec3<Reference>(y,z,w); }
};

template<size_t dim, typename T> constexpr size_t Vec<dim, T&>::dim;
template<typename T> constexpr size_t Vec<2, T&>::dim;
template<typename T> constexpr size_t Vec<3, T&>::dim;
template<typename T> constexpr size_t Vec<4, T&>::dim;

}

