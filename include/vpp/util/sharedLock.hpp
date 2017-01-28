// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

namespace vpp {

/// Locks the given mutex type shared.
/// Like std::lock_guard but for shared mutex locking.
/// \requiers Type 'T' shall fulfill the stl SharedMutex concept.
template<typename T>
struct SharedLockGuard {
	SharedLockGuard(T& mutex) : mutex_(mutex) { mutex_.lock_shared(); }
	~SharedLockGuard() { mutex_.unlock_shared(); }

	T& mutex_;
};

} // namespace vpp
