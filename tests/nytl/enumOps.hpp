#pragma once

#include <type_traits>

///\file
///\brief Defines binary operators for enums.
///\details Inlcuding this file enables all binary operations for strongly-typed c++11 enum classes.
///If you want to use them using the real c++ operators (not explicitly calling the namespaced
///functions) you have to use the nytl::enumOps namespace.

namespace nytl
{

namespace dt
{
	template<typename E> using EnumType = typename std::underlying_type<E>::type;
	template<typename E> using EnableEnum = typename std::enable_if<std::is_enum<E>::value>::type;
}

namespace enumOps
{

template<typename E, typename = dt::EnableEnum<E>> E operator~(E a) 
	{ return static_cast<E>(~static_cast<dt::EnumType<E>>(a)); }

template<typename E, typename = dt::EnableEnum<E>> E operator|(E a, E b) 
	{ return static_cast<E>(static_cast<dt::EnumType<E>>(a) | static_cast<dt::EnumType<E>>(b)); }

template<typename E, typename = dt::EnableEnum<E>> E operator&(E a, E b) 
	{ return static_cast<E>(static_cast<dt::EnumType<E>>(a) & static_cast<dt::EnumType<E>>(b)); }

template<typename E, typename = dt::EnableEnum<E>> E operator^(E a, E b) 
	{ return static_cast<E>(static_cast<dt::EnumType<E>>(a) ^ static_cast<dt::EnumType<E>>(b)); }

template<typename E, typename = dt::EnableEnum<E>> E& operator|=(E& a, E b) 
	{ a = a | b; return a; } 

template<typename E, typename = dt::EnableEnum<E>> E& operator&=(E& a, E b) 
	{ a = a & b; return a; } 

template<typename E, typename = dt::EnableEnum<E>> E& operator^=(E& a, E b) 
	{ a = a ^ b; return a; } 

}

}
