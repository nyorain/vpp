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
///\brief Includes the Matrix template class as well as operations and typedefs for it.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/refVec.hpp>

#include <iomanip>
#include <memory>
#include <cmath>
#include <tuple>
#include <cstddef>

namespace nytl
{

//typedefs
template<std::size_t R, std::size_t C, typename P>
class Mat;

#include <nytl/bits/matmp.inl>
#include <nytl/bits/matypes.inl>

///\ingroup math
///Matrix template class.
template<std::size_t R, std::size_t C, typename P> class Mat :
	DeriveDummy<typename std::enable_if<(R > 0) && (C > 0) && (!std::is_reference<P>::value)>::type>
{
public:
	using Size = std::size_t;

    using value_type = P;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using MatType = Mat<R, C, P>;
    using rows_Vec = Vec<R, P>;
    using cols_Vec = Vec<C, P>;

    static constexpr bool is_squared = (R == C);
    static constexpr size_type Mat_size = R * C;

	static constexpr Size rows() { return R; }
	static constexpr Size cols() { return C; }

public:
	Vec<R, Vec<C, P>> data_;

public:
    template<typename... Args, typename = typename
		std::enable_if_t<
			std::is_convertible<
				std::tuple<Args...>,
				TypeTuple<value_type, Mat_size>
			>::value>
		>
    Mat(Args&&... args) noexcept
		{ detail::InitMatData<R * C>::call(data_, std::make_tuple(args...)); }

	Mat(const P& val) noexcept
		{ detail::InitMatData<R * C>::call(data_, val); }

	Mat() noexcept = default;
	~Mat() noexcept = default;

	Mat(const MatType& other) noexcept = default;
	Mat(MatType&& other) noexcept = default;

	MatType& operator=(const MatType& other) noexcept = default;
	MatType& operator=(MatType&& other) noexcept = default;

	///Initialized the Matrix with the given values
    template<typename... Args, typename = typename
		std::enable_if_t<
			std::is_convertible<
				std::tuple<Args...>,
				TypeTuple<value_type, Mat_size>
			>::value>
		>
    void init(Args&&... args)
		{ detail::InitMatData<R * C>::call(data_, std::make_tuple(args...)); }

    ///Returns a reference of a certain row of the Matrix.
	///If captured with a reference (and got by a non-const object) the actual values of the
	///Matrix can be changed with it.
	Vec<C, P>& row(size_t i){ return data_[i]; }

	///Returns a conct-reference of a certain row of the Matrix.
	const Vec<C, P>& row(size_t i) const { return data_[i]; }

	///Returns a reference Vec of a certain column of the Matrix.
	///Must be captured by a RefVec (not possible with Vec&) to be able to change the actual
	///values of the Matrix object.
	RefVec<R, P> col(size_t i){ return detail::MakeRowRefVec<R>::call(data_, i); }

	///Returns a Vec (just the values, no references) of a certain row.
	Vec<R, P> col(size_t i) const { return detail::MakeRowVec<R>::call(data_, i); }

    ///Returns a plain pointer to the data of the Matrix.
    pointer data(){ return (pointer) data_.data(); }

	///Returns a const plain pointer to the data of the Matrix.
	const_pointer data() const { return (const_pointer) data_.data(); }

	///Copys the data of the Matrix as plain unique ptr on the heap.
	std::unique_ptr<P[]> copyData() const { return detail::CopyMatData<R * C>::call(data_); }

	///Returns a std::tuple filled with the components of the Matrix
	TypeTuple<P, Mat_size> asTuple() const { return detail::MatTuple<R * C>::call(data_); }

	///Swaps the both given columns.
	void swapCol(std::size_t a, std::size_t b){ std::swap(col(a), col(b)); }

	///Swaps the both given rows
	void swapRow(std::size_t a, std::size_t b){ std::swap(row(a), row(b)); }

    //math
    MatType& operator +=(const Mat<R, C, P>& other){ data_ += other.data_; return *this; }
   	MatType& operator -=(const Mat<R, C, P>& other){ data_ -= other.data_; return *this; }
    MatType& operator *=(const Mat<C, R, P>& other)
    {
		auto od = data_;
		for(size_t r(0); r < R; r++)
			for(size_t c(0); c < C; c++)
				data_[r][c] = sum(od[r] * other.col(c));
		return *this;
	}
    Mat<R, C, P>& operator *=(const P& other){ for(auto& val : *this) val *= other; return *this; }

    //invert TODO
	///\brief Only available for squared (R == C) Mat objects.
	///\return Returns whether the Mat object is invertible.
	template<typename TD = bool>
		typename std::enable_if<is_squared, TD>::type invertable() const { return 0; }

	///\brief Inverts the Mat object.
	///\details Only available for squared (R == C) Mat objects.
	///\exception nytl::InvalidMatrix if the matrix is not invertable.
	///Check this with invertable() before using invert().
    template<typename TD = void>
		typename std::enable_if<is_squared, TD>::type invert();

	///\brief Converts the Mat object to a Mat object with different template parameters.
    template<std::size_t OR, std::size_t OC, class OP> operator Mat<OR, OC, OP>() const;

    //stl container
    constexpr size_type size() const { return Mat_size; }
    constexpr bool empty() const { return size() == 0; }

    void fill(const value_type& val) { for(auto& r : data_)for(auto& c : r) c = val; }

    iterator begin() noexcept { return &data_[0][0]; }
    const_iterator begin() const noexcept { return &data_[0][0]; }
    const_iterator cbegin() const noexcept { return &data_[0][0]; }

    iterator end() noexcept { return begin() + Mat_size; }
    const_iterator end() const noexcept { return begin() + Mat_size; }
    const_iterator cend() const noexcept { return begin() + Mat_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    Vec<C, P>& operator[](size_t row){ return data_[row]; }
	const Vec<C, P>& operator[](size_t row) const { return data_[row]; }

    Vec<C, P>& at(size_t row)
		{ if(row >= R) throw std::out_of_range("nytl::Mat::at"); return data_[row]; }
	const Vec<C, P>& at(size_t row) const 
		{ if(row >= R || row < 0) throw std::out_of_range("nytl::Mat::at"); return data_[row]; }

	P& at(size_t r, size_t c)
		{ if(r >= R || c > C) throw std::out_of_range("nytl::Mat::at"); return data_[r][c]; }
	const P& at(size_t r, size_t c) const 
		{ if(r >= R || c > C) throw std::out_of_range("nytl::Mat::at"); return data_[r][c]; }

    reference front() noexcept { return data_[0][0]; }
    const_reference front() const noexcept { return data_[0][0]; }

    reference back() noexcept { return data_[R - 1][C - 1]; }
    const_reference back() const noexcept { return data_[R - 1][C - 1]; }
};

//operators and utility functions
#include <nytl/bits/mat.inl>

} //nytl
