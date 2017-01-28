// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <vector> // std::vector
#include <cstdint> // std::uint64_t

namespace vpp {

/// Vulkan DebugCallback base class.
/// By default this will just dump the received information for the given flags
/// to the standard output. Custom implementations can device from this class
/// and override the call member to handle debug callbacks in a custom way.
/// Note that making the debug callback virtual is reasonable regarding performance
/// since no DebugCallback should be created when using a release build.
/// NonMovable since it registers a pointer to itself as callback user data.
class DebugCallback : public nytl::NonMovable {
public:
	struct CallbackInfo {
		vk::DebugReportFlagsEXT flags;
		vk::DebugReportObjectTypeEXT objectType;
		uint64_t srcObject;
		size_t location;
		int32_t messageCode;
		const char* layer;
		const char* message;
	};
public:
	/// Returns error | warning | performanceWarning.
	/// Used as default debug report flags.
	/// Additional options would be information, debug or
	static vk::DebugReportFlagsEXT defaultFlags();

public:
	DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags = defaultFlags());
	~DebugCallback();

	vk::Instance vkInstance() const { return instance_; }
	vk::DebugReportCallbackEXT vkCallback() const { return debugCallback_; }

	/// This function is called from within the debug callback.
	/// It is expected to handle (e.g. simply output) the debug information in some way.
	/// Custom debug callbacks can override this function.
	/// Note that this function is not allowed to throw any exceptions since it
	/// is a callback from potential non C++ code.
	/// If this function returns false, the vulkan api call that triggered it
	/// will return a valiation failed error code.
	/// Note that this function might be called from multiple threads and therefore
	/// must be threadsafe (reason why it is marked const).
	/// The default implementation always returns true when the error flag is error and
	/// false otherwise.
	virtual bool call(const CallbackInfo& info) const noexcept;

protected:
	vk::Instance instance_ {};
	vk::DebugReportCallbackEXT debugCallback_ {};
};

} // namespace vpp
