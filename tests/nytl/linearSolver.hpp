/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Jan Kelling
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
///\brief Template classes and functions for solving Linear equotation system.

#pragma once

#include <nytl/mat.hpp>
#include <nytl/vec.hpp>
#include <nytl/dynVec.hpp>

#include <climits>
#include <algorithm>

namespace nytl
{

///Represents the solutions a Linear equotation system can have.
///\ingroup math
template<std::size_t N>
class SolutionSet
{
public:
	struct expression
	{
		double constPart;
		Vec<N, double> variablePart;
	};

public:
	int solvable_ = -1;
	Vec<N, expression> solution_;

public:
	SolutionSet() = default;
	SolutionSet(std::size_t numVars, const Vec<N, expression>& sol) 
		: solvable_(numVars), solution_(sol) {} 

	expression& operator[](std::size_t i){ return solution_[i]; }
	const expression& operator[](std::size_t i) const { return solution_[i]; }

	bool solvable() const { return (solvable_ != -1); }
	bool unambigouoslySolvable() const { return (solvable_ == 0); }
	std::size_t numberVariables() const { return (solvable()) ? solvable_ : 0; }

	template<std::size_t D, typename P>
	Vec<N, double> solution(const Vec<D, P>& variables) const;
	Vec<N, double> solution() const;
};

///Represents a Linear Domain that can is used for DomainedSolutionSet components.
///\ingroup math
class LinearDomain
{
public:
	double minimum;
	double maximum;
};

///Represents a Linear solution set with specified Domains for each set componnent.
///Useful for computing the range the variables can take for a given Domain.
///\ingroup math
template<std::size_t N>
class DomainedSolutionSet
{
public:
	using expressionType = typename SolutionSet<N>::expression;

	struct dependentDomain
	{
		std::vector<expressionType> min {};
		std::vector<expressionType> max {};

		double constMin = 0;
		double constMax = 1;
	};

public:
	SolutionSet<N> SolutionSet_;
	Vec<N, LinearDomain> Domains_;
	mutable std::vector<dependentDomain> dependentDomains_; //Cache

public:
	DomainedSolutionSet() = default;

	///Constructs the object and bakes its internal data.
	DomainedSolutionSet(const SolutionSet<N>& sset, const Vec<N, LinearDomain>& Domains);

	///Contructs all variables with the same Domain and bakes its internal data.
	DomainedSolutionSet(const SolutionSet<N>& sset, const LinearDomain& Domains);

	///Computes its internal representation of the SolutionSets components.
	///\exception nytl::invalid_solution_set if the SolutionSet has no solution at all.
	///\exception nytl::invalid_Domained_solution_set if there is no solutions for the given
	///combination of SolutionSet and Domains.
	void bake() const;

	///Returns a solution for the given parameters.
	///\paran seq Specifies the order in which the variables should be chosen. Must have
	///as many components as the SolutionSet has variables.
	///\param minmax Specifies for each SolutionSet variable whether the minimum or the maximum 
	///of the variables range should be chosen. Must have as many components as the SolutionSet
	///has variables.
	///\param bake Specifies whether the internal representation should be baked before computing
	///the solution. If multiple solutions are computing wihthout changing the SolutionSet in
	///the Domains in the meantime this parameter can be 0 (for optimization).
	///\return A solution whose components are in their given Domains while still 
	///Matching the SolutionSet.
	///\exception nytl::invalid_Vector_size if seq or minmax have less components than
	///the objects SolutionSet has variables.
	///\exception nytl::no_baked_data if there is no/invalid internal data representation to use.
	///Can occur if bake == 0 and the data has never been succesfully baked or changed since 
	///the last bake.
	///\warning If bake == 1 the bake() member function is called so all all exceptions from
	///this function might be thrown then.
	DynVecd solution(const DynVecui& seq, const DynVecb& minmax, bool bake = 1) const;

	unsigned int numberVariables() const { return SolutionSet_.numberVariables(); }
};


///A Linear equtations with variable coefficients and a result.
///Represents a Linear equotation with V variables and a result that have a precision of P.
///\ingroup math
template<std::size_t V, typename P>
class LinearEquotation
{
public:
	Vec<V, P> vars;
	P result;

	LinearEquotation& operator=(const Vec<V + 1, P>& values)
		{ vars = values; result = values.back(); return *this; }
};

///\brief A set on Linear equotations that may be solved.
///\ingroup math
///\details Represents a Linear equotation system with E equtations and V variables that have P 
///precision.
template<std::size_t E, std::size_t V, typename P>
class LinearEquotationSystem
{
public:
	using equotation_type = LinearEquotation<V, P>;
	using solution_type = SolutionSet<V>;
	using MatType = Mat<E, V + 1, P>;

public:
	Vec<E, equotation_type> equotations_;

public:
	LinearEquotationSystem() = default;
	LinearEquotationSystem(const Mat<E, V +1, P>& m) 
		: equotations_(*reinterpret_cast<const Vec<E, equotation_type>*>(&m)) {}

	equotation_type& operator[](std::size_t i){ return equotations_[i]; }
	const equotation_type& operator[](std::size_t i) const { return equotations_[i]; }

	MatType& asMat() { return *reinterpret_cast<MatType*>(&equotations_); }
	const MatType& asMat() const { return *reinterpret_cast<const MatType*>(&equotations_); }

	solution_type solve() const;
};

#include <nytl/bits/linearSolver.inl>

}
