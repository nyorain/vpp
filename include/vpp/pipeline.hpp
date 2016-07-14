#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>
#include <vpp/utility/range.hpp>
#include <vpp/utility/stringParam.hpp>

#include <vector>
#include <functional>

namespace vpp
{

///Returns the size in bytes of the given format.
///E.g. vk::Format::r8g8b8a8* will return 4, since it has a size of 4 * 8 bits = 32 bits = 4 bytes.
///For compressed formats this function will return the size of one block in bytes.
///\sa blockSize
unsigned int formatSize(vk::Format format);

///Returns the size of one compressed block of a compressed vulkan format.
///If the given format is not a compressed format, {1, 1} is returned.
///For vk::Format::undefined, {0, 0} is returned
///\sa formatSize
vk::Extent2D blockSize(vk::Format format);


///Defined the different vertex inputs a pipeline takes.
struct VertexBufferLayout
{
	std::vector<vk::Format> attributes;
	unsigned int binding {0};
};

///RAII vulkan pipeline layout wrapper.
///A pipeline layout specifies all descriptor layouts and push constant ranges an
///application can use.
///Multiple pipelines can share the same pipeline layout.
class PipelineLayout : public ResourceHandle<vk::PipelineLayout>
{
public:
	PipelineLayout() = default;
	PipelineLayout(const Device& dev, const vk::PipelineLayoutCreateInfo& info);
	PipelineLayout(const Device& dev,
		const Range<std::reference_wrapper<DescriptorSetLayout>>& layouts,
		const Range<vk::PushConstantRange>& ranges = {});
	~PipelineLayout();

	PipelineLayout(PipelineLayout&& other) noexcept = default;
	PipelineLayout& operator=(PipelineLayout&& other) noexcept = default;
};

///RAII vulkan pipeline cache wrapper.
///A pipeline cache can be used to increase pipeline compilation time.
///The application can save driver-specific cache data from created pipelines to disk
///and then load them during the next execution and pass them to the pipeline constructor
///which will decrease pipeline build time.
class PipelineCache : public ResourceHandle<vk::PipelineCache>
{
public:
	PipelineCache() = default;
	PipelineCache(const Device& dev);

	///Creates the pipeline cache with the given initial data.
	PipelineCache(const Device& dev, const Range<std::uint8_t>& data);

	///Loads the pipeline cache data from a file.
	///\exception std::runtime_error if opening the file fails
	PipelineCache(const Device& dev, const StringParam& filename);
	~PipelineCache();

	PipelineCache(PipelineCache&& other) noexcept = default;
	PipelineCache& operator=(PipelineCache&& other) noexcept = default;
};

///\{
///Saves a pipeline cache to the given filename.
///\exception std::runtime_error if opening/writing the file fails
void save(vk::Device dev, vk::PipelineCache cache, const StringParam& filename);
inline void save(const PipelineCache& cache, const StringParam& file)
	{ save(cache.device(), cache, file); }
///\}

///XXX: resourceRef: pipeline layout
///RAII Vulkan pipeline wrapper.
///A pipeline is basically a collection of draw/compute information that contains e.g.
///the vertex and descriptor layouts, used shaders and different state information.
class Pipeline : public ResourceHandle<vk::Pipeline>
{
public:
	Pipeline() = default;
	Pipeline(const Device& dev, vk::Pipeline pipeline) : ResourceHandle(dev, pipeline) {}
	~Pipeline();

	Pipeline(Pipeline&& other) noexcept = default;
	Pipeline& operator=(Pipeline&& other) noexcept = default;
};

}
