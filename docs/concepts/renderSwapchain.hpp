#include <a>

vk::SurfaceKHR surface;
vpp::Device dev;

SwapchainPrefs prefs = ...;
auto info = vpp::swapchainCreateInfo(prefs);

// #1
// create a swapchain
... use info.imageFormat ...
vpp::Swapchain swapchain(dev, info);
... or use info.imageFormat here ...

// #2
... use info.imageFormat to create render pass ...
MyRenderer renderer(renderPass, presentQueue, info);


// later, on resize event

// #1
// automatically queries/sets correct extents and oldswapchain,
// nothing else
swapchain.resize(size, info);

// #2
renderer.resize(size, info);
