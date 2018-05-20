// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/util/span.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <vector>
#include <deque>

namespace vpp {
namespace fwd {
	extern const vk::ShaderStageFlags allShaderStages;
} // namespace fwd

/// Represents a vulkan desciptor set layout which stores information
/// about the structure of a descriptor set.
class DescriptorSetLayout : public ResourceHandle<vk::DescriptorSetLayout> {
public:
	DescriptorSetLayout() = default;
	DescriptorSetLayout(const Device&,
		nytl::Span<const vk::DescriptorSetLayoutBinding>);
	DescriptorSetLayout(const Device&, vk::DescriptorSetLayout);
	~DescriptorSetLayout();

	DescriptorSetLayout(DescriptorSetLayout&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorSetLayout rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// Represents a vulkan descriptor set.
/// Note that this class cannot automatically destroy the DescriptorSet handle
/// on destruction since the vulkan spec does not allow this per default.
/// See also the trackedDescriptor api for more functionality.
class DescriptorSet : public ResourceHandle<vk::DescriptorSet> {
public:
	DescriptorSet() = default;
	DescriptorSet(const DescriptorPool&, const DescriptorSetLayout&);
	DescriptorSet(const DescriptorPool&, vk::DescriptorSetLayout);
	DescriptorSet(vk::DescriptorPool, const DescriptorSetLayout&);
	DescriptorSet(const Device&, vk::DescriptorPool, vk::DescriptorSetLayout);
	DescriptorSet(const Device&, vk::DescriptorSet);
	~DescriptorSet() = default;

	DescriptorSet(DescriptorSet&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorSet rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// RAII vulkan descriptor pool wrapper.
class DescriptorPool : public ResourceHandle<vk::DescriptorPool> {
public:
	DescriptorPool() = default;
	DescriptorPool(const Device&, const vk::DescriptorPoolCreateInfo&);
	DescriptorPool(const Device&, vk::DescriptorPool);
	~DescriptorPool();

	DescriptorPool(DescriptorPool&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(DescriptorPool rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// Allows convinient descriptorSet updates.
/// Does not perform any checking.
class DescriptorSetUpdate : public ResourceReference<DescriptorSetUpdate> {
public:
	using BufferInfos = std::vector<vk::DescriptorBufferInfo>;
	using BufferViewInfos = std::vector<vk::BufferView>;
	using ImageInfos = std::vector<vk::DescriptorImageInfo>;

public:
	DescriptorSetUpdate(const DescriptorSet& set);
	~DescriptorSetUpdate();

	/// When the range member of any buffer info is 0 (default constructed), it will
	/// be automatically set to vk::wholeSize.
	void uniform(BufferInfos, int binding = -1, unsigned int elem = 0);
	void storage(BufferInfos, int binding = -1, unsigned int elem = 0);
	void uniformDynamic(BufferInfos, int binding = -1, unsigned int elem = 0);
	void storageDynamic(BufferInfos, int binding = -1, unsigned int elem = 0);

	void sampler(ImageInfos, int binding = -1, unsigned int elem = 0);
	void image(ImageInfos, int binding = -1, unsigned int elem = 0);
	void storage(ImageInfos, int binding = -1, unsigned int elem = 0);
	void imageSampler(ImageInfos, int binding = -1, unsigned int elem = 0);
	void inputAttachment(ImageInfos, int binding = -1, unsigned int elem = 0);

	void uniformView(BufferViewInfos, int binding = -1, unsigned int elem = 0);
	void storageView(BufferViewInfos, int binding = -1, unsigned int elem = 0);

	void copy(const vk::CopyDescriptorSet& copySet);

	/// Appplies all queued updates.
	/// If never called, will be automatically triggered on destruction.
	void apply();

	/// Skips the given number of descriptors.
	void skip(unsigned int count = 1) { currentBinding_ += count; };
	const auto& resourceRef() const { return *set_; }

protected:
	std::vector<vk::WriteDescriptorSet> writes_;
	std::vector<vk::CopyDescriptorSet> copies_;

	// double vector to avoid reference (in writes_) invalidation
	// some values must be stored continuesly, so deque doesnt work
	std::vector<std::vector<vk::DescriptorBufferInfo>> buffers_;
	std::vector<std::vector<vk::BufferView>> views_;
	std::vector<std::vector<vk::DescriptorImageInfo>> images_;

	unsigned int currentBinding_ = 0;
	const DescriptorSet* set_;

	friend void apply(nytl::Span<const std::reference_wrapper<
		DescriptorSetUpdate>> updates);
};

/// Applies multiple descriptor set updates.
/// May be a bit more efficient than updating them individually.
void apply(nytl::Span<const std::reference_wrapper<DescriptorSetUpdate>>);

/// Alternative vk::DescriptorSetLayoutBinding constructor.
/// When passed to the DescriptorSetLayout constructor, will automatically
/// update binding number without spaces if it is -1.
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
	vk::ShaderStageFlags stages = fwd::allShaderStages,
	unsigned int binding = -1,
	unsigned int count = 1,
	const vk::Sampler* samplers = nullptr);

} // namespace vpp
