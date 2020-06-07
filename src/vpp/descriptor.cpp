// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/descriptor.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

namespace vpp {
namespace fwd {
	const vk::ShaderStageFlags allShaderStages = vk::ShaderStageBits::all;
} // namespace fwd

// DecriptorSetUpdate
DescriptorSetUpdate::DescriptorSetUpdate(const DescriptorSet& set)
		: set_(&set) {
	dlg_assert(set);
}

DescriptorSetUpdate::~DescriptorSetUpdate() {
	apply();
}

void DescriptorSetUpdate::apply() {
	if((writes_.empty() && copies_.empty()) || !set_) {
		return;
	}

	vk::updateDescriptorSets(device(), writes_, copies_);
	reset();
}

void DescriptorSetUpdate::reset() {
	writes_.clear();
	copies_.clear();
	buffers_.clear();
	images_.clear();
	views_.clear();
}

void apply(nytl::Span<DescriptorSetUpdate> updates) {
	if(updates.empty()) {
		return;
	}

	std::vector<vk::WriteDescriptorSet> writes;
	std::vector<vk::CopyDescriptorSet> copies;

	for(auto& update : updates) {
		writes.insert(writes.end(), update.writes().begin(), update.writes().end());
		copies.insert(copies.end(), update.copies().begin(), update.copies().end());
	}

	vk::updateDescriptorSets(updates[0].device(), writes, copies);
	for(auto& update : updates) {
		update.reset();
	}
}

void apply(nytl::Span<const std::reference_wrapper<DescriptorSetUpdate>> updates) {
	if(updates.empty()) {
		return;
	}

	std::vector<vk::WriteDescriptorSet> writes;
	std::vector<vk::CopyDescriptorSet> copies;

	for(auto& updateRef : updates) {
		auto& update = updateRef.get();

		writes.insert(writes.end(), update.writes().begin(), update.writes().end());
		copies.insert(copies.end(), update.copies().begin(), update.copies().end());
	}

	vk::updateDescriptorSets(updates[0].get().device(), writes, copies);
	for(auto& updateRef : updates) {
		auto& update = updateRef.get();
		update.reset();
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

void DescriptorSetUpdate::uniform(nytl::Span<const BufferSpan> bufs, int binding,
		unsigned elem) {
	std::vector<vk::DescriptorBufferInfo> infos;
	infos.reserve(bufs.size());
	for(const auto& b : bufs) {
		vk::DescriptorBufferInfo info;
		info.buffer = b.buffer();
		info.offset = b.offset();
		info.range = b.size();
		infos.push_back(info);
	}

	uniform(std::move(infos), binding, elem);
}
void DescriptorSetUpdate::storage(nytl::Span<const BufferSpan> bufs, int binding,
		unsigned elem) {
	std::vector<vk::DescriptorBufferInfo> infos;
	infos.reserve(bufs.size());
	for(const auto& b : bufs) {
		vk::DescriptorBufferInfo info;
		info.buffer = b.buffer();
		info.offset = b.offset();
		info.range = b.size();
		infos.push_back(info);
	}

	storage(std::move(infos), binding, elem);
}
void DescriptorSetUpdate::uniformDynamic(nytl::Span<const BufferSpan> bufs,
		int binding, unsigned elem) {
	std::vector<vk::DescriptorBufferInfo> infos;
	infos.reserve(bufs.size());
	for(const auto& b : bufs) {
		vk::DescriptorBufferInfo info;
		info.buffer = b.buffer();
		info.offset = b.offset();
		info.range = b.size();
		infos.push_back(info);
	}

	uniformDynamic(std::move(infos), binding, elem);
}
void DescriptorSetUpdate::storageDynamic(nytl::Span<const BufferSpan> bufs,
		int binding, unsigned elem) {
	std::vector<vk::DescriptorBufferInfo> infos;
	infos.reserve(bufs.size());
	for(const auto& b : bufs) {
		vk::DescriptorBufferInfo info;
		info.buffer = b.buffer();
		info.offset = b.offset();
		info.range = b.size();
		infos.push_back(info);
	}

	storageDynamic(std::move(infos), binding, elem);
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
		vk::ShaderStageFlags stages, std::uint32_t binding,
		std::uint32_t count, const vk::Sampler* samplers) {
	return {binding, type, count, stages, samplers};
}

vk::DescriptorSetLayoutBinding descriptorBinding(vk::DescriptorType type,
		vk::ShaderStageFlags stages, const vk::Sampler* samplers,
		std::uint32_t count, std::uint32_t binding) {
	return {binding, type, count, stages, samplers};
}

} // namespace vpp
