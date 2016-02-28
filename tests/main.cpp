#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/backend/win32.hpp>
#include <vpp/device.hpp>
#include <vpp/instance.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/renderer.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/graphicsPipeline.hpp>

#include <nytl/transform.hpp>

#include <windows.h>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>


static const std::vector<float> gVertices =
{
      -0.5f,-0.5f,-0.5f,    0.f, 1.f, 0.8f,
      -0.5f,-0.5f, 0.5f,    .2f, .3f, 0.5f,
      -0.5f, 0.5f, 0.5f,    .8f, .8f, 0.0f,
      0.5f, 0.5f,-0.5f,     .0f, 0.2f, 1.0f,
      -0.5f,-0.5f,-0.5f,   0.f, 1.f, 0.8f,
      -0.5f, 0.5f,-0.5f,   .0f, 0.2f, 1.0f,
      0.5f,-0.5f, 0.5f,   .2f, .3f, 0.5f,
     -0.5f,-0.5f,-0.5f,  0.f, 1.f, 0.8f,
     0.5f,-0.5f,-0.5f,   1.f, 0.f, 0.2f,
     0.5f, 0.5f,-0.5f,  .0f, 0.2f, 1.0f,
     0.5f,-0.5f,-0.5f,   1.f, 0.f, 0.2f,
     -0.5f,-0.5f,-0.5f,   0.f, 1.f, 0.8f,
     -0.5f,-0.5f,-0.5f,   0.f, 1.f, 0.8f,
     -0.5f, 0.5f, 0.5f,  .8f, .8f, 0.0f,
     -0.5f, 0.5f,-0.5f,   .0f, 0.2f, 1.0f,
     0.5f,-0.5f, 0.5f,   .2f, .3f, 0.5f,
     -0.5f,-0.5f, 0.5f,  .2f, .3f, 0.5f,
     -0.5f,-0.5f,-0.5f,   0.f, 1.f, 0.8f,
     -0.5f, 0.5f, 0.5f,   .8f, .8f, 0.0f,
     -0.5f,-0.5f, 0.5f,   .2f, .3f, 0.5f,
     0.5f,-0.5f, 0.5f,   .2f, .3f, 0.5f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.2f,
     0.5f,-0.5f,-0.5f,   1.f, 0.f, 0.2f,
     0.5f, 0.5f,-0.5f,   .0f, 0.2f, 1.0f,
     0.5f,-0.5f,-0.5f,   1.f, 0.f, 0.2f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.2f,
     0.5f,-0.5f, 0.5f,   .2f, .3f, 0.5f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.2f,
     0.5f, 0.5f,-0.5f,   .0f, 0.2f, 1.0f,
     -0.5f, 0.5f,-0.5f,   .0f, 0.2f, 1.0f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.2f,
     -0.5f, 0.5f,-0.5f,  .0f, 0.2f, 1.0f,
     -0.5f, 0.5f, 0.5f,   .8f, .8f, 0.0f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.2f,
     -0.5f, 0.5f, 0.5f,  .8f, .8f, 0.0f,
     0.5f,-0.5f, 0.5f,  .2f, .3f, 0.5f,
 };

/*
 static const std::vector<float> gVertices = {
	  1.0f,  1.0f, 0.0f , 	1.0f, 0.0f, 0.0f,
	  -1.0f,  1.0f, 0.0f,	 0.0f, 1.0f, 0.0f ,
	  0.0f, -1.0f, 0.0f ,	 0.0f, 0.0f, 1.0f
 };
 */

 class MyRenderer;

//
struct App
{
    HINSTANCE hinstance = nullptr;
    HWND window = nullptr;

    unsigned int width = 800;
    unsigned int height = 800;

    vpp::SwapChain* swapChain = nullptr;
    vpp::Instance* instance = nullptr;
	vpp::Device* device = nullptr;
    vpp::Surface* surface = nullptr;
	MyRenderer* renderer = nullptr;

	vk::Queue queue {};
};

App* gApp;

//renderer
class MyRenderer : public vpp::Renderer
{
protected:
	std::unique_ptr<vpp::DeviceMemoryAllocator> allocator_;

	std::unique_ptr<vpp::GraphicsPipeline> pipeline_;
	std::unique_ptr<vpp::Buffer> vertexBuffer_;
	std::unique_ptr<vpp::Buffer> descriptorBuffer_;
	std::unique_ptr<vpp::DescriptorSet> descriptorSet_;
	std::unique_ptr<vpp::DescriptorSetLayout> descriptorSetLayout_;

	vpp::VertexBufferLayout vertexBufferLayout_;
	vk::DescriptorPool descriptorPool_;

	nytl::Transform3 transform_;

protected:
	virtual void buildRenderer(vk::CommandBuffer cmdBuffer) const override
	{
		//std::cout << "drawCmd\n";
		//pipeline_->drawCommands(cmdBuffer, {vertexBuffer_.get()}, {});

		VkDeviceSize offsets[1] = { 0 };
		auto buf = vertexBuffer_->vkBuffer();

		//auto buf = vertexBuffer_;
		auto vkDesc = descriptorSet_->vkDescriptorSet();

		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
			pipeline_->vkPipelineLayout(), 0, 1, &vkDesc, 0, nullptr);
		vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, pipeline_->vkPipeline());
		vk::cmdBindVertexBuffers(cmdBuffer, 0, 1, &buf, offsets);
		vk::cmdDraw(cmdBuffer, 36, 1, 0, 0);

		//pipeline_->renderCommands(cmdBuffer);
	};

	void initBuffer()
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size(sizeof(float) * gVertices.size());
		bufInfo.usage(vk::BufferUsageFlagBits::VertexBuffer);

		 vertexBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));
	}

	void initDescriptorPool()
	{
		vk::DescriptorPoolSize typeCounts[1] {};
		typeCounts[0].type(vk::DescriptorType::UniformBuffer);
		typeCounts[0].descriptorCount(1);

		vk::DescriptorPoolCreateInfo descriptorPoolInfo;
		descriptorPoolInfo.poolSizeCount(1);
		descriptorPoolInfo.pPoolSizes(typeCounts);
		descriptorPoolInfo.maxSets(1);

		vk::createDescriptorPool(vkDevice(), &descriptorPoolInfo, nullptr, &descriptorPool_);
	}

	void initDescriptorSets()
	{
		//descriptorBuffer
		vk::BufferCreateInfo bufInfo;
		bufInfo.size(sizeof(float) * 16);
		bufInfo.usage(vk::BufferUsageFlagBits::VertexBuffer);

		 descriptorBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));

		//descriptor layout
		std::vector<vpp::DescriptorBinding> bindings =
		{
			{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Vertex}
		};

		descriptorSetLayout_.reset(new vpp::DescriptorSetLayout(device(), bindings));
		descriptorSet_.reset(new vpp::DescriptorSet(*descriptorSetLayout_, descriptorPool_));
	}

	void fillVertexBuffer()
	{
		auto map = vertexBuffer_->memoryMap();
		if(map.ptr())
		{
			std::memcpy(map.ptr(), gVertices.data(), sizeof(float) * gVertices.size());
		}
		else
		{
			throw std::runtime_error("unable to map");
		}
	}

	void fillDescriptorBuffer()
	{
		const auto& mat = transform_.transformMatrix();
		auto map = descriptorBuffer_->memoryMap();
		if(map.ptr())
		{
			for(int i(0); i < 4; ++i)
			{
				nytl::Vec4f vec = mat.col(i);
				std::memcpy(map.ptr() + (4 * sizeof(float) * i), vec.data(), sizeof(float) * 4);
			}
		}
		else
		{
			throw std::runtime_error("unable to map");
		}
	}

public:
	MyRenderer(const vpp::SwapChain& swapChainp)
	{
		Resource::create(swapChainp.device());
		swapChain_ = &swapChainp;

		allocator_.reset(new vpp::DeviceMemoryAllocator(device()));

		initBuffer();
		initDescriptorPool();
		initDescriptorSets();

		//stuff
		initCommandPool();
		initDepthStencil();
		initRenderPass();

		//vertex layout
		vertexBufferLayout_ = {{vk::Format::R32G32B32Sfloat, vk::Format::R32G32B32Sfloat}, 0};

		//info
		vpp::GraphicsPipeline::CreateInfo info;

		//vpp
		info.descriptorSetLayouts = {descriptorSetLayout_.get()};
		info.vertexBufferLayouts = {&vertexBufferLayout_};
		info.dynamicStates = {vk::DynamicState::Viewport, vk::DynamicState::Scissor};
		info.renderPass = vkRenderPass();

		info.shader.create(device());
		info.shader.addStage({"vert.spv", vk::ShaderStageFlagBits::Vertex});
		info.shader.addStage({"frag.spv", vk::ShaderStageFlagBits::Fragment});

		//vk
		//
		vk::PipelineColorBlendAttachmentState blendAttachmentState[1] {};
		blendAttachmentState[0].blendEnable(false);
		blendAttachmentState[0].colorWriteMask(
			vk::ColorComponentFlagBits::R |
			vk::ColorComponentFlagBits::G |
			vk::ColorComponentFlagBits::B |
			vk::ColorComponentFlagBits::A
		);

		//
		info.states.inputAssembly.topology(vk::PrimitiveTopology::TriangleList);

		info.states.rasterization.polygonMode(vk::PolygonMode::Line);
		info.states.rasterization.cullMode(vk::CullModeFlagBits::None);
		info.states.rasterization.frontFace(vk::FrontFace::CounterClockwise);
		info.states.rasterization.depthClampEnable(true);
		info.states.rasterization.rasterizerDiscardEnable(false);
		info.states.rasterization.depthBiasEnable(false);
		info.states.rasterization.lineWidth(3.f);

		info.states.colorBlend.attachmentCount(1);
		info.states.colorBlend.pAttachments(blendAttachmentState);

		info.states.viewport.viewportCount(1);
		info.states.viewport.scissorCount(1);

		vk::StencilOpState stencil;
		stencil.failOp(vk::StencilOp::Keep);
		stencil.passOp(vk::StencilOp::Keep);
		stencil.compareOp(vk::CompareOp::Always);

		info.states.depthStencil.depthTestEnable(true);
		info.states.depthStencil.depthWriteEnable(true);
		info.states.depthStencil.depthCompareOp(vk::CompareOp::LessOrEqual);
		info.states.depthStencil.depthBoundsTestEnable(false);
		info.states.depthStencil.stencilTestEnable(false);
		info.states.depthStencil.back(stencil);
		info.states.depthStencil.front(stencil);

		info.states.multisample.pSampleMask(nullptr);
		info.states.multisample.rasterizationSamples(vk::SampleCountFlagBits::e1);

		pipeline_.reset(new vpp::GraphicsPipeline(device(), info));

		allocator_.reset(); //destroy it -> allocates

		fillVertexBuffer();
		fillDescriptorBuffer();

		descriptorSet_->writeBuffers(0,
			{{descriptorBuffer_->vkBuffer(), 0, sizeof(float) * 16}});

		//builds renderers with overriden buildCommandBuffer function
		initRenderers();
	}

	void updateRotation()
	{
		static float angle = 0;
		angle += 0.0001;

		transform_.resetTransform();
		nytl::rotate(transform_.transformMatrix(), {1, 0.4, 1.5}, angle);
		//std::cout << transform_.transformMatrix() << "\n";
		fillDescriptorBuffer();
	}
};

//
LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
		case WM_ERASEBKGND:
		{
			//dont! erase it
			break;
		}

        case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		}

		case WM_SIZE:
		{
			if(gApp && gApp->swapChain && gApp->renderer)
			{
				std::cout << "resizing to " << LOWORD(lparam) << "," << HIWORD(lparam) << "\n";
				gApp->swapChain->resize({LOWORD(lparam), HIWORD(lparam)});
				gApp->renderer->reset(*gApp->swapChain);
				gApp->renderer->render(gApp->queue);
			}
			break;
		}

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }
}

//
void initWindow(App& app)
{
    std::string name = "test";

    WNDCLASSEX wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = wndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = app.hinstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = name.c_str();
    wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&wndClass))
    {
        throw std::runtime_error("Failed to register window class");
    }

    app.window = CreateWindowEx(0, name.c_str(), name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
        CW_USEDEFAULT, app.width, app.height, nullptr, nullptr, app.hinstance, nullptr);

    if(!app.window)
    {
        throw std::runtime_error("Failed to create window");
    }

    ShowWindow(app.window, SW_SHOW);
    SetForegroundWindow(app.window);
    SetFocus(app.window);
}

//
void render(App& app)
{
	app.renderer->updateRotation();
	app.renderer->render(app.queue);
}


//
void mainLoop(App& app)
{
	using clock = std::chrono::high_resolution_clock;

	auto point = clock::now();
	auto frames = 0u;

    while(1)
    {
        MSG msg;
        PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
        if(msg.message == WM_QUIT)
        {
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        render(app);

		++frames;
		if(clock::now() - point > std::chrono::seconds(1))
		{
			std::cout << frames << " fps\n";
			point = clock::now();
			frames = 0u;
		}
    }
}

//
PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback;
PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback;
PFN_vkDebugReportMessageEXT dbgBreakCallback;

VkDebugReportCallbackEXT msgCallback;

VkBool32 messageCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject,
	size_t location,
	int32_t msgCode,
	const char* pLayerPrefix,
	const char* pMsg,
	void* pUserData)
{
	char *message = (char *)malloc(strlen(pMsg) + 100);

	assert(message);

	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		std::cout << "ERROR: " << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg << "\n";
	}
	else
		if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		{
			// Uncomment to see warnings
			//std::cout << "WARNING: " << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg << "\n";
		}
		else
		{
			return false;
		}

	fflush(stdout);

	free(message);
	return false;
}

void setupDebugging(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportCallbackEXT callBack)
{
	CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	DestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	dbgBreakCallback = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT");

	VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
	dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	dbgCreateInfo.pNext = NULL;
	dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)messageCallback;
	dbgCreateInfo.pUserData = NULL;
	dbgCreateInfo.flags = flags;
	VkDebugReportCallbackEXT debugReportCallback;
	VkResult err = CreateDebugReportCallback(
		instance,
		&dbgCreateInfo,
		NULL,
		&debugReportCallback);
	assert(!err);
}

//
int main()
{
	{
		std::vector<const char*> validationLayerNames =
		{
			"VK_LAYER_LUNARG_threading",
			"VK_LAYER_LUNARG_mem_tracker",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_draw_state",
			"VK_LAYER_LUNARG_param_checker",
			"VK_LAYER_LUNARG_swapchain",
			"VK_LAYER_LUNARG_device_limits",
			"VK_LAYER_LUNARG_image",
			"VK_LAYER_GOOGLE_unique_objects",
		};

		App app;
		gApp = &app;

	    app.hinstance = GetModuleHandle(nullptr);
	    initWindow(app);

		vpp::Instance::CreateInfo iniInfo;
		iniInfo.extensions =
		{
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME
		};
		iniInfo.layers = validationLayerNames;
	    vpp::Instance instance(iniInfo);

		setupDebugging(instance.vkInstance(), VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT, NULL);

	    vpp::Win32Surface surface(instance.vkInstance(), app.hinstance, app.window);

		auto gpus = instance.enumeratePhysicalDevices();
		std::cout << "found " << gpus.size() << " PhysicalDevices\n";

		if(gpus.empty())
		{
			throw std::runtime_error("no gpu");
		}

		auto phdev = gpus[0];
		auto queues = surface.supportedQueueFamilies(phdev);
		auto queueProps = vk::getPhysicalDeviceQueueFamilyProperties(phdev);

		std::uint32_t queueFamily = 99;

		for(auto& queue : queues)
		{
			if(queueProps[queue].queueFlags() & vk::QueueFlagBits::Graphics)
			{
				queueFamily = queue;
				break;
			}
		}

		if(queueFamily == 99)
		{
			throw std::runtime_error("unable to get present & graphical queue");
		}

		std::vector<const char*> devExtensions {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		float priorities[1] = {0.0};
		std::vector<vk::DeviceQueueCreateInfo> queueInfos(1);
		queueInfos[0] = vk::DeviceQueueCreateInfo({}, queueFamily, 1, priorities);

		vk::DeviceCreateInfo devInfo;
		devInfo.enabledExtensionCount(devExtensions.size());
		devInfo.ppEnabledExtensionNames(devExtensions.data());
		devInfo.enabledLayerCount(validationLayerNames.size());
		devInfo.ppEnabledLayerNames(validationLayerNames.data());
		devInfo.queueCreateInfoCount(queueInfos.size());
		devInfo.pQueueCreateInfos(queueInfos.data());

		vpp::Device device(instance.vkInstance(), phdev, devInfo);

	    vpp::SwapChain swapChain(device, surface, {app.width, app.height});
		MyRenderer renderer(swapChain);
		//vpp::Renderer renderer(swapChain);

	    app.instance = &instance;
		app.device = &device;
	    app.surface = &surface;
	    app.swapChain = &swapChain;
		app.renderer = &renderer;
		app.queue = device.queue(queueFamily, 0);

		std::cout << "setup complete.\n";

	    mainLoop(app);

		std::cout << "main loop exited sucessful.\n";
	}

	std::cout << "exiting.\n";

    return 1;
}
