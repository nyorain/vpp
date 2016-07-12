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
///\sa ResourceReference
///\sa Device
class Resource : public NonCopyable
{
public:
	const Device& device() const { return *device_; }

	const vk::Instance& vkInstance() const { return device().vkInstance(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	const vk::Device& vkDevice() const { return device().vkDevice(); }

protected:
	Resource() = default;
	Resource(const Device& device) : device_(&device) {}
	Resource(const Resource& resource) : device_(&resource.device()) {}
	~Resource() = default;

	Resource(Resource&& other) noexcept = default;
	Resource& operator=(Resource&& other) noexcept = default;

	void init(const Device& device) { device_ = &device; };
	void destroy(){ device_ = {}; }

protected:
	const Device* device_ {nullptr};
};

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
class ResourceReference : public NonCopyable
{
public:
	const Device& device() const { return reinterpret_cast<const T&>(*this).resourceRef().device(); }

	const vk::Instance& vkInstance() const { return device().vkInstance(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	const vk::Device& vkDevice() const { return device().vkDevice(); }
};


///Utility template base class that makes RAII wrappers easier.
///Derives from Resource and holds a vulkan handle of type T.
///Implements the conversion and move operators/constructors.
template<typename T>
class ResourceHandle : public Resource
{
public:
	ResourceHandle(ResourceHandle&& other) noexcept { swap(*this, other); }
	ResourceHandle& operator=(ResourceHandle other) noexcept { swap(*this, other); }
	~ResourceHandle() = default;

	const T& vkHandle() const noexcept { return handle_; }
	operator T() const noexcept { return vkHandle(); }

	friend void swap(ResourceHandle<T>& a, ResourceHandle<T>& b) noexcept
	{
		std::swap(a.device_, b.device_);
		std::swap(a.handle_, b.handle_);
	}

protected:
	ResourceHandle() = default;
	ResourceHandle(const T& handle) : handle_(handle) {}

protected:
	T handle_;
};

}
