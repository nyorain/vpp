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
///\brief Defines a utility to implement easy conversions without conversion operator.

#pragma once

#include <type_traits>

namespace nytl
{

///Specialize this template with a static call function to enable explicit conversions
///from type O to type T using the nytl::convert function.
///By default it will just perform a static cast.
///\sa nytl::convert
template<typename T, typename O, typename = void>
struct Converter;

template<typename T, typename O>
struct Converter<T, O, decltype(static_cast<T>(std::declval<O>()))>
{
	static O call(const T& other) { return static_cast<O>(other); }
};

///The convert function can be used to call specialization of the Converted struct.
///This can be used to implement free conversion operations in a consistent way.
template<typename T, typename O>
auto convert(const O& other) -> decltype(Converter<T, O>::call(other))
{
	return Converter<T, O>::call(other);
}

///Utility wrapper around type O that will convert into any type to which the stored object
///can be converted to.
template<typename O>
class AutoCastable
{
public:
	template<typename T>
	operator T() const { return convert<T>(*data_); }

public:
	const O* data_;
};

///Convert overload that only takes one template parameter which will be deduced.
///Returns a wrapper object that will implicitly cast into any object into which the given
///object of type O can be converted.
template<typename O> AutoCastable<O> convert(const O& other) { return {&other}; }

}
