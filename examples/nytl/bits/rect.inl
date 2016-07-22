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

///\relates nytl::Rect
template<std::size_t D, typename P> bool 
intersects(const Rect<D, P>& r1, const Rect<D, P>& r2)
{
    return all(r2.position < r1.position + r1.size) && all(r1.position < r2.position + r2.size);
}

///\relates nytl::Rect
template<std::size_t D, typename P> bool 
intersects(const Rect<D, P>& r1, const Line<D, P>& l2)
{
    //TODO: algorithm might be wrong, might have bugs, !important
    if(contains(r1, l2.a) || contains(r1, l2.b)) return 1;
    if(!l2.definedFor(r1.position[0], 0) || !l2.definedFor(r1.position[0] + r1.size[0], 0)) 
		return 0;

    auto st = l2.valueAt(r1.position[0]);
    auto en = l2.valueAt(r1.position[0] + r1.size[0]);

    for(std::size_t i(1); i < D; ++i)
    {
        int stV = 1;
        int enV = 1;

        if(st[i] < r1.position[i]) stV = 0;
        else if(st[i] > r1.position[i] + r1.size[i]) stV = 2;

        if(en[i] < r1.position[i]) enV = 0;
        else if(en[i] > r1.position[i] + r1.size[i]) enV = 2;

        if(stV == enV && stV != 1 && enV != 1) return false;
    }

    return true;
}

///\relates nytl::Rect
template<std::size_t D, typename P> bool 
intersects(const Line<D, P>& a, const Rect<D, P>& b){ return intersects(b, a); }

//contains
///\relates nytl::Rect
template<std::size_t D, typename P> bool 
contains(const Rect<D, P>& r1, const Rect<D, P>& r2)
{
    return contains(r1, r2.position) && contains(r1, r2.position + r2.size);
}
///\relates nytl::Rect
template<std::size_t D, typename P> bool 
contains(const Rect<D, P>& r1, const Line<D, P>& l2)
{
    return contains(r1, l2.a) && contains(r1, l2.b);
}
///\relates nytl::Rect
template<std::size_t D, typename P> bool 
contains(const Rect<D, P>& r1, const Vec<D, P>& v2)
{
    return all(r1.position <= v2) && all(v2 <= r1.position + r1.size);
}


///\relates nytl::Rect
///\brief Returns the difference between two rangles [AND ~].
///\details Subtracts the second Rect from the first one and returns the rest of the first one. 
///Since the result cant be expressed as a single Rect, it is a Vector of those. 
///Asymmetrical operator.
template<std::size_t D, class P>
std::vector<Rect<D, P>> difference(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    std::vector<Rect<D, P>> ret;
    if(!intersects(ra, rb)) return std::vector<Rect<D, P>>{ra};

    for(std::size_t i(0); i < D; ++i)
    {
        if(ra.position[i] < rb.position[i]) //begin Rect
        {
            auto pos = ra.position;
            for(std::size_t o(0); o < i; ++o)
                pos[o] = rb.position[o];

            auto size = (ra.position + ra.size) - pos;
            size[i] = rb.position[i] - pos[i];

            ret.push_back(Rect<D,P>(pos, size));
        }

        if(ra.position[i] + ra.size[i] > rb.position[i] + rb.size[i]) //end Rect
        {
            auto pos = ra.position;
            pos[i] = rb.position[i] + rb.size[i];
            for(std::size_t o(0); o < i; ++o)
                pos[o] = std::max(rb.position[o], ra.position[o]);

            auto size = (ra.position + ra.size) - pos;
            for(std::size_t o(0); o < i; ++o)
                size[o] = (rb.position[o] + rb.size[o]) - pos[o];

            ret.push_back(Rect<D, P>(pos, size));
        }
    }

    return ret;
}

///\relates nytl::Rect
///\brief Returns the rangle in which area the two paramater rangles intersect [AND]. 
///\details Symmetrical operator.
template<std::size_t D, typename P>
Rect<D, P> intersection(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    auto pos = max(ra.topLeft(), rb.topLeft());
    auto size = min(ra.bottomRight(), rb.bottomRight()) - pos;
    return Rect<D, P>(pos, size); //max and min component-wise
}

///\relates nytl::Rect
///\brief Returns the union of two Rects [OR]. 
//\details Since it combines them the result cannot be expressed in a single rangle and is 
///therefore a Vector. Symmetrical operator.
template<std::size_t D, typename P>
std::vector<Rect<D, P>> combination(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    auto ret = difference(ra, rb);
    ret.push_back(rb);
    return ret;
}

///\relates nytl::Rect
///\brief Returns the symmetric difference between two rangles [XOR].
///\details Basically just the area where exactly one of them is placed. Symmetrical operator.
template<std::size_t D, typename P>
std::vector<Rect<D, P>> symmetricDifference(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    //return combination(difference(ra, rb), difference(rb, ra));
    auto result = combination(ra, rb);
    auto ret = result;

    for(std::size_t i(0); i < result.size(); i++)
    {
        auto vVec = difference(result[i], intersection(ra, rb));
        if(!vVec.empty())
        {
            ret[i] = vVec[0];
            ret.insert(ret.cend(), vVec.cbegin() + 1, vVec.cend());
        }
    }

    return ret;
}


///\relates nytl::Rect
template<std::size_t D, typename P>
Rect<D, P> operator&(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    return intersection(ra, rb);
}

///\relates nytl::Rect
template<std::size_t D, typename P>
std::vector<Rect<D, P>> operator^(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    return symmeticDifference(ra, rb);
}

///\relates nytl::Rect
template<std::size_t D, typename P>
std::vector<Rect<D, P>> operator|(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    return combination(ra, rb);
}

template<std::size_t D, typename P>
std::vector<Rect<D, P>> operator-(const Rect<D, P>& ra, const Rect<D, P>& rb)
{
    return difference(ra, rb);
}

///\relates nytl::Rect
template<size_t D, typename T> 
std::ostream& operator<<(std::ostream& os, const Rect<D, T>& obj)
{
    os << "position: " << obj.position << " size: " << obj.size;
    return os;
}

