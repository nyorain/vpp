/**
* \example for the usage of device memory resources initialization.
* In this example 3 framebuffers (and their attachments as specified in the info{n} structs)
* are created on only one DeviceMemory object; i.e. there is only one vulkan allocation
* required for all 3 objects.
*
* First, all frambuffer objects (initializers) mention their memorx requiremens to the device,
* which just sotres them instead of instantly allocating the memory. When the framebuffer objects
* then initialize their memory resources, they implicitly assure that their mentioned requirements
* are met, which will make the device (or to be more precise, the DeviceMemoryAllocators hold by the
* device) allocate the needed memory for all requirements.
*
* The MemoryResourceInitializers are needed because some part of the Framebuffer object
* initialization can only be done when the images have bound memory (e.g. create vk::ImageViews)
* so they will split up the initialization in two parts.
*
* In this example everything is done automatically implicitly, but if one wished to use a more
* explicit api or writes its own vulkan classes which need device memory, have a look at
* the DeivceMemory, DeviceMemoryAllocator and DeviceMemoryProvider classes
* (memory.hpp and provider.hpp).
**/

//Scope - usually some point of initialization
{

//memory resource initializer
//register the memory requirements at the allocator and create vulkan images etc.
vpp::MemoryResourceInitializer<vpp::Framebuffer> fb1Init(device, info1);
vpp::MemoryResourceInitializer<vpp::Framebuffer> fb2Init(device, info2);
vpp::MemoryResourceInitializer<vpp::Framebuffer> fb3Init(device, info3);

//second part of framebuffer initialization
//create imageViews etc. (can only be done with bound image memory)
//retrieve the framebuffer objects from the initializers
//the first init call will acutally implicitly allocate the device memory for all 3 framebuffers.
auto fb1 = fb1Init.init();
auto fb2 = fb21Init.init();
auto fb3 = fb3Init.init();

//note that the scope exits here
//initializer objects can be safely destroyed here
//the allocated device memory is automatically freed when it is no longer required by any
//objects.

//Also notice that this technique will work on all vpp classes that require device memory in some
//way, so one could init with this mehtods all its vertex, index and uniform buffers as well as all
//required textures and framebuffer objects on one single device memory allocation.

}


//The alternatvie:
//Although this might seem easier, in this case 3 seperate memory objects have to be allocated
//which is far less efficient than the first method.
vpp::Framebuffer fb1(device, info1);
vpp::Framebuffer fb2(device, info2);
vpp::Framebuffer fb3(device, info3);
