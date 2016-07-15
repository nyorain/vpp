#include <vpp/pipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <vpp/utility/file.hpp>

namespace vpp
{

//pipelineLayout
PipelineLayout::PipelineLayout(const Device& dev, const vk::PipelineLayoutCreateInfo& info)
	: ResourceHandle(dev)
{
	vkHandle() = vk::createPipelineLayout(dev, info);
}

PipelineLayout::PipelineLayout(const Device& dev,
	const Range<std::reference_wrapper<DescriptorSetLayout>>& layouts,
	const Range<vk::PushConstantRange>& ranges) : ResourceHandle(dev)
{
	std::vector<vk::DescriptorSetLayout> vklayouts;
	vklayouts.reserve(layouts.size());

	for(auto& layout : layouts) vklayouts.push_back(layout.get());

	vk::PipelineLayoutCreateInfo info;
	info.setLayoutCount = vklayouts.size();
	info.pSetLayouts = vklayouts.data();
	info.pushConstantRangeCount = ranges.size();
	info.pPushConstantRanges = ranges.data();

	vkHandle() = vk::createPipelineLayout(dev, info);
}

PipelineLayout::~PipelineLayout()
{
	if(vkHandle()) vk::destroyPipelineLayout(device(), vkHandle());
}

//pipeline cache
PipelineCache::PipelineCache(const Device& dev) : ResourceHandle(dev)
{
	vkHandle() = vk::createPipelineCache(dev, {});
}

PipelineCache::PipelineCache(const Device& dev, const Range<std::uint8_t>& data)
	: ResourceHandle(dev)
{
	vkHandle() = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::PipelineCache(const Device& dev, const StringParam& filename)
	: ResourceHandle(dev)
{
	auto data = readFile(filename);
	vkHandle() = vk::createPipelineCache(dev, {{}, data.size(), data.data()});
}

PipelineCache::~PipelineCache()
{
	if(vkHandle()) vk::destroyPipelineCache(device(), vkHandle());
}

void save(vk::Device dev, vk::PipelineCache cache, const StringParam& filename)
{
	auto data = vk::getPipelineCacheData(dev, cache);
	writeFile(filename, data);
}

//Pipeline
Pipeline::~Pipeline()
{
	if(vkHandle()) vk::destroyPipeline(device(), vkHandle());
}

}
