// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/resource.hpp>

namespace vpp {

#ifndef VPP_ONE_DEVICE_OPTIMIZATION

Resource::Resource(Resource&& other) noexcept
{
	device_ = other.device_;
	other.device_ = {};
}

Resource& Resource::operator=(Resource&& other) noexcept
{
	device_ = other.device_;
	other.device_ = {};
	return *this;
}

#else // VPP_ONE_DEVICE_OPTIMIZATION

const Device* Resource::deviceInstance_ {};
void Resource::init(const Device& dev)
{
	if(deviceInstance_ == &dev) return; // most likely

	if(!deviceInstance_) deviceInstance_ = &dev;
	else throw std::logic_error("vpp::Resource: VPP_ONE_DEVICE_OPTIMIZATION invalid device");
}

#endif // VPP_ONE_DEVICE_OPTIMIZATION

} // namespace vpp
