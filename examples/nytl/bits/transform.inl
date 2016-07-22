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

#ifdef DOXYGEN
namespace nytl{
#endif

namespace detail
{

//returns the plane (e.g. 0,1 for xy or 2,3 for zw) for a given id in a given dimension
inline Vec2<std::size_t> indexPlane(std::size_t dim, std::size_t idx)
{
	auto ret = Vec2<std::size_t> {};

	while(idx >= dim - 1)
	{
		ret[0]++;
		idx -= (dim - 1);
		dim--;
	}

	ret[1] = idx + 1;

	return ret;
}

}

//basic ops
///\relates Mat Transform
template<std::size_t D, typename P>
void scale(SquareMat<D, P>& mat, const Vec<D - 1, P>& scal)
{
	for(std::size_t i(0); i < D - 1; ++i)
		mat[i][i] *= scal[i];
}

///\relates Mat Transform
template<std::size_t D, typename P>
void translate(SquareMat<D, P>& mat, const Vec<D - 1, P>& trans)
{
	for(std::size_t i(0); i < D - 1; ++i)
		mat[i][D - 1] += trans[i];
}

///\relates Mat Transform
template<typename P>
void rotate(SquareMat<3, P>& mat, const Vec<1, P>& planeRot)
{
	auto rotMat = identityMat<3>();

	auto c = std::cos(planeRot[0]);
	auto s = std::sin(planeRot[0]);

	rotMat[0][0] = c;
	rotMat[0][1] = -s;
	rotMat[1][0] = s;
	rotMat[1][1] = c;

	mat *= rotMat;
}

template<typename P>
void rotate(SquareMat<4, P>& mat, const Vec3<P>& vec, const P& angle)
{
	const P c = std::cos(angle);
	const P s = std::sin(angle);

	Vec3<P> axis = normalize(vec);
	Vec3<P> temp = (P(1) - c) * axis;

	auto rotate = identityMat<4, P>();
	rotate[0][0] = c + temp[0] * axis[0];
	rotate[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
	rotate[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

	rotate[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
	rotate[1][1] = c + temp[1] * axis[1];
	rotate[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

	rotate[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
	rotate[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
	rotate[2][2] = c + temp[2] * axis[2];
/*
	mat[0] = mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2];
	mat[1] = mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2];
	mat[2] = mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2];
*/
	mat *= rotate;
}

template<typename P>
void rotate(SquareMat<4, P>& mat, const Vec3<P>& vec)
{
	rotate(mat, Vec3<P>(normalize(vec)), P(length(vec)));
}

//copy helper
///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> scaleCopy(SquareMat<D, P> mat, const Vec<D, P>& scal)
{
	scale(mat, scal);
	return mat;
}

///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> translateCopy(SquareMat<D, P> mat, const Vec<D, P>& trans)
{
	translate(mat, trans);
	return mat;
}

///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> rotateCopy(SquareMat<D, P> mat, const Vec<D, P>& axes, const P& angle)
{
	rotate(mat, axes, angle);
	return mat;
}

///\relates Mat Transform
template<std::size_t D, typename P>
SquareMat<D, P> rotateCopy(SquareMat<D, P> mat, const Vec<D, P>& rot)
{
	rotate(mat, rot);
	return mat;
}

template<typename P>
SquareMat<4, P> perspective3(P left, P right, P top, P bottom, P pnear, P far)
{
	auto ret = SquareMat<4, P>(0);

	ret[0][0] = (P(2) * pnear) / (right - left);
	ret[1][1] = (P(2) * pnear) / (top - bottom);

	ret[2][0] = (right + left) / (right - left);
	ret[2][1] = (top + bottom) / (top - bottom);
	ret[2][2] = (-far - pnear) / (far - pnear);
	ret[2][3] = -1;

	ret[3][2] = (P(-2) * far * pnear) / (far - pnear);
	return ret;
}

template<typename P>
SquareMat<4, P> perspective3Symmetrical(P width, P height, P pnear, P pfar)
{
	return perspective3(-width / P(2), width / P(2), height / P(2), -height / P(2), pnear, pfar);
}

template<typename P>
SquareMat<4, P> perspective3(P fov, P aspect, P pnear, P pfar)
{
	P const f = P(1) / std::tan(fov / P(2));

	auto ret = Mat4<P>(0);
	ret[0][0] = f / aspect;
	ret[1][1] = f;

	ret[2][2] = -(pfar + pnear) / (pfar - pnear);
	ret[2][3] = -1;

	ret[3][2] = -(P(2) * pfar * pnear) / (pfar - pnear);
	return ret;
}


template<typename P>
SquareMat<4, P> ortho3(P left, P right, P top, P bottom, P pnear, P far)
{
	auto ret = Mat4<P>(0);

	ret[0][0] = P(2) / (right - left);
	ret[1][1] = P(2) / (top - bottom);

	ret[2][2] = P(-2) / (far - pnear);

	ret[3][0] = - ((right + left) / (right - left));
	ret[3][1] = - ((top + bottom) / (top - bottom));
	ret[3][2] = - (far + pnear) / (far - pnear);
	ret[3][3] = 1;
	return ret;
}

template<typename P>
SquareMat<4, P> ortho3(P width, P height, P pnear, P pfar)
{
	return ortho3(-width / P(2), width / P(2), height / P(2), -height / P(2), pnear, pfar);
}

template<typename P>
SquareMat<4, P> lookAt(const Vec3<P>& eye, const Vec3<P>& center, const Vec3<P>& up)
{
	const auto f = normalize(center - eye);
	const auto s = normalize(cross(f, up));
	const auto u = cross(s, f);

	auto ret = identityMat<4, P>();
	ret.col(0) = s;
	ret.col(1) = u;
	ret.col(2) = -f;
	ret[3][0] = -dot(s, eye);
	ret[3][1] = -dot(u, eye);
	ret[3][2] = dot(f, eye);

	return ret;
}

#ifdef DOXYGEN
}
#endif
