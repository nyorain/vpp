// A basic example for using vpp.
// To not pull in a window library dependency, this just works on windows (for now).
// Will use vpp to set up a vulkan rendering context (instance, device, swapchain) and
// then also render a simple triangle on it
// Note how this example has way less code than the plain c vulkan hello world example
// while not losing any really revelant parts.
// Only the main function is really interesting for vpp.
// Tested with the latest amd windows driver on an r9 270x.

// pull in the vulkan winapi-specific type and prototype defs.
#define VK_USE_PLATFORM_WIN32_KHR

// Include the basic vpp headers needed in this example
// Most of them provide actually quite extensive documentation.
#include <vpp/instance.hpp>
#include <vpp/device.hpp>
#include <vpp/queue.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/renderer.hpp>
#include <vpp/buffer.hpp>
#include <vpp/bufferOps.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/surface.hpp>
#include <vpp/procAddr.hpp>
#include <vpp/debug.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>

#include <array>
#include <chrono>

// the needed vulkan extensions
// some functions for the rather uninteresting/usual parts
struct Window {
	vk::Instance instance;
	vpp::Surface surface;
	HWND window;

	Window(const vpp::Instance&);
	~Window();
};

vpp::RenderPass createRenderPass(const vpp::Swapchain&);
vpp::Pipeline createGraphicsPipeline(const vpp::Device&, vk::RenderPass, vk::PipelineLayout);

// Our vpp::RendererBuilder implementation. Used to build the render command buffer
// Our implementation simply clears with a black color and then renders one triangle.
// implemented after main
struct IntroRendererImpl : public vpp::RendererBuilder {
	void build(unsigned int, const vpp::RenderPassInstance&) override;
	std::vector<vk::ClearValue> clearValues(unsigned int) override;

	vk::Pipeline pipeline;
	vk::Buffer vertexBuffer;
};

int main()
{
	// first, create a vulkan instance
	// the needed/used instance extensions
	constexpr const char* iniExtensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	// enables all default layers
	constexpr auto layer = "VK_LAYER_LUNARG_standard_validation";

	// basic application info
	// we use vulkan api version 1.0
	vk::ApplicationInfo appInfo ("vpp-intro", 1, "vpp", 1, VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = sizeof(iniExtensions) / sizeof(iniExtensions[0]);
	instanceInfo.ppEnabledExtensionNames = iniExtensions;
	instanceInfo.enabledLayerCount = 1;
	instanceInfo.ppEnabledLayerNames = &layer;

	vpp::Instance instance(instanceInfo);

	// create a debug callback for our instance and the default layers
	// the default implementation will just output to std::cerr when a debug callback
	// is received
	vpp::DebugCallback debugCallback(instance);

	// this function will create a winapi window wrapper and also create a surface for it
	// this should usually be done by a cross platform window abstraction
	Window window(instance);

	// now create a device for the instance and surface
	// note how vpp will automatically select a suited physical device and query
	// queue families to create basic-needs queues with this constructor.
	// We also retrieve the present queue to present on our surface from this
	// constructor
	const vpp::Queue* presentQueue;
	vpp::Device device(instance, window.surface, presentQueue);

	// now we can create a vulkan swapchain
	// again, we just use the fast way that choses quite sane defaults for us but note
	// that the class offers many convininient configuration possibilities
	vpp::Swapchain swapchain(device, window.surface);

	// to render the triangle we also need to create a render pass
	vpp::RenderPass renderPass = createRenderPass(swapchain);

	// we also create the graphics pipeline that will render our triangle as well
	// as the buffer to hold our vertices
	vpp::PipelineLayout pipelineLayout(device, {});
	auto pipeline = createGraphicsPipeline(device, renderPass, pipelineLayout);

	// note how vpp takes care of buffer allocation (in an efficient way, even when used
	// for multiple resources)
	constexpr auto size = 3u * (2u + 4u) * 4u; // 3 vertices, vec2, vec4 with 4 bytes components
	constexpr auto usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::transferDst;
	vpp::Buffer vertexBuffer(device, {{}, size, usage});

	// vertex data (only positions and color)
	constexpr std::array<float, 6 * 3> vertexData = {{
		0.f, -0.75f,     1.f, 0.f, 0.f, 1.f, // top
		-0.75f, 0.75f,    0.f, 1.f, 0.f, 1.f, // left
		0.75f, 0.75f,    0.f, 0.f, 1.f, 1.f // right
	}};

	// vpp can now be used to fill the vertex buffer with data in the most efficient way
	// in this case the buffer layout does not matter since its a vertex buffer
	// note how vpp automatically unpacks the std::array
	vpp::fill140(vertexBuffer, vpp::raw(vertexData));

	// to render onto the created swapchain we can use vpp::SwapchainRenderer
	// the class implements the default framebuffer and commandbuffer handling
	// we simply implement the vpp::RendererBuilder interface that will be used
	// to build the render command buffers
	vpp::SwapchainRenderer::CreateInfo rendererInfo;
	rendererInfo.queueFamily = device.queue(vk::QueueBits::graphics)->family();
	rendererInfo.renderPass = renderPass;

	auto impl = std::make_unique<IntroRendererImpl>();
	impl->pipeline = pipeline;
	impl->vertexBuffer = vertexBuffer;

	vpp::SwapchainRenderer renderer(swapchain, rendererInfo, std::move(impl));
	renderer.record();

	// run the main loop
	// we just recevie windows events and render after all are processed
	// sry for windows again...
	using Clock = std::chrono::high_resolution_clock;
	auto frames = 0u;
	auto point = Clock::now();

	auto run = true;
	while(run) {
		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0) {
			if(msg.message == WM_QUIT) {
				run = false;
				break;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if(!run) break;
		renderer.renderBlock(*presentQueue);
		++frames;

		// output the average fps count ever second
		auto duration = Clock::now() - point;
		if(duration >= std::chrono::seconds(1)) {
			auto count = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			std::cout << static_cast<int>(frames * (1000.0 / count)) << " fps\n";
			point = Clock::now();
			frames = 0u;
		}
	}

	return EXIT_SUCCESS;
}

void IntroRendererImpl::build(unsigned int, const vpp::RenderPassInstance& renderInstance)
{
	auto cmdBuffer = renderInstance.vkCommandBuffer();
	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics, pipeline);
	vk::cmdBindVertexBuffers(cmdBuffer, 0, {vertexBuffer}, {0});
	vk::cmdDraw(cmdBuffer, 3, 1, 0, 0);
}

std::vector<vk::ClearValue> IntroRendererImpl::clearValues(unsigned int)
{
	// return only one clear value for the swapchain color attachment
	return {{0.f, 0.f, 0.f, 1.f}};
}

vpp::Pipeline createGraphicsPipeline(const vpp::Device& dev, vk::RenderPass rp,
	vk::PipelineLayout layout)
{
	// first load the shader modules and create the shader program for our pipeline
	if(!vpp::fileExists("intro.vert.spv") || !vpp::fileExists("intro.frag.spv"))
		throw std::runtime_error("Could not find shaders. Make sure to execute from binary dir");

	vpp::ShaderModule vertexShader(dev, "intro.vert.spv");
	vpp::ShaderModule fragmentShader(dev, "intro.frag.spv");

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
	vertexInfo.vertexBindingDescriptionCount = 1;
	vertexInfo.pVertexBindingDescriptions = &bufferBinding;
	vertexInfo.vertexAttributeDescriptionCount = 2;
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

	constexpr auto dynStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};

	vk::PipelineDynamicStateCreateInfo dynamicInfo;
	dynamicInfo.dynamicStateCount = dynStates.size();
	dynamicInfo.pDynamicStates = dynStates.begin();
	pipelineInfo.pDynamicState = &dynamicInfo;

	// we also use the vpp::PipelienCache in this case
	// we try to load it from an already existent cache
	constexpr auto cacheName = "grapihcsPipelineCache.bin";

	vpp::PipelineCache cache;
	if(vpp::fileExists(cacheName)) cache = {dev, cacheName};
	else cache = {dev};

	auto vkPipeline = vk::createGraphicsPipelines(dev, cache, {pipelineInfo}).front();

	// save the cache to the file we tried to load it from
	vpp::save(cache, cacheName);
	return {dev, vkPipeline};
}

vpp::RenderPass createRenderPass(const vpp::Swapchain& swapchain)
{
	vk::AttachmentDescription attachment {};

	// color from swapchain
	attachment.format = swapchain.format();
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

	return {swapchain.device(), renderPassInfo};
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message) {
		case WM_ERASEBKGND: break; // avoid flickering
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;
}

Window::Window(const vpp::Instance& instance)
{
	// create a simple winapi window
	constexpr auto name = "vpp::intro";
	constexpr auto width = 1200;
	constexpr auto height = 700;
	auto hinstance = ::GetModuleHandle(nullptr);

	WNDCLASSEX wndClass {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.hInstance = hinstance;
	wndClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = name;
	wndClass.hIconSm = ::LoadIcon(NULL, IDI_WINLOGO);

	if(!::RegisterClassEx(&wndClass))
		throw std::runtime_error("Failed to register window class");

	auto flags = WS_OVERLAPPEDWINDOW;
	window = ::CreateWindowEx(0, name, name, flags, CW_USEDEFAULT,
		CW_USEDEFAULT, width, height, nullptr, nullptr, hinstance, nullptr);

	if(!window)
		throw std::runtime_error("Failed to create window");

	::ShowWindow(window, SW_SHOW);
	::SetForegroundWindow(window);
	::SetFocus(window);
	::SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<ULONG_PTR>(this));

	// create the vulkan surface
	vk::Win32SurfaceCreateInfoKHR info;
	info.hinstance = hinstance;
	info.hwnd = window;

	vk::SurfaceKHR vksurf;
	VPP_PROC(instance, CreateWin32SurfaceKHR)(instance, &info, nullptr, &vksurf);
	surface = {instance, vksurf};
}

Window::~Window()
{
	surface = {};
	if(window)
		::DestroyWindow(window);
}
