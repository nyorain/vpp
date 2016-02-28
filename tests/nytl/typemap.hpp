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
///Provides the Typemap template class that allows psuedo-dynamic object creation/storage.

#pragma once

#include <nytl/any.hpp>
#include <nytl/bits/tmpUtil.inl>

#include <unordered_map>
#include <algorithm>
#include <typeinfo>
#include <memory>
#include <type_traits>
#include <stdexcept>

namespace nytl
{

namespace detail
{

template<typename T, typename = void>
struct Loader
{
	static bool call(std::istream& is, T& obj)
	{
		//using dummy = decltype(is >> obj);
		
		is >> obj;
		return 1;
	}
};

template<typename T>
struct Loader<T, void_t<decltype(T{}.load(std::cin))>>
{
	static bool call(std::istream& is, T& obj)
	{
		return obj.load(is);
	}
};

}

///\ingroup utility
///Load an object from an istream. If your type does neither overload the >> operator nor
///have a load member function, specialize this template function with your own type.
template<typename T>
bool load(std::istream& is, T& obj)
{
	return detail::Loader<T>::call(is, obj);
}

namespace detail
{

//TODO: perfect forwarding needed here? Args/CArgs are explicitly specified...
//make_unique_wrapper, returns a unique_ptr for usual types and a void* for void type
template<typename Base, typename T, typename... Args> struct make_unique_wrapper
{
    static std::unique_ptr<Base> call(Args... args) { return std::make_unique<T>(args...); }
};

template<typename T, typename... Args> struct make_unique_wrapper<void, T, Args...>
{
    static void* call(Args... args) { return new T(args...); };
};

template<typename T, typename... Args> struct make_unique_wrapper<Any, T, Args...>
{
    static Any call(Args... args) { return Any(T(args...)); };
};

//createLoad
template<typename Base, typename T, typename... Args> struct CreateLoadWrapper
{
    static std::unique_ptr<Base> call(std::istream& is, Args... args) 
	{ 
		auto ret = std::make_unique<T>(args...); 
		if(!load(is, *ret)) return nullptr;
		return ret;
	}
};

template<typename T, typename... Args> struct CreateLoadWrapper<void, T, Args...>
{
    static void* call(std::istream& is, Args... args) 
	{ 
		auto ret = new T(args...);
		if(!load(is, *ret)) return nullptr;
		return ret;
	};
};

template<typename T, typename... Args> struct CreateLoadWrapper<Any, T, Args...>
{
    static Any call(std::istream& is, Args... args) 
	{ 
		auto ret = T(args...);
		if(!load(is, ret)) return Any();
		return Any(ret);
	};
};

//wrapper for creating empty (invalid) objects of given type (ptr/any)
template<typename Pointer> struct make_empty_wrapper
{
    static Pointer call() { return nullptr; };
};

template<> struct make_empty_wrapper<Any>
{
    static Any call() { return Any(); };
};

//wrapper for checking if an object is valid
template<typename T>
struct CheckValidWrapper
{
	static bool call(const T& obj){ return obj != nullptr; }
};

template<>
struct CheckValidWrapper<Any>
{
	static bool call(const Any& obj){ return !obj.empty(); }
};

}


///\ingroup utility
///Can be thought of as map (like std::map) that holds pair of identifiers and types.
///Internally just uses type erasure to "store" its types.
///Therefore there are just a small set of operations that can then be done with those
///"stored" types, like e.g. creating them from identifier or std::type_info or receive the 
///type_info of the stored type by identifier.
///\tparam I Identifier type for the types (usually std::string)
///\tparam B Base type of all stored types. If there is none, use void (Created objects will then be
///passed as void* objects). Defaulted to void
///\tparam CA Additional construction args an objects needs on creation.
template<typename I, typename B = Any, typename... CArgs>
class Typemap
{
public:
	using Base = B;
	using Identifier = I;
    using Pointer = typename std::conditional<
		std::is_same<Base, void>::value, void*, typename std::conditional<
		std::is_same<Base, Any>::value, Any, std::unique_ptr<Base>>::type>::type;
	using EmptyFactory = detail::make_empty_wrapper<Pointer>;
	template <typename T> using Factory = detail::make_unique_wrapper<B, T>;
	template <typename T> using LoadFactory = detail::CreateLoadWrapper<B, T, CArgs...>;

    //base
    struct TypeBase
    {
    public:
        virtual ~TypeBase() = default;
        virtual const std::type_info& typeInfo() const = 0;
        virtual Pointer create(CArgs&&... args) const = 0;
        virtual Pointer createLoad(std::istream& is, CArgs&&... args) const = 0;
    };

    //impl
    template<typename T>
    struct TypeImpl : TypeBase
    {
    public:
        virtual ~TypeImpl() = default;
        virtual const std::type_info& typeInfo() const override { return typeid(T); };
        virtual Pointer create(CArgs&&... args) const override
            { return Factory<T>::call(std::forward<CArgs>(args)...); }
        virtual Pointer createLoad(std::istream& is, CArgs&&... args) const override
			{ return LoadFactory<T>::call(is, args...); }
    };

    using MapType = std::unordered_map<Identifier, std::unique_ptr<const TypeBase>>;
    using Iterator = typename MapType::iterator;
    using ConstIterator = typename MapType::const_iterator;

	//stl conformance
	using iterator = Iterator;
	using const_iterator = ConstIterator;
	using pointer = Pointer;

protected:
   MapType types_; //types stored

public:
    Iterator typeIterator(const Identifier& id){ return types_.find(id); }
    ConstIterator typeIterator(const Identifier& id) const { return types_.find(id); }

    //typeinfo
    Iterator typeIterator(const std::type_info& id)
    {
        auto it = types_.begin();
        for(; it != types_.end(); ++it)
            if(it->second->getTypeInfo() == id) break;
        return it;
    }
    ConstIterator typeIterator(const std::type_info& id) const
    {
        auto it = types_.cbegin();
        for(; it != types_.cend(); ++it)
            if(it->second->getTypeInfo() == id) break;
        return it;
    }

    //entry
    Iterator typeIterator(const Identifier& id, const std::type_info& typeID)
    {
		auto it = typeIterator(typeID); 
		if(it != types_.end() && it->first == id) 
			return it; 
		return types_.end(); 
	}
    ConstIterator typeIterator(const Identifier& id, const std::type_info& typeID) const
    {
	   	auto it = typeIterator(typeID); 
		if(it != types_.cend() && it->first == id) 
			return it; 
		return types_.cend(); 
	}

    bool valid(const ConstIterator& it) const { return it != types_.end(); }

public:
    template<typename T> std::size_t add(const Identifier& id)
    {
        types_[id].reset(new TypeImpl<T>());
        return types_.size();
    }
    template<typename T> std::size_t add(const Identifier& id, const T&)
    {
        types_[id].reset(new TypeImpl<T>());
        return types_.size();
    }

    bool remove(const Identifier& id)
    {
        auto it = typeIterator(id);
        if(valid(types_.cend()))
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }
    template<typename T> bool remove(const T& obj = {})
    {
        auto it = typeIterator(typeid(obj));
        if(valid(types_.cend()))
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }
    template<typename T> bool remove(const Identifier& id, const std::type_info& info)
    {
        auto it = typeIterator(id, info);
        if(valid(types_.cend()))
        {
            types_.erase(it);
            return 1;
        }
        return 0;
    }
    template<typename T> bool remove(const Identifier& id, const T& obj = T{})
    {
        return remove(id, typeid(obj));
    }

    bool exists(const Identifier& id) const 
		{ return typeIterator(id) != types_.cend(); }
    bool exists( const std::type_info& typeID) const 
		{ return typeIterator(typeID) != types_.cend(); }
    template<typename T> bool exists(const T& obj = T{}) const 
		{ return typeIterator(typeid(obj)) != types_.cend(); }
    template<typename T> bool exists(const Identifier& id, const T& obj = T{}) const 
		{ return typeIterator(id, typeid(obj)) != types_.cend(); }
    bool eists(const Identifier& id, const std::type_info& typeID) const 
		{ return typeIterator(id, typeID) != types_.cend(); }

	///{
	///Creates a object for the given type identifier or info with the given construction arguments.
	///\return A unique_ptr of Base type (or void* if Base is void) or nullptr if the object could 
	///not be constructed.
    Pointer create(const Identifier& id, CArgs&&... args) const
    { 
		auto it = typeIterator(id); 
		if(it != types_.cend()) 
			return it->second->create(std::forward<CArgs>(args)...); 
		return EmptyFactory::call();
	}
    Pointer create(const std::type_info& id, CArgs&&... args) const
    { 
		auto it = typeIterator(id); 
		if(it != types_.cend()) 
			return it->second->create(std::forward<CArgs>(args)...); 
		return EmptyFactory::call(); 
	}
	///}
	
	///{
	///Create an object with the given type and loads its by an istream.
	///If creating the object or loading it from the stream fails, returns a nullptr.
	///To load the object nytl::load(istream, object) is called, which may be overloaded for
	///custom types. 
    Pointer createLoad(const Identifier& id, std::istream& is, CArgs&&... args) const
    { 
		auto it = typeIterator(id); 
		if(it != types_.cend()) 
			return it->second->create(is, std::forward<CArgs>(args)...); 
		return EmptyFactory::call();
	}
    Pointer createLoad(const std::type_info& id, std::istream& is, CArgs&&... args) const
    { 
		auto it = typeIterator(id); 
		if(it != types_.cend()) 
			return it->second->createLoad(is, std::forward<CArgs>(args)...); 
		return EmptyFactory::call(); 
	}
	///}

	///\exception std::invalid_argument if there is no entry with the given type_info 
    const Identifier& id(const std::type_info& typeID) const 
	{ 
		auto it = typeIterator(typeID); 
		if(it != types_.cend()) 
			return it->first; 

		throw std::invalid_argument("nytl::Typemap::id: no entry with the given type_info");
	}

	///\exception std::invalid_argument if there is no entry for the given type
    template<typename T> const Identifier& id(const T& obj = T{}) const 
		{ return id(typeid(obj)); }

	///\exception std::invalid_argument if there is no entry for the given identifier
    const std::type_info& typeInfo(const Identifier& id) const 
	{ 
		if(typeExists(id)) 
			return types_[id].typeInfo(); 
		
		throw std::invalid_argument("nytl::Typemap::typeInfo: no entry with the given id");
	}
    const std::unordered_map<Identifier, std::unique_ptr<const TypeBase>>& types() const 
		{ return types_; }
};

//registerFunc
///\ingroup utility
template<typename T, typename Identifier, typename Base, typename... CArgs>
unsigned int addType(Typemap<Identifier, Base, CArgs...>& m, const Identifier& id)
{
    return m.template add<T>(id);
}

}
