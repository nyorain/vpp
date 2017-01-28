// Simple example on how to load functions with vpp

// load the function as local variable from an instance or device
// just leave out the vk prefix
// in this case a local variable with the name 'pfDestroySurfaceKHR' will be created
VPP_LOAD_PROC(instance, DestroySurfaceKHR);

// we can use the VPP_CALL macro for automatic result conversion and checking
// in debug mode
VPP_CALL(pfCreateSurfaceKHR(instance, surface, nullptr));

// we can also just load a proc without a local variable
VPP_PROC(instance, DestroySurfaceKHR)(vkInstance(), vkHandle(), nullptr);

// or declare a custom one
// note that one should prefer to NOT give it the exact
// vulkan name (vkDestroySurfaceKHR in this case) since this might
// lead to link errors
auto destroySurfaceFunc = VPP_PROC(instance, DestroySurfaceKHR);

// for device functions:
// on failure, these macros reduce/produce nullptr, so in some
// cases it makes sense to check them!
auto createSwapchainFunc = VPP_PROC(device, CreateSwapchainKHR);
