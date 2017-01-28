// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/work.hpp>
#include <vpp/transfer.hpp>
#include <vpp/bufferOps.hpp>
#include <vpp/memoryResource.hpp>

// These classes are used internally (buffer and image fill/retrieve) but might also be useful
// for custom upload/download work implementations

namespace vpp {

/// Utility template base class for all transfer work implementations using a TransferRange.
template<typename T>
class TransferWork : public CommandWork<T> {
public:
	TransferWork(CommandBuffer&& cmdBuf, const vpp::Queue& queue, TransferRange&& range)
		: CommandWork<T>(std::move(cmdBuf), queue), transferRange_(std::move(range)) {}

	TransferRange& transferRange() { return transferRange_; }
	const TransferRange& transferRange() const { return transferRange_; }

protected:
	TransferRange transferRange_;
};

/// Download work implementation.
/// Can be used to download data from a bufferor image using a
/// TransferManager.
class DownloadWork : public TransferWork<nytl::Span<const uint8_t>> {
public:
	using TransferWork::TransferWork;
	virtual nytl::Span<const uint8_t> data() override
	{
		finish();
		downloadWork_ = retrieve(transferRange_.buffer());
		return downloadWork_->data();
	}

protected:
	DataWorkPtr downloadWork_;
};

/// Download work implementation for mappable memory resources.
/// Returns the data span directly from the mapped memory range.
class MappableDownloadWork : public FinishedWork<nytl::Span<const uint8_t>> {
public:
	MappableDownloadWork(MemoryMapView&& view) : map_(std::move(view)) {}

	virtual nytl::Span<const uint8_t> data() override { return {map_.ptr(), map_.size()}; }
	MemoryMapView& memoryMapView() { return map_; }

protected:
	MemoryMapView map_;
};

/// Download work implementation for already stored data.
/// Simply stores retrieved data and returns it in the data function implementation.
class StoredDataWork : public FinishedWork<nytl::Span<const std::uint8_t>> {
public:
	StoredDataWork(std::vector<uint8_t>&& data) : data_(std::move(data)) {}
	virtual nytl::Span<const std::uint8_t> data() override { return data_; }

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
