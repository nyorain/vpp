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

///\file
///\brief The 3-dimensional Simplex spiecialization (Tetrahedron).

#pragma once

#include <nytl/vec.hpp>
#include <nytl/simplex.hpp>

namespace nytl
{

//typedefs
///Defines the 3 dimensional Simplex specialization.
template<typename P> using Tetrahedron3 = Tetrahedron<3, P>;
template<typename P> using Tetrahedron4 = Tetrahedron<4, P>;

using Tetrahedron3f = Tetrahedron<3, float>;
using Tetrahedron4f = Tetrahedron<4, float>;

using Tetrahedron3d = Tetrahedron<3, double>;
using Tetrahedron4d = Tetrahedron<4, double>;

using Tetrahedron3i = Tetrahedron<3, int>;
using Tetrahedron4i = Tetrahedron<4, int>;

using Tetrahedron3ui = Tetrahedron<3, unsigned int>;
using Tetrahedron4ui = Tetrahedron<4, unsigned int>;

template<size_t D, typename P>
class Simplex<D, P, 3>
{
public:
	static constexpr std::size_t dim = D;
	static constexpr std::size_t simplexDim = 3;

	using Precision = P;
    using VecType = Vec<D, P>;
    using TetrahedronType = Tetrahedron<D, P>;
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
    VecType d {};

public:
    Simplex(const VecType& xa, const VecType& xb, const VecType& xc, const VecType& xd) noexcept
		: a(xa), b(xb), c(xc), d(xd) {}
    Simplex() noexcept = default;

	//default
    double size() const;
	VecType center() const;
	bool valid() const;

	Vec<4, VecType>& points()
		{ return reinterpret_cast<Vec<4, VecType>&>(*this); }
	const Vec<4, VecType>& points() const 
		{ return reinterpret_cast<const Vec<4, VecType>&>(*this); }

    template<size_t OD, typename OP> constexpr
    operator Tetrahedron<OD, OP>() const { return Tetrahedron<OD, OP>(a, b, c, d); }

	//Tetrahedron specific
	//edges
    LineType ab() const { return LineType(a, b); }
    LineType ac() const { return LineType(a, c); }
    LineType ad() const { return LineType(a, d); }

    LineType ba() const { return LineType(b, a); }
    LineType bc() const { return LineType(b, c); }
    LineType bd() const { return LineType(b, d); }

    LineType ca() const { return LineType(c, a); }
    LineType cb() const { return LineType(c, b); }
    LineType cd() const { return LineType(c, d); }

    LineType da() const { return LineType(d, a); }
    LineType db() const { return LineType(d, b); }
    LineType dc() const { return LineType(d, c); }

	//faces
	TriangleType abc() const { return TriangleType(a, b, c); }
	TriangleType abd() const { return TriangleType(a, b, d); }
	TriangleType acd() const { return TriangleType(a, c, d); }
	TriangleType bcd() const { return TriangleType(b, c, d); }

	//TODO: angle
};

//utility and operators/test
#include <nytl/bits/tetrahedron.inl>

}
