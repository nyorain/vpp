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

//member
template<std::size_t D, typename P>
double Triangle<D, P>::size() const
{
	return 0.5 * length(ab()) * length(bc()) * std::sin(angleB());
}

template<std::size_t D, typename P>
Vec<D, P> Triangle<D, P>::center() const
{
	return (a + b + c) / 3;

}

///Efficient barycentric coordinates implementation for triangles.
template<typename P>
Vec3f barycentric(const Triangle<2, P>& tri, const Vec<2, P>& cart)
{
	if(tri.size() == 0.f) throw std::logic_error("nytl::barycentric: invalid triangle");

	auto v0 = tri.b - tri.a;
	auto v1 = tri.c - tri.a;
	auto v2 = cart - tri.a;
	auto d = 1.f / (v0.x * v1.y - v1.x * v0.y);

	Vec3f ret;
	ret.x = (v2.x * v1.y - v1.x * v2.y) * d;
	ret.y = (v0.x * v2.y - v2.x * v0.y) * d;
	ret.z = 1.f - (ret.x + ret.y);
	return ret;
}

///Returns whether the given triangle contains the given point
template<typename P>
bool contains(const Triangle<2, P>& tri, const Vec<2, P>& p)
{
	if(tri.size() == 0.f) return false;
	auto bar = barycentric(tri, p);
	return (all(bar <= 1.f) && all(bar >= 0.f) && sum(bar) == 1.f);
}
