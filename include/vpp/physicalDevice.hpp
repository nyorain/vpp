// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/span.hpp>

#include <vector> // std::vector
#include <utility> // std::pair
#include <string> // std::string
#include <array> // std::array

namespace vpp {

/// Tries to select the best physical device from the given span by querying its properties.
/// Will usually choose the one with the best properties like device type, biggest memory and
/// needed queue families. Can be used if an application has no special needs and just wants
/// to choose the device that is usual the best.
vk::PhysicalDevice choose(nytl::Span<vk::PhysicalDevice>);

/// Chooses the best physical device that can be used to render on the given surface.
/// Usually tries to select the best/most powerful physical device.
/// Returns a null handle if the given span is empty or no physical device supports
/// rendering on the surface.
/// The given surface and physical devices must have been retrieved from the
/// given instance which is needed for querying the surface function pointers.
vk::PhysicalDevice choose(nytl::Span<vk::PhysicalDevice>, vk::Instance, vk::SurfaceKHR);

/// Returns the name of the given enum value.
const char* name(vk::PhysicalDeviceType);

/// Returns the version (major, minor, patch) of the given driverVersion (from
/// vk::PhysicalDeviceProperties)
std::array<unsigned int, 3> apiVersion(uint32_t driverVersion);

/// Returns a (by default multiline) string identifying the given physical device.
/// Useful as debug mechanism, users usually only need the name.
std::string description(vk::PhysicalDevice, const char* sep = "\n");

/// Specifies in which way a queue family should be selected.
/// When 'none' is used, the first valid queue family is returned.
/// When 'highestCount' is used, the valid queue family with the highest queue count is returned.
/// When 'mimImageGranularity' is used, the valid queue family with the smallest minimal image
/// transfer granularity is returned. Note that this treats the granularity (0,0,0) as
/// the worst, i.e. the greatest.
enum class OptimizeQueueFamily {
	none,
	highestCount,
	minImageGranularity
};

/// Returns a queue family on the given physical device that fulfills the given queue bits.
/// The optimize parameter can be used to select the best matching queue for the callers needs.
/// See also enum class OptimizeQueueFamily.
/// If there is no such queue family returns -1.
int findQueueFamily(vk::PhysicalDevice, vk::QueueFlags, OptimizeQueueFamily optimize = {});

/// Returns a queue family that can be used ot present on the given surface.
/// The additional queue bits and optimize parameters can be used to optimize the returned
/// queue if multiple are available, or try to find a queue family that additionally
/// has the given bits set.
/// The physical device and instance must have been
/// If there is no such queue family returns -1.
int findQueueFamily(vk::PhysicalDevice, vk::Instance, vk::SurfaceKHR, vk::QueueFlags = {},
	OptimizeQueueFamily optimize = {});

} // namespace vpp
