// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <utility> // std::forward

namespace vpp {

/// Safe wrapper for deferred two step initialization.
/// It safely ensures two-step-initiazation for local objects.
/// First, create multiple resources (via the Init constructor), then
/// call init on all of them. This way, all resources can be batch-allocated
/// which will result in much less pools and base resources being needed.
template<typename T>
class Init {
public:
	/// Constructs an internal object of type T with the given arguments
	/// and the internally stored InitData as first argument.
	template<typename... A>
	Init(A&&... args) : obj_(data_, std::forward<A>(args)...) {}

	/// Calls 'init' on the object to be initialized with the given arguments
	/// and the internally stored InitData as first argument.
	/// Since this finished initialization, move returns the object.
	template<typename... A> [[nodiscard]]
	T init(A&&... args) {
		obj_.init(data_, std::forward<A>(args)...);
		return std::move(obj_);
	}

private:
	using InitData = typename T::InitData;
	InitData data_ {};
	T obj_;
};

} // namespace vpp
