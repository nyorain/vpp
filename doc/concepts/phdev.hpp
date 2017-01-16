// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/span.hpp>

#include <vector> // std::vector
#include <utility> // std::pair

namespace vpp {

/// Tries to select the best phsycial device from the given span by queyring its properties.
/// Will usually choose the one with the best properties like device type, biggest memory and
/// needed queue families. Can be used if an application has no special needs and just wants
/// to choose the device that is usuall the best.
vk::PhysicalDevice choose(nytl::Span<vk::PhysicalDevice>);

/// Chooses the best physical device that can be used to render on the given surface.
/// Usually tries to select the best/most powerful physical device.
/// Returns a null handle if the given span is empty or no physical device supports
/// rendering on the surface.
vk::PhysicalDevice choose(nytl::Span<vk::PhysicalDevice>, vk::SurfaceKHR);

/// Specifies in which way a queue family should be selected.
/// When 'none' is used, the first valid queue family is returned.
/// When 'highestCount' is used, the valid queue familiy with the highest queue count is returned.
/// When 'miminalGranularity' is used, the valid queue family with the smallest minimal image
/// transfer granularity is returned. Note that this treats the granularity (0,0,0) as
/// the worst, i.e. the greatest.
enum class OptimizeQueueFamily {
	none,
	highestCount,
	minimalGranularity
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
/// If there is no such queue family returns -1.
int findQueueFamily(vk::PhysicalDevice, vk::SurfaceKHR, vk::QueueFlags = {},
	OptimizeQueueFamily optimize = {});

} // namespace vpp

/// An example code snippted that shows some example physical device and queue querying.
/// ```cpp
/// auto surface = ... ; // from window library or manually created
/// auto phDevs = ... ; // something like vk::enumeratePhysicalDevices(instance);
///
/// auto phdev = vpp::choose(phdevs, surface);
/// if(!phdev)
/// 	throw std::runtime_error("There are no usable vulkan physical devices!");
///
/// auto presentFamily = int {}; // present queue family
/// auto gfxCompFamily = int {}; // graphics and compute queue family
///
/// auto combined = vk::QueueBits::graphics | vk::QueueBits::compute;
/// presentFamily = gfxCompFamily = vpp::findQueueFamily(phdev, surface, combined);
/// if(presentFamily == -1) {
/// 	presentFamily = vpp::findQueueFamily(phdev, surface);
/// 	gfxCompFamily = vpp::findQueueFamily(phdev, surface, combined);
/// }
///
/// if(presentFam == -1 || gfxCompFam == -1)
/// 	throw std::runtime_error("Could not find all needed queue features!");
/// ```
