// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp>

namespace vpp {

/// Instance RAII wrapper.
class Instance : public nytl::NonCopyable {
public:
	Instance() = default;
	Instance(const vk::InstanceCreateInfo& info);
	Instance(vk::Instance instance);
	~Instance();

	Instance(Instance&& other) noexcept;
	Instance& operator=(Instance&& other) noexcept;

	vk::Instance vkInstance() const { return instance_; }
	vk::Instance vkHandle() const { return instance_; }
	operator vk::Instance() const { return instance_; }

protected:
	vk::Instance instance_ {};
};

} // namespace vpp
