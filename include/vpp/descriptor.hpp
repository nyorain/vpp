// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/handles.hpp>
#include <vpp/util/span.hpp>
#include <vpp/util/nonCopyable.hpp>

#include <vector>
#include <deque>

namespace vpp {
namespace fwd {
	extern const vk::ShaderStageFlags allShaderStages;
} // namespace fwd

/// Allows convenient descriptorSet updates.
/// Does not perform any checking and has the overhead of internally
/// allocating memory.
class DescriptorSetUpdate : public nytl::NonCopyable {
public:
	using BufferInfos = std::vector<vk::DescriptorBufferInfo>;
	using BufferViewInfos = std::vector<vk::BufferView>;
	using ImageInfos = std::vector<vk::DescriptorImageInfo>;

public:
	DescriptorSetUpdate() = default;
	DescriptorSetUpdate(const DescriptorSet& set);
	~DescriptorSetUpdate();

	DescriptorSetUpdate(DescriptorSetUpdate&&) noexcept = default;
	DescriptorSetUpdate& operator=(DescriptorSetUpdate&&) noexcept = default;

	/// When the range member of any buffer info is 0 (default constructed), it will
	/// be automatically set to vk::wholeSize.
	void uniform(BufferInfos, int binding = -1, unsigned int elem = 0);
	void storage(BufferInfos, int binding = -1, unsigned int elem = 0);
	void uniformDynamic(BufferInfos, int binding = -1, unsigned int elem = 0);
	void storageDynamic(BufferInfos, int binding = -1, unsigned int elem = 0);

	// NOTE: we could also just added a vk::DescriptorBufferInfo conversion
	// operator to BufferSpan, but this is probably cleaner
	void uniform(nytl::Span<const BufferSpan>, int binding = -1,
		unsigned elem = 0);
	void storage(nytl::Span<const BufferSpan>, int binding = -1,
		unsigned elem = 0);
	void uniformDynamic(nytl::Span<const BufferSpan>, int binding = -1,
		unsigned elem = 0);
	void storageDynamic(nytl::Span<const BufferSpan>, int binding = -1,
		unsigned elem = 0);

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

	/// Resets all queued updates without executing them.
	void reset();

	/// Skips the given number of descriptors.
	void skip(unsigned int count = 1) { currentBinding_ += count; };

	const auto& device() const { return set_->device(); }
	auto vkDevice() const { return device().vkDevice(); }
	auto vkInstance() const { return device().vkInstance(); }
	auto vkPhysicalDevice() const { return device().vkPhysicalDevice(); }

	const auto& writes() const { return writes_; }
	const auto& copies() const { return copies_; }

protected:
	std::vector<vk::WriteDescriptorSet> writes_;
	std::vector<vk::CopyDescriptorSet> copies_;

	// double vector to avoid reference (in writes_) invalidation
	// some values must be stored continuesly, so deque doesnt work
	std::vector<std::vector<vk::DescriptorBufferInfo>> buffers_;
	std::vector<std::vector<vk::BufferView>> views_;
	std::vector<std::vector<vk::DescriptorImageInfo>> images_;

	unsigned int currentBinding_ = 0;
	const DescriptorSet* set_ {};
};

/// Applies multiple descriptor set updates.
/// May be a bit more efficient than updating them individually.
void apply(nytl::Span<const std::reference_wrapper<DescriptorSetUpdate>>);
void apply(nytl::Span<DescriptorSetUpdate>);

/// Alternative vk::DescriptorSetLayoutBinding constructor.
/// When passed to the DescriptorSetLayout constructor, will automatically
/// update binding number without spaces if it is autoDescriptorBinding (-1).
constexpr auto autoDescriptorBinding = std::uint32_t(0xFFFFFFFF);
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
	vk::ShaderStageFlags stages = fwd::allShaderStages,
	const vk::Sampler* samplers = nullptr,
	std::uint32_t count = 1, std::uint32_t binding = autoDescriptorBinding);

/// Less intuitive overload, kept for legacy reason.
[[deprecated("Use more intuitive sampler-first overload")]]
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
	vk::ShaderStageFlags stages, std::uint32_t binding,
	std::uint32_t count = 1, const vk::Sampler* samplers = nullptr);

} // namespace vpp
