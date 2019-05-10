// Copyright (c) 2016-2019 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/resource.hpp>
#include <vpp/device.hpp>

namespace vpp {

#ifdef VPP_ONE_DEVICE_OPTIMIZATION

void Resource::init(const Device& dev) {
	if(Device::instance() == &dev) {
		return;
	}

	auto msg = "vpp::Resource: invalid device instance (compiled with"
		"VPP_ONE_DEVICE_OPTIMIZATION)"
	throw std::logic_error(msg);
}

#else // VPP_ONE_DEVICE_OPTIMIZATION

Resource::Resource(Resource&& other) noexcept {
	device_ = other.device_;
	other.device_ = {};
}

Resource& Resource::operator=(Resource&& other) noexcept {
	device_ = other.device_;
	other.device_ = {};
	return *this;
}

#endif // VPP_ONE_DEVICE_OPTIMIZATION

} // namespace vpp
