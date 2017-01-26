// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/pipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>

namespace vpp {

// PipelineLayout
PipelineLayout::PipelineLayout(const Device& dev, const vk::PipelineLayoutCreateInfo& info)
	: ResourceHandle(dev)
{
	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::PipelineLayout(const Device& dev,
	nytl::Span<const std::reference_wrapper<DescriptorSetLayout>> layouts,
	nytl::Span<const vk::PushConstantRange> ranges) : ResourceHandle(dev)
{
	std::vector<vk::DescriptorSetLayout> vklayouts;
	vklayouts.reserve(layouts.size());

	for(auto& layout : layouts) vklayouts.push_back(layout.get());

	vk::PipelineLayoutCreateInfo info;
	info.setLayoutCount = vklayouts.size();
	info.pSetLayouts = vklayouts.data();
	info.pushConstantRangeCount = ranges.size();
	info.pPushConstantRanges = ranges.data();

	handle_ = vk::createPipelineLayout(dev, info);
}

PipelineLayout::~PipelineLayout()
{
	if(vkHandle()) vk::destroyPipelineLayout(device(), vkHandle());
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

PipelineCache::PipelineCache(const Device& dev, nytl::StringParam filename)
	: ResourceHandle(dev)
{
	auto data = readFile(filename);
	handle_ = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::~PipelineCache()
{
	if(vkHandle()) vk::destroyPipelineCache(device(), vkHandle());
}

void save(vk::Device dev, vk::PipelineCache cache, nytl::StringParam filename)
{
	auto data = vk::getPipelineCacheData(dev, cache);
	writeFile(filename, data);
}

// Pipeline
Pipeline::~Pipeline()
{
	if(vkHandle())
		vk::destroyPipeline(device(), vkHandle());
}

} // namespace vpp
