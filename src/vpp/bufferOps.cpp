// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/bufferOps.hpp>
#include <vpp/queue.hpp>
#include <vpp/vk.hpp>
#include <dlg/dlg.hpp>
#include <cstring>

namespace vpp {

vpp::SubBuffer fillStaging(vk::CommandBuffer cb, const BufferSpan& span,
		nytl::Span<const std::byte> data) {
	dlg_assert(vk::DeviceSize(data.size()) <= span.size());
	auto& dev = span.device();
	vpp::SubBuffer stage = {dev.bufferAllocator(), vk::DeviceSize(data.size()),
		vk::BufferUsageBits::transferSrc, dev.hostMemoryTypes()};
	auto map = stage.memoryMap(0, data.size());
	std::memcpy(map.ptr(), data.data(), data.size());

	vk::BufferCopy copy;
	copy.dstOffset = span.offset();
	copy.srcOffset = stage.offset();
	copy.size = data.size();
	vk::cmdCopyBuffer(cb, stage.buffer(), span.buffer(), {{copy}});

	return stage;
}

void fillDirect(vk::CommandBuffer cb, const BufferSpan& span,
		nytl::Span<const std::byte> data) {
	dlg_assert(vk::DeviceSize(data.size()) <= span.size());
	vk::cmdUpdateBuffer(cb, span.buffer(), span.offset(), data.size(),
		data.data());
}

vpp::SubBuffer readStaging(vk::CommandBuffer cb, const BufferSpan& src) {
	auto& dev = src.device();
	vpp::SubBuffer stage = {dev.bufferAllocator(), src.size(),
		vk::BufferUsageBits::transferSrc, dev.hostMemoryTypes()};

	vk::BufferCopy copy;
	copy.dstOffset = stage.offset();
	copy.srcOffset = src.offset();
	copy.size = src.size();
	vk::cmdCopyBuffer(cb, src.buffer(), stage.buffer(), {{copy}});

	return stage;
}

} // namespace vpp
