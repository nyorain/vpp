// Copyright (c) 2017 nyorain
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

DataWorkPtr retrieve(const Buffer& buf, vk::DeviceSize offset, vk::DeviceSize size)
{
	dlg_assertm(buf.memoryEntry().allocated(), "receiving unbound buffer");

	if(size == vk::wholeSize) {
		size = buf.memoryEntry().size() - offset;
	}

	// retrieve by mapping if possible
	if(buf.mappable()) {
		return std::make_unique<MappableDownloadWork>(buf.memoryMap(offset, size));
	} else {
		//use transfer buffer
		const Queue* queue;
		auto qFam = transferQueueFamily(buf.device(), &queue);
		auto cmdBuffer = buf.device().commandProvider().get(qFam);
		auto memBits = buf.device().memoryTypeBits(
			vk::MemoryPropertyBits::hostVisible);
		auto downloadBuffer = buf.device().bufferAllocator().alloc(size,
			vk::BufferUsageBits::transferDst, memBits);

		vk::BufferCopy region {offset, 0, size};

		vk::beginCommandBuffer(cmdBuffer, {});
		vk::cmdCopyBuffer(cmdBuffer, buf, downloadBuffer.buffer(), {region});
		vk::endCommandBuffer(cmdBuffer);

		return std::make_unique<DownloadWork>(std::move(cmdBuffer), *queue,
			std::move(downloadBuffer));
	}
}

WorkPtr write(const Buffer& buf, nytl::Span<const uint8_t> data)
{
	BufferUpdate update(buf, BufferLayout::std140); // the layout does not matter in this case
	update.addSingle(data);
	return update.apply();
}

// BufferUpdate
BufferUpdate::BufferUpdate(const Buffer& buf, BufferLayout align, bool direct)
	: BufferOperator(align), buffer_(&buf), direct_(direct)
{
	buf.ensureMemory();
	if(buf.mappable() && !direct) {
		map_ = buf.memoryMap();
		work_ = std::make_unique<FinishedWork<void>>();
	} else {
		const Queue* queue;
		auto qFam = transferQueueFamily(device(), &queue);
		if(qFam == -1) throw std::logic_error("vpp::BufferUpdate: device has no valid queue");
		auto cmdBuffer = device().commandProvider().get(qFam);
		copies_.push_back({0, 0, 0});

		if(direct) {
			data_.resize(buf.memorySize());
			work_ = std::make_unique<CommandWork<void>>(std::move(cmdBuffer), *queue);
		} else {
			// TODO: maybe don't allocate this now? maybe only a small part
			// of the buffer is updated...
			auto memBits = buf.device().memoryTypeBits(
				vk::MemoryPropertyBits::hostVisible);
			auto uploadBuffer = device().bufferAllocator().alloc(
				buf.memorySize(), vk::BufferUsageBits::transferSrc, memBits);
			map_ = uploadBuffer.buffer().memoryMap();
			work_ = std::make_unique<UploadWork>(std::move(cmdBuffer), *queue,
				std::move(uploadBuffer));
		}
	}
}

BufferUpdate::~BufferUpdate()
{
	if(work_) {
		try {
			apply()->finish();
		} catch(const std::exception& error) {
			dlg_warnt(("~BufferUpdate"), "apply()->finish(): {}", error.what());
		}
	}
}

void BufferUpdate::offset(size_t size, bool update)
{
	if(size == 0) return;

	offset_ += size;
	if(update) {
		std::memset(&data(), 0, size);
		internalOffset_ += size;
		if(!buffer().mappable()) copies_.back().size += size;
	} else if(!buffer().mappable()) {
		if(!copies_.back().size) {
			copies_.back().srcOffset = internalOffset_;
			copies_.back().dstOffset = offset_;
		} else {
			copies_.push_back({internalOffset_, offset_, 0});
		}
	}

	checkCopies();
}

void BufferUpdate::align(size_t align, bool update)
{
	if(align == 0 || !(offset_ % align)) return;
	auto delta = (std::ceil(offset_ / double(align)) * align) - offset_;
	offset(delta, update);
}

void BufferUpdate::operate(const void* ptr, std::size_t size)
{
	dlg_checkt(("BufferUpdate::operate"), {
		if(!ptr) dlg_error("BufferUpdate: invalid data ptr given to operate");
	});

	std::memcpy(&data(), ptr, size);
	offset_ = std::max(offset_, nextOffset_) + size;
	internalOffset_ += size;
	if(!buffer().mappable()) copies_.back().size += size;
	checkCopies();
}

void BufferUpdate::checkCopies()
{
	dlg_checkt(("BufferUpdate::checkCopies"), {
		if(offset_ > buffer().memorySize()) dlg_error("BufferUpdate write overflow");
	});

	while(direct_ && copies_.back().size > 65536) {
		auto delta = copies_.back().size - 65536;
		copies_.back().size = 65536;
		copies_.push_back({internalOffset_ - delta, offset_ - delta, delta});
	}
}

std::uint8_t& BufferUpdate::data()
{
	if(!direct_ && buffer().mappable()) return *(map_.ptr() + offset_);
	else if(!direct_) return *(map_.ptr() + internalOffset_);
	else return data_[internalOffset_];
}

WorkPtr BufferUpdate::apply()
{
	dlg_checkt(("BufferUpdate::apply"), {
		if(!work_) dlg_error("BufferUpdate work is null, apply was already called");
		if(offset_ == 0) dlg_warn("BufferUpdate offset is 0, no update data");
	});

	if(!direct_ && !map_.coherent()) map_.flush();

	if(direct_) {
		auto commandWork = static_cast<CommandWork<void>*>(work_.get());
		auto& cmdBuf = commandWork->commandBuffer();

		vk::beginCommandBuffer(cmdBuf, {});
		for(auto& upd : copies_) {
			auto* data = static_cast<void*>(&data_[upd.srcOffset]);
			vk::cmdUpdateBuffer(cmdBuf, buffer(), upd.dstOffset, upd.size, data);
		}

		vk::endCommandBuffer(cmdBuf);
	} else if(!buffer().mappable()) {
		auto uploadWork = static_cast<UploadWork*>(work_.get());
		auto& cmdBuf = uploadWork->commandBuffer();
		auto& transferRange = uploadWork->bufferRange();

		vk::beginCommandBuffer(cmdBuf, {});
		for(auto& update : copies_) {
			vk::cmdCopyBuffer(cmdBuf, transferRange.buffer(), buffer(), {update});
		}
		vk::endCommandBuffer(cmdBuf);
	}

	data_ = {};
	map_ = {};
	copies_ = {};

	return std::move(work_);
}

void BufferUpdate::alignUniform() noexcept
{
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}

void BufferUpdate::alignStorage() noexcept
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

void BufferUpdate::alignTexel() noexcept
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

// BufferSizer
void BufferSizer::alignUniform() noexcept
{
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}

void BufferSizer::alignStorage() noexcept
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

void BufferSizer::alignTexel() noexcept
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

// BufferReader
BufferReader::BufferReader(const Device& dev, BufferLayout align,
	nytl::Span<const uint8_t> data) : BufferOperator(align), Resource(dev), data_(data)
{
}

void BufferReader::operate(void* ptr, std::size_t size)
{
	offset_ = std::max(offset_, nextOffset_);
	dlg_checkt(("BufferReader::operate"), {
		if(offset_ > data_.size())
			dlg_error("BufferUpdate read overflow");
	});

	std::memcpy(ptr, &data_[offset_], size);
	offset_ += size;
}

void BufferReader::alignUniform() noexcept
{
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}

void BufferReader::alignStorage() noexcept
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

void BufferReader::alignTexel() noexcept
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

} // namespace vpp
