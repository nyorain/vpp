// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp> // vpp::Device
#include <utility> // std::swap

/// Resource definition.
/// When VPP_ONE_DEVICE_OPTIMIZATION is defined, the Resource class stores
/// no device member but uses Device::instance instead. That means that
/// resources don't have to store the extra word for the device but also
/// that applications can only use one logcial vulkan device at a time (which
/// almost all do anyways).
/// Both implementations provide the same public and protected interface.
#ifdef VPP_ONE_DEVICE_OPTIMIZATION
	#include <vpp/resourceOneDev.hpp> // vpp::Resource
#else
	#include <vpp/resourceDev.hpp> // vpp::Resource
#endif

namespace vpp {

/// Utility template base class that makes RAII wrappers easier.
/// Note that move constructor and assignment operator can be defined using the
/// swap member function if there are no additional data members. They cannot be
/// implemented here instead of '= delete' and then simply defaulted in derived
/// classes since the destructor of the derived classes needs to trigger in the
/// case of operator=(ResourceHandle&&).
template<typename Handle>
class ResourceHandle : public Resource {
public:
	const Handle& vkHandle() const noexcept { return handle_; }
	operator const Handle&() const noexcept { return vkHandle(); }

	Handle release() {
		auto ret = handle_;
		handle_ = {};
		return ret;
	}

protected:
	ResourceHandle() = default;
	ResourceHandle(const Device& dev, const Handle& h = {}) :
		Resource(dev), handle_(h) {}
	~ResourceHandle() = default;

	ResourceHandle(ResourceHandle&& other) noexcept = delete;
	ResourceHandle& operator=(ResourceHandle&& other) noexcept = delete;

	friend void swap(ResourceHandle<Handle>& a,
			ResourceHandle<Handle>& b) noexcept {
		using std::swap;
		swap(static_cast<Resource&>(a), static_cast<Resource&>(b));
		swap(a.handle_, b.handle_);
	}

protected:
	Handle handle_ {};
};

/// Can be used for non owned resources. Does not store any additional data.
/// Derives from the given type and simply calls its protected release function
/// (which should release the owned resource) before calling its destructor
/// which then cannot destroy the not owned resource.
template <typename T>
class NonOwned : public T {
public:
	using T::T;
	~NonOwned() { T::release(); }

	NonOwned(NonOwned&& other) = default;
	NonOwned& operator=(NonOwned&& other) {
		T::release();
		T::operator=(std::move(other));
		return *this;
	}
};

} // namespace vpp
