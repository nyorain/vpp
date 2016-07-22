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
///\brief Makes the dynamic-sized Vec specialization work

#pragma once

#include <nytl/vec.hpp>
#include <vector>

//notes:
//for DynVec<bool>, the Vector<bool> implementation is used which can lead to problems.
//extra specialization for it? better use different container for internal DynVec storage?
//use valarray for implemenation instead?

namespace nytl
{

template<typename T> using DynVec = Vec<dynamicSize, T>;
template<typename T> using DynRefVec = Vec<dynamicSize, T&>;

using DynVecf = DynVec<float>;
using DynVeci = DynVec<int>;
using DynVecui = DynVec<unsigned int>;
using DynVecc = DynVec<char>;
using DynVecuc = DynVec<unsigned char>;
using DynVecl = DynVec<long>;
using DynVecul = DynVec<unsigned long>;
using DynVecd = DynVec<double>;
using DynVecb = DynVec<bool>;

template<typename T>
class Vec<dynamicSize, T>
{
public:
    constexpr static size_t dim = dynamicSize;

	using Value = T;
	using Precision = T;
    using VecType = Vec;
	using VectorType = std::vector<T>;
    using Size = typename VectorType::size_type;

    using Reference = typename VectorType::reference;
    using ConstReference = typename VectorType::const_reference;
    using Pointer = typename VectorType::pointer;
    using ConstPointer = typename VectorType::const_pointer;
    using Iterator = typename VectorType::iterator;
    using ConstIterator = typename VectorType::const_iterator;
    using ReverseIterator = typename VectorType::reverse_iterator;
    using ConstReverseIterator = typename VectorType::const_reverse_iterator;
    using Difference = typename VectorType::difference_type;

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
    Size max_size() const noexcept { return data_.max_size(); }
    Size size() const noexcept { return data_.size(); }
    bool empty() const noexcept { return size() == 0; }

public:
	VectorType data_ {};

public:
	Vec(std::initializer_list<T> init) : data_(init) {}
	explicit Vec(Size count) : data_(count) {}
	explicit Vec(Size count, const T& val) : data_(count, val) {}

	template<Size OD, typename OT>
	Vec(const Vec<OD, OT>& other) : data_(other.size())
	{
		for(std::size_t i(0); i < other.size(); ++i) data_[i] = other[i];
	}

    Vec() = default;
    ~Vec() noexcept = default;

    Vec(const VecType& other) = default;
    VecType& operator=(const VecType& other) = default;

    Vec(VecType&& other) noexcept = default;
    VecType& operator=(VecType&& other) noexcept = default;

    //operator
    template <Size OD, typename ot> VecType& operator +=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] += lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator -=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] -= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator *=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] *= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator /=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] /= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator %=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] %= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator |=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] |= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator ^=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] ^= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator &=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] &= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator >>=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] >>= lhs[i]; return *this; }
    template <Size OD, typename ot> VecType& operator <<=(const Vec<OD, ot>& lhs)
		{ for(Size i = 0; i < min(lhs.size(), size()); i++) (*this)[i] <<= lhs[i]; return *this; }

    template<typename ot> VecType& operator +=(const ot& lhs)
		{ for(auto& val : *this) val += lhs; return *this; }
    template<typename ot> VecType& operator -=(const ot& lhs)
		{ for(auto& val : *this) val -= lhs; return *this; }
    template<typename ot> VecType& operator *=(const ot& lhs)
		{ for(auto& val : *this) val *= lhs; return *this; }
    template<typename ot> VecType& operator /=(const ot& lhs)
		{ for(auto& val : *this) val /= lhs; return *this; }
    template<typename ot> VecType& operator %=(const ot& lhs)
		{ for(auto& val : *this) val %= lhs; return *this; }
    template<typename ot> VecType& operator |=(const ot& lhs)
		{ for(auto& val : *this) val |= lhs; return *this; }
    template<typename ot> VecType& operator ^=(const ot& lhs)
		{ for(auto& val : *this) val ^= lhs; return *this; }
    template<typename ot> VecType& operator &=(const ot& lhs)
		{ for(auto& val : *this) val &= lhs; return *this; }
    template<typename ot> VecType& operator >>=(const ot& lhs)
		{ for(auto& val : *this) val >>= lhs; return *this; }
    template<typename ot> VecType& operator <<=(const ot& lhs)
		{ for(auto& val : *this) val <<= lhs; return *this; }

    VecType operator-() const 
		{ VecType ret{}; for(Size i(0); i < size(); i++) ret[i] -= (*this)[i]; return ret; }

    template <Size OD, typename ot, typename = typename 
		std::enable_if<!std::is_reference<ot>::value>::type> operator Vec<OD, ot>() const 
		{ 
			Vec<OD, ot> ret(size()); 
			ret.fill(ot()); 
			for(Size i(0); i < min(ret.size(), size()); i++) 
				ret[i] = (*this)[i]; 
			return ret; 
		}

	template<Size psize>
	Vec<psize, T> subVec(Size position = 0) const
	{
		auto ret = Vec<psize, T>{};
		for(Size i(0); i < min(psize, size() - position); ++i)
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

	//dynamic stuff, todo. Just forward all Vec member functions here??
	void clear(){ data_.clear(); }
	void add(const T& val){ data_.push_back(val); }
	void resize(Size size){ data_.resize(size); }
	void resize(Size size, const T& val){ data_.resize(size, val); }
	void reserve(Size size){ data_.reserve(size); }

    //stl norm stuff
    ConstPointer data() const noexcept { return data_.data(); }
    Pointer data() noexcept { return data_.data(); }

    void fill(const Value& val) { for(auto& v : data_) v = val; }

    Iterator begin() noexcept { return data_.begin(); }
	ConstIterator begin() const noexcept { return data_.cbegin(); }
    ConstIterator cbegin() const noexcept { return data_.cbegin(); }

    Iterator end() noexcept { return data_.end(); }
    ConstIterator end() const noexcept { return data_.cend(); }
    ConstIterator cend() const noexcept { return data_.cend(); }

    ReverseIterator rbegin() noexcept { return data_.rbegin(); }
    ConstReverseIterator rbegin() const noexcept { return data_.crbegin(); }
    ConstReverseIterator crbegin() const noexcept { return data_.crbegin(); }

    ReverseIterator rend() noexcept { return data_.rend(); }
    ConstReverseIterator rend() const noexcept { return data_.crend(); }
    ConstReverseIterator crend() const noexcept { return data_.crend(); }

    Reference operator[](Size i){ return data_[i]; }
    ConstReference operator[](Size i) const { return data_[i]; }

    Reference at(Size i){ return data_.at(i); }
    ConstReference at(Size i) const { return data_.at(i); }

    Reference front() noexcept { return data_.front(); }
    ConstReference front() const noexcept { return data_.front(); }

    Reference back() noexcept { return data_.back(); }
    ConstReference back() const noexcept { return data_.back(); }

	void swap(VecType& other){ data_.swap(other.data_); }
};

}
