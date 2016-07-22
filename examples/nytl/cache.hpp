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
///\brief Utility base classes to be able to store Cache diRectly into its associated object.

#pragma once

#include <nytl/clone.hpp>
#include <unordered_map>
#include <memory>

namespace nytl
{

struct Cache
{
	Cache() = default;
	virtual ~Cache() = default;
};

///\brief Base class for classes that carry associated Cache objects.
///\details Objects of classes dervied from MultiCache are able to hold multiple associated Cache
///objects that have Base as common base class and can be individually accessed by a given key.
template<typename Key, typename Base = Cache>
class MultiCache
{
protected:
	mutable std::unordered_map<Key, std::shared_ptr<Base>> cache_; //unordered_map with ids?

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

	MultiCache(const MultiCache& other) : cache_(other.cache_) {}
	MultiCache& operator=(const MultiCache& other) { cache_ = other.cache_; return *this; }

	MultiCache(MultiCache&& other) noexcept : cache_(std::move(other.cache_)) {}
	MultiCache& operator=(MultiCache&& other) noexcept
		{ cache_ = std::move(other.cache_); return *this; }

	///Gets a const Cache object pointer for a given key if existent.
	///\return The associated Cache for a given key, nullptr if none is found for that key.
	const Base* cache(const Key& id) const
	{
        auto it = cache_.find(id);
		if(it != cache_.end())
			return it->second.get();

        return nullptr;
	}

	///Stores a Cache object with for a given key. If there exists already some Cache object for
	///the given key it will be replaced.
	///\return A reference to the moved Cache object.
	template<typename T>
	T& cache(const Key& id, std::shared_ptr<T> c) const
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
