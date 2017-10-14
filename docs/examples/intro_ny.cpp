#include <vpp/renderer2.hpp> // vpp::DefaultRenderer
#include <vpp/pipeline.hpp> // vpp::GraphicsPipeline
#include <vpp/instance.hpp> // vpp::Instance
#include <vpp/debug.hpp> // vpp::DebugCallback
#include <vpp/device.hpp> // vpp::Device
#include <vpp/renderPass.hpp> // vpp::RenderPass
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

// shader
#include "data/intro.frag.h"
#include "data/intro.vert.h"

// Pretty much all ny code was taken from the basic example (except
// the vulkan surface creation):
// 	- d: try to toggle server decorations
//	- f: toggle fullscreen
//	- m: toggle maximized state
// 	- i: iconize (minimize) the window
//	- n: reset to normal toplevel state
//	- Escape: close the window

vpp::RenderPass createRenderPass(const vpp::Device&, vk::Format);
vpp::Pipeline createGraphicsPipeline(const vpp::Device&, vk::RenderPass, 
	vk::PipelineLayout);

// TODO: move impl to below main
class MyRenderer : public vpp::DefaultRenderer {
public:
	MyRenderer(vk::RenderPass rp, vk::SwapchainCreateInfoKHR& scInfo,
		const vpp::Queue& present) : scInfo_(scInfo)
	{
		// pipeline
		auto& dev = present.device();
		pipelineLayout_ = {dev, {}};
		pipeline_ = createGraphicsPipeline(dev, rp, pipelineLayout_);

		init(rp, scInfo, present);
	}

	void resize(nytl::Vec2ui size) 
	{
		vpp::DefaultRenderer::resize({size[0], size[1]}, scInfo_);
	}

protected:
	void record(const RenderBuffer& buf) override {
		static const auto clearValue = vk::ClearValue {{0.f, 0.f, 0.f, 1.f}};
		const auto width = scInfo_.imageExtent.width;
		const auto height = scInfo_.imageExtent.height;

		auto cmdBuf = buf.commandBuffer;
		vk::beginCommandBuffer(cmdBuf, {});
		vk::cmdBeginRenderPass(cmdBuf, {
			renderPass(),
			buf.framebuffer,
			{0u, 0u, width, height},
			1,
			&clearValue
		}, {});

		vk::Viewport vp {0.f, 0.f, (float) width, (float) height, 0.f, 1.f};
		vk::cmdSetViewport(cmdBuf, 0, 1, vp);
		vk::cmdSetScissor(cmdBuf, 0, 1, {0, 0, width, height});

		vk::cmdBindPipeline(cmdBuf, vk::PipelineBindPoint::graphics, pipeline_);
		// vk::cmdBindVertexBuffers(cmdBuffer, 0, {vertexBuffer}, {0});
		vk::cmdDraw(cmdBuf, 3, 1, 0, 0);

		vk::cmdEndRenderPass(cmdBuf);
		vk::endCommandBuffer(cmdBuf);
	}

	vpp::PipelineLayout pipelineLayout_;
	vpp::Pipeline pipeline_;
	vk::SwapchainCreateInfoKHR& scInfo_;
};


class MyWindowListener : public ny::WindowListener {
public:
	ny::AppContext* appContext;
	ny::WindowContext* windowContext;
	ny::ToplevelState toplevelState;
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
	ws.size = {800u, 500u};
	ws.vulkan.storeSurface = &reinterpret_cast<std::uintptr_t&>(vkSurface);
	ws.vulkan.instance = reinterpret_cast<VkInstance>(instance.vkInstance());
	// ws.initState = ny::ToplevelState::fullscreen;
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
	auto renderPass = createRenderPass(device, scInfo.imageFormat);

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

	/*
	while(run && ac->pollEvents()) {
		renderer.renderBlock();
	}
	*/

	dlg_info("Returning from main with grace");
}

vpp::Pipeline createGraphicsPipeline(const vpp::Device& dev, vk::RenderPass rp,
	vk::PipelineLayout layout)
{
	// first load the shader modules and create the shader program for our pipeline
	// if the shaders cannot be found/compiled, this will throw (and end the application)
	vpp::ShaderModule vertexShader(dev, intro_vert_spv_data);
	vpp::ShaderModule fragmentShader(dev, intro_frag_spv_data);

	vpp::ShaderProgram shaderStages({
		{vertexShader, vk::ShaderStageBits::vertex},
		{fragmentShader, vk::ShaderStageBits::fragment}
	});

	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.renderPass = rp;
	pipelineInfo.layout = layout;

	pipelineInfo.stageCount = shaderStages.vkStageInfos().size();
	pipelineInfo.pStages = shaderStages.vkStageInfos().data();

	constexpr auto stride = (2 + 4) * 4; // 2 pos floats, 4 color floats (4 byte floats)
	vk::VertexInputBindingDescription bufferBinding {0, stride, vk::VertexInputRate::vertex};

	// vertex position, color attributes
	vk::VertexInputAttributeDescription attributes[2];
	attributes[0].format = vk::Format::r32g32Sfloat;

	attributes[1].location = 1;
	attributes[1].format = vk::Format::r32g32b32a32Sfloat;
	attributes[1].offset = 2 * 4; // pos: vec2f

	vk::PipelineVertexInputStateCreateInfo vertexInfo;
	// vertexInfo.vertexBindingDescriptionCount = 1;
	vertexInfo.vertexBindingDescriptionCount = 0;
	vertexInfo.pVertexBindingDescriptions = &bufferBinding;
	// vertexInfo.vertexAttributeDescriptionCount = 2;
	vertexInfo.vertexAttributeDescriptionCount = 0;
	vertexInfo.pVertexAttributeDescriptions = attributes;
	pipelineInfo.pVertexInputState = &vertexInfo;

	vk::PipelineInputAssemblyStateCreateInfo assemblyInfo;
	assemblyInfo.topology = vk::PrimitiveTopology::triangleList;
	pipelineInfo.pInputAssemblyState = &assemblyInfo;

	vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.polygonMode = vk::PolygonMode::fill;
	rasterizationInfo.cullMode = vk::CullModeBits::none;
	rasterizationInfo.frontFace = vk::FrontFace::counterClockwise;
	rasterizationInfo.depthClampEnable = false;
	rasterizationInfo.rasterizerDiscardEnable = false;
	rasterizationInfo.depthBiasEnable = false;
	rasterizationInfo.lineWidth = 1.f;
	pipelineInfo.pRasterizationState = &rasterizationInfo;

	vk::PipelineMultisampleStateCreateInfo multisampleInfo;
	multisampleInfo.rasterizationSamples = vk::SampleCountBits::e1;
	pipelineInfo.pMultisampleState = &multisampleInfo;

	vk::PipelineColorBlendAttachmentState blendAttachment;
	blendAttachment.blendEnable = false;
	blendAttachment.colorWriteMask =
		vk::ColorComponentBits::r |
		vk::ColorComponentBits::g |
		vk::ColorComponentBits::b |
		vk::ColorComponentBits::a;

	vk::PipelineColorBlendStateCreateInfo blendInfo;
	blendInfo.attachmentCount = 1;
	blendInfo.pAttachments = &blendAttachment;
	pipelineInfo.pColorBlendState = &blendInfo;

	vk::PipelineViewportStateCreateInfo viewportInfo;
	viewportInfo.scissorCount = 1;
	viewportInfo.viewportCount = 1;
	pipelineInfo.pViewportState = &viewportInfo;

	static auto dynStates = {
		vk::DynamicState::viewport, 
		vk::DynamicState::scissor};

	vk::PipelineDynamicStateCreateInfo dynamicInfo;
	dynamicInfo.dynamicStateCount = dynStates.size();
	dynamicInfo.pDynamicStates = dynStates.begin();
	pipelineInfo.pDynamicState = &dynamicInfo;

	// we also use the vpp::PipelienCache in this case
	// we try to load it from an already existent cache
	constexpr auto cacheName = "grapihcsPipelineCache.bin";
	vpp::PipelineCache cache {dev, cacheName};

	auto vkPipeline = vk::createGraphicsPipelines(dev, cache, {pipelineInfo}).front();

	// save the cache to the file we tried to load it from
	vpp::save(cache, cacheName);
	return {dev, vkPipeline};
}

vpp::RenderPass createRenderPass(const vpp::Device& dev, vk::Format format)
{
	vk::AttachmentDescription attachment {};

	// color from swapchain
	attachment.format = format;
	attachment.samples = vk::SampleCountBits::e1;
	attachment.loadOp = vk::AttachmentLoadOp::clear;
	attachment.storeOp = vk::AttachmentStoreOp::store;
	attachment.stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachment.stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachment.initialLayout = vk::ImageLayout::undefined;
	attachment.finalLayout = vk::ImageLayout::presentSrcKHR;

	vk::AttachmentReference colorReference;
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::colorAttachmentOptimal;

	// only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::graphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorReference;

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &attachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	return {dev, renderPassInfo};
}

void MyWindowListener::draw(const ny::DrawEvent&)
{
	if(!renderer) { // should not happen
		dlg_warn("draw: no renderer");
		return;
	}

	dlg_info("drawing the window");

	// TODO: correctly handle result (e.g. outOfDate)
	auto res = renderer->renderBlock();
	if(res != vk::Result::success) {
		dlg_warn("swapchain out of date");
		windowContext->refresh(); // TODO: needed?
	}
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
	renderer->resize(sizeEvent.size);
}

void MyWindowListener::surfaceCreated(const ny::SurfaceCreatedEvent& surfaceEvent)
{
	// TODO
}

void MyWindowListener::surfaceDestroyed(const ny::SurfaceDestroyedEvent&)
{
	// TODO
}