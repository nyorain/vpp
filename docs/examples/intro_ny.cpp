// Basic example on how to use vpp.
// We are using here the highest abstraction/utility vpp has to offer,
// vpp::DefaultRenderer (see vpp/renderer.hpp).
// We use the ny window library (working on windows, unix (wayland and x11)
// and android).
// We already have pre-processed (compiled, parsed as headers) shaders,
// using glslangValidator (see data/*.h).

// Pretty much all ny code was taken from the basic example (except
// the vulkan surface creation):
// 	- d: try to toggle server decorations
//	- f: toggle fullscreen
//	- m: toggle maximized state
// 	- i: iconize (minimize) the window
//	- n: reset to normal toplevel state
//	- Escape: close the window

// Encapsulates render logic, window library independent
#include "render.hpp"

#include <vpp/renderer.hpp> // vpp::DefaultRenderer
#include <vpp/handles.hpp> // vpp::Instance, vpp::RenderPass, ...
#include <vpp/debug.hpp> // vpp::DebugMessenger
#include <vpp/device.hpp> // vpp::Device
#include <vpp/vk.hpp> // vulkan commands

#include <ny/backend.hpp> // ny::Backend
#include <ny/appContext.hpp> // ny::AppContext
#include <ny/windowContext.hpp> // ny::WindowContext
#include <ny/windowListener.hpp> // ny::WindowListener
#include <ny/windowSettings.hpp> // ny::WindowSettings
#include <ny/keyboardContext.hpp> // ny::KeyboardContext
#include <ny/bufferSurface.hpp> // ny::BufferSurface
#include <ny/key.hpp> // ny::Keycode
#include <ny/mouseButton.hpp> // ny::MouseButton
#include <ny/image.hpp> // ny::Image
#include <ny/event.hpp> // ny::*Event

#include <dlg/dlg.hpp> // logging
#include <nytl/vecOps.hpp> // print nytl::Vec

// ny::WindowListener implementation
class MyWindowListener : public ny::WindowListener {
public:
	ny::AppContext* appContext;
	ny::WindowContext* windowContext;
	ny::ToplevelState toplevelState {ny::ToplevelState::normal};
	MyRenderer* renderer;
	nytl::Vec2ui windowSize {800u, 500u};
	bool* run;

public:
	void draw(const ny::DrawEvent&) override;
	void mouseButton(const ny::MouseButtonEvent&) override;
	void key(const ny::KeyEvent&) override;
	void state(const ny::StateEvent&) override;
	void close(const ny::CloseEvent&) override;
	void resize(const ny::SizeEvent&) override;
	void focus(const ny::FocusEvent&) override;
	void surfaceCreated(const ny::SurfaceCreatedEvent&) override;
	void surfaceDestroyed(const ny::SurfaceDestroyedEvent&) override;
};

int main(int, char**) {
	// ny backend/ac setup
	auto& backend = ny::Backend::choose();
	auto ac = backend.createAppContext();

	// vulkan setup since ny needs the instance to create a surface
	auto iniExtensions = ac->vulkanExtensions();
	iniExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	iniExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	// enables all default layers
	constexpr const char* layers[] = {
		"VK_LAYER_LUNARG_standard_validation",
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
	vk::SurfaceKHR vkSurface {};
	auto ws = ny::WindowSettings {};

	auto listener = MyWindowListener {};
	ws.listener = &listener;
	ws.surface = ny::SurfaceType::vulkan;
	ws.size = {800u, 500u};
	ws.vulkan.storeSurface = &reinterpret_cast<std::uintptr_t&>(vkSurface);
	ws.vulkan.instance = reinterpret_cast<VkInstance>(instance.vkInstance());
	auto wc = ac->createWindowContext(ws);

	// now (if everything went correctly) we have the window (and a
	// vulkan surface) and can create the device and renderer.
	const vpp::Queue* present;

	vpp::Device device(instance, vkSurface, present);
	dlg_assert(present);

	// we can construct everything for our renderer
	// swapchain info and renderpass
	// here we could try to use vsync or alpha/transform settings
	auto scInfo = vpp::swapchainCreateInfo(device, vkSurface, {800u, 500u});
	dlg_info("size: {}, {}", scInfo.imageExtent.width, scInfo.imageExtent.height);
	auto renderPass = createRenderPass(device, scInfo.imageFormat);
	vpp::nameHandle(renderPass, "main:renderPass");

	// our renderer
	MyRenderer renderer(renderPass, scInfo, *present);

	// finish the WindowListener setup for events...
	auto run = true;
	listener.appContext = ac.get();
	listener.windowContext = wc.get();
	listener.appContext = ac.get();
	listener.run = &run;
	listener.renderer = &renderer;

	// ... and start the main loop.
	// We actually render only when needed (when we get a draw event)
	// which is probably not what you would want to do in a game but
	// rather in a gui application (e.g. CAD app or high performance ui).
	dlg_info("Entering main loop");
	while(run && ac->waitEvents());

	// The loop below would be a typical game main loop.
	// Note however that we currently also handle draw events which
	// a game would usually not do (since it draws all the time anyways).

	// while(run && ac->pollEvents()) {
	// 	renderer.renderStall(); // render and wait, will block cpu
	// }

	vk::deviceWaitIdle(device);
	dlg_info("Returning from main with grace");
}


// MyWindowListener
void MyWindowListener::draw(const ny::DrawEvent&) {
	if(!renderer) { // should not happen
		dlg_warn("draw: no renderer");
		return;
	}

	dlg_info("drawing the window");

	auto res = renderer->renderStall();
	if(res != vk::Result::success) {
		dlg_warn("swapchain out of date");
		windowContext->refresh();
	}
}

void MyWindowListener::key(const ny::KeyEvent& keyEvent) {
	std::string name = "<unknown>";
	if(appContext->keyboardContext()) {
		auto utf8 = appContext->keyboardContext()->utf8(keyEvent.keycode);
		if(!utf8.empty() && !ny::specialKey(keyEvent.keycode)) name = utf8;
		else name = "<unprintable>";
	}

	auto utf8 = (keyEvent.utf8.empty() || ny::specialKey(keyEvent.keycode)) ?
		"<unprintable>" : keyEvent.utf8;
	dlg_info("Key {} with keycode ({}: {}) {}, generating: {} {}", name,
		(unsigned int) keyEvent.keycode, ny::name(keyEvent.keycode),
		keyEvent.pressed ? "pressed" : "released", utf8,
		keyEvent.repeat ? "(repeated)" : "");

	if(keyEvent.pressed) {
		auto keycode = keyEvent.keycode;
		if(keycode == ny::Keycode::f) {
			dlg_info("Toggling fullscreen");
			if(toplevelState != ny::ToplevelState::fullscreen) {
				windowContext->fullscreen();
				toplevelState = ny::ToplevelState::fullscreen;
			} else {
				windowContext->normalState();
				toplevelState = ny::ToplevelState::normal;
			}
		} else if(keycode == ny::Keycode::n) {
			dlg_info("Resetting window to normal state");
			toplevelState = ny::ToplevelState::normal;
			windowContext->normalState();
		} else if(keycode == ny::Keycode::escape) {
			dlg_info("Closing window and exiting");
			*run = false;
		} else if(keycode == ny::Keycode::m) {
			dlg_info("Toggle window maximize");
			if(toplevelState != ny::ToplevelState::maximized) {
				windowContext->maximize();
				toplevelState = ny::ToplevelState::maximized;
			} else {
				windowContext->normalState();
				toplevelState = ny::ToplevelState::normal;
			}
		} else if(keycode == ny::Keycode::i) {
			dlg_info("Minimizing window");
			toplevelState = ny::ToplevelState::minimized;
			windowContext->minimize();
		} else if(keycode == ny::Keycode::d) {
			dlg_info("Trying to toggle decorations");
			windowContext->customDecorated(!windowContext->customDecorated());
			windowContext->refresh();
		}
	}
}

void MyWindowListener::close(const ny::CloseEvent&) {
	dlg_info("Window was closed by server side. Exiting");
	*run = false;
}

void MyWindowListener::mouseButton(const ny::MouseButtonEvent& event) {
	dlg_info("mouseButton {} {} at {}", ny::mouseButtonName(event.button),
		event.pressed ? "pressed" : "released", event.position);
	if(event.pressed && event.button == ny::MouseButton::left) {
		if(toplevelState != ny::ToplevelState::normal ||
			event.position[0] < 0 || event.position[1] < 0 ||
			static_cast<unsigned int>(event.position[0]) > windowSize[0] ||
			static_cast<unsigned int>(event.position[1]) > windowSize[1])
				return;

		ny::WindowEdges resizeEdges = ny::WindowEdge::none;
		if(event.position[0] < 100) {
			resizeEdges |= ny::WindowEdge::left;
		} else if(static_cast<unsigned int>(event.position[0]) > windowSize[0] - 100) {
			resizeEdges |= ny::WindowEdge::right;
		}

		if(event.position[1] < 100) {
			resizeEdges |= ny::WindowEdge::top;
		} else if(static_cast<unsigned int>(event.position[1]) > windowSize[1] - 100) {
			resizeEdges |= ny::WindowEdge::bottom;
		}

		auto caps = windowContext->capabilities();
		if(resizeEdges != ny::WindowEdge::none && caps & ny::WindowCapability::beginResize) {
			dlg_info("Starting to resize window");
			windowContext->beginResize(event.eventData, resizeEdges);
		} else if(caps & ny::WindowCapability::beginMove) {
			dlg_info("Starting to move window");
			windowContext->beginMove(event.eventData);
		}
	}
}

void MyWindowListener::focus(const ny::FocusEvent& ev) {
	dlg_info("focus: {}", ev.gained);
}

void MyWindowListener::state(const ny::StateEvent& stateEvent) {
	dlg_info("window state changed: {}", (int) stateEvent.state);
	toplevelState = stateEvent.state;
}

void MyWindowListener::resize(const ny::SizeEvent& sizeEvent) {
	dlg_info("window resized to {}", sizeEvent.size);
	windowSize = sizeEvent.size;

	// a woraround but should be valid per spec
	vk::deviceWaitIdle(renderer->device());
	auto s = sizeEvent.size;
	renderer->resize({s.x, s.y});
}

// android:
void MyWindowListener::surfaceCreated(const ny::SurfaceCreatedEvent& surfaceEvent) {
	// TODO: set surface
}

void MyWindowListener::surfaceDestroyed(const ny::SurfaceDestroyedEvent&) {
	// TODO: unset surface and stop rendering
}

