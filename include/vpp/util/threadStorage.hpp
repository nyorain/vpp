// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <unordered_map> // std::unordered_map
#include <thread> // std::thread::id
#include <mutex> // std::scoped_lock
#include <shared_mutex> // std::shared_mutex
#include <utility> // std::pair
#include <vector> // std::vector
#include <memory> // std::unique_ptr
#include <algorithm> // std::find

namespace vpp {

/// Utility base class that can be used (wrapped in a unique_ptr) as template parameter for
/// ThreadLocalStorage to make it possible to store any object in it without having
/// to fulfill the reqruirements std::any has (e.g. CopyConstructible).
/// Only has a virtual destructor.
struct DynamicStorageBase {
	virtual ~DynamicStorageBase() noexcept = default;
};

/// Derives From DynamicStorageBase and allows to store one value of the
/// given type.
template<typename T>
struct ValueStorage : public DynamicStorageBase {
	template<typename... Args>
	ValueStorage(Args&&... args) :
		value(std::forward<Args>(args)...) {}

	T value;
};

/// Implements a thread-dependent collection of 'T'.
/// Simulates threadlocal storage since every thread will have its own value.
/// Allows to register and remove storage entries.
/// Uses a shared mutex for synchronization, so obtaining the storage objects
/// should be really fast as long as no entries are added or removed.
/// \requires Type 'T' shall be default constructible and movable.
template<typename T>
class ThreadStorage {
public:
	/// Adds a new storage object.
	/// Returns the id of the added segment over which it can be accessed in all
	/// threads and also removed.
	/// \param obj [out] Will be set to the address of the added storage object.
	/// At the beginning (in every thread) the storage object will be empty.
	/// Is guaranteed to be valid until this ThreadLocalStorage is destructed or the
	/// added storage is removed.
	/// Parameter can be a nullptr, in which case no object for the current thread will
	/// be created.
	unsigned int add(T** obj = nullptr);

	/// Returns the storage object with the given id for the calling thread.
	/// If the given id is invalid (i.e. was never added or already removed), returns nullptr.
	/// Will return a default-constructed object if the storage was never changed.
	/// The returned pointer (if not nullptr) is guaranteed to be valid until
	/// this ThreadLocalStorage is destructed or the storage with the given id is removed.
	/// \param id The storage segment id to retrieve. Must be returned from add and be
	/// removed yet.
	T* get(unsigned int id);
	const T* get(unsigned int id) const;

	/// Removes the storage object with the given id.
	/// This will erase the storage object with the given id for all threads.
	/// The caller has to make sure that no thread is currently accessing this
	/// storage and never again will. Will also call the destructor of the stored
	/// objects (i.e. all will be destruced before this call returns).
	/// \param id The id of the storage to remove.
	/// \return Whether a storage with the given id was found.
	/// Returns false if it was already removed or has never existed.
	bool remove(unsigned int id);

protected:
	unsigned int highestID_ {};
	std::vector<unsigned int> ids_;
	std::unordered_map<std::thread::id, std::unordered_map<unsigned int, T>> objects_;
	mutable std::shared_mutex mutex_;
};

using DynamicStoragePtr = std::unique_ptr<DynamicStorageBase>;
using DynamicThreadStorage = ThreadStorage<DynamicStoragePtr>;

// - implementation -
template<typename T>
unsigned int ThreadStorage<T>::add(T** obj)
{
	std::scoped_lock lock(mutex_);
	auto id = ++highestID_;
	ids_.push_back(id);
	if(obj) {
		*obj = &objects_[std::this_thread::get_id()][id];
	}

	return id;
}

template<typename T>
T* ThreadStorage<T>::get(unsigned int id)
{
	std::shared_lock lock(mutex_);
	if(std::find(ids_.begin(), ids_.end(), id) == ids_.end()) {
		return nullptr;
	}

	return &objects_[std::this_thread::get_id()][id];
}

template<typename T>
const T* ThreadStorage<T>::get(unsigned int id) const
{
	std::shared_lock lock(mutex_);
	if(std::find(ids_.begin(), ids_.end(), id) == ids_.end()) {
		return nullptr;
	}

	return &objects_[std::this_thread::get_id()][id];
}

template<typename T>
bool ThreadStorage<T>::remove(unsigned int id)
{
	std::scoped_lock lock(mutex_);
	auto it = std::find(ids_.begin(), ids_.end(), id);
	if(it == ids_.end()) {
		return false;
	}

	ids_.erase(it);
	for(auto& obj : objects_) {
		obj.second.erase(id);
	}

	return true;
}

} // namespace vpp
