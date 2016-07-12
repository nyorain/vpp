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

//implementation specialization
namespace detail
{

template<std::size_t D, typename P>
struct SimplexContainsPoint<D, P, 1>
{
	static bool call(const Simplex<D, P, 1>& a, const Vec<D, P>& v)
	{
		return (a.definedAt(v[0]) && all(a.valueAt(v[0]) == v));
	}
};

template<std::size_t D, typename P>
struct SimplexIntersects<D, P, 1>
{
	static bool call(const Simplex<D, P, 1>& la, const Simplex<D, P, 1>& lb)
	{  
		Mat<D, 3, double> eqs;
		eqs.col(0) = la.b - la.a;
		eqs.col(1) = -lb.b + lb.a;
		eqs.col(2) = lb.a - la.a;

		rrefMat(eqs);

		//unsolveable
		if((all(Vec2d(eqs.row(D - 1)) == 0) && eqs.row(D - 1)[2] != 0)) return false;

		//solveable, but not in Line segment
		if(any(eqs.col(2) >= 1) || any(eqs.col(2) <= 0)) return false;

		return true;
	};
};

} //detail

template<size_t D, typename P> bool 
Line<D, P>::definedAt(const P& value, std::size_t dim) const
{
    return ((smallestValue(dim) <= value) &&
            (greatestValue(dim) >= value));
}

template<size_t D, typename P> Vec<D, P>
Line<D, P>::valueAt(const P& value, std::size_t dim) const
{
    if(!definedAt(value, dim))
    {
		//throw?
        return {};
    }
    else if(gradient()[dim] == 0)
    {
        auto ret = a;
        ret[dim] = value;
        return ret;
    }
    else
    {
        auto ret = a + ((value - a[dim]) * gradient(dim));
        return ret;
    }
}

template<std::size_t D, typename P>
bool Line<D, P>::valid() const
{
	return detail::SimplexValid(*this);
}

