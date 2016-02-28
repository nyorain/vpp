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
///\brief Utility base classes to be able to store Cache diRectly into its associated object.

#pragma once

#include <nytl/cloneable.hpp>
#include <unordered_map>
#include <memory>

namespace nytl
{

//TODO: implement copy on write with shared ptr instead of always-copy

///Default Cache base class used by the MultiCache template.
class Cache : public Cloneable<Cache>
{
};


///\brief Base class for classes that carry associated Cache objects.
///\details Objects of classes dervied from MultiCache are able to hold multiple associated Cache 
//objects that have Base as common base class and can be individually accessed by a given key.
///Copying one MultiCache object does clone all its Cache objects, it may be therefore an
///expensive operation. If the MultiCache object is move-constructed or move-assigned no Cache
///object has usually to be moved or copied.
template<typename Key, typename Base = Cache>
class MultiCache
{
protected:
	mutable std::unordered_map<Key, std::unique_ptr<Base>> cache_; //unordered_map with ids?

protected:
	///This function clears all Cache objects and should be called whenever the object is changed
	///in a way that invalidates its Cache objects.
	void invalidateCache() const
	{
		cache_.clear();
	}

public:
	MultiCache() noexcept = default;
	~MultiCache() noexcept = default;

	MultiCache(const MultiCache& other) : cache_() 
		{ for(auto& c : other.cache_) cache_[c.first] = clone(*c.second); }
	MultiCache(MultiCache&& other) noexcept : cache_(std::move(other.cache_)) {}

	MultiCache& operator=(const MultiCache& other) 
	{ 
		cache_.clear(); 
		for(auto& c : other.cache_) 
			cache_[c.first] = clone(*c.second); 
		return *this; 
	}

	MultiCache& operator=(MultiCache&& other) noexcept 
		{ cache_ = std::move(other.cache_); return *this; }

	///Gets a Cache object pointer for a given key if existent.
	///\return The associated Cache for a given key, nullptr if none is found for that key.
	Base* cache(const Key& id) const
	{
        auto it = cache_.find(id);
		if(it != cache_.end())
			return it->second.get();

        return nullptr;
	}

	///Stores a Cache object with for a given key. If there exists already some Cache object for
	///the given key it will be replaced. 
	///\return A reference to the moved Cache object.
	Base& cache(const Key& id, std::unique_ptr<Base>&& c) const
	{
		auto& ret = *c;
        cache_[id] = std::move(c);
		return ret;
	}

	///Clears the set Cache object for the given key. 
	///\return 1 if an found Cache object was cleared, 0 otherwise.
	bool resetCache(const Key& id) const
	{
        auto it = cache_.find(id);
		if(it != cache_.cend())
		{
			cache_.erase(it);
			return 1;
		}

        return 0;
	}
};

}
