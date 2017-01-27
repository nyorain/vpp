// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

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
	Initializer(A&&... args) : obj_()
		{ obj_.create(std::forward<A>(args)...); }

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

/// General documentation for vpp two-step-initiazation classes.
/// Constructors shall always either fully construct the object or be default constructors.
/// This way users of classes can always be sure that constructing the object with arguments
/// will initialize it wihtout having to look into some documentation.

/// For two-step-initiazation only the two member functions create(...) and init(...) will
/// be used on a default constructed object. Calling create() for an object that was
/// not default constructed and since then unchanged, calling init() for an object
/// on that create() was not called before, or calling one of the functions
/// more than one time is usually undefined behaviour.
/// It therefore might work but class writers are encouraged throw an exception in such
/// a case.
/// Both functions can be const but are not required to be so.
/// There might also be mutliple overloads of both functions, taking 0 or more arguments.
/// Classes should try to avoid redundant information in the both functions,
/// e.g. if both of them need certain information they should simply take it as paramter
/// in create and then store it (if this does not introduce an unacceptable overheat).

/// If one wants to re-two-step-initialized an already initialized object, it must first
/// move assign (or copy assign if available) with a default constructed object and then
/// call the two functions.
/// Using an uninitialized object will result in undefined behaviour. This time class
/// authors are explicitly encouraged to NOT check for this case, since that would result
/// in high overheads, so this will likely lead to a memory error e.g. when dereferencing
/// an nullptr.
/// Destructors (as an exception) should work for default constructed object as well,
/// i.e. they must not assume that the destructing object was ever valid.

/// Classes should generally avoid having something like a destroy method (and if, then protected).
/// In move operators the destructor can be directly called.
/// Usually classes implement a free friend swap function for themselves and then use it
/// for the move operator.

} // namespace vpp
