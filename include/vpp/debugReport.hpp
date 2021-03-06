// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/debug.hpp>
#include <vpp/util/span.hpp>

#include <vector> // std::vector
#include <cstdint> // std::uint64_t

namespace vpp {

/// Vulkan DebugCallback base class.
/// Requires VK_EXT_debug_report to be enabled in instance.
/// Prefer to vpp::DebugMesssenger via VK_EXT_debug_utils, that extension
/// was added later as a replacement for VK_EXT_debug_report.
/// By default this will just dump the received information for the given flags
/// to the standard output. Custom implementations can derive from this class
/// and override the call member to handle debug callbacks in a custom way.
/// Making DebugCallback virtual is reasonable regarding performance
/// since no DebugCallback should be created when using a release build.
/// NonMovable since it registers a pointer to itself as callback user data.
class VPP_API DebugCallback {
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
	/// Additional options would be e.g. information or debug.
	static vk::DebugReportFlagsEXT defaultFlags();

	/// Returns all possible bits.
	static vk::DebugReportFlagsEXT allBits();

	/// The default flags for which to return a validation error,
	/// i.e. make the vulkan call fail. Returns only the error flag.
	static vk::DebugReportFlagsEXT defaultErrorFlags();

public:
	[[deprecated("Prefer vpp::DebugMessenger and VK_EXT_debug_utils")]]
	DebugCallback(vk::Instance instance,
		vk::DebugReportFlagsEXT flags = defaultFlags(), bool verbose = false,
		vk::DebugReportFlagsEXT errorFlags = defaultErrorFlags());
	virtual ~DebugCallback();

	DebugCallback(DebugCallback&&) = delete;
	DebugCallback& operator=(DebugCallback&&) = delete;

	vk::Instance vkInstance() const { return instance_; }
	vk::DebugReportCallbackEXT vkCallback() const { return debugCallback_; }

	/// This function is called from within the debug callback. It is expected
	/// to handle (e.g. simply output) the debug information in some way.
	/// Custom debug callbacks can override this function. Note that this
	/// function is not allowed to throw any exceptions since it is a callback
	/// from potential non C++ code. If this function returns false, the
	/// vulkan api call that triggered it will return a valiation failed error
	/// code. Note that this function might be called from multiple threads
	/// and therefore must be threadsafe (reason why it is marked const). The
	/// default implementation always returns true when the error flag is error
	/// and false otherwise.
	virtual bool call(const CallbackInfo& info) noexcept;

protected:
	vk::Instance instance_ {};
	vk::DebugReportCallbackEXT debugCallback_ {};
	vk::DebugReportFlagsEXT errorFlags_ {};
	bool verbose_ {};
};

// NOTE: could add functions for naming, tagging and debug markers again.
// They were broken and therefore removed in commit 012252c on 7.12.2019
// that fixed them for debug utils since the debug callback extension
// is deprecated anyways.

} // namespace vpp

