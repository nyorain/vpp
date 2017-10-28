// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/work.hpp>
#include <vpp/sharedBuffer.hpp>

// These classes are used internally (buffer and image fill/retrieve) but might also be useful
// for custom upload/download work implementations

namespace vpp {

/// Utility template base class for all transfer work implementations using 
/// a shared BufferRange for uploading/downloading.
template<typename T>
class TransferWork : public CommandWork<T> {
public:
	TransferWork(CommandBuffer&& cmdBuf, QueueSubmitter& submitter,
		BufferRange&& range) : 
			CommandWork<T>(submitter, std::move(cmdBuf)), 
			bufferRange(std::move(range)) {}
	~TransferWork() {
		if(this->submitter_) {
			tryFinish(*this, "~TransferWork");
		}
	}

	TransferWork(TransferWork&&) noexcept = default;
	TransferWork& operator=(TransferWork&&) noexcept = default;

	BufferRange bufferRange;
};

/// Download work implementation.
/// Can be used to download data from a bufferor image using a
/// TransferManager.
class DownloadWork : public TransferWork<nytl::Span<const std::byte>> {
public:
	using TransferWork::TransferWork;

	DownloadWork(DownloadWork&&) noexcept = default;
	DownloadWork& operator=(DownloadWork&&) noexcept = default;

	nytl::Span<const std::byte> data() override {
		finish();
		if(!stagingMap.valid()) {
			stagingMap = bufferRange.memoryMap();
		}

		return {stagingMap.ptr(), stagingMap.size()};
	}

	MemoryMapView stagingMap;
};

/// Upload work implementation.
/// Does not implement any further functionality since it only needs the passed
/// command buffer and upload range.
class UploadWork : public TransferWork<void> {
public:
	using TransferWork::TransferWork;

	UploadWork(UploadWork&&) noexcept = default;
	UploadWork& operator=(UploadWork&&) noexcept = default;
};

} // namespace vpp
