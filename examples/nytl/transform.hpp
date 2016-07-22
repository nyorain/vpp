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
///\brief Provides a transform class for translation, rotation and scaling of several primitives.

#pragma once

#include <nytl/mat.hpp>
#include <nytl/vec.hpp>
#include <nytl/rect.hpp>
#include <nytl/scalar.hpp>

#include <cmath>

namespace nytl
{

///\relates Transform
///Returns the possible axis of rotation for a given dimension.
///To be more precise it returns the planes of rotations that exist in the given
///dimension. For 2 or 3 dimensions, thinking about rotations of axis might work, while
///for higher dimensions it is easier to not think about rotations as axis, but rather 
///about rotations ON planes. 
///E.g. in 2D, there is only one plane (xy), in 3D there are 3 planes (xy, xz, yz) and in
///4D there are 6 such planes on which a shape can be rotated.
///\note Constexpr function, can be used for template parameters.
constexpr std::size_t rotationPlanes(std::size_t dimension) { return fac(dimension) / 2; }

#include <nytl/bits/transform.inl>

///\ingroup math
///Transform state able to hold rotation,scale and translation and express them in a Matrix.
///
///\tparam D The dimension of transforMation.
///\tparam P the precision of the transformations, defaulted to float.
///
///The Transform class effectively just holds a SquareMat<D + 1, P> object in which it stores
///the applied transforMations. Therefore it is not possible to retrieve the scaling/rotation/
///translation values sperately from the Matrix.
///Since it does therefore apply all transforMations immediatly to the Matrix, the order in which
///the transforMations are applied may change the outcome.
///
///\code
///auto transform = nytl::Transform2 {};
///
/////First translates and then rotates the transform Matrix.
////The rotation will be around the origin, so in respect of the previous translation.
///transform.translate({100, 100});
///transform.rotate(45);
///
/////First rotates and then translated the transform Matrix. 
///transform.rotate(45);
///transform.translate({100, 100});
///
/////To use the transform now (e.g. for a graphics library) you can work with the (D+1) sized
/////SqaureMat (e.g. as openGL uniform)
///auto mat = transform.mat(); //< holds all transforms in the applied order
///\endcode
///
///Alternatively to the transform object, one can use the raw nytl::translate, nytl::scale or
///nytl::rotate functions when operating directly on a Matrix.
///The Transform class i escpecially useful as base class for transformable types (e.g. shapes).
template<std::size_t D, typename P = float> class Transform;

using Transform2 = Transform<2>;
using Transform3 = Transform<3>;
using Transform4 = Transform<4>;

template<std::size_t D, typename P>
class Transform
{
public:
    static constexpr std::size_t dim = D;

	using Precision = P;
	using VecType = Vec<dim, Precision>;
	using MatType = SquareMat<dim + 1, Precision>;
	using RotType = Vec<rotationPlanes(dim), Precision>;
	using RectType = Rect<dim, Precision>;

protected:
	MatType mat_ {};

public:
    Transform() noexcept : mat_(identityMat<dim + 1, P>()) {} 
    ~Transform() noexcept = default;

    void rotate(const RotType& rotation){ nytl::rotate(mat_, rotation); }
    void translate(const VecType& translation){ nytl::translate(mat_, translation); }
    void scale(const VecType& scaling){ nytl::scale(mat_, scaling); }

	void resetTransform() { identity(mat_); };

    MatType& transformMatrix() { return mat_; }
    const MatType& transformMatrix() const { return mat_; }
};


}
