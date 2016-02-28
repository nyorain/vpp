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
///Utility templates to name classes/types and serialize them.

#pragma once

#include <nytl/named.hpp>
#include <nytl/typemap.hpp>
#include <nytl/cloneable.hpp>

#include <string>
#include <memory>
#include <ostream>
#include <istream>

namespace nytl
{

//load
//Serialize Templates
///\ingroup utility
class SerializedBase : public AbstractCloneable<SerializedBase>
{
public:
	virtual std::string objectTypeName() const = 0;
	virtual const std::type_info& objectTypeInfo() const = 0;

	virtual bool load(std::istream&) { return 1; };
	virtual bool save(std::ostream& out) const
	{	
		out << objectTypeName() << '\n';
		return 1;
	};
};

///\ingroup utility
template<typename T>
class Serialized : public DeriveCloneable<SerializedBase, T>
{
public:
	virtual std::string objectTypeName() const override { return typeName<T>(); }
	virtual const std::type_info& objectTypeInfo() const override { return typeid(T); }
};

///\ingroup utility
template<typename Base, typename Derived>
class DeriveSerialized : public DeriveCloneable<Base, Derived>
{
public:
	virtual std::string objectTypeName() const override { return typeName<Derived>(); }
	virtual const std::type_info& objectTypeInfo() const override { return typeid(Derived); }
};

///\ingroup utility
template<typename Base = Any, typename... CArgs>
class Serializer : public Typemap<std::string, Base, CArgs...>
{
public:
	using TypemapBase = Typemap<std::string, Base, CArgs...>;
	using typename TypemapBase::Pointer;

public:
	Pointer createLoad(std::istream& in, CArgs... args) const 
	{
		std::string name;
		in >> name;
		auto it = TypemapBase::typeIterator(name); 
		if(TypemapBase::valid(it))
			return it->second->createLoad(in);

		return TypemapBase::EmptyFactory::call();
	}

	using TypemapBase::add;

	template<typename T> std::size_t add(const T&) //easier to call
	{
		return add<T>(typeName<T>());
	}
	template<typename T> std::size_t add()
	{
		return add<T>(typeName<T>());
	}
};

using DefaultSerializer = Serializer<>;

//registerFunc
///\ingroup utility
template<typename T, typename Base, typename... CArgs>
unsigned int addType(Serializer<Base, CArgs...>& m)
{
    return m.template add<T>();
}

}
