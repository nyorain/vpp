#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>

namespace vpp
{

///The Resource class represents a vulkan resource associated to a vulkan device.
///Almost all resources or resource references may be in an uninitialized state, since they
///do not fully follow RAII to make themselves movable. The resources classes theirself
///will NOT perform any checks whether they are initialized in their member function (the
///destructor excluded), so calling member functions of uninitialized (default constructed)
///resources is undefined behaviour (usually ends in a crash).
///There are two implementations of the Resource class depending on whether the
///VPP_ONE_DEVICE_OPTIMIZATION macro is defined. Since many applications will never have more than
///one vpp::Device instance, it is enough to store a single Device pointer for all resources
///instead of every resource keeping a device pointer.
///Both implementations have the same functions and derived classes or users of the
///Resource class shall not depend on any further (implementation defined) functionality.
///\sa ResourceReference
///\sa Device
#ifdef VPP_ONE_DEVICE_OPTIMIZATION
class Resource
{
public:
	const Device& device() const { return *device_; }

	const vk::Instance& vkInstance() const { return device().vkInstance(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	const vk::Device& vkDevice() const { return device().vkDevice(); }

protected:
	Resource() noexcept = default;
	Resource(const Device& device) noexcept : device_(&device) {}
	~Resource() noexcept = default;

	Resource(const Resource&& other) noexcept = default;
	Resource& operator=(const Resource other) noexcept = default;

	Resource(Resource&& other) noexcept { swap(*this, other); }
	Resource& operator=(Resource other) noexcept { swap(*this, other); return *this; }

	friend void swap(Resource& a, Resource& b) noexcept
	{
		std::swap(a.device_, b.device_);
	}

private:
	const Device* device_ {nullptr};
};

#else //VPP_ONE_DEVICE_OPTIMIZATION

class Resource
{
public:
	const Device& device() const { return *deviceRef; }

	const vk::Instance& vkInstance() const { return device().vkInstance(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	const vk::Device& vkDevice() const { return device().vkDevice(); }

protected:
	Resource() = default;
	Resource(const Device& dev) { check(dev); }
	~Resource() = default;

	Resource(const Resource& other) noexcept = default;
	Resource& operator=(const Resource& other) noexcept = default;

	Resource(Resource&& other) noexcept = default;
	Resource& operator=(Resource&& other) noexcept = default;

	friend void swap(Resource& a, Resource& b) noexcept {}

private:
	static const Device* deviceRef;
	void check(const Device& dev);
};

#endif //VPP_ONE_DEVICE_OPTIMIZATION

///Resource class that already holds another resource and does therefore not have to hold a second
///vulkan device reference.
///Classes inheriting from this template have to give theirselfs as template paramater (CRTP) and
///implement a <resourceRef() const> member function which returns a (const) reference to a Resource
///or another ResourceReference.
///This class exists just as an optimization (1 word less memory needed) for Resource classes.
///For an example look e.g. at the CommandBuffer class, which naturally holds a reference to the
///CommandPool it is allocated from and therefore does not have to hold a additional reference to
///its vulkan devcie, since this information can be retrieved from the CommandPool reference (done
///by this template class).
///\sa Resource
///\sa Device
template <typename T>
class ResourceReference
{
public:
	const Device& device() const { return reinterpret_cast<const T&>(*this).resourceRef().device(); }

	const vk::Instance& vkInstance() const { return device().vkInstance(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	const vk::Device& vkDevice() const { return device().vkDevice(); }
};

///Base class for all classes that own a vulkan resource, i.e. classes that cannot be copied.
class VulkanResource : public Resource, public NonCopyable {};

///ResourceReference class for classes that own a vulkan resource.
template<typename T>
class VulkanResourceReference : public ResourceReference<T>, public NonCopyable {};

///Utility template base class that makes RAII wrappers easier.
///Derives from Resource and holds a vulkan handle of type T.
///Implements the conversion and move operators/constructors.
template<typename T, typename Base = VulkanResource>
class ResourceHandle : public Base
{
public:
	ResourceHandle(ResourceHandle&& other) noexcept { swap(*this, other); }
	ResourceHandle& operator=(ResourceHandle other) noexcept { swap(*this, other); }
	~ResourceHandle() = default;

	const T& vkHandle() const noexcept { return handle_; }
	operator T() const noexcept { return vkHandle(); }

	friend void swap(ResourceHandle<T>& a, ResourceHandle<T>& b) noexcept
	{
		std::swap(static_cast<Base>(a), static_cast<Base>(b));
		std::swap(a.handle_, b.handle_);
	}

protected:
	ResourceHandle() = default;
	ResourceHandle(const T& handle) : handle_(handle) {}

protected:
	T handle_;
};

///Convinience Typedef for ResourceHandle classes that don't need to store their own device ref.
template<typename T, typename B>
using ResourceHandleReference = ResourceHandle<T, ResourceReference<B>>;

}
