// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/work.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/bufferOps.hpp>
#include <vpp/memoryResource.hpp>

// These classes are used internally (buffer and image fill/retrieve) but might also be useful
// for custom upload/download work implementations

namespace vpp {

/// Utility template base class for all transfer work implementations using 
/// a shared BufferRange for uploading/downloading.
template<typename T>
class TransferWork : public CommandWork<T> {
public:
	TransferWork(CommandBuffer&& cmdBuf, const vpp::Queue& queue, 
		BufferRange&& range) : CommandWork<T>(std::move(cmdBuf), queue), 
			bufferRange_(std::move(range)) {}

	BufferRange& bufferRange() { return bufferRange_; }
	const BufferRange& bufferRange() const { return bufferRange_; }

protected:
	BufferRange bufferRange_;
};

/// Download work implementation.
/// Can be used to download data from a bufferor image using a
/// TransferManager.
class DownloadWork : public TransferWork<nytl::Span<const uint8_t>> {
public:
	using TransferWork::TransferWork;
	nytl::Span<const uint8_t> data() override {
		finish();
		downloadWork_ = retrieve(bufferRange_.buffer(), 
			bufferRange_.offset(), bufferRange_.size());
		auto data = downloadWork_->data();
		return data;
	}

protected:
	DataWorkPtr downloadWork_;
};

/// Download work implementation for mappable memory resources.
/// Returns the data span directly from the mapped memory range.
class MappableDownloadWork : public FinishedWork<nytl::Span<const uint8_t>> {
public:
	MappableDownloadWork(MemoryMapView&& view) : map_(std::move(view)) {}
	nytl::Span<const uint8_t> data() override { 
		return {map_.ptr(), map_.size()}; 
	}

protected:
	MemoryMapView map_;
};

/// Download work implementation for already stored data.
/// Simply stores retrieved data and returns it in the data function implementation.
class StoredDataWork : public FinishedWork<nytl::Span<const std::uint8_t>> {
public:
	StoredDataWork(std::vector<uint8_t>&& data) : data_(std::move(data)) {}
	nytl::Span<const std::uint8_t> data() override { return data_; }

protected:
	std::vector<std::uint8_t> data_;
};

/// Upload work implementation.
/// Does not implement any further functionality since it only needs the passed
/// command buffer and upload range.
class UploadWork : public TransferWork<void> {
public:
	using TransferWork::TransferWork;
};

} // namespace vpp
