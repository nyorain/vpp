// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/instance.hpp>
#include <vpp/vk.hpp>

namespace vpp {

Instance::Instance(const vk::InstanceCreateInfo& info)
{
	instance_ = vk::createInstance(info);
}

Instance::Instance(vk::Instance instance) : instance_(instance)
{
}

Instance::~Instance()
{
	if(instance_) vk::destroyInstance(instance_);
}

Instance::Instance(Instance&& lhs) noexcept : instance_(lhs.instance_)
{
	lhs.instance_ = {};
}

Instance& Instance::operator=(Instance&& lhs) noexcept
{
	if(instance_) vk::destroyInstance(instance_);
	instance_ = lhs.instance_;
	lhs.instance_ = {};
	return *this;
}

} // namespace vpp
