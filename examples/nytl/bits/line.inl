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

#pragma once

template<size_t D, typename P> bool 
Line<D, P>::definedAt(const P& value, std::size_t dim) const
{
    return ((smallestValue(dim) <= value) &&
            (greatestValue(dim) >= value));
}

template<size_t D, typename P> Vec<D, P>
Line<D, P>::valueAt(const P& value, std::size_t dim) const
{
	//Will NOT check for defined at, can be done by function user, if needed
    if(gradient()[dim] == 0)
	{
		return a;
	}
    else
    {
        auto ret = a + ((value - a[dim]) * gradient(dim));
        return ret;
    }
}

///\ingroup math
///Returns the length of the given line.
///Just a wrapper around the Line::length function.
template<size_t D, typename P>
float length(const Line<D, P>& line)
{
	return line.length();
}
