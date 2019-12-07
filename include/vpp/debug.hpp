// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/nonCopyable.hpp>
#include <vpp/util/span.hpp>
#include <vkpp/enums.hpp> // for vk::Result::errorExtensionNotPresent

#include <vector> // std::vector
#include <cstdint> // std::uint64_t

namespace vpp {

/// Vulkan DebugMessenger base class.
/// Requires VK_EXT_debug_utils to be enabled in instance.
/// By default this will just dump the received information for the given flags
/// to the standard output. Custom implementations can derive from this class
/// and override the call member to handle debug callbacks in a custom way.
/// Making DebugCallback virtual is reasonable regarding performance
/// since no DebugCallback should be created when using a release build.
/// NonMovable since it registers a pointer to itself as callback user data.
class DebugMessenger : public nytl::NonMovable {
public:
	// god, these names are long.
	using Data = vk::DebugUtilsMessengerCallbackDataEXT;
	using MsgSeverity = vk::DebugUtilsMessageSeverityBitsEXT;
	using MsgSeverityFlags = vk::DebugUtilsMessageSeverityFlagsEXT;
	using MsgType = vk::DebugUtilsMessageTypeBitsEXT;
	using MsgTypeFlags = vk::DebugUtilsMessageTypeFlagsEXT;

	/// Returns error | warning.
	/// Used as default debug report flags.
	/// Additional options would be verbose or info.
	static MsgSeverityFlags defaultSeverity();

	/// The default types for which to receive messages.
	/// Returns general | performance | validation, i.e. all possible types.
	static MsgTypeFlags defaultTypes();

	/// Can be set to the message names to ignore (are discarded in the
	/// default call implementation). Default implementation always outputs
	/// the names behind the messages. Useful to ignore validation layer
	/// messages that are known to be wrong (but haven't been fixed yet/
	/// no fixed version released yet).
	/// NOTE: probably makes more sense to use the message ids here
	/// but they are not reported correctly/not assigned?
	std::vector<std::string> ignore;

public:
	DebugMessenger(vk::Instance instance,
		MsgSeverityFlags severity = defaultSeverity(),
		MsgTypeFlags type = defaultTypes());
	virtual ~DebugMessenger();

	vk::Instance vkInstance() const { return instance_; }
	vk::DebugUtilsMessengerEXT vkMessenger() const { return messenger_; }

	/// This function is called from within the debug callback.
	/// It is expected to handle (e.g. simply output) the debug information in
	/// some way. Custom debug callbacks can override this function.
	/// Note that this function is not allowed to throw any exceptions since it
	/// is a callback from potential non C++ code.
	/// Will always be called from the thread in which the vulkan call takes
	/// place, so might be called from multiple threads over time.
	/// See the VK_EXT_debug_utils documentation for more details.
	virtual void call(MsgSeverity severity,
		MsgTypeFlags type, const Data& data) noexcept;

protected:
	vk::Instance instance_ {};
	vk::DebugUtilsMessengerEXT messenger_ {};
};

/// Useful for obtaining vk::ObjectType from a vpp RAII Handle.
/// Specialized in debug.cpp for supported types.
/// Optimally, we could specialize these for vk:: handles (e.g.
/// debugReportHandleType<vk::Image>) but this doesn't work on 32-bit
/// where vulkan defines some handles to just be a uint64_t typedef.
template<typename T> vk::ObjectType handleType();
template<typename T> vk::DebugReportObjectTypeEXT debugReportHandleType();

/// Converts a vk:: handle to std::uint64_t.
/// This isn't as trivial as a simple cast when dispatchable, non-dispatchable
/// and 32- as well as 64-bit should be supported without warning.
template<typename T> constexpr std::uint64_t handleToUint(T handle) {
	if constexpr(sizeof(T) == sizeof(std::uint64_t)) {
		return reinterpret_cast<std::uint64_t>(handle);
	} else if constexpr(sizeof(T) == sizeof(std::uintptr_t)) {
		return reinterpret_cast<std::uintptr_t>(handle);
	} else if constexpr(sizeof(T) != 64) {
		// This else if will catch all cases not covered by the
		// first cases, we just include the if constexpr to
		// not always trigger this assertion.
		// The condition is just a more elaborate way to write "false"
		static_assert(sizeof(T) < 0, "Invalid handle");
	}
}

// You usually don't want to use symbols in the debug or nodebug namespaces
// explicitly. See the using declaration based on VPP_NO_DEBUG_MARKER
// below. This is done so that if VPP_NO_DEBUG_MARKER is defined and
// we define the dummy symbols inline, we don't get clashes with the
// real symbols that are always defined in vpp.
namespace debug {

/// Set the name of the given handle.
/// Also see the templated version below.
/// When `hasDebugUtils` of the given device is false,
/// returns vk::Result::errorExtensionNotPresent.
vk::Result nameHandle(const vpp::Device&, std::uint64_t handle,
	vk::ObjectType, const char* name);

/// Sets the tag of the given handle.
/// Also see the templated version below.
/// When `hasDebugUtils` of the given device is false,
/// returns vk::Result::errorExtensionNotPresent.
vk::Result tagHandle(const vpp::Device&, std::uint64_t handle,
	vk::ObjectType, std::uint64_t name,
	nytl::Span<const std::byte> data);

/// Has no effect `hasDebugUtils` of the given device is false.
void beginDebugLabel(const vpp::Device& dev, vk::CommandBuffer cb,
	const char* name, std::array<float, 4> col = {});
void endDebugLabel(const vpp::Device& dev, vk::CommandBuffer);
void insertDebugLabel(const vpp::Device& dev, vk::CommandBuffer,
	const char* name, std::array<float, 4> col = {});

void beginDebugLabel(const vpp::CommandBuffer&, const char* name,
	std::array<float, 4> col = {});
void endDebugLabel(const vpp::CommandBuffer&);
void insertDebugLabel(const vpp::CommandBuffer&,
	const char* name, std::array<float, 4> col = {});

template<typename T>
vk::Result nameHandle(const T& handle, const char* name) {
	return nameHandle(handle.device(), handleToUint(handle.vkHandle()),
		handleType<T>(), name);
}

template<typename T>
vk::Result tagHandle(const T& handle, std::uint64_t name,
		nytl::Span<const std::byte> d) {
	return tagHandle(handle.device(), handleToUint(handle.vkHandle()),
		handleType<T>(), name, d);
}

/// RAII wrapper around a command buffer debug label.
/// Has no effect `hasDebugUtils` of the given device is false.
class DebugLabel {
public:
	DebugLabel(const vpp::CommandBuffer&, const char* name,
		std::array<float, 4> color = {});
	DebugLabel(const vpp::Device& dev, vk::CommandBuffer,
		const char* name, std::array<float, 4> color = {});
	~DebugLabel();

protected:
	vk::CommandBuffer cb_ {};
	const vpp::Device* dev_ {};
};

} // namespace debug

namespace nodebug {

inline vk::Result nameHandle(const vpp::Device&, std::uint64_t,
		vk::ObjectType, const char*) {
	return vk::Result::errorExtensionNotPresent;
}

inline vk::Result tagHandle(const vpp::Device&, std::uint64_t,
		vk::ObjectType, std::uint64_t,
		nytl::Span<const std::byte>) {
	return vk::Result::errorExtensionNotPresent;
}

template<typename T>
vk::Result tagHandle(const T&, std::uint64_t, nytl::Span<const std::byte>) {
	return vk::Result::errorExtensionNotPresent;
}
template<typename T>
vk::Result nameHandle(const T&, const char*) {
	return vk::Result::errorExtensionNotPresent;
}

inline void beginDebugLabel(const vpp::Device&, vk::CommandBuffer,
	const char*, std::array<float, 4> = {}) {
}
inline void endDebugLabel(const vpp::Device&, vk::CommandBuffer) {
}
inline void insertDebugLabel(const vpp::Device&, vk::CommandBuffer,
	const char*, std::array<float, 4> = {}) {
}

inline void beginDebugLabel(const vpp::CommandBuffer&, const char*,
	std::array<float, 4> = {}) {
}
inline void endDebugLabel(const vpp::CommandBuffer&) {
}
inline void insertDebugLabel(const vpp::CommandBuffer&,
	const char*, std::array<float, 4> = {}) {
}

class DebugLabel {
public:
	inline DebugLabel(const vpp::CommandBuffer&, const char*,
		std::array<float, 4> = {}) {}
	inline DebugLabel(const vpp::Device&, vk::CommandBuffer,
		const char*, std::array<float, 4> = {}) {}
	inline ~DebugLabel() {}
};

} // namespace nodebug

// When VPP_NO_DEBUG_MARKER is defined, all debug marker functions
// will be empty stubs. Useful in release builds.
#ifdef VPP_NO_DEBUG_MARKER
using namespace nodebug;
#else
using namespace debug;
#endif

} // namespace vpp
