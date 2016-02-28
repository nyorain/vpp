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

#pragma once

#ifdef DOXYGEN
namespace nytl {
#endif

//utility
///\relates Mat SolutionSet
///Interprets the given Matrix as a LinearEquotationSystem and solves it.
template<std::size_t R, std::size_t C, typename P>
SolutionSet<C - 1> solve(const Mat<R, C, P>& m)
{
	constexpr static auto V = C - 1; //number Variables
	constexpr static auto E = R; //number Equotations

	auto res = rrefCopy(m);
	Vec<V, typename SolutionSet<V>::expression> solution = {};

	Vec<V, int> varNumbers; //Variable ID of the column
	varNumbers.fill(-1);

	std::size_t varCount = 0;
	int currVar = 0;

	for(std::size_t r(0); r < E; ++r)
	{
		currVar = -1;
		for(std::size_t c(0); c < V; ++c)
		{
			if(res[r][c] != 0)
			{
				if(currVar == -1)
				{
					currVar = c;
				}
				else
				{
					if(varNumbers[c] == -1) //not registered yet
					{
						varNumbers[c] = varCount++;
						solution[c].variablePart[varNumbers[c]] = 1;
					}

					solution[currVar].variablePart[varNumbers[c]] = -res[r][c];
				}			
			}
		}

		if(currVar == -1) //all vars == 0
		{
			//result != 0 -> not solvable
			if(res[r].back() != 0)  return SolutionSet<V>();
			else continue; //just an empty row
		}

		solution[currVar].constPart = res[r].back();
	}

	return SolutionSet<V>(varCount, solution);
}

///\relates DomainedSolutionSet
template<std::size_t D>
std::vector<Vec<D, double>> outLinePoints(const DomainedSolutionSet<D>& solution)
{
	//generator
	struct uniqueGenerator 
	{ 
		unsigned int current = 0; 
		unsigned int operator()(){ return current++; }
	};

	//all possible permutations of dimensions (variable order)
	std::vector<unsigned int> varSeq(solution.numberVariables());
	std::generate(varSeq.begin(), varSeq.end(), uniqueGenerator{});

	std::vector<DynVecui> varSequences(fac(solution.numberVariables()), 
			DynVecui(solution.numberVariables()));

	for(std::size_t i(0); i < fac(solution.numberVariables()); ++i)
	{
		varSequences[i].data_ = varSeq;
		if(!std::next_permutation(varSeq.begin(), varSeq.end()))
			break; //needed?
	}	
	
	//all possible min/max (binary - 1=max, 0=min)
	std::vector<DynVecb> minmaxSequences(std::pow(2, solution.numberVariables()), 
			DynVecb(solution.numberVariables()));

	for(std::size_t i(0); i < (1 << solution.numberVariables()); ++i)
	{
		for(std::size_t o(0); o < solution.numberVariables(); ++o)
		{
			minmaxSequences[i][o] = (i & (1 << o));
		}
	}

	//assure baked - try - catch?
	//solution.bake();

	//compute points
	std::vector<Vec<D, double>> ret; //ret.reserve?
	for(auto& varSeqi : varSequences)
	{
		for(auto& minmaxSeqi : minmaxSequences)
		{
			DynVecd sol;
			try
			{
				sol = solution.solution(varSeqi, minmaxSeqi, 0);
			}
			catch(std::invalid_argument& err)
			{
				continue;
			}

			ret.push_back(sol);
		}
	}

	//erase doubled points (no valid method to sort -> std::set, std::unique not useable)
	for(std::size_t i(0); i < ret.size(); ++i)
	{
		for(auto it2 = ret.cbegin(); it2 != ret.cend();)
		{
			if(&(*it2) != &ret[i] && all(ret[i] == *it2)) it2 = ret.erase(it2);
			else ++it2;
		}
	}

	return ret;
}

//SolutionSet
template<std::size_t N>
template<std::size_t D, typename P>
Vec<N, double> SolutionSet<N>::solution(const Vec<D, P>& vars) const
{
	Vec<N, double> ret;
	if(!solvable())
	{
		ret.fill(std::numeric_limits<double>::quiet_NaN());
	}
	else if(unambigouoslySolvable())
	{
		for(std::size_t i(0); i < N; ++i)
			ret[i] = solution_[i].constPart;		
	}
	else
	{
		for(std::size_t i(0); i < N; ++i)
			ret[i] = solution_[i].constPart + sum(solution_[i].variablePart * vars);		
	}

	return ret;
}

template<std::size_t N>
Vec<N, double> SolutionSet<N>::solution() const
{
	Vec<N, double> ret;
	if(!solvable())
	{
		ret.fill(std::numeric_limits<double>::quiet_NaN());
	}
	else if(unambigouoslySolvable())
	{
		for(std::size_t i(0); i < N; ++i)
			ret[i] = solution_[i].constPart;		
	}
	else
	{
		//will convert to Vec of needed size, full of 0's
		return solution(Vec2i(0, 0));
	}
	return ret;
}

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, const SolutionSet<N>& sol)
{
	os << "{(";
	const char* sep = "";
	for(auto& exp : sol.solution_)
	{
	   	os << sep;
		sep = " | ";

		bool first = 1;

		if(exp.constPart != 0 || all(exp.variablePart == 0))
		{
			first = 0;
			os << exp.constPart;
		}
		for(std::size_t i(0); i < exp.variablePart.size(); ++i)
		{
			if(exp.variablePart[i] == 0) continue;

			char name = 97 + i;

			if(first)
			{
				if(exp.variablePart[i] == 1) os << name;
				else if(exp.variablePart[i] == -1)os << "-" << name;
				else os << exp.variablePart[i] << name;

				first = 0;
			}
			else
			{
				using std::abs;

				char sign = '+';
				if(exp.variablePart[i] < 0) sign = '-';

				if(abs(exp.variablePart[i]) == 1) os << " " << sign << " " << name;
				else os << " " << sign << " " << abs(exp.variablePart[i]) << name;
			}
		}

	}
	os << ")}";
	return os;
}

//DomainedSolutionSet
template<std::size_t N>
DomainedSolutionSet<N>::DomainedSolutionSet(const SolutionSet<N>& sset, 
		const Vec<N, LinearDomain>& Domains) : SolutionSet_(sset), Domains_(Domains)
{
	bake();
	return;

	try
	{
		bake();
	}
	catch(const std::exception& err)
	{
		//TODO: warning and/or rethrow here
	}
}

template<std::size_t N>
DomainedSolutionSet<N>::DomainedSolutionSet(const SolutionSet<N>& sset, 
		const LinearDomain& Domains) : SolutionSet_(sset), Domains_()
{
	Domains_.fill(Domains);
	bake();
	return;

	try
	{
		bake();
	}
	catch(const std::exception& err)
	{
		//TODO: warning and/or rethrow here
	}
}

template<std::size_t N>
void DomainedSolutionSet<N>::bake() const
{
	dependentDomains_.clear();
	dependentDomains_.resize(SolutionSet_.numberVariables());

	for(std::size_t i(0); i < N; ++i)
	{
		auto& expr = SolutionSet_.solution_[i];
		if(all(expr.variablePart == 0)) //const
		{
			if(Domains_[i].minimum > expr.constPart || Domains_[i].maximum < expr.constPart)
			{
				//does not Match Domain (no variable part -> NEVER Matches Domain), throw
				throw std::invalid_argument(
						std::string("DomainedSolutionSet::bake: invalid SolutionSet"));
			}

			continue;
		}

		//variablePart
		for(std::size_t v(0); v < SolutionSet_.numberVariables(); ++v)
		{
			if(expr.variablePart[v] == 0) continue;

			bool gZ = (expr.variablePart[v] > 0); //greaterZero
			auto& dom = dependentDomains_[v];

			//check if expression or const
			auto cpy = expr.variablePart;
			cpy[v] = 0;

			if(any(cpy != 0)) //get expression
			{
				//expressions
				auto exp = expressionType{};

				exp.constPart = (Domains_[i].minimum - expr.constPart) / expr.variablePart[v];
				exp.variablePart = -cpy / expr.variablePart[v];

				if(gZ) dom.min.push_back(exp);
				else dom.max.push_back(exp);

				exp.constPart = (Domains_[i].maximum - expr.constPart) / expr.variablePart[v];
				exp.variablePart = -cpy / expr.variablePart[v];

				if(gZ) dom.max.push_back(exp);
				else dom.min.push_back(exp);
			}
			else //get const
			{
				//const
				if(gZ)
				{
					dom.constMin = max(dom.constMin, 
							(Domains_[i].minimum - expr.constPart) / expr.variablePart[v]);
					dom.constMax = min(dom.constMax, 
							Domains_[i].maximum - expr.constPart / expr.variablePart[v]);
				}
				else
				{
					dom.constMin = max(dom.constMin, 
							(Domains_[i].maximum - expr.constPart) / expr.variablePart[v]);
					dom.constMax = min(dom.constMax, 
							(Domains_[i].minimum - expr.constPart) / expr.variablePart[v]);
				}
			}
		}
	}

	//constPart
	for(std::size_t i(0); i < N; ++i)
	{
		auto& expr = SolutionSet_.solution_[i];
		if(all(expr.variablePart == 0)) continue;

		for(std::size_t v(0); v < SolutionSet_.numberVariables(); ++v)
		{
			if(expr.variablePart[v] == 0) continue;

			bool gZ = (expr.variablePart[v] > 0); //greaterZero
			auto& dom = dependentDomains_[v];

			auto lhs = Domains_[i].minimum - expr.constPart; //left
			auto rhs = Domains_[i].maximum - expr.constPart; //right

			for(std::size_t c(0); c < SolutionSet_.numberVariables(); ++c)
			{
				if(c == v || expr.variablePart[c] == 0) continue;

				if(gZ)
				{
					lhs -= dependentDomains_[c].constMax * expr.variablePart[c];
					rhs -= dependentDomains_[c].constMin * expr.variablePart[c];
				}
				else
				{
					lhs -= dependentDomains_[c].constMin * expr.variablePart[c];
					rhs -= dependentDomains_[c].constMax * expr.variablePart[c];
				}
			}

			std::cout << expr.variablePart[v] << ";;\n";

			lhs = lhs / expr.variablePart[v];
			rhs = rhs / expr.variablePart[v];

			auto emptyVec = Vec<N, double>{};
			emptyVec.fill(0);

			if(gZ)
			{
				dom.min.push_back({lhs, emptyVec});
				dom.max.push_back({rhs, emptyVec});
			}
			else
			{
				dom.min.push_back({rhs, emptyVec});
				dom.max.push_back({lhs, emptyVec});
			}
		}
	}
}

template<std::size_t N> DynVecd 
DomainedSolutionSet<N>::solution(const DynVecui& seq, const DynVecb& minmax, bool pbake) const
{
	if(pbake) bake();

	auto vars = DynVecd(SolutionSet_.numberVariables());
	for(std::size_t i(0); i < SolutionSet_.numberVariables(); ++i)
	{
		auto s = seq[i];
		auto m = minmax[s];

		if(m) vars[s] = dependentDomains_[s].constMax;
		else vars[s] = dependentDomains_[s].constMin;

		for(auto& d : (m ? dependentDomains_[s].max : dependentDomains_[s].min))
		{
			//ignore(skip) dependentDomains_ with unresolved variables
			bool skip = 0;
			for(std::size_t vp(i); vp < SolutionSet_.numberVariables(); ++vp)
			{
				if(d.variablePart[seq[vp]] != 0) skip = 1;
			}
			if(skip) continue;

			if(m)
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				vars[s] = std::min(vars[s], n);
			}
			else
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				vars[s] = std::max(vars[s], n);
			}
		}

		//check
		for(auto& d : (m ? dependentDomains_[s].min : dependentDomains_[s].max))
		{
			//ignore(skip) dependentDomains with unresolved variables
			bool skip = 0;
			for(std::size_t vp(i); vp < SolutionSet_.numberVariables(); ++vp)
			{
				if(d.variablePart[seq[vp]] != 0) skip = 1;
			}
			if(skip) continue;

			if(m)
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				if(vars[s] < n)
				{
					throw std::invalid_argument("DomainedSolutionSet::solution: not solveable");
				}
			}
			else
			{
				auto n = d.constPart + sum(d.variablePart * vars);
				if(vars[s] > n)
				{
					throw std::invalid_argument("DomainedSolutionSet::solution: not solveable");
				}
			}
		}
	}

	//return vars;
	std::cout << "vars: " << vars << "\n";
	return SolutionSet_.solution(vars);
}

//LinearEquotationSystem
template<std::size_t E, std::size_t V, typename P>
SolutionSet<V> LinearEquotationSystem<E, V, P>::solve() const
{
	return nytl::solve(asMat());
}

#ifdef DOXYGEN
}
#endif

