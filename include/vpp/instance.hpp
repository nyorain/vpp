#pragma once

#include <vpp/fwd.hpp>
#include <vpp/utility/nonCopyable.hpp>

namespace vpp
{

///Instance RAII wrapper.
///Note that this class is (like device) NonMovable since it will be referenced by all its
///resources.
class Instance : public NonMovable
{
public:
	Instance(const vk::InstanceCreateInfo& info);
	Instance(vk::Instance instance);
	~Instance();

	vk::Instance vkInstance() const { return instance_; }
	operator vk::Instance() const { return instance_; }

protected:
	vk::Instance instance_ {};
};

}
