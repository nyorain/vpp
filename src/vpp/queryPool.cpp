// Copyright (c) 2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/queryPool.hpp>
#include <vpp/vk.hpp>

namespace vpp {

QueryPool::QueryPool(const Device& dev, const vk::QueryPoolCreateInfo& info) :
	ResourceHandle(dev, vk::createQueryPool(dev, info))
{
}

QueryPool::QueryPool(const Device& dev, vk::QueryPool pool) :
	ResourceHandle(dev, pool)
{
}

QueryPool::~QueryPool() {
	if(vkHandle()) {
		vk::destroyQueryPool(device(), vkHandle());
	}
}

} // namespace vpp
