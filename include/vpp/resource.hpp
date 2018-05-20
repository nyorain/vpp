// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp> // vpp::Device
#include <utility> // std::swap

namespace vpp {

/// The Resource class represents a vulkan resource associated to a vulkan device.
/// Almost all resources may be in an uninitialized state, since they are usually
/// movable. For the Resource class uninitialized means that they have no associated device.
/// The Resource class will not perform any checks whether it are initialized
/// in its member function (the destructor excluded), so calling member functions of
/// uninitialized (default constructed) resources may trigger undefined behaviour (crash).
/// There are two implementations of the Resource class depending on whether the
/// VPP_ONE_DEVICE_OPTIMIZATION macro is defined. Since many applications will never have more than
/// one vpp::Device instance, it is enough to store a single Device pointer for all resources
/// instead of every resource keeping a device pointer.
/// Both implementations have the same functions and derived classes or users of the
/// Resource class shall not depend on any further (implementation defined) functionality.
/// The main reason resources hold a reference to their associated device is that it is
/// needed for resource destruction. But since there only the vulkan device handle is needed,
/// Resource always hold a const device reference.
/// \sa ResourceReference
/// \sa Device
#ifndef VPP_ONE_DEVICE_OPTIMIZATION

class Resource {
public:
	Resource(const Resource& other) noexcept = default;
	~Resource() noexcept = default;

	Resource(Resource&& other) noexcept;
	Resource& operator=(Resource&& other) noexcept;

	const Device& device() const noexcept { return *device_; }
	vk::Instance vkInstance() const noexcept { return device().vkInstance(); }
	vk::Device vkDevice() const noexcept { return device().vkDevice(); }
	vk::PhysicalDevice vkPhysicalDevice() const noexcept { return device().vkPhysicalDevice(); }

protected:
	Resource() noexcept = default;
	Resource(const Device& device) noexcept : device_(&device) {}

	void init(const Device& dev) noexcept { device_ = &dev; }
	friend void swap(Resource& a, Resource& b) noexcept { std::swap(a.device_, b.device_); }

private:
	const Device* device_ {}; // should not be accessed directly by members. Use device()
};

#else // VPP_ONE_DEVICE_OPTIMIZATION

class Resource {
public:
	Resource(const Resource& other) noexcept = default;
	~Resource() = default;

	Resource(Resource&& other) noexcept = default;
	Resource& operator=(Resource&& other) noexcept = default;

	const Device& device() const noexcept { return *deviceInstance_; }
	vk::Instance vkInstance() const noexcept { return device().vkInstance(); }
	vk::Device vkDevice() const noexcept { return device().vkDevice(); }
	vk::PhysicalDevice vkPhysicalDevice() const noexcept { return device().vkPhysicalDevice(); }

protected:
	Resource() = default;
	Resource(const Device& dev) { init(dev); }

	void init(const Device& dev);
	friend void swap(Resource& a, Resource& b) noexcept {}

private:
	static const Device* deviceInstance_;
};

#endif // VPP_ONE_DEVICE_OPTIMIZATION

/// Can be used for non owned resources. Does not store any additional data.
/// Derives from the given type and simply calls its protected release function (which should
/// release the owned resource) before calling its destructor which then cannot destroy the
/// not owned resource.
/// If you want to enable your custom classes for this template simply implement a release memeber
/// function which is visible to NonOwned.
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

/// Resource class that already holds another resource and does therefore not have to hold a second
/// vulkan device reference.
/// Classes inheriting from this template have to pass themself as first template paramater (CRTP) and
/// implement a <resourceRef() const> member function which returns a (const) reference to a Resource
/// or another ResourceReference.
/// This class exists just as an optimization (1 word less memory needed) for Resource classes.
/// For an example look e.g. at the CommandBuffer class, which naturally holds a reference to the
/// CommandPool it is allocated from and therefore does not have to hold a additional reference to
/// its vulkan devcie, since this information can be retrieved from the CommandPool reference (done
/// by this template class).
/// \sa Resource
/// \sa Device
template <typename B>
class ResourceReference {
public:
	const Device& device() const noexcept
		{ return static_cast<const B&>(*this).resourceRef().device(); }

	vk::Instance vkInstance() const noexcept { return device().vkInstance(); }
	vk::PhysicalDevice vkPhysicalDevice() const noexcept { return device().vkPhysicalDevice(); }
	vk::Device vkDevice() const noexcept { return device().vkDevice(); }
};

/// Utility template base class that makes RAII wrappers easier.
/// Note that move constructor and assignment operator can be defined using the swap
/// member function if there are no additional data members. They cannot
/// be implemented here instead of '= delete' and then simply defaulted
/// in derived classes since the destructor of the derived classes needs to
/// trigger in the case of operator=(Handle&&).
/// \tparam H The vulkan handle type.
template<typename Handle>
class ResourceHandle : public Resource, public nytl::NonCopyable {
public:
	const Handle& vkHandle() const noexcept { return handle_; }
	operator const Handle&() const noexcept { return vkHandle(); }

protected:
	ResourceHandle() = default;
	ResourceHandle(const Device& dev, const Handle& hndl = {}) : Resource(dev), handle_(hndl) {}
	~ResourceHandle() = default;

	ResourceHandle(ResourceHandle&& other) noexcept = delete;
	ResourceHandle& operator=(ResourceHandle&& other) noexcept = delete;

	void release() { handle_ = {}; }
	friend void swap(ResourceHandle<Handle>& a, ResourceHandle<Handle>& b) noexcept
	{
		using std::swap;
		swap(static_cast<Resource&>(a), static_cast<Resource&>(b));
		swap(a.handle_, b.handle_);
	}

protected:
	Handle handle_ {};
};

/// ResourceHandle base for classes that already hold a Device reference in some way.
/// \tparam B The deriving class, needed for the ResourceReference base
/// \tparam H The vulkan handle type.
template<typename B, typename Handle>
class ResourceReferenceHandle : public ResourceReference<B>, public nytl::NonCopyable {
public:
	const Handle& vkHandle() const noexcept { return handle_; }
	operator const Handle&() const noexcept { return vkHandle(); }

protected:
	ResourceReferenceHandle() = default;
	ResourceReferenceHandle(const Handle& handle) : handle_(handle) {}
	~ResourceReferenceHandle() noexcept = default;

	ResourceReferenceHandle(ResourceReferenceHandle&& other) noexcept = delete;
	ResourceReferenceHandle& operator=(ResourceReferenceHandle&& other) noexcept = delete;

	void release() { handle_ = {}; }
	friend void swap(ResourceReferenceHandle& a, ResourceReferenceHandle& b) noexcept
	{
		using std::swap;
		using RR = ResourceReference<B>;

		swap(static_cast<RR&>(a), static_cast<RR&>(b));
		swap(a.handle_, b.handle_);
	}

protected:
	Handle handle_ {};
};

} // namespace vpp
