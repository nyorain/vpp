#pragma once

#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <utility>

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
#ifndef VPP_ONE_DEVICE_OPTIMIZATION
class Resource
{
public:
	Resource(const Resource& other) noexcept = default;

	Resource(Resource&& other) noexcept { swap(*this, other); }
	Resource& operator=(Resource other) noexcept { swap(*this, other); return *this; }

	~Resource() noexcept = default;

	const Device& device() const noexcept { return *device_; }

	const vk::Instance& vkInstance() const noexcept { return device().vkInstance(); }
	const vk::Device& vkDevice() const noexcept { return device().vkDevice(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const noexcept
		{ return device().vkPhysicalDevice(); }

	Resource& resourceBase() noexcept { return *this; }
	const Resource& resourceBase() const noexcept { return *this; }

protected:
	Resource() noexcept = default;
	Resource(const Device& device) noexcept : device_(&device) {}

	void init(const Device& dev) noexcept { device_ = &dev; }
	friend void swap(Resource& a, Resource& b) noexcept { std::swap(a.device_, b.device_); }

private:
	const Device* device_ {nullptr};
};

#else //VPP_ONE_DEVICE_OPTIMIZATION

class Resource
{
public:
	Resource(const Resource& other) noexcept = default;
	Resource& operator=(const Resource& other) noexcept = default;

	Resource(Resource&& other) noexcept = default;
	Resource& operator=(Resource&& other) noexcept = default;

	~Resource() = default;

	const Device& device() const noexcept { return *deviceRef; }

	const vk::Instance& vkInstance() const noexcept { return device().vkInstance(); }
	const vk::Device& vkDevice() const noexcept { return device().vkDevice(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const noexcept
		{ return device().vkPhysicalDevice(); }

	Resource& resourceBase() noexcept { return *this; }
	const Resource& resourceBase() const noexcept { return *this; }

protected:
	Resource() = default;
	Resource(const Device& dev) { init(dev); }

	void init(const Device& dev);
	friend void swap(Resource& a, Resource& b) noexcept {}

private:
	static const Device* deviceRef;
};

#endif //VPP_ONE_DEVICE_OPTIMIZATION

///Can be used for non owned resources. Does not store any additional data.
///Derives from the given type and simply calls its protected release function (which should
///release the owned resource) before calling its destructor which then cannot destroy the
///not owned resource.
///If you want to enable your custom classes for this template simply implement a release memeber
///function which is visible to NonOwned.
template <typename T>
class NonOwned : public T
{
public:
	using T::T;
	~NonOwned() { T::release(); }

	// NonOwned(NonOwned&& other) noexcept(noexcept(T(std::declval<T>()))) = default;
	// NonOwned& operator=(NonOwned&& other)
	// 	noexcept(noexcept(std::declval<T> = std::declval<T>)) = default;
	NonOwned(NonOwned&& other) = default;
	NonOwned& operator=(NonOwned&& other) = default;
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
template <typename B>
class ResourceReference
{
public:
	const Device& device() const { return tSelf().resourceRef().device(); }

	const vk::Instance& vkInstance() const { return device().vkInstance(); }
	const vk::PhysicalDevice& vkPhysicalDevice() const { return device().vkPhysicalDevice(); }
	const vk::Device& vkDevice() const { return device().vkDevice(); }

	ResourceReference& resourceBase() noexcept { return *this; }
	const ResourceReference& resourceBase() const noexcept { return *this; }

	const B& tSelf() const { return reinterpret_cast<const B&>(*this); }
};

///Utility template base class that makes RAII wrappers easier.
///Note that move constructor and assignment operator can be defined using the swap
///member function if there are no additional data members.
///\tparam H The vulkan handle type.
template<typename H>
class ResourceHandle : public Resource, public NonCopyable
{
public:
	using Handle = H;

public:
	const Handle& vkHandle() const noexcept { return handle_; }
	Handle& vkHandle() noexcept { return handle_; } //XXX: public for now. Why cant this be protected??

	operator Handle() const noexcept { return vkHandle(); }

protected:
	ResourceHandle() = default;
	ResourceHandle(const Device& dev, const Handle& hndl = {}) : Resource(dev), handle_(hndl) {}
	~ResourceHandle() = default;

	ResourceHandle(ResourceHandle&& other) noexcept = delete;
	ResourceHandle& operator=(ResourceHandle&& other) noexcept = delete;

	ResourceHandle& resourceBase() noexcept { return *this; }
	const ResourceHandle& resourceBase() const noexcept { return *this; }

	void swap(ResourceHandle<H>& other) noexcept
	{
		using std::swap;
		swap(static_cast<Resource&>(*this), static_cast<Resource&>(other));
		swap(handle_, other.handle_);
	}

	friend void swap(ResourceHandle<H>& a, ResourceHandle<H>& b) noexcept { return a.swap(b); }

	template<typename> friend class NonOwned;
	void release() { handle_ = {}; }

protected:
	Handle handle_ {};
};

///ResourceHandle base for classes that already hold a Device reference in some way.
///\tparam B The deriving class, needed for the ResourceReference base
///\tparam H The vulkan handle type.
template<typename B, typename H>
class ResourceReferenceHandle : public ResourceReference<B>, public NonCopyable
{
public:
	using Handle = H;

public:
	const Handle& vkHandle() const noexcept { return handle_; }
	Handle& vkHandle() noexcept { return handle_; } //XXX: public for now. Why cant this be protected??

	operator Handle() const noexcept { return vkHandle(); }

protected:
	ResourceReferenceHandle() = default;
	ResourceReferenceHandle(const Handle& handle) : handle_(handle) {}
	~ResourceReferenceHandle() noexcept = default;

	ResourceReferenceHandle(ResourceReferenceHandle&& other) noexcept = delete;
	ResourceReferenceHandle& operator=(ResourceReferenceHandle&& other) noexcept = delete;

	ResourceReferenceHandle& resourceBase() noexcept { return *this; }
	const ResourceReferenceHandle& resourceBase() const noexcept { return *this; }

	void swap(ResourceReferenceHandle<B, H>& other) noexcept
	{
		using std::swap;
		swap(static_cast<ResourceReference<B>&>(*this), static_cast<ResourceReference<B>&>(other));
		swap(handle_, other.handle_);
	}

	friend void swap(ResourceReferenceHandle<B, H>& a, ResourceReferenceHandle<B, H>& b) noexcept
	{
		return a.swap(b);
	}

	template<typename> friend class NonOwned;
	void release() { handle_ = {}; }

protected:
	Handle handle_ {};
};

}
