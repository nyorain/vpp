// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

/// \file Various useful template helpers without dependencies.

#pragma once

#ifndef NYTL_INCLUDE_TMP_UTIL
#define NYTL_INCLUDE_TMP_UTIL

#include <utility> // std::forward

namespace nytl {

/// \brief Useful typedef for expanding the use of variadic template arguments.
/// In C++17, most of the Expand shortcuts can be done with fold expressions.
/// ```cpp
/// // Prints the variadic template pack args to std::cout.
/// (void) nytl::Expand {((void) std::cout << args , 0), ...};
/// ```
/// \module utility
using Expand = int[];

/// \brief Class that can be derived from to check if given template parameters are valid.
/// \details Really useful for template classes that use SFINAE.
/// ```cpp
/// class D : public nytl::DeriveDummy<std::enable_if_t<Condition>> {};
/// ```
/// \module utility
template<typename...> struct DeriveDummy {};

// TODO C++17: remove this (std::void_t)
/// C++17 alias template for void, used to detect ill-formed types in a SFINAE-context.
/// ```
/// // Function foo must be callable with a value of type A.
/// template<typename A, typename = void_t<decltype(foo(std::declval<A>()))>>
/// void bar();
/// ```
/// \module utility
template<typename...> using void_t = void;

/// Utility template function that can be used to hide unused compiler warnings.
/// Has usually no additional cost. Is meant as placeholder for future code.
/// ```cpp
/// void foo(int a, int b, int c) { nytl::unused(a, b, c); } // to be implemented later on
/// ```
/// \module utility
template<typename... T> void unused(T&&...) {}

/// \brief Can be used to ensure that variadic arguments have the same type.
/// ```cpp
/// // implementation function that takes a variable number of integers
/// template<typename... Args>
/// void funcImpl(nytl::VariadicDummy<int, Args>... ints);
///
/// // public interface function that is called
/// // results in compile time error if not all arguments are convertible to integers
/// template<typename... Args>
/// void func(Args... args) { funcImpl<Args...>(args...); }
/// ```
/// \module utility
template<typename A, typename> using Variadic = A;

/// \brief Assures that the returned value can be used as template-dependent expressions.
/// This allows e.g. to defer operator of function lookup so that specific headers don't
/// have to be included by a template functions if a function is not used.
/// \module utility
template<typename V, typename T>
decltype(auto) templatize(T&& value) { return std::forward<T>(value); }


namespace detail {
	template<template<class...> typename E, typename C, typename... T>
	struct ValidExpressionT;
} // namespace detail

/// \brief Can be used to determine whether a given expression (a templated type) is valid.
/// \tparam E The expression that should be tested. Usually a templated 'using' typedef.
/// \tparam T The types for which the expression should be tested.
/// Note that the size of the T parameter pack should match the template parameters required by the
/// given expression.
/// Really useful when the templated typedef 'E' is/contains a decltype declaration since
/// then SFINAE can be used to detect that the expression is invalid.
/// Can be used with C++17 constexpr if for way easier templated branches.
/// Example:
/// ```cpp
/// // Some declarations for callables (e.g. functions) foo and bar here
///
/// template<typename T> using FooCallbable = decltype(foo(std::declval<T>()));
/// template<typename T> using BarCallbable = decltype(bar(std::declval<T>()));
///
/// template<typename T> auto dispatch(const T& obj)
/// {
/// 	if constexpr(nytl::validExpression<FooCallable<T>>) return foo(obj);
///		else if constexpr(nytl::validExpression<BarCallable<T>>) return bar(obj);
///		else return fallback(obj); // otherwise templated static_assert to generate error
/// }
/// ```
template<template<typename...> typename E, typename... T>
constexpr auto validExpression = detail::ValidExpressionT<E, void, T...>::value;

// ValidExpression impl
namespace detail {
template<template<class...> typename E, typename C, typename... T> struct ValidExpressionT {
	static constexpr auto value = false;
};

template<template<class...> typename E, typename... T>
struct ValidExpressionT<E, void_t<E<T...>>, T...> {
	static constexpr auto value = true;
};

} // namespace detail
} // namespace nytl

#endif //header guard
