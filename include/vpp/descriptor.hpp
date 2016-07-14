#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

namespace fwd { extern const vk::ShaderStageFlags allShaderStages; }

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
	void imageSampler(ImageInfos images, int binding = -1, unsigned int elem = 0);
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
	vk::ShaderStageFlags stages = fwd::allShaderStages, unsigned int binding = -1,
	unsigned int count = 1, const vk::Sampler* samplers = nullptr);

}
