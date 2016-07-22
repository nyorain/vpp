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
///\brief Includes the Rect template class as well as several operators for it.

#pragma once

#include <nytl/vec.hpp>
#include <nytl/line.hpp>

#include <vector>
#include <ostream>
#include <algorithm>

namespace nytl
{
//Rect
template<std::size_t D, class P> class Rect;

template<typename P> using Rect2 = Rect<2, P>;
template<typename P> using Rect3 = Rect<3, P>;
template<typename P> using Rect4 = Rect<4, P>;

using Rect2i = Rect<2, int>;
using Rect2ui = Rect<2, unsigned int>;
using Rect2d = Rect<2, double>;
using Rect2f = Rect<2, float>;
using Rect2c = Rect<2, char>;
using Rect2uc = Rect<2, unsigned char>;
using Rect2l = Rect<2, long>;
using Rect2ul = Rect<2, unsigned long>;

using Rect3i = Rect<3, int>;
using Rect3ui = Rect<3, unsigned int>;
using Rect3d = Rect<3, double>;
using Rect3f = Rect<3, float>;
using Rect3c = Rect<3, char>;
using Rect3uc = Rect<3, unsigned char>;
using Rect3l = Rect<3, long>;
using Rect3ul = Rect<3, unsigned long>;

using Rect4i = Rect<4, int>;
using Rect4ui = Rect<4, unsigned int>;
using Rect4d = Rect<4, double>;
using Rect4f = Rect<4, float>;
using Rect4c = Rect<4, char>;
using Rect4uc = Rect<4, unsigned char>;
using Rect4l = Rect<4, long>;
using Rect4ul = Rect<4, unsigned long>;

///\ingroup math
///\brief Templated class that represents the matheMatic hyperRect (n-box) concept.
///\tparam D The dimension of the hyperRectangle
///\tparam P The precision of the hyperRectangle.
///\details The hyperRectangle is the generalization of a Rectangle for higher dimensions.
///It represents an area that is aligned with the spaces dimensions at a given position with
///a given size. There exist various operators for the Rect template class e.g. to check for
///intersection, compute unions or differences.
///There exist specialization for a 2-dimensional hyperRect (just a Rectangle), a 3-dimensional
///hyperRect (also called box) with additional features.
template<std::size_t D, typename P>
class Rect
{
public:
	static constexpr std::size_t dim = D;

	using Size = std::size_t;
	using Precision = P;
    using VecType = Vec<dim, Precision>;
	using RectType = Rect;

	//stl
    using value_type = Precision;
	using size_type = Size;

public:
	VecType position;
	VecType size;

public:
	///Constructs the Rect with a given position and size.
	Rect(VecType pposition = VecType(), VecType psize = VecType()) noexcept
		: position(pposition), size(psize) {}

	///Returns the center point.
	VecType center() const { return static_cast<Precision>(position + (size / Precision(2))); };

	///Returns the area/volume/... of the given rect.
	Precision space() const { return multiply(size); }

	///Returns whether the Rect has an area/volume/...
	bool empty() const { return !allEqual(size, 0); }

	///Converts the Rect to another Rect object of different dimension and/or precision.
	template<Size OD, class OP>
	operator Rect<OD, OP>() const { return Rect<OD, OP>(position, size); }
};

//Rect2 specialization
template<typename P>
class Rect<2, P>
{
public:
    static constexpr std::size_t dim = 2;

	using Size = std::size_t;
	using Precision = P;
    using VecType = Vec<dim, Precision>;
	using RectType = Rect;

	//stl
    using value_type = Precision;
	using size_type = Size;

public:
	VecType position;
	VecType size;

public:
	Rect(const VecType& pposition, const VecType& psize = VecType()) noexcept
		: position(pposition), size(psize) {}
	Rect(const P& x = {}, const P& y = {}, const P& width = {}, const P& height = {}) noexcept
		: position(x, y), size(width, height) {}

	VecType center() const { return static_cast<Precision>(position + (size / Precision(2))); };

    VecType topLeft() const { return position; }
	VecType topRight() const { return position + VecType(size.x, 0); }
	VecType bottomLeft() const { return position + VecType(0, size.y);}
	VecType bottomRight() const { return position + size; }

	const P& left() const { return position.x; }
	P right() const { return position.x + size.x; }
	const P& top() const { return position.y; }
	P bottom() const { return position.y + size.y; }

    P& left() { return position.x; }
	P& top() { return position.y; }

	const P& width() const { return size.x; }
	const P& height() const { return size.y; }

    P& width() { return size.x; }
	P& height() { return size.y; }

	Precision area() const { return multiply(size); }
	Precision inside() const { return area(); }
    bool empty() const { return all(size > 0); }

    //conversion
	template<Size OD, typename OP>
	operator Rect<OD, OP>() const { return Rect<OD, OP>(position, size); }
};

//Rect3 specialization
template<typename P>
class Rect<3, P>
{
public:
    static constexpr std::size_t dim = 3;

	using Size = std::size_t;
	using Precision = P;
    using VecType = Vec<dim, Precision>;
	using VecType2 = Vec<2, Precision>;
	using RectType = Rect;

	//stl
    using value_type = Precision;
	using size_type = Size;

public:
	VecType position;
	VecType size;

public:
	Rect(const VecType& pposition, const VecType& psize = VecType()) noexcept
		: position(pposition), size(psize) {}
	Rect(P x = P(), P y = P(), P z = P(), P width = P(), P height = P(), P depth = P()) noexcept
        : position(x,y,z), size(width,height,depth) {}

	VecType center() const { return static_cast<Precision>(position + (size / Precision(2))); };

    VecType2 topLeft() const { return position.xy(); }
	VecType2 topRight() const { return position.xy() + VecType2(size.x, 0); }
	VecType2 bottomLeft() const { return position.xy() + VecType2(0, size.y);}
	VecType2 bottomRight() const { return position.xy() + size.xy(); }

    VecType frontTopLeft() const { return position; }
	VecType frontTopRight() const { return position + VecType(size.x, 0, 0); }
	VecType frontBottomLeft() const { return position + VecType(0, size.y, 0);}
	VecType frontBottomRight() const { return position + VecType(size.x, size.y, 0); }
    VecType backTopLeft() const { return position + VecType(0, 0, size.z); }
	VecType backTopRight() const { return position + VecType(size.x, 0, size.z); }
	VecType backBottomLeft() const { return position + VecType(0, size.y, size.z);}
	VecType backBottomRight() const { return position + size; }

	const P& left() const { return position.x; }
	P right() const { return position.x + size.x; }
	const P& top() const { return position.y; }
	P bottom() const { return position.y + size.y; }
	const P& front() const { return position.z + size.z; }
	P back() const { return position.z + size.z; }

    P& left() { return position.x; }
	P& top() { return position.y; }
	P& front() { return position.z; }

	const P& width() const { return size.x; }
	const P& height() const { return size.y; }
	const P& depth() const { return size.z; }

    P& width() { return size.x; }
	P& height() { return size.y; }
	P& depth() { return size.z; }

	Precision volume() const { return multiply(size); }
	Precision inside() const { return volume(); }
    bool empty() const { return any(size > 0); }

    //conversion
	template<size_t oD, class oP>
	operator Rect<oD, oP>() const { return Rect<oD, oP>(position, size); }
};

//operators and utility
#include <nytl/bits/rect.inl>

}
