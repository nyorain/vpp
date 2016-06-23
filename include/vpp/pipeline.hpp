#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/shader.hpp>

#include <vector>
#include <functional>

namespace vpp
{

namespace fwd { extern const vk::ShaderStageFlags allShaderStages; }

unsigned int formatSize(vk::Format format);

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
	void uniform(const BufferInfos& buffers, int binding = -1, unsigned int elem = 0);
	void storage(const BufferInfos& buffers, int binding = -1, unsigned int elem = 0);
	void uniformDynamic(const BufferInfos& buffers, int binding = -1, unsigned int elem = 0);
	void storageDynamic(const BufferInfos& buffers, int binding = -1, unsigned int elem = 0);
	///\}

	void sampler(const ImageInfos& images, int binding = -1, unsigned int elem = 0);
	void sampled(const ImageInfos& images, int binding = -1, unsigned int elem = 0);
	void storage(const ImageInfos& images, int binding = -1, unsigned int elem = 0);
	void combinedSampler(const ImageInfos& images, int binding = -1, unsigned int elem = 0);
	void inputAttachment(const ImageInfos& images, int binding = -1, unsigned int elem = 0);

	void uniform(const BufferViewInfos& views, int binding = -1, unsigned int elem = 0);
	void storage(const BufferViewInfos& views, int binding = -1, unsigned int elem = 0);

	void copy(const vk::CopyDescriptorSet& copySet);

	///Will be automatically triggered on destruction.
	void apply();

	// more convinient copy function?
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

	friend void apply(const std::vector<std::reference_wrapper<DescriptorSetUpdate>>& updates);
};

///Applies multiple descriptor set updates.
///May be a bit more efficient than updating them individually.
void apply(const std::vector<std::reference_wrapper<DescriptorSetUpdate>>& updates);

struct DescriptorBinding
{
	vk::DescriptorType type;
	vk::ShaderStageFlags stages { fwd::allShaderStages };
	unsigned int count {1};
};

struct VertexBufferLayout
{
	std::vector<vk::Format> attributes;
	unsigned int binding {0};
};

///Represents a vulkan desciptor set layout which stores information about the structure of a
///descriptor set. Needed to create a descriptor set. This class will internally store the
///given information about a descriptor set so that they can be retrieved later.
class DescriptorSetLayout : public Resource
{
public:
	DescriptorSetLayout() = default;
	DescriptorSetLayout(const Device& dev, const std::vector<DescriptorBinding>& bindings);
	~DescriptorSetLayout();

	DescriptorSetLayout(DescriptorSetLayout&& other) noexcept;
	DescriptorSetLayout& operator=(DescriptorSetLayout other) noexcept;

	vk::DescriptorSetLayout vkDescriptorSetLayout() const { return layout_; }
	const std::vector<DescriptorBinding>& bindings() const { return bindings_; }

	operator vk::DescriptorSetLayout() const { return vkDescriptorSetLayout(); }
	friend void swap(DescriptorSetLayout& a, DescriptorSetLayout& b) noexcept;

protected:
	std::vector<DescriptorBinding> bindings_;
	vk::DescriptorSetLayout layout_;
};

///Represents a vulkan descriptor set.
class DescriptorSet : public Resource
{
public:
	DescriptorSet() = default;
	DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool);
	~DescriptorSet();

	DescriptorSet(DescriptorSet&& other) noexcept;
	DescriptorSet& operator=(DescriptorSet other) noexcept;

	///Updates the descriptorSet with the given writes and copies.
	void update(const std::vector<vk::WriteDescriptorSet>& writes,
		const std::vector<vk::CopyDescriptorSet>& copies = {}) const;

	///Updates the descriptorSet with the given copies.
	void update(const std::vector<vk::CopyDescriptorSet>& copies) const;

	vk::DescriptorSet vkDescriptorSet() const { return descriptorSet_; }
	const DescriptorSetLayout& layout() const { return *layout_; }

	operator vk::DescriptorSet() const { return vkDescriptorSet(); }
	friend void swap(DescriptorSet& a, DescriptorSet& b) noexcept;

protected:
	const DescriptorSetLayout* layout_;
	vk::DescriptorSet descriptorSet_ {};
};

///Pipeline base class.
class Pipeline : public Resource
{
public:
	~Pipeline();
	Pipeline(Pipeline&& other) noexcept;
	Pipeline& operator=(Pipeline other) noexcept;

	vk::Pipeline vkPipeline() const { return pipeline_; }
	vk::PipelineLayout vkPipelineLayout() const { return pipelineLayout_; }

	operator vk::Pipeline() const { return vkPipeline(); }
	friend void swap(Pipeline& a, Pipeline& b) noexcept;

protected:
	vk::PipelineLayout pipelineLayout_ {};
	vk::Pipeline pipeline_ {};

protected:
	Pipeline() = default;
	Pipeline(const Device& dev);
};

///Can be used to load a pipeline cache from a file.
vk::PipelineCache loadPipelineCache(vk::Device dev, const char* name);

///Can be used to save a pipeline cache to a file.
void savePipelineCache(vk::Device dev, vk::PipelineCache cache, const char* name);

}
