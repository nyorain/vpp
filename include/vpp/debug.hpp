#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <vector> // std::vector
#include <cstdint> // std::uint64_t

namespace vpp {

// flags
// vk::DebugReportFlagBitsEXT::ErrorEXT
// vk::DebugReportFlagBitsEXT::WarningEXT
// vk::DebugReportFlagBitsEXT::InformationEXT
// vk::DebugReportFlagBitsEXT::DebugEXT
// vk::DebugReportFlagBitsEXT::PerformanceWarningEXT

/// Vulkan DebugCallback.
/// NonMovable since it registers a pointer to itself as callback user data.
class DebugCallback : public nytl::NonMovable {
public:
	struct CallbackInfo {
		vk::DebugReportFlagsEXT flags;
		vk::DebugReportObjectTypeEXT objectType;
		std::uint64_t srcObject;
		std::size_t location;
		std::int32_t messageCode;
		const char* layer;
		const char* message;
	};

public:
	/// Returns the common names of all useful instance validation layers.
	static const std::vector<const char*>& defaultInstanceLayerNames();

	/// Returns the common names of all useful device validation layers.
	static const std::vector<const char*>& defaultDeviceLayerNames();

	/// Returns the default debug report flags, which are all except debug and information.
	static vk::DebugReportFlagsEXT defaultFlags();

public:
	DebugCallback(vk::Instance instance, vk::DebugReportFlagsEXT flags);
	~DebugCallback();

	vk::Instance vkInstance() const { return instance_; }
	vk::DebugReportCallbackEXT vkCallback() const { return debugCallback_; }

	/// This function is called from within the debug callback.
	/// It is expected to handle (e.g. simply output) the debug information in some way.
	/// Custom debug callbacks can derive from this function.
	virtual bool call(const CallbackInfo& info);

protected:
	vk::Instance instance_ {};
	vk::DebugReportCallbackEXT debugCallback_ {};
};

} // namespace vpp
