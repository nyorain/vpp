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
///\brief The 2-dimensional Simplex specialization (Triangle).

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

namespace nytl
{

//typedefs
template<typename P> using Triangle2 = Triangle<2, P>;
template<typename P> using Triangle3 = Triangle<3, P>;
template<typename P> using Triangle4 = Triangle<4, P>;

using Triangle2f = Triangle<2, float>;
using Triangle3f = Triangle<3, float>;
using Triangle4f = Triangle<4, float>;

using Triangle2d = Triangle<2, double>;
using Triangle3d = Triangle<3, double>;
using Triangle4d = Triangle<4, double>;

using Triangle2i = Triangle<2, int>;
using Triangle3i = Triangle<3, int>;
using Triangle4i = Triangle<4, int>;

using Triangle2ui = Triangle<2, unsigned int>;
using Triangle3ui = Triangle<3, unsigned int>;
using Triangle4ui = Triangle<4, unsigned int>;

template<size_t D, typename P>
class Simplex<D, P, 2>
{
public:
	static constexpr std::size_t dim = D;
	static constexpr std::size_t simplexDim = 3;

	using Precision = P;
    using VecType = Vec<D, P>;
    using LineType = Line<D, P>;
    using TriangleType = Triangle<D, P>;
	using Size = std::size_t;

	//stl
    using value_type = Precision;
	using size_type = Size;

public:
    VecType a {};
    VecType b {};
    VecType c {};

public:
    Simplex(const VecType& xa, const VecType& xb, const VecType& xc) noexcept
		: a(xa), b(xb), c(xc) {}
    Simplex() noexcept = default;

	//default
    double size() const;
	VecType center() const;
	bool valid() const;

	Vec<3, VecType>& points()
		{ return reinterpret_cast<Vec<3, VecType>&>(*this); }
	const Vec<3, VecType>& points() const 
		{ return reinterpret_cast<const Vec<3, VecType>&>(*this); }

    template<size_t OD, typename OP> constexpr
    operator Triangle<OD, OP>() const { return Triangle<OD, OP>(a, b, c); }

	//Triangle specific
    double angleA() const { return angle(ab().difference(), ac().difference()); }
    double angleB() const { return angle(ba().difference(), bc().difference()); }
    double angleC() const { return angle(cb().difference(), ca().difference()); }

    LineType ab() const { return LineType(a, b); }
    LineType ac() const { return LineType(a, c); }
    LineType bc() const { return LineType(b, c); }
    LineType ba() const { return LineType(b, a); }
    LineType ca() const { return LineType(c, a); }
    LineType cb() const { return LineType(c, b); }
};

//utility and operators/test
#include <nytl/bits/triangle.inl>

}
