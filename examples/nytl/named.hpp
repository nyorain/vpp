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
///Utility templates for associating a type with a name

#pragma once

#include <nytl/bits/templateString.inl>
#include <nytl/bits/tmpUtil.inl>
#include <nytl/bits/typeNameFunc.inl>
#include <string>

namespace nytl
{

///\ingroup utility
template<typename Name>
class Named;

template<char... Name>
class Named<TemplateString<Name...>>
{
public:
	using NameString = TemplateString<Name...>;
	using NamedType = Named;

	static constexpr ConstString typeName() { return NameString::constString(); }
};

///\ingroup utility
template<typename Base, typename Name>
class DeriveNamed;

template<typename Base, char... Name>
class DeriveNamed<Base, TemplateString<Name...>> : public Base
{
public:
	using NameString = TemplateString<Name...>;
	using NamedType = DeriveNamed;

	static constexpr ConstString typeName() { return NameString::constString(); }
};


//prototypes
///\ingroup utility
template<typename T, typename = void> struct TypeName;

///\ingroup utility
template<typename... T> struct TypeNames;

//convinience functions
///\ingroup utility
template<typename T>
std::string typeName(bool space = 0)
	{ return TypeName<T>::name(space); }

///\ingroup utility
template<typename... T>
std::string typeNames(bool space = 0)
	{ return TypeNames<T...>::names(space); }

//specialized structs
template<typename T, typename>
struct TypeName
	{ static std::string name(bool){ return detail::typeNameFunc<T>(); } };

template<typename T> 
struct TypeName<T, void_t<decltype(T::typeName())>>
	{ static std::string name(bool space){ return T::typeName(space); } };

template<typename T> 
struct TypeName<const T>
	{ static std::string name(bool space){ return "const " + typeName<T>(space); } };

template<typename T> 
struct TypeName<T*>
	{ static std::string name(bool){ return typeName<T>() + "*"; } };

template<typename T> 
struct TypeName<T&>
	{ static std::string name(bool){ return typeName<T>() + "&"; } };

template<typename T> 
struct TypeName<T* const>
	{ static std::string name(bool){ return typeName<T>() + "* const"; } };

template<typename Ret, typename... Args> 
struct TypeName<Ret(Args...)>
	{ static std::string name(bool)
		{ return typeName<Ret>() + "("  + typeNames<Args...>() + ")"; } 
	};

//multiple
template<typename... T>
struct TypeNames
{ 
	static std::string names(bool space)
	{ 
		std::string ret {};
        bool first = 1;
        auto fnc = [&](const std::string& s)
            {
                if(!first) ret.append(", ");
                else first = 0;
                ret.append(s);
            };

        using expander = int[];
		int counter = 0;
        expander{((void) fnc(typeName<T>((++counter == sizeof...(T) && space) ? 1 : 0)) , 0)...};
        return ret;
	}
};


}

#define NYTL_GEN_TYPENAME(Type) \
	namespace nytl { \
	template<> struct TypeName<Type> \
		{ static std::string name(bool){ return #Type; }; }; \
	}

#define NYTL_GEN_TEMPLATE_TYPENAME(Type) \
	namespace nytl { \
	template<typename... P> \
	struct TypeName<Type<P...>> \
	{ \
		static std::string name(bool space) \
			{ return std::string(#Type) + "<" + typeNames<P...>(1) + \
				(space ? "> " : ">"); }; \
	}; \
	}

