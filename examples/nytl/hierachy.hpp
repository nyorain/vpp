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
///\brief Contains a template class for creating Hierachy-structered clas objects.

#pragma once

#include <nytl/nonCopyable.hpp>
#include <nytl/bits/referenceIteration.inl>
#include <nytl/bits/recursiveIteration.inl>
#include <vector>

namespace nytl
{

///\brief Virtual utility base template class for objects that are part of a Hierachy.
///\ingroup utility
template <typename Root, typename Child = Root>
class HierachyBase
{
public:
	using node_type = HierachyBase<Root, Child>;
	using Vector_type = std::vector<Child*>;

	using iterator = ReferenceIterator<typename Vector_type::iterator>;
	using const_iterator = ReferenceIterator<typename Vector_type::const_iterator>;
	using reverse_iterator = ReferenceIterator<typename Vector_type::reverse_iterator>;
	using const_reverse_iterator = ReferenceIterator<typename Vector_type::const_reverse_iterator>;

	using rec_iterator = RecursiveIterator<node_type>;
	using const_rec_iterator = ConstRecursiveIterator<node_type>;
	using reverse_rec_iterator = std::reverse_iterator<rec_iterator>;
	using const_reverse_rec_iterator = std::reverse_iterator<const_rec_iterator>;

	using recursive_iteration = RecursiveIteration<node_type>;
	using const_recursive_iteration = RecursiveIteration<const node_type>;

	using root_type = Root;
	using child_type = Child;
		
private:
	Vector_type children_;

protected:
    HierachyBase() = default;
	virtual ~HierachyBase(){ destroy(); }

public:
	//iterator
	iterator begin(){ return children_.begin(); }
	const_iterator begin() const { return children_.cbegin(); }
	const_iterator cbegin() const { return children_.cbegin(); }
	reverse_iterator rbegin(){ return children_.rbegin(); }
	const_reverse_iterator rbegin() const { return children_.crbegin(); }
	const_reverse_iterator crbegin() const { return children_.crbegin(); }

	iterator end(){ return children_.end(); }
	const_iterator end() const { return children_.cend(); }
	const_iterator cend() const { return children_.cend(); }
	reverse_iterator rend(){ return children_.rend(); }
	const_reverse_iterator rend() const { return children_.crend(); }
	const_reverse_iterator crend() const { return children_.crend(); }

	//recursive_iterator
	rec_iterator recursive_begin()
		{ return rec_iterator(children_.begin()); }
	const_rec_iterator recursive_begin() const 
		{ return const_rec_iterator(children_.cbegin()); }
	const_rec_iterator recursive_cbegin() const 
		{ return const_rec_iterator(children_.cbegin()); }
	reverse_rec_iterator recursive_rbegin()
		{ return reverse_rec_iterator(children_.rbegin()); }
	const_reverse_rec_iterator recursive_rbegin() const
		{ return const_reverse_rec_iterator(children_.crbegin()); }
	const_reverse_rec_iterator recursive_crbegin() const
		{ return const_reverse_rec_iterator(children_.crbegin()); }

	rec_iterator recursive_end() { return children_.empty() ? 
		rec_iterator(children_.end()) : children_[0]->recursive_end(); }
	const_rec_iterator recursive_end() const { return children_.empty() ?
		const_rec_iterator(children_.cend()) : children_[0]->recursive_cend(); }
	const_rec_iterator recursive_cend() const { return children_.empty() ?
		const_rec_iterator(children_.cend()) : children_[0]->recursive_cend(); }
	reverse_rec_iterator recursive_rend() { return children_.empty() ?
		reverse_rec_iterator(children_.rend()) : children_[0]->recursive_rend(); }
	const_reverse_rec_iterator recursive_rend() const { return children_.empty() ?
		const_reverse_rec_iterator(children_.crend()) : children_[0]->recursive_crend(); }
	const_reverse_rec_iterator recursive_crend() const { return children_.empty() ?
		const_reverse_rec_iterator(children_.crend()) : children_[0]->recursive_crend(); }

	//
	recursive_iteration recursive() { return recursive_iteration(*this); }
	const_recursive_iteration recursive() const { return const_recursive_iteration(*this); }

	const Vector_type& children() const { return children_; } 
	std::size_t childrenCount() const { return children_.size(); }

    virtual void addChild(Child& child) { children_.push_back(&child); }
	virtual bool removeChild(Child& child)	
	{
	    for(auto it = children_.cbegin(); it != children_.cend(); ++it)
	    {
            if(*it == &child)
            {
                children_.erase(it);
                return 1;
            }
        }
	    return 0;
	}

	virtual void destroy()
	{
		//c->destroy will trigger removeChild for this object, which may invalidate iterators
		auto cpy = children_;
	    for(auto* c : cpy)
        {
            static_cast<node_type*>(c)->destroy();
        }

	    children_.clear();
    };

	virtual const Root& root() const = 0;
	virtual Root& root() = 0;
};

template<typename T, typename Root = typename T::root_type, typename Child = typename T::child_type>
class HierachyNode : public T
{
public:
	using root_type = Root;
	using child_type = Child;

protected:
	T* parent_;

protected:
	HierachyNode() = default;

	virtual void destroy() override
	{
		T::destroy();
	    if(parent_) 
		{
			parent_->removeChild(reinterpret_cast<Child&>(*this));
			parent_ = nullptr;
		}
	}

	void create(T& parent)
	{
		parent_ = &parent;
		parent.addChild(reinterpret_cast<Child&>(*this));
	}

public:
	HierachyNode(T& parent) : parent_(&parent) { create(parent); }

	virtual root_type& root() override { return parent_->root(); }
	virtual const root_type& root() const override { return parent_->root(); }

	T& parent() const { return *parent_; }
};

template<typename T, typename Root = typename T::root_type, typename Child = typename T::child_type>
class HierachyRoot : public T
{
public:
	using root_type = Root;
	using child_type = Child;

public:
	virtual const root_type& root() const override 
		{ return reinterpret_cast<const root_type&>(*this); }
	virtual root_type& root() override 
		{ return reinterpret_cast<root_type&>(*this); }
};

}
