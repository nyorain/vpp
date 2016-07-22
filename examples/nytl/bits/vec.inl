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

namespace detail
{

constexpr std::size_t dMin(std::size_t a, std::size_t b)
	{ return (a == dynamicSize || b == dynamicSize) ? dynamicSize : min(a, b); }

constexpr std::size_t dMax(std::size_t a, std::size_t b)
	{ return (a == dynamicSize || b == dynamicSize) ? dynamicSize : max(a, b); }

}

//general operators
///\relates nytl::Vec
///Prints all values individually to an output stream.
template<std::size_t D, typename T> std::ostream& 
operator<<(std::ostream& os, const Vec<D, T>& obj)
{
    const char* c = "";

    os << "(";

    for(const auto& val : obj)
    {
        os << c << val;
        c = "; ";
    }

    os << ")";

    return os;
}

///\relates nytl::Vec
///Loads all components from an input stream.
template<std::size_t D, typename T> std::istream& 
operator>>(std::istream& is, Vec<D, T>& obj)
{
    for(auto& val : obj)
    {
        is >> val;
    }

    return is;
}

//+todo: Vec and Vec unefficient atm, some proxy class 
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator+(const Vec<D, T>& mVec, const O& other)
	-> Vec<D, decltype(mVec[0] + other)>
{
	Vec<D, decltype(mVec[0] + other)> ret = mVec;
	ret += other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator+(const O& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other + mVec[0])>
{
	Vec<D, decltype(mVec[0] + other)> ret = mVec;
	ret += other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator+(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] + b[0])>;

    auto ret = static_cast<V>(a);
    ret += static_cast<V>(b);
    return ret;
}

//-
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator-(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] - other)>
{
    mVec -= other;
    return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>  
auto operator-(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other - mVec[0])>
{
	Vec<D, decltype(other - mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other - mVec[i];

    return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator-(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] - b[0])>;

    auto ret = static_cast<V>(a);
    ret -= static_cast<V>(b);
    return ret;
}

//*
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator*(const Vec<D, T>& mVec, const O& other)
	-> Vec<D, decltype(mVec[0] * other)>
{
	Vec<D, decltype(mVec[0] * other)> ret = mVec;
	ret *= other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator*(const O& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other * mVec[0])>
{
	Vec<D, decltype(mVec[0] * other)> ret = mVec;
	ret *= other;
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator*(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] * b[0])>;

    auto ret = static_cast<V>(a);
    ret *= static_cast<V>(b);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator/(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] / other)>
{
    mVec /= other;
    return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>  
auto operator/(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other / mVec[0])>
{
	Vec<D, decltype(other / mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other / mVec[i];

    return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator/(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] / b[0])>;

    auto ret = static_cast<V>(a);
    ret /= static_cast<V>(b);
    return ret;
}

//%
///\relates nytl::Vec
template<std::size_t D, typename T, typename O>  
auto operator%(Vec<D, T> mVec, const O& other)
	-> Vec<D, decltype(mVec[0] % other)>
{
    mVec %= other;
    return mVec;
}

///\relates nytl::Vec
template<std::size_t D, typename T, typename OT>  
auto operator%(const OT& other, const Vec<D, T>& mVec)
	-> Vec<D, decltype(other % mVec[0])>
{
	Vec<D, decltype(other % mVec[0])> ret(mVec.size());
	for(std::size_t i(0); i < ret.size(); ++i)
		ret[i] = other % mVec[i];

    return mVec;
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, std::size_t DB, typename TB>  
auto operator%(const Vec<DA, TA>& a, const Vec<DB, TB>& b) 
	-> Vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>
{
	using V = Vec<detail::dMax(DA, DB), decltype(a[0] % b[0])>;

    auto ret = static_cast<V>(a);
    ret %= static_cast<V>(b);
    return ret;
}

//equal
///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> equal(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = (va[i] == vb[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> equal(const Vec<D, TA>& va, const TB& value)
{
    Vec<D, bool> ret(va.size());
    for(std::size_t i(0); i < D; ++i)
        ret[i] = (va[i] == value);
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> notEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = (va[i] != vb[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> notEqual(const Vec<D, TA>& va, const TB& value)
{
    Vec<D, bool> ret(va.size());
    for(std::size_t i(0); i < D; ++i)
        ret[i] = (va[i] != value);
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> lessThan(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = va[i] < vb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<min(DA, DB), bool> greaterThan(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    Vec<min(DA, DB), bool> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = va[i] > vb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
Vec<detail::dMin(DA, DB), bool> lessThanEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = va[i] <= vb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> greaterThanEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    Vec<detail::dMin(DA, DB), bool> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = va[i] >= vb[i];
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> lessThan(const Vec<D, TA>& va, const TB& value)
{
    Vec<D, bool> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = va[i] < value;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> greaterThan(const Vec<D, TA>& va, const TB& value)
{
    Vec<D, bool> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = va[i] > value;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> lessThanEqual(const Vec<D, TA>& va, const TB& value)
{
    Vec<D, bool> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = va[i] <= value;
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, bool> greaterThanEqual(const Vec<D, TA>& va, const TB& value)
{
    Vec<D, bool> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = va[i] >= value;
    return ret;
}

//todo: allow comparison of Vec and value with operator? 
//Vec<D, bool> operator==(Vec<D, TA> v, TB value);
///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA, DB), bool> operator==(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return equal(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> operator!=(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return notEqual(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA, DB), bool> operator<(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return lessThan(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB>  
Vec<detail::dMin(DA, DB), bool> operator>(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return greaterThan(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA,DB), bool> operator<=(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return lessThanEqual(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
Vec<detail::dMin(DA, DB), bool> operator>=(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return greaterThanEqual(va, vb);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator==(const Vec<DA, TA>& va, const TB& b)
{
    return equal(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator!=(const Vec<DA, TA>& va, const TB& b)
{
    return notEqual(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator<(const Vec<DA, TA>& va, const TB& b)
{
    return lessThan(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator>(const Vec<DA, TA>& va, const TB& b)
{
    return greaterThan(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator<=(const Vec<DA, TA>& va, const TB& b)
{
    return lessThanEqual(va, b);
}

///\relates nytl::Vec
template<std::size_t DA, typename TA, typename TB> 
Vec<DA, bool> operator>=(const Vec<DA, TA>& va, const TB& b)
{
    return greaterThanEqual(va, b);
}

///\relates nytl::Vec
///Tests whether all (common) components of two Vecs are equal.
///If the two given Vecs have different sizes, this function will only test the components
///which exist in both Vecs, so {5, 7, 8} and {5, 7} will be considered equal.
///More efficient alternative to all(a == b) since it does only have to run 1 loops instead of 2.
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
bool allEqual(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        if(va[i] != vb[i]) return 0;

    return 1;
}

///\relates nytl::Vec
///Tests whether all components of the given Vec are equal to the given value.
///More efficient alternative to all(a == b) since it does only have to run 1 loops instead of 2.
template<std::size_t D, typename TA, typename TB>  
bool allEqual(const Vec<D, TA>& va, const TB& value)
{
    for(std::size_t i(0); i < D; ++i)
		if(va[i] != value) return 0;

	return 1; 
}

//utility
///\relates nytl::Vec
///\return The sum of all Vector components.
template<std::size_t D, typename T>  
auto sum(const Vec<D, T>& v) -> decltype(v[0] + v[0])
{
    auto ret = decltype(v[0] + v[0]) {};
    for(auto& val : v) ret += val;
    return ret;
}

///\relates nytl::Vec
///\return The product of all Vector components.
template<std::size_t D, typename T>  
auto multiply(const Vec<D, T>& v) -> decltype(v[0] * v[0])
{
    auto ret = decltype(v[0] * v[0]) (1);
    for(auto& val : v) ret *= val;
    return ret;
}

///\relates nytl::Vec
///\return The length of the Vector (square-root of the sum of all its component squared).
template<std::size_t D, typename T>
auto length(const Vec<D, T>& v) -> decltype(sqrt(v[0] * v[0]))
{
    decltype(v[0] * v[0]) ret{};
    for(auto& val : v) ret += val * val;
    return std::sqrt(ret);
}

///\relates nytl::Vec
///Alias function for length.
///\return the length (norm) of the given Vector.
template<std::size_t D, typename T>
auto norm(const Vec<D, T>& v) -> decltype(length(v))
{
    return length(v);
}

///\relates nytl::Vec
///\return The dot product of the given Vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
auto dot(const Vec<DA, TA>& va, const Vec<DB, TB>& vb) -> decltype(sum(va * vb))
{
    return sum(va * vb);
}

///\relates nytl::Vec
///Alias function for dot.
///\return The dot (scalar) product of the given Vectors.
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
auto scalar(const Vec<DA, TA>& va, const Vec<DB, TB>& vb) -> decltype(sum(va * vb))
{
    return sum(va * vb);
}

///\relates nytl::Vec
///\return The cross product for 2 3-Densional Vectors.
template<typename TA, typename TB> 
auto cross(const Vec<3, TA>& va, const Vec<3, TB>& vb) -> Vec<3, decltype(va[0] * vb[0])>
{
    return Vec<3, decltype(va[0] * vb[0])>
		{
			va[2] * va[3] - va[3] * vb[2], 
			va[3] * va[1] - va[1] * vb[3], 
			va[1] * va[2] - va[2] * vb[1]
		};
}

///\relates nytl::Vec
///\return The angle between 2 Vecs in radiant form. Returns always the smaller one; angle <= PI. 
template<std::size_t DA, std::size_t DB, typename TA, typename TB>
double angle(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return std::acos(sum(va * vb) / (length(va) * length(vb)));
}

///\relates nytl::Vec
///\return The smallest angle between two Lines with the given Vectors as diRection in radiant 
///form. The Returned angle is always <= PI/2. 
template<std::size_t DA, std::size_t DB, typename TA, typename TB> 
double smallerAngle(const Vec<DA, TA>& va, const Vec<DB, TB>& vb)
{
    return std::acos(abs(sum(va * vb)) / (length(va) * length(vb)));
}

//todo: cangle for 3-Densional (or all) Vectors
///\relates nytl::Vec
///\return The absolute, clockwise angle between two 2-Densional Vectors in radian form.
template<typename TA, typename TB> 
double cangle(const Vec<2, TA>& va, const Vec<2, TB>& vb)
{
    auto val = atan2(va.y, va.x) - atan2(vb.y, vb.x);
    if(val <= 0) return (2 * cPi) + val;
    return val;
}

///\relates nytl::Vec
///\return A normalized (i.e. length = 1, preserving the direction) copy of the given Vec.
template<std::size_t D, typename T>
auto normalize(const Vec<D, T>& va) -> decltype(va / length(va))
{
    return va / length(va);
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>
auto pow(Vec<D, TA> base, const TB& exp) -> Vec<D, decltype(std::pow(base[0], exp))>
{
	for(auto& val : base)
	   val = std::pow(val, exp);

	return base;	
}

///\relates nytl::Vec
///\return The distance between two points represented as Vecs.
template<std::size_t D, typename T>
auto distance(const Vec<D, T>& va, const Vec<D, T>& vb) -> decltype(length(va - vb))
{
    return length(vb - va);
}

///\relates nytl::Vec
template<std::size_t D, typename T> 
Vec<D, double> radians(Vec<D, T> va)
{
    for(auto& val : va) val = radians(val);
    return va;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, double> degrees(Vec<D, T> va)
{
    for(auto& val : va) val = degrees(val);
    return va;
}

///\relates nytl::Vec
template<std::size_t D, typename T>
Vec<D, T> abs(Vec<D, T> va)
{
    for(auto& val : va) val = std::abs(val);
    return va;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB, typename Tc>  
Vec<D, TA> clamp(Vec<D, TA> val, const Vec<D, TB>& minVal, const Vec<D, Tc>& maxVal)
{
    for(std::size_t i(0); i < min(min(val.size(), minVal.size()), maxVal.size()); ++i) 
		val[i] = clamp(val[i], minVal[i], maxVal[i]);
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB, typename Tc>  
Vec<D, TA> clamp(Vec<D, TA> val, const TB& minVal, const Tc& maxVal)
{
    for(auto& v : val) v = clamp(v, minVal, maxVal);
    return val;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, TA> mix(Vec<D, TA> x, const Vec<D, TA>& y, const Vec<D, TB>& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a[i]);
    return x;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, TA> mix(Vec<D, TA> x, const Vec<D, TA>& y, const TB& a)
{
    for(std::size_t i(0); i < min(min(x.size(), y.size()), a.size()); ++i) 
		x[i] = mix(x[i], y[i], a);
    return x;
}

///\relates nytl::Vec
template<std::size_t D, typename TA, typename TB>  
Vec<D, TA> mix(const TA& x, const TA& y, const Vec<D, TB>& a)
{
	Vec<D, TA> ret(a.size());
    for(std::size_t i(0); i < a.size(); ++i) ret[i] = mix(x, y, a[i]);
    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
auto smallest(const Vec<D, T>& a)
{
    auto ret = a[0];
    for(auto& val : a)
		if(val < ret) ret = val;

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
auto greatest(const Vec<D, T>& a)
{
    auto ret = a[0];
    for(auto& val : a)
        if(val > ret) ret = val;

    return ret;
}

//boolean Vec operations
///\relates nytl::Vec
template<std::size_t D>  
bool any(const Vec<D, bool>& v)
{
    for(auto val : v) if(val) return 1;
    return 0;
}

///\relates nytl::Vec
template<std::size_t D>  
bool all(const Vec<D, bool>& v)
{
    for(auto val : v) if(!val) return 0;
    return 1;
}

///\relates nytl::Vec
template<std::size_t D>  
bool none(const Vec<D, bool>& v)
{
    for(auto val : v) if(val) return 0;
    return 1;
}

///\relates nytl::Vec
///Helper function accessing the nytl::Vec::subvec member template.
template<std::size_t N, std::size_t D, typename T>
Vec<N, T> subVec(const Vec<D, T>& va, std::size_t pos = 0)
{
	return va.template subVec<N>(pos);
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> operator!(Vec<D, T> v)
{
    for(auto& val : v) val = !val;
    return v;
}

//component-wise
///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> max(const Vec<D, T>& va, const Vec<D, T>& vb)
{
    Vec<D, T> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = max(va[i], vb[i]);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> max(const Vec<D, T>& va, const T& value)
{
    Vec<D, T> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = max(va[i], value);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> min(const Vec<D, T>& va, const Vec<D, T>& vb)
{
    Vec<D, T> ret(min(va.size(), vb.size()));
    for(std::size_t i(0); i < min(va.size(), vb.size()); ++i)
        ret[i] = min(va[i], vb[i]);

    return ret;
}

///\relates nytl::Vec
template<std::size_t D, typename T>  
Vec<D, T> min(const Vec<D, T>& va, const T& value)
{
    Vec<D, T> ret(va.size());
    for(std::size_t i(0); i < va.size(); ++i)
        ret[i] = min(va[i], value);
    
    return ret;
}

