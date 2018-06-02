// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp>
#include <vpp/util/span.hpp>
#include <vkpp/enums.hpp>

#include <vector> // std::vector
#include <cstdint> // std::uint64_t

namespace vpp {

/// Vulkan DebugCallback base class.
/// Requires VK_EXT_debug_report to be enabled in instance (will throw otherwise).
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
	/// Additional options would be e.g. information or debug.
	static vk::DebugReportFlagsEXT defaultFlags();

	/// Returns all possible bits.
	static vk::DebugReportFlagsEXT allBits();

	/// The default flags for which to return a validation error,
	/// i.e. make the vulkan call fail. Returns only the error flag.
	static vk::DebugReportFlagsEXT defaultErrorFlags();

public:
	DebugCallback(vk::Instance instance,
		vk::DebugReportFlagsEXT flags = defaultFlags(), bool verbose = false,
		vk::DebugReportFlagsEXT errorFlags = defaultErrorFlags());
	virtual ~DebugCallback();

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
	virtual bool call(const CallbackInfo& info) noexcept;

protected:
	vk::Instance instance_ {};
	vk::DebugReportCallbackEXT debugCallback_ {};
	vk::DebugReportFlagsEXT errorFlags_ {};
	bool verbose_ {};
};

/// - Debug marker utitlity -
/// Require VK_EXT_debug_marker in instance, will have no effect otherwise.
/// Will return vk::Result::errorExtensionNotPresent otherwise.

/// Set the name of the given handle.
/// Also see the templated version below.
vk::Result nameHandle(vk::Device, std::uint64_t handle,
	vk::DebugReportObjectTypeEXT, const char* name);

/// Sets the tag of the given handle.
/// Also see the templated version below.
vk::Result tagHandle(vk::Device, std::uint64_t handle,
	vk::DebugReportObjectTypeEXT, std::uint64_t name,
	nytl::Span<const std::byte> data);

/// Return false if the extension (its function) could not be loaded.
bool beginDebugRegion(vk::Device, vk::CommandBuffer, const char* name,
	std::array<float, 4> col = {});
bool endDebugRegion(vk::Device, vk::CommandBuffer);
bool insertDebugMarker(vk::Device, vk::CommandBuffer, const char* name,
	std::array<float, 4> col = {});

namespace detail {

template<typename T>
struct DebugHandleType;

#define DebugHandleSpec(handle, name) \
	template<> struct DebugHandleType<handle> { \
		static constexpr auto value = vk::DebugReportObjectTypeEXT::name; \
	}

DebugHandleSpec(vk::Instance, instance);
DebugHandleSpec(vk::PhysicalDevice, physicalDevice);
DebugHandleSpec(vk::Device, device);
DebugHandleSpec(vk::CommandBuffer, commandBuffer);
DebugHandleSpec(vk::Queue, queue);
DebugHandleSpec(vk::Image, image);
DebugHandleSpec(vk::ImageView, imageView);
DebugHandleSpec(vk::Buffer, buffer);
DebugHandleSpec(vk::BufferView, bufferView);
DebugHandleSpec(vk::Framebuffer, framebuffer);
DebugHandleSpec(vk::Sampler, sampler);
DebugHandleSpec(vk::DeviceMemory, deviceMemory);
DebugHandleSpec(vk::DescriptorSetLayout, descriptorSetLayout);
DebugHandleSpec(vk::DescriptorSet, descriptorSet);
DebugHandleSpec(vk::DescriptorPool, descriptorPool);
DebugHandleSpec(vk::PipelineLayout, pipelineLayout);
DebugHandleSpec(vk::Pipeline, pipeline);
DebugHandleSpec(vk::RenderPass, renderPass);
DebugHandleSpec(vk::Semaphore, semaphore);
DebugHandleSpec(vk::Fence, fence);
DebugHandleSpec(vk::Event, event);
DebugHandleSpec(vk::QueryPool, queryPool);
DebugHandleSpec(vk::ShaderModule, shaderModule);

// Only supported in newer versions
// DebugHandleSpec(vk::SurfaceKHR, surfaceKHR);
// DebugHandleSpec(vk::SwapchainKHR, swapchainKHR);
// DebugHandleSpec(vk::DebugReportCallbackEXT, debugReportCallbackEXT);
// DebugHandleSpec(vk::DisplayKHR, displayKHR);
// DebugHandleSpec(vk::DisplayModeKHR, displayModeKHR);

#undef DebugHandleSpec

} // namespace detail

template<typename T> constexpr auto debugReportHandleType =
	detail::DebugHandleType<T>::value;

template<typename T>
vk::Result nameHandle(vk::Device dev,T handle, const char* name) {
	return nameHandle(dev, (std::uint64_t) handle,
		debugReportHandleType<T>, name);
}

template<typename T>
void tagHandle(vk::Device dev, T handle, std::uint64_t name,
		nytl::Span<const std::byte> d) {
	return tagHandle(dev, (std::uint64_t) handle, debugReportHandleType<T>,
		name, d);
}

} // namespace vpp
