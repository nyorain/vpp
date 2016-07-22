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
///\brief Defines utility templates to get inforMation about callable types.

#pragma once

#include <tuple>

namespace nytl
{

namespace detail
{

template<typename T, typename = void>
struct IsCallableImpl
{
	static constexpr bool value = 0;
};

template<typename T>
struct IsCallableImpl<T, typename std::enable_if<std::is_class<T>::value>::type>
{
private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    struct Fallback
    {
        void operator()();
    };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;
    template<typename> static yes test(...);
    template<typename C> static no test(Check<void (Fallback::*)(), &C::operator()>*);

public:
    static constexpr bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

template<typename R, typename S, typename... Args>
struct IsCallableImpl<R(S::*)(Args...)>
{
public:
	static constexpr bool value = 1;
};

template<typename R, typename... Args>
struct IsCallableImpl<R(*)(Args...)>
{
public:
	static constexpr bool value = 1;
};

template<typename R, typename... Args>
struct IsCallableImpl<R(&)(Args...)>
{
public:
	static constexpr bool value = 1;
};

template<typename R, typename... Args>
struct IsCallableImpl<R(Args...)>
{
public:
	static constexpr bool value = 1;
};

}

///\ingroup function
///Meta-Template to check if a type can be called like a function.
template<typename T> static constexpr bool IsCallable = detail::IsCallableImpl<T>::value;


///\ingroup function
///Meta-Template class to retrieve inforMation about a function type.
template<typename F> class FunctionTraits;

//for raw function signature
template<typename Ret, typename... Args>
class FunctionTraits<Ret(Args...)>
{
public:
	using Size = std::size_t;
    using ArgTuple = std::tuple<Args...>;

protected:
    template<std::size_t i>
	struct ArgImpl
    {
        using type = typename std::tuple_element<i, ArgTuple>::type;
    };

public:
    using ReturnType = Ret;
	using Signature = Ret(Args...);
    template<std::size_t i> using ArgType = typename ArgImpl<i>::type;
    constexpr static Size ArgSize = std::tuple_size<ArgTuple>::value;
};

//function pointer
template<typename Ret, typename... Args>
class FunctionTraits<Ret(*)(Args...)> : public FunctionTraits<Ret(Args...)> {};

//member function pointer
template<typename C, typename Ret, typename... Args>
class FunctionTraits<Ret(C::*)(Args...)> : public FunctionTraits<Ret(Args...)> {};

//const member function pointer
template<typename C, typename Ret, typename... Args>
class FunctionTraits<Ret(C::*)(Args...) const> : public FunctionTraits<Ret(Args...)> {};

//functor, class
template<typename F>
class FunctionTraits : public FunctionTraits<decltype(&F::operator())> {};

template<typename F>
class FunctionTraits<F&> : public FunctionTraits<F> {};

template<typename F>
class FunctionTraits<F&&> : public FunctionTraits<F> {};

}
