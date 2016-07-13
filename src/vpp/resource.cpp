#include <vpp/resource.hpp>

namespace vpp
{

#ifdef VPP_ONE_DEVICE_OPTIMIZATION

const Device* Resource::deviceRef;
void Resource::init(const Device& dev)
{
	if(deviceRef == &dev) return; //most likely

	if(!deviceRef) deviceRef = &dev;
	else throw std::logic_error("vpp::Resource: VPP_ONE_DEVICE_OPTIMIZATION invalid dev");
}

#endif

}
