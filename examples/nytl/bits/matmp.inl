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

#include <nytl/bits/tmpUtil.inl>

namespace detail
{

//makeRowRefVec
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>> 
struct MakeRowRefVec;

template<std::size_t Size, std::size_t... I> 
struct MakeRowRefVec<Size, std::index_sequence<I...>>
{
    template<std::size_t R, std::size_t C, typename P>
    static RefVec<sizeof...(I), P> call(Vec<R, Vec<C, P>>& v, std::size_t i)
    {
        return RefVec<sizeof...(I), P>(v[I][i]...);
    }
};

//makeRowVec
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>> 
struct MakeRowVec;

template<std::size_t Size, std::size_t... I> 
struct MakeRowVec<Size, std::index_sequence<I...>>
{
    template<size_t R, size_t C, typename P>
    static Vec<sizeof...(I), P> call(const Vec<R, Vec<C, P>>& v, std::size_t i)
    {
        return Vec<sizeof...(I), P>(v[I][i]...);
    }
};

//initMat
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>> 
struct InitMatData;

template<std::size_t Size, std::size_t... I> 
struct InitMatData<Size, std::index_sequence<I...>>
{
    template<size_t R, size_t C, typename P, typename... Args>
    static void call(Vec<R, Vec<C, P>>& v, std::tuple<Args...> args)
    {
        Expand{0, ((v[I / C][I % C] = std::get<I>(args)), 0)... };
    }

    template<size_t R, size_t C, typename P>
    static void call(Vec<R, Vec<C, P>>& v, const P& val)
    {
        Expand{0, ((v[I / C][I % C] = val), 0)... };
    }
};

//copyMatData
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>> 
struct CopyMatData;

template<std::size_t Size, size_t... I> 
struct CopyMatData<Size, std::index_sequence<I...>>
{
    template<size_t R, size_t C, typename P>
    static std::unique_ptr<P[]> call(const Vec<R, Vec<C, P>>& v)
    {
        return std::unique_ptr<P[]>(new P[R * C]{v[I / C][I % R]...});
    }
};

//makeRowRefVec
template<std::size_t Size, typename Seq = std::make_index_sequence<Size>> 
struct MatTuple;

template<std::size_t Size, size_t... I> 
struct MatTuple<Size, std::index_sequence<I...>>
{
    template<size_t R, size_t C, typename P>
    static TypeTuple<P, sizeof...(I)> call(const Vec<R, Vec<C, P>>& v)
    {
        return TypeTuple<P, sizeof...(I)>{v[I / C][I % R]...};
    }
};

}
