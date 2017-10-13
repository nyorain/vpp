// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/pipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>
#include <dlg/dlg.hpp>

namespace vpp {

// PipelineLayout
PipelineLayout::PipelineLayout(const Device& dev, const vk::PipelineLayoutCreateInfo& info)
	: ResourceHandle(dev)
{
	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::PipelineLayout(const Device& dev,
	nytl::Span<const vk::DescriptorSetLayout> layouts,
	nytl::Span<const vk::PushConstantRange> ranges) : ResourceHandle(dev)
{
	vk::PipelineLayoutCreateInfo info;
	info.setLayoutCount = layouts.size();
	info.pSetLayouts = layouts.data();
	info.pushConstantRangeCount = ranges.size();
	info.pPushConstantRanges = ranges.data();

	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::~PipelineLayout()
{
	if(vkHandle()) {
		vk::destroyPipelineLayout(device(), vkHandle());
	}
}

// PipelineCache
PipelineCache::PipelineCache(const Device& dev) : ResourceHandle(dev)
{
	handle_ = vk::createPipelineCache(dev, {});
}

PipelineCache::PipelineCache(const Device& dev, nytl::Span<const std::uint8_t> data)
	: ResourceHandle(dev)
{
	handle_ = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::PipelineCache(const Device& dev, std::string_view filename,
	bool expect) : ResourceHandle(dev)
{
	std::vector<std::uint8_t> data;
	try {
		data = readFile(filename);
	} catch(const std::exception& err) {
		dlg_info("PipelineCache: {}", err.what());
		if(expect) {
			throw;
		}
	}

	handle_ = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::~PipelineCache()
{
	if(vkHandle()) {
		vk::destroyPipelineCache(device(), vkHandle());
	}
}

void save(vk::Device dev, vk::PipelineCache cache, std::string_view filename)
{
	auto data = vk::getPipelineCacheData(dev, cache);
	writeFile(filename, data);
}

// Pipeline
Pipeline::~Pipeline()
{
	if(vkHandle()) {
		vk::destroyPipeline(device(), vkHandle());
	}
}

} // namespace vpp
