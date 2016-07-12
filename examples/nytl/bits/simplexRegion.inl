/* The MIT License (MIT)
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
namespace nytl{
#endif

//utility
///\relates SimplexRegion DomainedSolutionSet
///Creates a convex shape (SimplexRegion) as a geometrical representation of a DomainedSolutionSet.
template<std::size_t D>
SimplexRegion<D, double, D> createConvex(const DomainedSolutionSet<D>& solution)
{
	auto points = outLinePoints(solution);
	return createConvex<D, double>(points);
}

///\relates SimplexRegion
///Creates a convex shape(SimplexRegion) from its outLine points.
template<std::size_t D, typename P, std::size_t A = D>
SimplexRegion<D, P, A> createConvex(const std::vector<Vec<D, P>>& points)
{
	auto lines = std::vector<Line<D, P>> {};
	for(auto it = points.cbegin(); it != points.cend(); ++it)
	{
		auto& p = *it;
		for(auto it2 = it; it2 != points.cend(); ++it2)
		{
			auto& p2 = *it2;

			if(&p == &p2) continue;
			lines.push_back({p, p2});
		}
	}

	auto lines2 = lines;
	for(auto it = lines.cbegin(); it != lines.cend(); ++it)
	{
		auto& l1 = *it;
		for(auto l2 : lines2)
		{
			if(all(l1.a == l2.a) || all(l1.b == l2.b) || all(l1.b == l2.a) || all(l1.a == l2.b)) 
			{
				continue;
			}

			if(intersects(l1, l2))
			{
				it = lines.erase(it);
				--it;
				break;
			}
		}
	}

	return createConvex(lines);
}

///\relates SimplexRegion
///Creates a convex shape(SimplexRegion) from its outlining Lines.
///If the lines given do not form a convex shape (i.e. there are angles > 90 degrees) the
///result in undefined.
template<std::size_t D, typename P, std::size_t A = D>
SimplexRegion<D, P, A> createConvex(std::vector<Line<D, P>> lines)
{
	auto ret = SimplexRegion<D, P, A> {};

	while(lines.size() > A + 1)
	{
		//newly created Lines, inserted AFTER Simplex is completly found
		auto newLines = std::vector<Line<D, P>> {};
		auto line = lines.front();
		lines.erase(lines.cbegin());

		auto simp = Simplex<D, P, A> {};
		simp.points()[0] = line.a;
		simp.points()[1] = line.b;
		
		std::size_t idx = 1;

		//Lines at Line.a (center point)
		for(std::size_t i2(0); i2 < lines.size(); ++i2)
		{
			auto& line2 = lines[i2];

			if(all(line2.a == line.a))
			{
				simp.points()[++idx] = line2.b;
				lines.erase(lines.cbegin() + i2);

				for(std::size_t i3(1); i3 < idx; ++i3)
					newLines.push_back(simp.points()[i3], simp.points()[idx]);
			}
			else if(all(line2.b == line.a))
			{
				simp.points()[++idx] = line2.a;
				lines.erase(lines.cbegin() + i2);

				for(std::size_t i3(1); i3 < idx; ++i3)
					newLines.push_back(simp.points()[i3], simp.points()[idx]);
			}
			else
			{
				++i2; //only do it when no line is erased
			}
		
			if(idx == A) break; //all found
		}

		assert(idx == A); //error here! could not construct Simplex

		//remove already captures lines from newLines
		for(std::size_t i2(0); i2 < newLines.size();)
		{
			for(std::size_t i3(0); i3 < lines.size(); ++i3)
			{
				if(newLines[i2] == lines[i3])
				{
					newLines.erase(newLines.cbegin() + i2);
					break;
				}

				if(i3 == lines.size() - 1) ++i2; //was not erased
			}
		}


		lines.insert(lines.cend(), newLines.cbegin(), newLines.cend());
		ret.addNoCheck(simp);
	}

	//code dupilcation here -- argh -> search for better loop above
	/*
	if(lines.size() == A + 1) //should be always false, needed?
	{
		Simplex<D, P, A> simp;
		std::size_t idx = 0;

		for(std::size_t i(0); i < lines.size(); ++i)
		{
			auto& line2 = lines[i];
			bool foundA = 0, foundB = 0;

			for(std::size_t i2(0); i2 < idx; ++i2)
			{
				auto& p = simp.points()[i2];
				if(all(p == line2.a)) foundA = 1;
				if(all(p == line2.b)) foundB = 1;

				if(foundA && foundB) break;
			}

			if(!foundA)
			{
				simp.points()[idx++] = line2.a;
			}
			if(!foundB)
			{
				simp.points()[idx++] = line2.b;
			}
		}
	
		ret.addNoCheck(simp);
	}
	*/

	return ret;
}

//member
template<std::size_t D, typename P, std::size_t A> void
	SimplexRegion<D, P, A>::add(const SimplexType& simplex)
{
	auto toadd = SimplexRegion<D, P, A> {};
	for(auto& simp : simplices())
		toadd.addNoCheck(subtract(simplex, simp));

	addNoCheck(toadd);

}

template<std::size_t D, typename P, std::size_t A> void
	SimplexRegion<D, P, A>::add(const SimplexRegionType& simplexRegion)
{
	for(auto& simp : simplexRegion)
		add(simp);
}

template<std::size_t D, typename P, std::size_t A> void
	SimplexRegion<D, P, A>::subtract(const SimplexType& simplex)
{
}

template<std::size_t D, typename P, std::size_t A> void
	SimplexRegion<D, P, A>::subtract(const SimplexRegionType& simplexRegion)
{
}

template<std::size_t D, typename P, std::size_t A> 
template<std::size_t OD, typename OP> 
	SimplexRegion<D, P, A>::operator SimplexRegion<OD, OP, A>() const
{
	auto ret = SimplexRegion<OD, OP, A> {};
	ret.simplices().reserve(count());

	for(auto& simp : simplices())
		ret.emplace_back(simp);

	return ret;
}

template<std::size_t D, typename P, std::size_t A> double
	SimplexRegion<D, P, A>::size() const
{
	auto ret = 0.;

	for(auto& simp : simplices())
		ret += simp.size();

	return ret;
}

//tests
///\relates SimplexRegion
///Tests if a SimplexRegion intersects with a Simplex. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const SimplexRegion<D, P, A>& r, const Simplex<D, P, A>& s)
{
	for(auto& rs : r.simplices())
		if(intersects(rs, s)) return 1;

	return 0;
}

///\relates SimplexRegion
///Tests two SimplexRegions for intersection. Symmetrical operator.
template<std::size_t D, typename P, std::size_t A> bool 
	intersects(const SimplexRegion<D, P, A>& r1, const SimplexRegion<D, P, A>& r2)
{
	for(auto& rs : r2.simplices())
		if(intersects(r1, rs)) return 1;

	return 0;
}

///\relates SimplexRegion
///Tests if the given SimplexRegion fully contains the given Simplex. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const SimplexRegion<D, P, A>& r, const Simplex<D, P, A>& s)
{
	for(auto& rs : r.simplices())
		if(!contains(rs, s)) return 0;

	return 1;
}

///\relates SimplexRegion
///Tests if the first SimplexRegion fully contains the second one. Asymmetrical operator.
template<std::size_t D, typename P, std::size_t A>bool 
	contains(const SimplexRegion<D, P, A>& r1, const SimplexRegion<D, P, A>& r2)
{
	for(auto& s : r2.simplices())
		if(!contains(r1, s)) return 0;

	return 1;
}

///\relates SimplexRegion
///Tests if the given SimplexRegion contains the given point.
template<std::size_t D, typename P, std::size_t A> bool 
	contains(const SimplexRegion<D, P, A>& r, const Vec<D, P>& v)
{
	for(auto& s : r.simplices())
		if(contains(s, v)) return 1;

	return 0;
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	intersection(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
{
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	combination(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
{
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	symmetricDifference(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
{
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> SimplexRegion<D, P, A> 
	difference(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
{
}

//operators//////////
///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A1, std::size_t A2> auto
	operator&(const SimplexRegion<D, P, A1>& a, const SimplexRegion<D, P, A2>& b)
	-> decltype(intersection(a, b))
{
	return intersection(a, b);
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> auto
	operator|(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
	-> decltype(combination(a, b))
{
	return combination(a, b);
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> auto
	operator^(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
	-> decltype(symmetricDifference(a, b))
{
	return symmeticDifference(a, b);
}

///\relates SimplexRegion
template<std::size_t D, typename P, std::size_t A> auto
	operator-(const SimplexRegion<D, P, A>& a, const SimplexRegion<D, P, A>& b)
	-> decltype(difference(a, b))
{
	return difference(a, b);
}

#ifdef DOXYGEN
}
#endif
