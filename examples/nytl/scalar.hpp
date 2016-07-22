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
///\brief Defines several useful constants and conversion/utility functions for scalar numbers.

#pragma once

namespace nytl
{

constexpr const double cPi = 3.14159265359;
constexpr const double cDeg = cPi / 180.0;
constexpr const double cE = 2.71828182845;

//degrees/radians
template<typename prec>
constexpr auto degrees(const prec& rad) -> decltype(rad / cDeg)
{
    return rad / cDeg;
}

template<typename prec>
constexpr auto radians(const prec& deg) -> decltype(deg * cDeg)
{
    return deg * cDeg;
}

//clamp
constexpr double clamp(double val, double minVal, double maxVal)
{
    return
        val <= minVal ? minVal :
        val >= maxVal ? maxVal:
        val;
}

//mix
constexpr double mix(double x, double y, double a)
{
    return (x * (1 - a)) + (y * a);
}

//c++14 constexpr, if the constexpr keyword should only be used, if c++14 is used
#ifndef NYTL_CPP14_CONSTEXPR
    #if __cplusplus >= 201402L
        #define NYTL_CPP14_CONSTEXPR constexpr
    #else
        #define NYTL_CPP14_CONSTEXPR
    #endif
#endif //NYTL_CPP14_CONSTEXPR


//constexpr min/max
#if __cplusplus >= 201402L
    using std::min;
    using std::max;

#else
    template<typename T> constexpr T min(const T& a, const T& b) { return a < b ? a : b; }
    template<typename T> constexpr T max(const T& a, const T& b) { return a > b ? a : b; }

#endif // c++14

constexpr unsigned long fac(unsigned long n)
{
	return (n <= 1ul) ? 1ul : n * fac(n - 1);
}

}
