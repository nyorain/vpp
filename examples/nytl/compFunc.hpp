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
///\brief Defines the CompatibleFunction (CompFunc typedef'd) template class.

#pragma once

#include <nytl/functionTraits.hpp>
#include <nytl/bits/tupleMap.inl>
#include <nytl/bits/apply.inl>

#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

namespace nytl
{

//TODO: always correct reference (rvalue, lvalue) handling, perfmance improvement
//does a lamba object ever use the std::function stack storage buffer most impl have?
//custom function object (type erasure) to use then to stroe the function?
//correct forwarding of args

//Not specified, use the function-signature template <Ret(Args...)> like std::function.
template<typename S> class CompatibleFunction;

///\brief A Function object that is able to hold all functions with a compatible signature.
///\ingroup function
template<typename R, typename... A>
class CompatibleFunction<R(A...)>
{
public:
	using Ret = R;
	using Signature = Ret(A...);
	using Function = std::function<Signature>;
	using CompFuncType = CompatibleFunction<Signature>;
	using ArgsTuple = std::tuple<A...>;

protected:
    Function func_ {};

public:
    CompatibleFunction() = default;
    ~CompatibleFunction() = default;

    //constructor
    template<typename F, typename = std::enable_if_t<IsCallable<F>>>
    CompatibleFunction(F func) noexcept { set(func); }

    template<typename F, typename O, typename = std::enable_if_t<IsCallable<F>>>
    CompatibleFunction(F func, O& object) noexcept { set(memberCallback(func, object)); }

    CompatibleFunction(const CompFuncType& other) noexcept
		: func_(other.func_) {}
    template<typename Sig> CompatibleFunction(const CompatibleFunction<Sig>& other) noexcept
		{ set(other.func_); }

    //assignement
    template<typename F, typename = typename std::enable_if_t<IsCallable<F>>>
    CompFuncType& operator=(F func) noexcept { set(func); return *this; }

    CompFuncType& operator=(const CompFuncType& other) noexcept
		{ func_ = other.func_; return *this; }
    template<typename Sig> CompFuncType& operator=(const CompatibleFunction<Sig>& other) noexcept
		{ set(other.func_); return *this; }

    //set
    template<typename F>
    void set(F func) noexcept
    {
		using FuncTraits = FunctionTraits<F>;
        using RealArgsTuple = typename FuncTraits::ArgTuple;
		using RealRet = typename FuncTraits::ReturnType;
        using MapType = detail::TupleMap<ArgsTuple, RealArgsTuple>;

        static_assert(std::is_convertible<R, RealRet>::value, "Return types not compatible");
        static_assert(MapType::Seq::size() == FuncTraits::arg_size, "Arguments not compatible");

        func_ = [=](A... args) -> Ret {
                return static_cast<Ret>(apply(func, MapType::map(std::forward<A>(args)...)));
            };
    }

//get
    Function function() const noexcept { return func_; }

    //call
    Ret call(A... args) const { func_(std::forward<A>(args)...); }
    Ret operator()(A... args) const { func_(std::forward<A>(args)...); }

	operator bool() const { return function(); }
};

template<typename... A>
class CompatibleFunction<void(A...)>
{
public:
	using Ret = void;
	using Signature = Ret(A...);
	using Function = std::function<Signature>;
	using CompFuncType = CompatibleFunction<Signature>;
	using ArgsTuple = std::tuple<A...>;

protected:
	Function func_;

public:
    CompatibleFunction() = default;
    ~CompatibleFunction() = default;

    //constructor
    template<typename F, typename = typename std::enable_if_t<IsCallable<F>>>
    CompatibleFunction(F func) noexcept { set(func); }

    template<typename F, typename O, typename = typename std::enable_if_t<IsCallable<F>>>
    CompatibleFunction(F func, O object) noexcept { set(memberCallback(func, object)); }

    CompatibleFunction(const CompFuncType& other) noexcept
		: func_(other.func_) {}
    template<typename Sig> CompatibleFunction(const CompatibleFunction<Sig>& other) noexcept
		{ set(other.func_); }

    //assignement
    template<typename F, typename = typename std::enable_if_t<IsCallable<F>>>
    CompFuncType& operator=(F func) noexcept { set(func); return *this; }

    CompFuncType& operator=(const CompFuncType& other) noexcept
		{ func_ = other.func_; return *this; }
    template<typename Sig> CompFuncType& operator=(const CompatibleFunction<Sig>& other) noexcept
		{ set(other.func_); return *this; }

    //set
    template<typename F>
    void set(F func) noexcept
    {
		using FuncTraits = FunctionTraits<F>;
        using RealArgsTuple = typename FuncTraits::ArgTuple;
        using MapType = detail::TupleMap<ArgsTuple, RealArgsTuple>;

        static_assert(MapType::Seq::size() == FuncTraits::ArgSize, "Arguments not compatible");

        func_ = [=](A... args) -> Ret {
                apply(func, MapType::map(std::forward<A>(args)...));
            };
    }

    //get
    Function function() const noexcept { return func_; }

    //call
    Ret call(A... args) const { func_(std::forward<A>(args)...); }
    Ret operator()(A... args) const { func_(std::forward<A>(args)...); }

	operator bool() const { return function(); }
};

//typedef CompFunc
template<typename S> using CompFunc = CompatibleFunction<S>;


}
