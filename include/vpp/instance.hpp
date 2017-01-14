// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/util/nonCopyable.hpp>

// TODO: make this class movable (it is not referenced by any other class)

namespace vpp {

/// Instance RAII wrapper.
class Instance : public nytl::NonMovable {
public:
	Instance(const vk::InstanceCreateInfo& info);
	Instance(vk::Instance instance);
	~Instance();

	vk::Instance vkInstance() const { return instance_; }
	operator vk::Instance() const { return instance_; }

protected:
	vk::Instance instance_ {};
};

} // namespace vpp
