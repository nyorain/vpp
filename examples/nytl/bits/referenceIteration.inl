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

#pragma once

#include <type_traits>
#include <iterator>

namespace nytl
{

///\ingroup utitliy
///Transforms the T iterator over pointers into a reference-iterator.
template<typename T>
class ReferenceIterator : public T
{
public:
	ReferenceIterator(const T& other) : T(other) {}

	auto operator->() -> decltype(*T{}) { return *(T::operator->()); }
	auto operator*() -> typename std::remove_pointer<decltype(*T{})>::type 
		{ return *(T::operator*()); }
};

///\ingroup utitliy
///\brief Automatically transforms a pointer iterator into a ReferenceIterator.
template<typename T> ReferenceIterator<T> 
makeReferenceIterator(const T& it)
{
	return ReferenceIterator<T>(it);
}

///\ingroup utitliy
///Allows reference iteration (like range-based for loop) over an object.
template<typename T>
class ReferenceIteration
{
public:
	T* object_;

	using iterator = ReferenceIterator<decltype(object_->begin())>;
	using const_iterator = ReferenceIterator<decltype(object_->cbegin())>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	ReferenceIteration(T& object) : object_(&object) {}

	iterator begin(){ object_->begin(); }	
	const_iterator begin() const { return object_->begin(); }
	const_iterator cbegin() const { return object_->cbegin(); }
	reverse_iterator rbegin() { return object_->rbegin(); }
	const_reverse_iterator rbegin() const { return object_->rbegin(); }
	const_reverse_iterator crbegin() const { return object_->crbegin(); }

	iterator end(){ object_->end(); }	
	const_iterator end() const { return object_->end(); }
	const_iterator cend() const { return object_->cend(); }
	reverse_iterator rend(){ return object_->rend(); }
	const_reverse_iterator rend() const { return object_->rend(); }
	const_reverse_iterator crend() const { return object_->crend(); }
};

///\ingroup utitliy
template<typename T> ReferenceIterator<T> 
makeReferenceIteration(const T& obj)
{
	return ReferenceIteration<T>(obj);
}

}
