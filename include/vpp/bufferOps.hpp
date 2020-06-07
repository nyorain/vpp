// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/buffer.hpp>
#include <vpp/sharedBuffer.hpp>
#include <vpp/util/span.hpp>
#include <cstring>

namespace vpp {

/// Fills the given buffer span by creating a temporary host visible
/// staging buffer, filling that and recording a copy command on the given
/// command buffer for copying the data to the given buffer span.
/// Returns the host visible staging SubBuffer, must not be destroyed
/// until the command buffer has finished execution.
[[nodiscard]] vpp::SubBuffer fillStaging(vk::CommandBuffer cb,
	const BufferSpan& dst, nytl::Span<const std::byte> data);

/// Simply uses a call to vkCmdUpdateBuffer, therefore uses no staging buffer.
/// The vulkan spec states that this is less efficient than a staging buffer
/// and only allowed for a data size up to 65536. Should be avoided for
/// large amounts of data, see fillStaging for that.
void fillDirect(vk::CommandBuffer cb, const BufferSpan& dst,
	nytl::Span<const std::byte> data);

template<typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
[[nodiscard]] vpp::SubBuffer fillStaging(vk::CommandBuffer cb,
		const BufferSpan& span, const T& obj) {
	auto ptr = reinterpret_cast<const std::byte*>(&obj);
	auto size = sizeof(obj);
	return fillStaging(cb, span, {ptr, ptr + size});
}

template<typename T, typename = std::enable_if_t<std::is_trivially_copyable_v<T>>>
void fillDirect(vk::CommandBuffer cb, const BufferSpan& span, const T& obj) {
	auto ptr = reinterpret_cast<const std::byte*>(&obj);
	auto size = sizeof(obj);
	fillDirect(cb, span, {ptr, ptr + size});
}

/// Creates a host visible staging buffer, records a command for copying data
/// from the given BufferSpan into the staging buffer and returns the
/// staging buffer. It must not be destroyed until the given command buffer
/// has finished execution. Then it can be memory mapped to read the data.
[[nodiscard]] vpp::SubBuffer readStaging(vk::CommandBuffer cb,
	const BufferSpan& src);

} // namespace vpp
