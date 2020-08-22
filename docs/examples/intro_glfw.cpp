// Encapsulates render logic, window library independent
#include "render.hpp"

#include <vpp/renderer.hpp> // vpp::DefaultRenderer
#include <vpp/handles.hpp> // vpp::Instance, vpp::RenderPass, ...
#include <vpp/debug.hpp> // vpp::DebugMessenger
#include <vpp/device.hpp> // vpp::Device
#include <vpp/vk.hpp> // vulkan commands

#include <dlg/dlg.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstring>
#include <vector>

int main(int, char**) {
	if(!::glfwInit()) {
		throw std::runtime_error("Failed to init glfw");
	}

	uint32_t count;
	const char** extensions = ::glfwGetRequiredInstanceExtensions(&count);
	std::vector<const char*> iniExtensions {extensions, extensions + count};
	iniExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	iniExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	// enables all default layers
	constexpr const char* layers[] = {
		"VK_LAYER_KHRONOS_validation",
		// "VK_LAYER_RENDERDOC_Capture",
	};

	// basic application info
	// we use vulkan api version 1.0
	vk::ApplicationInfo appInfo ("vpp-intro", 1, "vpp", 1, VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = iniExtensions.size();
	instanceInfo.ppEnabledExtensionNames = iniExtensions.data();
	instanceInfo.enabledLayerCount = sizeof(layers) / sizeof(const char*);
	instanceInfo.ppEnabledLayerNames = layers;

	vpp::Instance instance(instanceInfo);

	// create a debug messenger for our instance and the default layers.
	// the default implementation will just output the debug messages
	vpp::DebugMessenger debugMessenger(instance);

	// create the ny window and vukan surface
	// init glfw window
	vk::Extent2D size {800u, 500u};
	::glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = ::glfwCreateWindow(size.width, size.height,
		"vpp glfw example", NULL, NULL);
	if(!window) {
		const char* error;
		::glfwGetError(&error);
		std::string msg = "Failed to create glfw window: ";
		msg += error;
		throw std::runtime_error(msg);
	}

	// avoiding reinterpret_cast due to aliasing warnings
	VkInstance vkini;
	auto handle = instance.vkHandle();
	std::memcpy(&vkini, &handle, sizeof(vkini));

	VkSurfaceKHR vkSurf {};
	VkResult err = ::glfwCreateWindowSurface(vkini, window, NULL, &vkSurf);
	if(err) {
		auto str = std::string("Failed to create vulkan surface: ");
		str += vk::name(static_cast<vk::Result>(err));
		throw std::runtime_error(str);
	}

	vk::SurfaceKHR surface {};
	std::memcpy(&surface, &vkSurf, sizeof(surface));

	// now (if everything went correctly) we have the window (and a
	// vulkan surface) and can create the device and renderer.
	const vpp::Queue* present;
	vpp::Device device(instance, surface, present);
	dlg_assert(present);

	// we can construct everything for our renderer
	// swapchain info and renderpass
	// here we could try to use vsync or alpha/transform settings
	auto scInfo = vpp::swapchainCreateInfo(device, surface, size);
	dlg_info("size: {}, {}", scInfo.imageExtent.width, scInfo.imageExtent.height);
	auto renderPass = createRenderPass(device, scInfo.imageFormat);
	vpp::nameHandle(renderPass, "main:renderPass");

	// our renderer
	{
		MyRenderer renderer(renderPass, scInfo, *present);

		// setup events
		::glfwSetWindowUserPointer(window, &renderer);
		auto sizeCallback = [](auto* window, int width, int height) {
			auto ptr = ::glfwGetWindowUserPointer(window);
			auto& renderer = *static_cast<MyRenderer*>(ptr);
			renderer.resize({unsigned(width), unsigned(height)});
		};

		::glfwSetFramebufferSizeCallback(window, sizeCallback);

		// ... and start the main loop.
		// We have a classicaly dumb game loop here that just renders as fast
		// as it can. See the ny example for only rendering when it's needed,
		// glfw doesn't support any draw events.
		dlg_info("Entering main loop");
		while(!::glfwWindowShouldClose(window)) {
			// render and wait, will block cpu
			// you probably rather want to use renderer.render, then do
			// work on the cpu and then wait for the rendering to
			// complete in a real application
			renderer.renderStall();
			::glfwWaitEvents();
		}

		// wait for rendering to finish
		vk::deviceWaitIdle(device);
	}

	// needed, but must be done *after* destroying the swapchain
	vk::destroySurfaceKHR(instance, surface);

	dlg_info("Returning from main with grace");
}
