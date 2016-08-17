#include <vpp/instance.hpp>
#include <vpp/vk.hpp>

namespace vpp
{

Instance::Instance(const vk::InstanceCreateInfo& info)
{
	instance_ = vk::createInstance(info);
}

Instance::Instance(vk::Instance instance) : instance_(instance)
{
}

Instance::~Instance()
{
	if(instance_) vk::destroyInstance(instance_);
}

}
