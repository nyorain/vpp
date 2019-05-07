// Copyright (c) 2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>

namespace vpp {

class QueryPool : public ResourceHandle<vk::QueryPool> {
public:
	QueryPool() = default;
	QueryPool(const Device&, const vk::QueryPoolCreateInfo&);
	QueryPool(const Device&, vk::QueryPool);
	~QueryPool();

	QueryPool(QueryPool&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(QueryPool rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

} // namespace vpp
