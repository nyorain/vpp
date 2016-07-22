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

//members
template<std::size_t R, std::size_t C, typename P>
template<std::size_t OR, std::size_t OC, typename OP>
Mat<R, C, P>::operator Mat<OR, OC, OP>() const
{
	Mat<OR, OC, OP> ret({});
	detail::InitMatData<min(OR, R) * min(OC, C)>::call(ret.data_, asTuple());
	return ret;
}

template<std::size_t R, std::size_t C, typename P>
template<typename TD>
typename std::enable_if_t<Mat<R, C, P>::is_squared, TD> Mat<R, C, P>::invert()
{
	//TODO
}


//exceptions
///\relates Mat
///\brief Exception class deriving std::invalid_argument.
///\details Thrown by operational Matrix functions that do not work for singular Matrices
///but receive a singular Matrix as argument.
class InvalidMatrix : public std::invalid_argument
{
public:
	InvalidMatrix() : std::invalid_argument("Invalid Matrix argument") {}
	InvalidMatrix(const std::string& func)
		: std::invalid_argument("Invalid Matrix argument in function " + func) {}
};

///\relates Mat
///\brief Returns a tranposed copy of the given matrix, i.e. switches rows and colums.
///\details Useful when column-major matrix buffers are needed (e.g. glsl).
template<std::size_t R, std::size_t C, typename P>
Mat<C, R, P> transpose(const nytl::Mat<R, C, P>& mat)
{
	Mat<C, R, P> ret;
	for(std::size_t i(0); i < R; ++i)
		ret.col(i) = mat.row(i);

	return ret;
}

///\relates Mat
///\brief Returns a tranposed copy of the given matrix, i.e. switches rows and colums.
///\details Useful when column-major matrix buffers are needed (e.g. glsl).
///\sa transpose
template<std::size_t R, std::size_t C, typename P>
Mat<C, R, P> transposition(const Mat<R, C, P>& mat)
{
	Mat<C, R, P> ret;
	for(std::size_t i(0); i < R; ++i)
		ret.col(i) = mat.row(i);

	return ret;
}

///\relates Mat
///\brief Transposes a given SquareMatrix in place, i.e. switches rows and colums.
///\sa transposition
template<std::size_t D, typename P>
void transpose(SquareMat<D, P>& mat)
{
	auto cpy = mat; //copy of the given matrix to not override (->lose) values
	for(std::size_t i(0); i < D; ++i)
		mat.col(i) = cpy.row(i);
}

///\relates Mat
///\brief Returns the identityMatrix for the given dimension and precision.
template<std::size_t D, typename P = float>
SquareMat<D, P> identityMat()
{
	SquareMat<D, P> ret(0);
	for(size_t i(0); i < D; i++) ret[i][i] = 1;
	return ret;
}

///\relates Mat
///\return The inverse of the given square Matrix
template<std::size_t D, typename P>
SquareMat<D, P> inverse(const SquareMat<D, P>& m)
{
	auto cpy = m;
	return m.invert();
}

//XXX: corRect implementation?
//full pivot?

///\relates Mat
///\brief Rearranges the Matrix rows.
///\return The sign of the used pivot Matrix.
template<std::size_t R, std::size_t C, typename P>
int pivot(Mat<R, C, P>& m)
{
	int ret = 1;

	for(std::size_t c(0); c < C; ++c)
	{
		std::size_t maxR = c;
		for(std::size_t r(c); r < R; ++r)
		{
			if(std::abs(m[r][c]) > std::abs(m[maxR][c]))
			maxR = r;
		}

		if(maxR != c)
		{
			m.swapRow(c, maxR);
			ret *= -1;
		}
	}

	return ret;
}

///\relates Mat
///\brief Computes a luDecomposition of a non-singular Matrix.
///\return 2 Mats, The lower (l, first Mat) and the upper one(u, second Mat).
///\exception nytl::InvalidMatrix if the given Matrix is
///not corRectly pivotized.
template<std::size_t D, typename P>
Vec2<Mat<D, D, double>> luDecomposition(const Mat<D, D, P>& m)
{
	Vec2<Mat<D, D, P>> lu{};
	lu[0] = identityMat<D, P>();
	lu[1].fill(0);

	for(std::size_t r(0); r < D; ++r)
	{
		for(std::size_t c(0); c < D; ++c)
		{
			P vsum = 0;

			//u
			if(c >= r)
			{
				for(std::size_t k(0); k < r; ++k)
					vsum += lu[1][k][c] * lu[0][r][k];

				lu[1][r][c] = m[r][c] - vsum;
			}

			//l
			else
			{
				if(lu[1][c][c] == 0)
				{
					throw InvalidMatrix("nytl::luDecomposition, needs pivoting");
				}

				vsum = 0;

				for(std::size_t k(0); k < c; ++k)
					vsum += lu[1][k][c] * lu[0][r][k];

				lu[0][r][c] = (m[r][c] - vsum) / lu[1][c][c];
			}
		}
	}

	return lu;
}

///\relates Mat
///\brief Composutes the product of all diagonal elements of a square-Mat.
template<std::size_t D, typename P>
P diagonalMult(const Mat<D, D, P>& m)
{
	P ret = 1;
	for(std::size_t i(0); i < D; ++i)
		ret *= m[i][i];

	return ret;
}

///\relates Mat
///\brief Computes the determinant of a given non-singular Matrix.
template<std::size_t D, typename P>
double det(const Mat<D, D, P>& m)
{
	auto cpy = m;
	auto psign = pivot(cpy);
	auto luMats = luDecomposition(cpy);

	return diagonalMult(luMats[0]) * diagonalMult(luMats[1]) * psign;
}


///\relates Mat
///\brief Brings a given Mat in the row-echolon-form (ref).
///\details The given Mat does not have to be pivotized.
template<std::size_t R, std::size_t C, typename P>
void ref(Mat<R, C, P>& m)
{
	std::size_t c = 0;
    for(std::size_t r = 0; r < R; ++r, ++c)
    {
		for(; c < C; ++c)
		{
			//basically pivotize
			std::size_t maxR = r;
			for(std::size_t r2 = r + 1; r2 < R; ++r2)
			{
				if(std::abs(m[r2][c]) > std::abs(m[maxR][c]))
					maxR = r2;
			}

			if(m[maxR][c] != 0)
			{
				if(r != maxR) m.swapRow(r, maxR);
				break;
			}
			else if(c == C - 1)
			{
				return;
			}
		}

		m[r] /= m[r][c];

		for(std::size_t r2 = r + 1; r2 < R; ++r2)
		{
			if(m[r2][c] != 0)
			{
				m[r2] -= (m[r2][c] / m[r][c]) * m[r];
			}
		}
	}
}

///\relates Mat
///\brief Returns the row-echolon-form (ref) of a given Mat.
template<size_t R, size_t C, typename P>
Mat<R, C, P> refCopy(Mat<R, C, P> m)
{
	ref(m);
	return m;
}

/*
//TODO: Some kind of solution set class for possible Matrix solutions?
///\relates Mat
///Analzyes a Matrix in row echelon form
///\return
///Returns 0 if the corresponding Linear equotation system is not solvable.
///Returns 1 if it is unabiguously solveavle by exactly one solution.
///Returns 2 if it has infinity solutions.
template<size_t R, size_t C, typename P>
unsigned int analyzeRefMat(const Mat<R, C, P>& m)
{
	//TODO
	return 0;
}
*/

///\relates Mat
///\brief Brings a given Matrix in the reduced-row-echolon-form (rref).
///\details The Mat will first be brought into the row-echolon-form, so it does not have
///to fulfill any requirements.
template<size_t R, size_t C, typename P>
void rref(Mat<R, C, P>& m)
{
    ref(m);

    for(int r = R - 1; r >= 0; --r)
    {
		std::size_t c = 0;
        for(; c < C; ++c)
		{
			if(m[r][c] != 0) break;
		}

		if(m[r][c] == 0)
		{
			continue;
		}

        m[r] /= m[r][c];

        for(std::size_t r2 = 0; r2 < std::size_t(r); ++r2)
        {
            m[r2] -= m[r2][c] * m[r];
        }
    }
}

///\relates Mat
///\brief Computes the reduced-row-echolon-form (rref) of a given Mat.
template<size_t R, size_t C, typename P>
Mat<R, C, P> rrefCopy(Mat<R, C, P> m)
{
	rref(m);
	return m;
}


///\relates Mat
///\brief Transforms the given SquareMat object to the identity matrix (1).
template<std::size_t D, typename P>
void identity(SquareMat<D, P>& mat)
{
	mat = identityMat<D, P>();

	/*
	for(std::size_t i(0); i < D; ++i)
		mat[i][i] = P(1);
	*/
}

//operators
namespace detail
{

constexpr const unsigned int cDWidth = 6;
inline unsigned int getNumberOfDigits(double i)
{
    return ((i < 10 && i > 0) || i == 0) ? 1 :
    (i > -10 && i < 0) ? 2 :
    (i > 0) ? (unsigned int) std::log10((double) i) + 1 : (unsigned int) std::log((double) -i) + 2;
}

}

///\relates Mat
template<size_t R, size_t C, class P>
std::ostream& operator<<(std::ostream& os, const Mat<R, C, P>& obj)
{
    auto org = os.precision();
    os << "{" << "\n";

    for(unsigned int i(0); i < R; i++)
    {
        os << "  " << "(";
        for(unsigned int o(0); o < C; o++)
        {
			using namespace detail;
            os	<< std::setw(cDWidth)
				<< std::setprecision(cDWidth - getNumberOfDigits(obj[i][o]) + 4)
				<< obj[i][o];

            if(o != C - 1)
                os << ", ";
        }

        os << ")" << "\n";
    }

    os << "}";
    os.precision(org);

    return os;
}

//todo: more efficiency with wrapper classes for operations
//+
///\relates Mat
template<size_t R, size_t C, typename P>
Mat<R, C, P> operator+(Mat<R, C, P> ma, const Mat<R, C, P>& mb)
{
    return std::move(ma += mb);
}


//-
///\relates Mat
template<size_t R, size_t C, typename P> Mat<R, C, P>
operator-(Mat<R, C, P> ma, const Mat<R, C, P>& mb)
{
    return std::move(ma -= mb);
}


//*
//Mat and value
///\relates Mat
template<size_t R, size_t C, typename P>
Mat<R, C, P> operator*(Mat<R, C, P> ma, const P& other)
{
    return std::move(ma *= other);
}

///\relates Mat
template<size_t R, size_t C, typename P>
Mat<R, C, P> operator*(const P& other, Mat<R, C, P> ma)
{
    return std::move(ma *= other);
}

//Mat and Mat
///\relates Mat
template <size_t RA, size_t CA, size_t CB, typename P> Mat<RA, CB, P>
operator*(const Mat<RA, CA, P>& ma, const Mat<CA, CB, P>& mb)
{
    Mat<RA, CB, P> ret {};

    for(size_t r(0); r < RA; ++r)
        for(size_t c(0); c < CB; ++c)
            ret[r][c] = sum(ma.row(r) * mb.col(c));

    return ret;
}

//Mat and Vector
///\relates Mat
template<size_t R, size_t C, typename P>
Vec<R, P> operator*(const Mat<R, C, P>& ma, const Vec<C, P>& v)
{
    Vec<R, P> ret {};
    ret.fill(P());

    for(size_t i(0); i < R; i++)
        ret[i] = sum(ma.row(i) * v);

    return ret;
}

///\relates Mat
template<size_t R, size_t C, typename P>
Vec<R, P> operator*(const Vec<C, P>& v, const Mat<R, C, P>& ma)
{
    return (ma * v);
}

#ifdef DOXYGEN
}
#endif
