// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/nonCopyable.hpp>
#include <vpp/util/span.hpp>

// TODO: only 32-bit
#include <vkpp/enums.hpp>

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

/// Useful for obtaining vk::ObjectType from a vk:: Handle.
/// specialized in debug.cpp for supported types.
template<typename T> vk::ObjectType handleType();
template<typename T> vk::DebugReportObjectTypeEXT debugReportHandleType();

// when VPP_NO_DEBUG_MARKER is defined, all debug marker functions
// will be empty stubs. Useful in release builds.
#ifndef VPP_NO_DEBUG_MARKER

// Require VK_EXT_debug_utils respectively in instance,
// will have no effect otherwise and return vk::Result::errorExtensionNotPresent.

/// Set the name of the given handle.
/// Also see the templated version below.
vk::Result nameHandle(vk::Device, std::uint64_t handle,
	vk::ObjectType, const char* name);

/// Sets the tag of the given handle.
/// Also see the templated version below.
vk::Result tagHandle(vk::Device, std::uint64_t handle,
	vk::ObjectType, std::uint64_t name,
	nytl::Span<const std::byte> data);

#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)

/// Return false if the extension (its function) could not be loaded.
bool beginDebugLabel(vk::CommandBuffer, const char* name,
	std::array<float, 4> col = {});
bool endDebugLabel(vk::CommandBuffer);
bool insertDebugLabel(vk::CommandBuffer, const char* name,
	std::array<float, 4> col = {});

template<typename T>
vk::Result nameHandle(vk::Device dev, const T& handle, const char* name) {
	return nameHandle(dev, (std::uint64_t) handle, handleType<T>(), name);
}

template<typename T>
vk::Result nameHandle(const ResourceHandle<T>& handle, const char* name) {
	return nameHandle(handle.device(), handle.vkHandle(), name);
}

template<typename T>
vk::Result tagHandle(vk::Device dev, const T& handle, std::uint64_t name,
		nytl::Span<const std::byte> d) {
	return tagHandle(dev, (std::uint64_t) handle, handleType<T>(), name, d);
}

template<typename T>
vk::Result tagHandle(const ResourceHandle<T>& handle, std::uint64_t name,
		nytl::Span<const std::byte> d) {
	return tagHandle(handle.device(), handle.vkHandle(), name, d);
}

#else // 64-bit

template<typename T>
vk::Result tagHandle(vk::Device, const T&, std::uint64_t,
		nytl::Span<const std::byte>) {
	return vk::Result::errorExtensionNotPresent;
}
template<typename T>
vk::Result nameHandle(vk::Device, const T&, const char*) {
	return vk::Result::errorExtensionNotPresent;
}

template<typename T>
vk::Result nameHandle(const ResourceHandle<T>& handle, const char* name) {
	return vk::Result::errorExtensionNotPresent;
}

template<typename T>
vk::Result tagHandle(const ResourceHandle<T>& handle, std::uint64_t name,
		nytl::Span<const std::byte> d) {
	return vk::Result::errorExtensionNotPresent;
}

#endif // 64-bit

#else // VPP_NO_DEBUG_MARKER

inline vk::Result nameHandle(vk::Device, std::uint64_t,
		vk::ObjectType, const char*) {
	return vk::Result::errorExtensionNotPresent;
}
inline vk::Result tagHandle(vk::Device, std::uint64_t,
		vk::ObjectType, std::uint64_t, nytl::Span<const std::byte>) {
	return vk::Result::errorExtensionNotPresent;
}
inline bool beginDebugLabel(vk::CommandBuffer, const char*,
		std::array<float, 4> = {}) {
	return false;
}
inline bool endDebugLabel(vk::CommandBuffer) {
	return false;
}
inline bool insertDebugLabel(vk::CommandBuffer, const char*,
		std::array<float, 4> = {}) {
	return false;
}
template<typename T>
vk::Result tagHandle(vk::Device, const T&, std::uint64_t,
		nytl::Span<const std::byte>) {
	return vk::Result::errorExtensionNotPresent;
}
template<typename T>
vk::Result nameHandle(vk::Device, const T&, const char*) {
	return vk::Result::errorExtensionNotPresent;
}

template<typename T>
vk::Result nameHandle(const ResourceHandle<T>& handle, const char* name) {
	return vk::Result::errorExtensionNotPresent;
}

template<typename T>
vk::Result tagHandle(const ResourceHandle<T>& handle, std::uint64_t name,
		nytl::Span<const std::byte> d) {
	return vk::Result::errorExtensionNotPresent;
}

#endif

// RAII wrapper around a command buffer debug label.
class DebugLabel {
public:
	DebugLabel() = default;
	DebugLabel(vk::CommandBuffer, const char* name,
		std::array<float, 4> color = {});
	~DebugLabel();

	DebugLabel(DebugLabel&&) noexcept = default;
	DebugLabel& operator=(DebugLabel&&) noexcept = default;

protected:
	vk::CommandBuffer cb_ {};
};

} // namespace vpp
