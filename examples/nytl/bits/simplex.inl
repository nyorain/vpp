/* The MIT License (MIT)
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

#ifdef DOXYGEN
namespace nytl{
#endif

///\relates Simplex
///Outputs all Simplex points to an ostream.
template<std::size_t D, typename P, std::size_t A>
std::ostream& operator<<(std::ostream& os, const Simplex<D, P, A>& s)
{
	for(auto& p : s.points())
		os << p;

	return os;
}

//member
template<std::size_t D, typename P, std::size_t A>
double Simplex<D, P, A>::size() const
{
	SquareMat<D, P> m;
	for(std::size_t i(1); i <= A; ++i) m.col(i) = points()[i] - points()[0];
	return std::abs(det(m)) / fac(D);
}

template<std::size_t D, typename P, std::size_t A>
Vec<D, P> Simplex<D, P, A>::center() const
{
	return (sum(points()) / points().size());
}

template<std::size_t D, typename P, std::size_t A>
template<std::size_t OD, typename OP>
Simplex<D, P, A>::operator Simplex<OD, OP, A>() const
{
	return Simplex<OD, OP, A>{points()};
}

//equality
///\relates Simplex
template<std::size_t D, typename P, std::size_t A>
	bool operator==(const Simplex<D, P, A>& a, const Simplex<D, P, A>& b)
{
	for(std::size_t i(0); i < A + 1; ++i)
		if(!all(a.points()[i] == b.points()[i])) return 0;

	return 1;
}

#ifdef DOXYGEN
} //nytl
#endif
