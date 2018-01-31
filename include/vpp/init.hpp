// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <utility> // std::forward

namespace vpp {

/// Safe wrapper for two step initialization.
/// This class is a utility wrapper around one of the most important concepts of vpp.
/// It safely ensures two-step-initiazation for local used objects.
/// By constructing an initializer for an object (e.g. a buffer), you pass the needed
/// arguments to the constructor which will then be forwarded to the buffer constructor.
/// The buffer constructor will async request some memory for itself (other constructors will
/// request other resources or submit/queue work for the vulkan device).
/// When then the init member function of the initializer is called with the needed
/// initialization arguments (none for the buffer class) the second step of the initialization
/// will be executed, i.e. the buffer will make sure it retrieves and binds the requested
/// memory (other types will here e.g. wait for submitted work completion or retrieve those
/// other requested resources).
/// This makes two-step-initiazation really efficient if multiple objects
/// (even of different types) have to be initialized.
/// First construct all intializers for the objects you want to construct. Then retrieve the
/// objects with calls to the Initializer::init member function which will initialize the
/// more complex resources of the objects. This way those complex initializations can
/// be batched togehter, e.g. there have to be only one memory allocation, only one work
/// submit and only one descriptor pool creation for many resources.
/// This object makes it safer to deal with two step initilization, since it forces you
/// to fully initiliaze the object before using it.
template<typename T>
class Initializer {
public:
	///Constructs an internal object of type T with the given arguments.
	template<typename... A>
	Initializer(A&&... args) : obj_(defer, std::forward<A>(args)...) {}

	///Initialzed the object constructed by this initializer with the given arguments
	///which completes the initialization process and returns (moves) the object.
	///Calling this function will make the initializer invalid and any further calls to
	///init may result in undefined behaviour.
	template<typename... A>
	T init(A&&... args)
		{ obj_.init(std::forward<A>(args)...); return std::move(obj_); }

private:
	T obj_;
};

} // namespace vpp
