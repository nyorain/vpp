/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distbute, sublicense, and/or sell
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

//tests
///\relates RectRegion
template<std::size_t D, typename P>
bool intersects(const RectRegion<D, P>& ra, const RectRegion<D, P>& rb)
{
    for(auto& r1 : ra.rects())
    {
        for(auto& r2 : rb.rects())
        {
            if(intersects(r1, r2)) return true;
        }
    }

    return false;
}
template<std::size_t D, typename P>
bool intersects(const RectRegion<D, P>& ra, const Rect<D, P>& rb)
{
    for(auto& r1 : ra.rects())
    {
        if(intersects(r1, rb)) return true;
    }

    return false;
}
template<std::size_t D, typename P>
bool intersects(const RectRegion<D, P>& ra, const Line<D, P>& lb)
{
    for(auto& ra : ra.rects())
    {
        if(intersects(ra, lb)) return true;
    }

    return false;
}
template<std::size_t D, typename P>
bool intersects(const RectRegion<D, P>& ra, const Triangle<D, P>& tb)
{
    for(auto& ra : ra.rects())
    {
        if(intersects(ra, tb)) return true;
    }

    return false;
}
template<std::size_t D, typename P> 
bool intersects(const Rect<D, P>& a, const RectRegion<D, P>& b){ return intersects(b, a); }

template<std::size_t D, typename P> 
bool intersects(const Line<D, P>& a, const RectRegion<D, P>& b){ return intersects(b, a); }

template<std::size_t D, typename P>
bool intersects(const Triangle<D, P>& a, const RectRegion<D, P>& b){ return intersects(b, a); }

//contains
template<std::size_t D, typename P>
bool contains(const RectRegion<D, P>& ra, const RectRegion<D, P>& rb)
{
    for(auto& r1 : ra.rects())
    {
        for(auto& r2 : rb.rects())
        {
            if(!contains(r1, r2)) return false;
        }
    }

    return true;
}
template<std::size_t D, typename P>
bool contains(const RectRegion<D, P>& ra, const Rect<D, P>& rb)
{
    for(auto& ra : ra.rects())
    {
        if(!contains(ra, rb)) return false;
    }

    return true;
}
template<std::size_t D, typename P>
bool contains(const RectRegion<D, P>& ra, const Line<D, P>& lb)
{
    for(auto& ra : ra.rects())
    {
        if(!contains(ra, lb)) return false;
    }

    return true;
}
template<std::size_t D, typename P>
bool contains(const RectRegion<D, P>& ra, const Triangle<D, P>& tb)
{
    for(auto& ra : ra.rects())
    {
        if(!contains(ra, tb)) return false;
    }

    return true;
}
template<std::size_t D, typename P>
bool contains(const RectRegion<D, P>& ra, const Vec<D, P>& pb)
{
    for(auto& r1 : ra.rects())
    {
        if(contains(r1, pb)) return true;
    }

    return false;
}

template<std::size_t D, typename P>  
bool contains(const Rect<D, P>& a, const RectRegion<D, P>& b)
{
	for(auto& r1 : b.rects())
	{
		if(!contains(a, r1)) return false;
	}

	return true;
}

template<std::size_t D, typename P>  
bool contains(const Triangle<D, P>& a, const RectRegion<D, P>& b)
{
	for(auto& r1 : b.rects())
	{
		if(!contains(a, r1)) return false;
	}

	return true;
}

//functions/////////////////////////////////////
template<std::size_t D, typename P>
void RectRegion<D, P>::add(const RectType& r)
{
    std::vector<Rect<D, P>> rVec{r};
    for(auto& r1 : rects_)
    {
		std::size_t size = rVec.size();
        for(std::size_t i(0); i < size; ++i)
        {
            if(intersects(r1, rVec[i]))
            {
                auto result = difference(rVec[i], r1);
                if(!result.empty())
                {
                    rVec[i] = result[0];
                    rVec.insert(rVec.cend(), result.cbegin() + 1, result.cend());
                }
                else
                {
                    rVec.erase(rVec.cbegin() + i);
                    --i;
                }
            }
        }
    }

    if(!rVec.empty()) rects_.insert(rects_.cend(), rVec.cbegin(), rVec.cend());
}

template<std::size_t D, typename P>
void RectRegion<D, P>::add(const RectRegionType& r)
{
    for(auto& r1 : r.rects())
        add(r1);
}

template<std::size_t D, typename P>
void RectRegion<D, P>::subtract(const RectType& r)
{
	std::vector<Rect<D,P>> rVec{r};
	std::vector<Rect<D, P>> addVec{};

    for(auto it = rects_.begin(); it != rects_.end(); ++it)
    {
		auto& r1 = *it;

		std::size_t size = rVec.size();
		for(std::size_t i(0); i < size; ++i)
		{
			if(intersects(r1, rVec[i]))
			{
				auto result = difference(rVec[i], r1);
				if(!result.empty())
				{
					r1 = result[0];
					addVec.insert(addVec.cend(), result.cbegin() + 1, result.cend());				
				}
				else
				{
					it = rects_.erase(it);
				}
			}
		}
    }
}

template<std::size_t D, typename P>
void RectRegion<D, P>::subtract(const RectRegionType& r)
{
    for(auto& r1 : r.rects())
        subtract(r1);
}

//operators//////
template<std::size_t D, typename P>
RectRegion<D, P>& RectRegion<D, P>::operator|=(const RectType& r)
{
    add(r);
    return *this;
}

template<std::size_t D, typename P>
RectRegion<D, P>& RectRegion<D, P>::operator|=(const RectRegionType& r)
{
    add(r);
    return *this;
}

template<std::size_t D, typename P>
RectRegion<D, P>& RectRegion<D, P>::operator&=(const RectType& r)
{
    std::vector<Rect<D,P>> newRects;
    for(auto& rec : rects_)
    {
        if(intersects(rec, r)) newRects.push_back(r & rec);
    }

    rects_ = std::move(newRects);
    return *this;
}

template<std::size_t D, typename P>
RectRegion<D, P>& RectRegion<D, P>::operator&=(const RectRegionType& r)
{
    std::vector<Rect<D,P>> newRects;
    for(auto& rec : rects_)
    {
        for(auto& rec2 : r.rects_)
        {
            if(intersects(rec, rec2)) newRects.push_back(rec & rec2);
        }
    }

    rects_ = std::move(newRects);
    return *this;
}

template<std::size_t D, typename P>
RectRegion<D, P>& RectRegion<D, P>::operator^=(const RectType& r)
{
    auto reg = r & *this;
    add(r);
    subtract(reg);

    return *this;
}

template<std::size_t D, typename P>
RectRegion<D, P>& RectRegion<D, P>::operator^=(const RectRegionType& r)
{
    auto reg = r & *this;
    add(r);
    subtract(reg);

    return *this;
}

//external operators///////
//move needed because it is a expression, not an identifier
template<std::size_t D, typename P>
RectRegion<D, P> operator|(RectRegion<D, P> a, const RectRegion<D, P>& b)
{
    return std::move(a |= b);
}

template<std::size_t D, typename P>
RectRegion<D, P> operator|(RectRegion<D, P> a, const Rect<D, P>& b)
{
    return std::move(a |= b);
}

template<std::size_t D, typename P>
RectRegion<D, P> operator&(RectRegion<D, P> a, const RectRegion<D, P>& b)
{
    return std::move(a &= b);
}

template<std::size_t D, typename P>
RectRegion<D, P> operator&(RectRegion<D, P> a, const Rect<D, P>& b)
{
    return std::move(a &= b);
}


template<std::size_t D, typename P>
RectRegion<D, P> operator^(RectRegion<D, P> a, const RectRegion<D, P>& b)
{
    return std::move(a ^= b);
}

template<std::size_t D, typename P>
RectRegion<D, P> operator^(RectRegion<D, P> a, const Rect<D, P>& b)
{
    return std::move(a ^= b);
}

#ifdef DOXYGEN
}
#endif
