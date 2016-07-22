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
///Provides support for the c++17 any template class.

#pragma once

#if __cplusplus >= 201411 || 1
 #include <experimental/any>
#else
 #include <stdexcept>
 #include <typeinfo>
 #include <memory>
 #include <utility>
 #include <type_traits>
#endif

namespace nytl
{

#if __cplusplus >= 201411 || 1
using Any = std::experimental::any;
using std::experimental::bad_any_cast;

#else

///Implementation for std::any (c++17 stl type).
class Any
{
protected:
	struct Interface
	{
		virtual const std::type_info& typeinfo() const = 0;
		virtual std::unique_ptr<Interface> clone() const = 0;
		virtual void* get() = 0;
	};

	template<typename T>
	struct Implementation : public Interface
	{
		T value_;

		template<typename O>
		Implementation(O&& value) : value_(std::forward<O>(value)) {}

		virtual const std::type_info& typeinfo() const override final
		{ return typeid(value_); }

		virtual std::unique_ptr<Interface> clone() const override final
		{ return std::make_unique<Implementation<T>>(*this); }

		virtual void* get() override final { return &value_; }
	};

protected:
	std::unique_ptr<Interface> ptr_ {nullptr};

public:
	Any() = default;

	template<typename T, typename = std::enable_if_t<!std::is_same_v<Any, T>>>
	Any(T&& value)
		: ptr_(new Implementation<typename std::decay<T>::type>(std::forward<T>(value))) {}

	Any(const Any& other) { if(other.ptr_) ptr_ = other.ptr_->clone(); }
	Any(Any&& other) : ptr_(std::move(other.ptr_)) {}

	void clear() { ptr_.reset(); }
	bool empty() const { return !ptr_.get(); }

	const std::type_info* typeinfo() const
		{ if(empty()) return nullptr; return &ptr_->typeinfo(); }

	void swap(Any& other){ std::swap(ptr_, other.ptr_); }
	void* get() const { if(!ptr_) return nullptr; return ptr_->get(); }
};

class bad_any_cast : public std::bad_cast
{
protected:
	const char* msg_;

public:
	bad_any_cast(const char* msg = "nytl::bad_any_cast") noexcept : msg_(msg) {}
	virtual const char* what() const noexcept override final { return msg_; }
};

template<typename T>
T* any_cast(Any* operand) noexcept
{
	if(operand->empty() || typeid(T) != *operand->typeinfo()) return nullptr;
	return static_cast<T*>(operand->get());
}

template<typename T>
const T* any_cast(const Any* operand) noexcept
{
	if(operand->empty() || typeid(T) != *operand->typeinfo()) return nullptr;
	return static_cast<const T*>(operand->get());
}

template<typename T>
T any_cast(const Any& operand)
{
	auto* ret = any_cast<typename std::add_const<typename std::remove_reference<T>::type>::type>
		(&operand);
	if(!ret) throw bad_any_cast();
	return *ret;
}

template<typename T>
T any_cast(Any& operand)
{
	auto* ret = any_cast<typename std::remove_reference<T>::type>(&operand);
	if(!ret) throw bad_any_cast();
	return *ret;
}

template<typename T>
T any_cast(Any&& operand)
{
	auto* ret = any_cast<typename std::remove_reference<T>::type>(&operand);
	if(!ret) throw bad_any_cast();
	return *ret;
}

#endif

}

//ehhhh...
namespace std
{
#if __cplusplus < 201411 && 0
	using any = nytl::Any;
	using bad_any_cast = nytl::bad_any_cast;

	template<typename T, typename A> auto any_cast(A&& operand)
	{
		return nytl::any_cast<T>(std::forward<A>(operand));
	}
#else
	using namespace experimental::fundamentals_v1;
#endif
}
