// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>

namespace vpp {
namespace fwd {
	const vk::ShaderStageFlags allShaderStages = vk::ShaderStageBits::all;
} // namespace fwd

// DescriptorSetLayout
DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
		nytl::Span<const vk::DescriptorSetLayoutBinding> bindings) :
			ResourceHandle(dev) {

	static constexpr auto defaultBinding = std::uint32_t(-1);

	std::vector<vk::DescriptorSetLayoutBinding> vkbindings;
	vkbindings.reserve(bindings.size());

	unsigned int highestBinding = 0u;
	for(auto& binding : bindings) {
		vkbindings.emplace_back(binding);
		auto& bid = vkbindings.back().binding;
		if(bid == defaultBinding) bid = highestBinding++;
		else highestBinding = std::max(highestBinding, bid + 1);
	}

	vk::DescriptorSetLayoutCreateInfo descriptorLayout;
	descriptorLayout.bindingCount = vkbindings.size();
	descriptorLayout.pBindings = vkbindings.data();

	handle_ = vk::createDescriptorSetLayout(vkDevice(), descriptorLayout);
}

DescriptorSetLayout::DescriptorSetLayout(const Device& dev,
	vk::DescriptorSetLayout layout) : ResourceHandle(dev, layout) {
}

DescriptorSetLayout::~DescriptorSetLayout() {
	if(vkHandle()) {
		vk::destroyDescriptorSetLayout(vkDevice(), vkHandle());
	}
}

// DescriptorSet
DescriptorSet::DescriptorSet(const DescriptorPool& p,
		const DescriptorSetLayout& l) : DescriptorSet(p, l.vkHandle()) {
}

DescriptorSet::DescriptorSet(const DescriptorPool& pool,
	vk::DescriptorSetLayout layout) :
	DescriptorSet(pool.device(), pool, layout) {
}

DescriptorSet::DescriptorSet(vk::DescriptorPool pool,
	const DescriptorSetLayout& layout) :
	DescriptorSet(layout.device(), pool, layout) {
}

DescriptorSet::DescriptorSet(const Device& dev, vk::DescriptorPool pool,
		vk::DescriptorSetLayout layout) : ResourceHandle(dev) {

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;
	vk::allocateDescriptorSets(vkDevice(), allocInfo, handle_);
}

DescriptorSet::DescriptorSet(const Device& dev, vk::DescriptorSet set)
	: ResourceHandle(dev, set) {
}

// DescriptorPool
DescriptorPool::DescriptorPool(const Device& dev,
		const vk::DescriptorPoolCreateInfo& info) : ResourceHandle(dev) {
	handle_ = vk::createDescriptorPool(dev, info);
}

DescriptorPool::DescriptorPool(const Device& dev, vk::DescriptorPool pool)
	: ResourceHandle(dev, pool) {
}

DescriptorPool::~DescriptorPool() {
	if(vkHandle()) {
		vk::destroyDescriptorPool(device(), vkHandle());
	}
}

// DecriptorSetUpdate
DescriptorSetUpdate::DescriptorSetUpdate(const DescriptorSet& set)
	: set_(&set) {
}

DescriptorSetUpdate::~DescriptorSetUpdate() {
	apply();
}

void DescriptorSetUpdate::apply() {
	if((writes_.empty() && copies_.empty()) || !set_) {
		return;
	}

	vk::updateDescriptorSets(device(), writes_, copies_);
	writes_.clear();
	copies_.clear();
	buffers_.clear();
	images_.clear();
	views_.clear();
}

void apply(nytl::Span<const std::reference_wrapper<DescriptorSetUpdate>> updates)
{
	if(updates.empty()) {
		return;
	}

	std::vector<vk::WriteDescriptorSet> writes;
	std::vector<vk::CopyDescriptorSet> copies;

	for(auto& updateRef : updates) {
		auto& update = updateRef.get();

		writes.insert(writes.end(), update.writes_.begin(), update.writes_.end());
		copies.insert(copies.end(), update.copies_.begin(), update.copies_.end());

		update.writes_.clear();
		update.copies_.clear();
	}

	vk::updateDescriptorSets(updates[0].get().device(), writes, copies);

	for(auto& updateRef : updates) {
		auto& update = updateRef.get();

		update.buffers_.clear();
		update.images_.clear();
		update.views_.clear();
	}
}

void DescriptorSetUpdate::uniform(BufferInfos buffers, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) {
		if(b.range == 0) {
			b.range = vk::wholeSize;
		}
	}

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::uniformBuffer, nullptr, buffers_.back().data(),
		nullptr);
}

void DescriptorSetUpdate::storage(BufferInfos buffers, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) {
		if(b.range == 0) {
			b.range = vk::wholeSize;
		}
	}

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::storageBuffer, nullptr, buffers_.back().data(),
		nullptr);
}

void DescriptorSetUpdate::uniformDynamic(BufferInfos buffers, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) {
		if(b.range == 0) {
			b.range = vk::wholeSize;
		}
	}

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::uniformBufferDynamic, nullptr,
		buffers_.back().data(), nullptr);
}

void DescriptorSetUpdate::storageDynamic(BufferInfos buffers, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	buffers_.emplace_back(std::move(buffers));
	for(auto& b : buffers_.back()) {
		if(b.range == 0) {
			b.range = vk::wholeSize;
		}
	}

	writes_.emplace_back(*set_, binding, elem, buffers_.back().size(),
		vk::DescriptorType::storageBufferDynamic, nullptr,
		buffers_.back().data(), nullptr);
}

void DescriptorSetUpdate::sampler(ImageInfos images, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	images_.emplace_back(std::move(images));
	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::sampler, images_.back().data(), nullptr, nullptr);
}
void DescriptorSetUpdate::image(ImageInfos images, int binding,
			unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	images_.emplace_back(std::move(images));
	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::sampledImage, images_.back().data(), nullptr,
		nullptr);
}
void DescriptorSetUpdate::storage(ImageInfos images, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	images_.emplace_back(std::move(images));
	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::storageImage, images_.back().data(), nullptr,
		nullptr);
}
void DescriptorSetUpdate::imageSampler(ImageInfos images, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	images_.emplace_back(std::move(images));
	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::combinedImageSampler, images_.back().data(),
		nullptr, nullptr);
}
void DescriptorSetUpdate::inputAttachment(ImageInfos images, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	images_.emplace_back(std::move(images));
	writes_.emplace_back(*set_, binding, elem, images_.back().size(),
		vk::DescriptorType::inputAttachment, images_.back().data(), nullptr,
		nullptr);
}

void DescriptorSetUpdate::uniformView(BufferViewInfos views, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	views_.emplace_back(std::move(views));
	writes_.emplace_back(*set_, binding, elem, views_.back().size(),
		vk::DescriptorType::uniformTexelBuffer, nullptr, nullptr,
		views_.back().data());
}
void DescriptorSetUpdate::storageView(BufferViewInfos views, int binding,
		unsigned int elem) {

	if(binding == -1) {
		binding = currentBinding_++;
	} else {
		currentBinding_ = binding + 1;
	}

	views_.emplace_back(std::move(views));
	writes_.emplace_back(*set_, binding, elem, views_.back().size(),
		vk::DescriptorType::storageTexelBuffer, nullptr, nullptr,
		views_.back().data());
}
void DescriptorSetUpdate::copy(const vk::CopyDescriptorSet& copySet) {
	copies_.push_back(copySet);
}

// utility
vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
		vk::ShaderStageFlags stages, unsigned int binding,
		unsigned int count, const vk::Sampler* samplers) {
	return {binding, type, count, stages, samplers};
}

} // namespace vpp
