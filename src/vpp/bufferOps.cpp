// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/bufferOps.hpp>
#include <vpp/transferWork.hpp>
#include <vpp/queue.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>

#include <cstring> // std::memset
#include <utility> // std::move
#include <memory> // std::make_unique

namespace vpp {

BufferSpan::BufferSpan(const SubBuffer& b) : allocation_(b.allocation()) {
	if(size()) {
		buffer_ = &b.buffer();
	}
}

BufferSpan::BufferSpan(const Buffer& b, vk::DeviceSize size,
		vk::DeviceSize offset) : buffer_(&b), allocation_{offset, size} {
	dlg_assert(size == 0 || b);
}

MemoryMapView BufferSpan::memoryMap() const {
	return buffer().memoryMap(offset(), size());
}

// MappedBufferWriter
MappedBufferWriter::MappedBufferWriter(MemoryMapView&& view,
	BufferLayout layout, bool tight, vk::DeviceSize srcOffset) :
		BufferOperator(layout), view_(std::move(view)), srcOffset_(srcOffset),
		tight_(tight) {
	dlg_assert(view_.valid());
	regions_.push_back({srcOffset_, 0u, 0u});
}

void MappedBufferWriter::operate(const void* ptr, vk::DeviceSize size) {
	dlg_assertm(viewOffset_ + size <= view_.size(), "Buffer overflow");
	dlg_assertm(size > 0, "Invalid operation");

	offset_ = std::max(offset_, nextOffset_);
	std::memcpy(view_.ptr() + viewOffset_, ptr, size);
	viewOffset_ += size;
	offset_ += size;
	regions_.back().size += size;
}

void MappedBufferWriter::offset(vk::DeviceSize size, bool update) {
	if(size == 0) {
		return;
	}

	offset_ += size;
	if(update) {
		std::memset(view_.ptr() + viewOffset_, 0, size);
		viewOffset_ += size;
		regions_.back().size += size;
	} else {
		if(!tight_) {
			viewOffset_ += size;
		}

		if(regions_.back().size == 0u) {
			regions_.back().srcOffset = srcOffset_ + viewOffset_;
			regions_.back().dstOffset = offset_;
		} else {
			regions_.push_back({srcOffset_ + viewOffset_, offset_, 0u});
		}
	}

	dlg_assert(viewOffset_ <= view_.size());
}
void MappedBufferWriter::alignUniform() noexcept {
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}
void MappedBufferWriter::alignStorage() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}
void MappedBufferWriter::alignTexel() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

// DirectBufferWriter
DirectBufferWriter::DirectBufferWriter(const BufferSpan& span,
		BufferLayout layout) : BufferOperator(layout), span_(span) {
	dlg_assert(span_.valid());
	offset_ = span.offset();
	copies_.push_back({0u, span.offset(), 0u});
}

void DirectBufferWriter::operate(const void* ptr, vk::DeviceSize size) {
	dlg_assertm(size > 0, "Invalid operation");

	offset_ = std::max(offset_, nextOffset_);
	auto prev = data_.size();
	data_.resize(data_.size() + size);
	copies_.back().size += size;
	std::memcpy(&data_[prev], ptr, size);
	offset_ += size;

	dlg_assertm(offset_ <= span_.end(), "Buffer overflow");
}

void DirectBufferWriter::offset(vk::DeviceSize size, bool update) {
	offset_ += size;
	if(!update) {
		if(!copies_.empty() && copies_.back().size == 0u) {
			copies_.back().dstOffset = offset_;
		} else {
			copies_.push_back({data_.size(), offset_, 0u});
		}
	} else {
		auto prev = data_.size();
		data_.resize(data_.size() + size);
		copies_.back().size += size;
		std::memset(&data_[prev], 0, size);
	}

	dlg_assertm(offset_ < span_.end(), "Buffer overflow");
}

void DirectBufferWriter::alignUniform() noexcept {
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}
void DirectBufferWriter::alignStorage() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}
void DirectBufferWriter::alignTexel() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

// BufferReader
BufferReader::BufferReader(const Device& dev, BufferLayout layout,
	nytl::Span<const std::byte> data) :
		BufferOperator(layout), Resource(dev), data_(data) {
}

void BufferReader::operate(void* ptr, vk::DeviceSize size) {
	offset_ = std::max(offset_, nextOffset_);
	dlg_assertm(size > 0, "Invalid operation");
	dlg_assertm(offset_ + size <= data_.size(), "Buffer underflow");

	std::memcpy(ptr, &data_[offset_], size);
	offset_ += size;
}

void BufferReader::alignUniform() noexcept {
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}
void BufferReader::alignStorage() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}
void BufferReader::alignTexel() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

// BufferSizer
BufferSizer::BufferSizer(const Device& dev, BufferLayout align) :
	BufferOperator(align), Resource(dev) {
}

void BufferSizer::alignUniform() noexcept {
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}
void BufferSizer::alignStorage() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}
void BufferSizer::alignTexel() noexcept {
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

namespace detail {

void apply(vk::CommandBuffer cmdBuf, const BufferSpan& dst,
		SubBuffer&& stage, nytl::Span<const vk::BufferCopy> copies) {

	dlg_check({
		for(auto& copy : copies) {
			dlg_assert(copy.srcOffset >= stage.offset());
			dlg_assert(copy.srcOffset + copy.size <= stage.end());
			dlg_assert(copy.dstOffset >= dst.offset());
			dlg_assert(copy.dstOffset + copy.size <= dst.end());
		}
	});

	vk::cmdCopyBuffer(cmdBuf, stage.buffer(), dst.buffer(), copies);
}

void apply(vk::CommandBuffer cmdBuf, const BufferSpan& dst,
		const DirectBufferWriter& writer) {

	for(auto& copy : writer.copies()) {
		dlg_assert(copy.srcOffset + copy.size <= writer.data().size());
		dlg_assert(copy.dstOffset >= dst.offset());
		dlg_assert(copy.dstOffset + copy.size <= dst.end());

		vk::cmdUpdateBuffer(cmdBuf, dst.buffer(), copy.dstOffset, copy.size,
			&writer.data()[copy.srcOffset]);
	}
}

CommandBuffer copyCmdBuf(QueueSubmitter& qs, const BufferSpan& dst,
		const BufferSpan& stage, vk::DeviceSize size) {

	dlg_assert(dst.valid() && stage.valid());
	dlg_assert(dst.size() >= size);
	dlg_assert(stage.size() >= size);

	auto& dev = qs.device();
	auto cmdBuf = dev.commandAllocator().get(qs.queue().family());
	vk::beginCommandBuffer(cmdBuf, {});
	vk::cmdCopyBuffer(cmdBuf, dst.buffer(), stage.buffer(), {{dst.offset(),
		stage.offset(), size}});
	vk::endCommandBuffer(cmdBuf);
	return cmdBuf;
}

void beginCommandBuffer(vk::CommandBuffer cmdb) {
	vk::beginCommandBuffer(cmdb, {});
}

void endCommandBuffer(vk::CommandBuffer cmdb) {
	vk::endCommandBuffer(cmdb);
}

} // namespace detail
} // namespace vpp
