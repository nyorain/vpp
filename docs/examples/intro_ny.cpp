#include <vpp/renderer2.hpp> // vpp::DefaultRenderer
#include <vpp/pipeline.hpp> // vpp::GraphicsPipeline
#include <vpp/instance.hpp> // vpp::Instance
#include <vpp/debug.hpp> // vpp::DebugCallback
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
#include <cstring> // std::memset

// Pretty much all ny code was taken from the basic example (except
// the vulkan surface creation):
// 	- d: try to toggle server decorations
//	- f: toggle fullscreen
//	- m: toggle maximized state
// 	- i: iconize (minimize) the window
//	- n: reset to normal toplevel state
//	- Escape: close the window

class MyWindowListener : public ny::WindowListener {
public:
	ny::AppContext* appContext;
	ny::WindowContext* windowContext;
	ny::ToplevelState toplevelState;
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

class MyRenderer : public vpp::DefaultRenderer {
public:
	using vpp::DefaultRenderer;
};

vpp::RenderPass createRenderPass(const vpp::Swapchain&);
vpp::Pipeline createGraphicsPipeline(const vpp::Device&, vk::RenderPass, vk::PipelineLayout);

int main(int, char**)
{
	// ny backend/ac setup
	auto& backend = ny::Backend::choose();
	auto ac = backend.createAppContext();

	// vulkan setup since ny needs the instance to create a surface
	auto iniExtensions = ac->vulkanExtensions();
	iniExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	iniExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	// enables all default layers
	constexpr auto layer = "VK_LAYER_LUNARG_standard_validation";

	// basic application info
	// we use vulkan api version 1.0
	vk::ApplicationInfo appInfo ("vpp-intro", 1, "vpp", 1, VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = iniExtensions.size();
	instanceInfo.ppEnabledExtensionNames = iniExtensions.data();
	instanceInfo.enabledLayerCount = 1;
	instanceInfo.ppEnabledLayerNames = &layer;

	vpp::Instance instance(instanceInfo);

	// create a debug callback for our instance and the default layers
	// the default implementation will just output to std::cerr when a debug callback
	// is received
	vpp::DebugCallback debugCallback(instance);

	// create the ny window and vukan surface
	vk::SurfaceKHR vkSurface {};
	auto ws = ny::WindowSettings {};

	auto listener = MyWindowListener {};
	ws.listener = &listener;
	ws.surface = ny::SurfaceType::vulkan;
	ws.vulkan.storeSurface = &reinterpret_cast<std::uintptr_t&>(vkSurface);
	ws.vulkan.instance = reinterpret_cast<VkInstance>(instance.vkInstance());
	ws.initState = ny::ToplevelState::fullscreen;
	auto wc = ac->createWindowContext(ws);

	// now (if everything went correctly) we have the window (and a 
	// vulkan surface) and can create the device and renderer.
	const vpp::Queue* present;
	vpp::Device device(instance, vkSurface, present);

	// we can construct our renderer.
	// renderPass

	// finish the WindowListener setup for events...
	auto run = true;
	listener.appContext = ac.get();
	listener.windowContext = wc.get();
	listener.appContext = ac.get();
	listener.run = &run;

	// ... and start the main loop.
	// We actually render only when needed (when we get a draw event)
	// which is probably not what you would want to do in a game but
	// rather in a gui application (e.g. CAD app or high performance ui).
	dlg_info("Entering main loop");
	while(run && ac->waitEvents());
	dlg_info("Returning from main with grace");
}

void MyWindowListener::draw(const ny::DrawEvent&)
{
	if(!bufferSurface) {
		dlg_info("draw: no bufferSurface");
		return;
	}

	auto guard = bufferSurface->buffer();
	auto image = guard.get();
	auto size = ny::dataSize(image);
	dlg_info("drawing the window: size {}", image.size);

	std::memset(image.data, 0xFF, size); // opaque white
}

void MyWindowListener::key(const ny::KeyEvent& keyEvent)
{
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

void MyWindowListener::close(const ny::CloseEvent&)
{
	dlg_info("Window was closed by server side. Exiting");
	*run = false;
}

void MyWindowListener::mouseButton(const ny::MouseButtonEvent& event)
{
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

void MyWindowListener::focus(const ny::FocusEvent& ev)
{
	dlg_info("focus: {}", ev.gained);
}

void MyWindowListener::state(const ny::StateEvent& stateEvent)
{
	dlg_info("window state changed: {}", (int) stateEvent.state);
	toplevelState = stateEvent.state;
}

void MyWindowListener::resize(const ny::SizeEvent& sizeEvent)
{
	dlg_info("window resized to {}", sizeEvent.size);
	windowSize = sizeEvent.size;
}

void MyWindowListener::surfaceCreated(const ny::SurfaceCreatedEvent& surfaceEvent)
{
	bufferSurface = surfaceEvent.surface.buffer;
	windowContext->refresh();
}

void MyWindowListener::surfaceDestroyed(const ny::SurfaceDestroyedEvent&)
{
	bufferSurface = nullptr;
}