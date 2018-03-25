// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>
#include <vpp/util/span.hpp>

#include <string_view>
#include <vector>
#include <functional>

namespace vpp {

/// RAII vulkan pipeline layout wrapper.
/// A pipeline layout specifies all descriptor layouts and push constant ranges an
/// application can use.
/// Multiple pipelines can share the same pipeline layout.
class PipelineLayout : public ResourceHandle<vk::PipelineLayout> {
public:
	PipelineLayout() = default;
	PipelineLayout(const Device& dev, const vk::PipelineLayoutCreateInfo& info);
	PipelineLayout(const Device& dev,
		nytl::Span<const vk::DescriptorSetLayout> layouts,
		nytl::Span<const vk::PushConstantRange> ranges);
	~PipelineLayout();

	PipelineLayout(PipelineLayout&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(PipelineLayout&& rhs) noexcept { swap(*this, rhs); return *this; }
};

/// RAII vulkan pipeline cache wrapper.
/// A pipeline cache can be used to decrease pipeline compilation time.
/// The application can save driver-specific cache data from created pipelines to disk
/// and then load them during the next execution and pass them to the pipeline constructor
/// which will decrease pipeline build time.
class PipelineCache : public ResourceHandle<vk::PipelineCache> {
public:
	PipelineCache() = default;
	PipelineCache(const Device& dev);

	/// Loads the pipeline cache data from a file.
	/// If except is true and the file cannot be read, throws a
	/// std::runtime_error, otherwise just starts with an empty cache.
	PipelineCache(const Device&, std::string_view file, bool except = false);

	/// Creates the pipeline cache with the given initial data.
	PipelineCache(const Device& dev, nytl::Span<const std::uint8_t> data);

	~PipelineCache();

	PipelineCache(PipelineCache&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(PipelineCache rhs) noexcept { swap(*this, rhs); return *this; }
};

/// Saves a pipeline cache to the given filename.
/// Returns whether saving was succesful. Will not throw on failure.
bool save(vk::Device dev, vk::PipelineCache cache, std::string_view filename);
inline bool save(const PipelineCache& cache, std::string_view file) {
	return save(cache.device(), cache, file);
}

/// RAII Vulkan pipeline wrapper.
/// A pipeline is basically a collection of draw/compute information that contains e.g.
/// the vertex and descriptor layouts, used shaders and different state information.
class Pipeline : public ResourceHandle<vk::Pipeline> {
public:
	Pipeline() = default;
	Pipeline(const Device& dev, vk::Pipeline pipeline) : ResourceHandle(dev, pipeline) {}
	~Pipeline();

	Pipeline(Pipeline&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(Pipeline&& rhs) noexcept { swap(*this, rhs); return *this; }
};

} // namespace vpp
