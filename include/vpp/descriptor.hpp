// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/span.hpp>
#include <vector>

namespace vpp {
namespace fwd {
	extern const vk::ShaderStageFlags allShaderStages;
} // namespace fwd

/// Represents a vulkan desciptor set layout which stores information about the structure of a
/// descriptor set.
/// Can imagined as the type (description) of which objects can be created (descriptorSets).
class DescriptorSetLayout : public ResourceHandle<vk::DescriptorSetLayout> {
public:
	DescriptorSetLayout() = default;
	DescriptorSetLayout(const Device& dev,
		nytl::Span<const vk::DescriptorSetLayoutBinding> bindings);
	~DescriptorSetLayout();

	DescriptorSetLayout(DescriptorSetLayout&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorSetLayout rhs) noexcept { swap(*this, rhs); return *this; }
};

// TODO: store a reference to pool/layout instead of the device pointer for additional information.
// which one?
/// TODO: currently DescriptorSet does not/cannot free it on desctruction

/// Represents a vulkan descriptor set.
/// A descriptor set is basically a set of different data types (uniform & storage buffer or
/// different image/sampler handles) that can be passed to the pipeline shader stages.
/// A pipelines can have multiple descriptor sets which theirselfs can have multiple desctiptors
/// of different types.
class DescriptorSet : public ResourceHandle<vk::DescriptorSet> {
public:
	DescriptorSet() = default;
	DescriptorSet(const Device&, vk::DescriptorSetLayout, vk::DescriptorPool);
	DescriptorSet(const DescriptorSetLayout& layout, vk::DescriptorPool pool);
	DescriptorSet(const Device&, vk::DescriptorSet);
	~DescriptorSet();

	DescriptorSet(DescriptorSet&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorSet rhs) noexcept { swap(*this, rhs); return *this; }
};

/// RAII vulkan descriptor pool wrapper.
class DescriptorPool : public ResourceHandle<vk::DescriptorPool> {
public:
	DescriptorPool() = default;
	DescriptorPool(const Device& dev, const vk::DescriptorPoolCreateInfo& info);
	~DescriptorPool();

	DescriptorPool(DescriptorPool&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorPool rhs) noexcept { swap(*this, rhs); return *this; }
};

// TODO: rename to DescriptorUpdate? easier and better

/// Allows convinient descriptorSet updates.
/// Does not perform any checking.
class DescriptorSetUpdate : ResourceReference<DescriptorSetUpdate> {
public:
	using BufferInfos = std::vector<vk::DescriptorBufferInfo>;
	using BufferViewInfos = std::vector<vk::BufferView>;
	using ImageInfos = std::vector<vk::DescriptorImageInfo>;

public:
	DescriptorSetUpdate(const DescriptorSet& set);
	~DescriptorSetUpdate();

	/// When the range member of any buffer info is 0 (default constructed), it will
	/// be automatically set to vk::wholeSize.
	void uniform(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	void storage(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	void uniformDynamic(BufferInfos buffers, int binding = -1, unsigned int elem = 0);
	void storageDynamic(BufferInfos buffers, int binding = -1, unsigned int elem = 0);

	void sampler(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void image(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void storage(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void imageSampler(ImageInfos images, int binding = -1, unsigned int elem = 0);
	void inputAttachment(ImageInfos images, int binding = -1, unsigned int elem = 0);

	void uniformView(BufferViewInfos views, int binding = -1, unsigned int elem = 0);
	void storageView(BufferViewInfos views, int binding = -1, unsigned int elem = 0);

	void copy(const vk::CopyDescriptorSet& copySet);

	/// Will be automatically triggered on destruction.
	void apply();

	/// Skips the given number of descriptors.
	void skip(unsigned int count = 1) { currentBinding_ += count; };

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

	friend void apply(nytl::Span<const std::reference_wrapper<DescriptorSetUpdate>> updates);
};

/// Applies multiple descriptor set updates.
/// May be a bit more efficient than updating them individually.
void apply(nytl::Span<const std::reference_wrapper<DescriptorSetUpdate>> updates);

/// Alternative vk::DescriptorSetLayoutBinding constructor.
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
	vk::ShaderStageFlags stages = fwd::allShaderStages, unsigned int binding = -1,
	unsigned int count = 1, const vk::Sampler* samplers = nullptr);

} // namespace vpp
