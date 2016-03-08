/**
* \example for the usage of device memory resources initialization.
* In this example 3 framebuffers (and their attachments as specified in the info{n} structs)
* are created on only one DeviceMemory object; i.e. there is only one vulkan allocation
* required for all 3 objects. The vpp::DeviceMemoryAllocator first takes all required
* MemoryRequirements from the created objects (images need device memory) and then on a
* call to alloctae() it allocated the needed vulkan device memory and aligns/binds the different
* entries properly on it.
* The MemoryResourceInitializers are needed because some part of the Framebuffer object
* initialization can only be done when the images have bound memory (e.g. create vk::ImageViews)
* so they will split up the initialization in two parts.
**/

//Scope - usually some point of initialization
{

//allocator object
vpp::DeviceMemoryAllocator allocator(dev);

//memory resource initializer
//register the memory requirements at the allocator and create vulkan images etc.
vpp::MemoryResourceInitializer<vpp::Framebuffer> fb1Init(allocator, info1);
vpp::MemoryResourceInitializer<vpp::Framebuffer> fb2Init(allocator, info2);
vpp::MemoryResourceInitializer<vpp::Framebuffer> fb3Init(allocator, info3);

//allocate and bind the required memory entries.
allocator.allocate();

//second part of framebuffer initialization
//create imageViews etc. (can only be done with bound image memory)
//retrieve the framebuffer objects from the initializers
auto fb1 = fb1Init.init();
auto fb2 = fb21Init.init();
auto fb3 = fb3Init.init();

//note that the scope exits here
//Allocator as well as initializer objects can be safely destroyed here
//the allocated device memory is automatically freed when it is no longer required by any
//objects.

}
