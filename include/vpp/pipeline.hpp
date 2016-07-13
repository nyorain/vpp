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

namespace fwd { extern const vk::ShaderStageFlags allShaderStages; }

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

///Allows convinient descriptorSet updates.
///Does not perform any checking.
class DescriptorSetUpdate : ResourceReference<DescriptorSetUpdate>
{
public:
	using BufferInfos = std::vector<vk::DescriptorBufferInfo>;
	using BufferViewInfos = std::vector<vk::BufferView>;
	using ImageInfos = std::vector<vk::DescriptorImageInfo>;

public:
	DescriptorSetUpdate(const DescriptorSet& set);
	~DescriptorSetUpdate();

	///\{
	///When the range member of any buffer info is 0 (default constructed), it will
	///be automatically set to vk::wholeSize.
	void uniform(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	void storage(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	void uniformDynamic(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	void storageDynamic(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	///\}

	void sampler(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void image(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void storage(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void combinedSampler(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void inputAttachment(ImageInfos images, int binding = -1, unsigned int elem = 0);

	void uniform(BufferViewInfos views, int binding = -1, unsigned int elem = 0);
	void storage(BufferViewInfos views, int binding = -1, unsigned int elem = 0);

	void copy(const vk::CopyDescriptorSet& copySet);

	///Will be automatically triggered on destruction.
	void apply();

	//XXX: more convinient copy function?
	// void copy(const DescriptorSet& set, const std::uint32_t (&binding)[2],
	// 	const std::uint32_t (&elem)[2] = {0, 0}, unsigned int count = 1);

	const auto& resourceRef() const { return *set_; }

protected:
	std::vector<vk::WriteDescriptorSet> writes_;
	std::vector<vk::CopyDescriptorSet> copies_;

	//double vecotr to avoid reference (in writes_) invalidation
	//some values must be stored continuesly, so deque doesnt work
	std::vector<std::vector<vk::DescriptorBufferInfo>> buffers_;
	std::vector<std::vector<vk::BufferView>> views_;
	std::vector<std::vector<vk::DescriptorImageInfo>> images_;

	unsigned int currentBinding_ = 0;
	const DescriptorSet* set_;

	friend void apply(const Range<std::reference_wrapper<DescriptorSetUpdate>>& updates);
};

///Applies multiple descriptor set updates.
///May be a bit more efficient than updating them individually.
void apply(const Range<std::reference_wrapper<DescriptorSetUpdate>>& updates);

///Alternative vk::DescriptorSetLayoutBinding constructor.
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
	vk::ShaderStageFlags stages = fwd::allShaderStages, unsigned int count = 1,
	unsigned int binding = -1, const vk::Sampler* samplers = nullptr);

///Defined the different vertex inputs a pipeline takes.
struct VertexBufferLayout
{
	std::vector<vk::Format> attributes;
	unsigned int binding {0};
};

///Represents a vulkan desciptor set layout which stores information about the structure of a
///descriptor set.
///Can imagined as the type (description) of which objects can be created (descriptorSets).
class DescriptorSetLayout : public ResourceHandle<vk::DescriptorSetLayout>
{
public:
	DescriptorSetLayout() = default;
	DescriptorSetLayout(const Device& dev, const Range<vk::DescriptorSetLayoutBinding>& bindings);
	~DescriptorSetLayout();

	DescriptorSetLayout(DescriptorSetLayout&& other) noexcept = default;
	DescriptorSetLayout& operator=(DescriptorSetLayout&& other) noexcept = default;
};

///XXX: store a reference to pool/layout instead of the device pointer for additional information.
///Represents a vulkan descriptor set.
///A descriptor set is basically a set of different data types (uniform & storage buffer or
///different image/sampler handles) that can be passed to the pipeline shader stages.
///A pipelines can have multiple descriptor sets which theirselfs can have multiple desctiptors
///of different types.
class DescriptorSet : public ResourceHandle<vk::DescriptorSet>
{
public:
	DescriptorSet() = default;
	DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool);
	~DescriptorSet();

	DescriptorSet(DescriptorSet&& other) noexcept = default;
	DescriptorSet& operator=(DescriptorSet&& other) noexcept = default;
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

///RAII vulkan descriptor pool wrapper.
class DescriptorPool : public ResourceHandle<vk::DescriptorPool>
{
public:
	DescriptorPool() = default;
	DescriptorPool(const Device& dev, const vk::DescriptorPoolCreateInfo& info);
	~DescriptorPool();

	DescriptorPool(DescriptorPool&& other) noexcept = default;
	DescriptorPool& operator=(DescriptorPool&& other) noexcept = default;
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
void save(const PipelineCache& cache, const StringParam& file){ save(cache.device(), cache, file); }
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
