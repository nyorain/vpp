// An example code snippted that shows some example physical device and queue querying.

auto surface = ... ; // from window library or manually created
auto phDevs = ... ; // something like vk::enumeratePhysicalDevices(instance);

// let vpp choose a suited physical device for the given surface
auto phdev = vpp::choose(phdevs, surface);
if(!phdev)
	throw std::runtime_error("There are no usable vulkan physical devices!");

auto presentFamily = int {}; // present queue family
auto gfxCompFamily = int {}; // graphics and compute queue family

// make sure to find a present and graphics queue family for the
// chosen physical device.
// first try to find one family that has both flags set, if that does
// not work, try to find two seperate queue families.
auto combined = vk::QueueBits::graphics | vk::QueueBits::compute;
presentFamily = gfxCompFamily = vpp::findQueueFamily(phdev, surface, combined);
if(presentFamily == -1) {
	presentFamily = vpp::findQueueFamily(phdev, surface);
	gfxCompFamily = vpp::findQueueFamily(phdev, surface, combined);
}

if(presentFam == -1 || gfxCompFam == -1)
	throw std::runtime_error("Could not find all needed queue features!");
