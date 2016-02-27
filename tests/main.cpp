#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/backend/win32.hpp>
#include <vpp/device.hpp>
#include <vpp/instance.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/renderer.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>
#include <vpp/graphicsPipeline.hpp>

#include <windows.h>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>

static const std::vector<float> gVertices =
{
      -1.0f,-1.0f,-1.0f, // triangle 1 : begin
      -1.0f,-1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f, // triangle 1 : end
      1.0f, 1.0f,-1.0f, // triangle 2 : begin
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f, // triangle 2 : end
      1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     -1.0f,-1.0f,-1.0f,
     -1.0f,-1.0f,-1.0f,
     -1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f,-1.0f,
     -1.0f, 1.0f, 1.0f,
     -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
     -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f,-1.0f,
     -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f
 };

//
struct App
{
    HINSTANCE hinstance = nullptr;
    HWND window = nullptr;

    unsigned int width = 800;
    unsigned int height = 500;

    vpp::SwapChain* swapChain = nullptr;
    vpp::Instance* instance = nullptr;
	vpp::Device* device = nullptr;
    vpp::Surface* surface = nullptr;
	vpp::Renderer* renderer = nullptr;

	vk::Queue queue {};
};

App* gApp;

//renderer
class MyRenderer : public vpp::Renderer
{
protected:
	std::unique_ptr<vpp::GraphicsPipeline> pipeline_;

	vpp::VertexLayout vertexLayout_;
	vpp::DescriptorSetLayout descriptorSetLayout_;

	vpp::VertexBuffer vertexBuffer_;
	vpp::DescriptorSet descriptorSet_;

protected:
	virtual void buildRenderer(vk::CommandBuffer cmdBuffer) const override
	{
		pipeline_->renderCommands(cmdBuffer, {vertexBuffer_}, {});
	};

public:
	MyRenderer(const vpp::SwapChain& swapChainp)
	{
		swapChain_ = &swapChainp;

		initCommandPool();
		initRenderPass();

		vertexLayout_ = {{vk::Format::R32G32B32Sfloat}};
		descriptorSetLayout_ = {};

		//needed
		//shader
		vpp::Shader shaderprog;
		shaderprog.addStage({"vert.spriv", vk::ShaderStageFlagBits::Vertex});
		shaderprog.addStage({"frag.spriv", vk::ShaderStageFlagBits::Fragment});

		//
		vk::PipelineColorBlendAttachmentState blendAttachmentState[1];
		blendAttachmentState[0].blendEnable(false);
		blendAttachmentState[0].colorWriteMask(
			vk::ColorComponentFlagBits::R |
			vk::ColorComponentFlagBits::G |
			vk::ColorComponentFlagBits::B |
			vk::ColorComponentFlagBits::A
		);

		//info
		vpp::GraphicsPipeline::CreateInfo info;

		//vpp
		info.descriptorSetLayout = descriptorSetLayout_;
		info.vertexBufferLayout = vertexLayout_;
		info.dynamicStates = {vk::DynamicState::Viewport, vk::DynamicState::Scissor};
		info.renderPass = vkRenderPass();
		info.shaderProgram = shader;

		//vk
		info.states.inputAssembly.topology(vk::PrimitiveTopology::TriangleList);

		info.states.rasterization.polygonMode(vk::PolygonMode::Fill);
		info.states.rasterization.cullMode(vk::CullModeFlagBits::None);
		info.states.rasterization.frontFace(vk::FrontFace::CounterClockwise);

		info.states.colorBlend.attachmentCount(1);
		info.states.colorBlend.pAttachments(blendAttachmentState);

		info.states.viewport.viewportCount(1);
		info.states.viewport.scissorCount(1);

		info.states.depthStencil.depthTestEnable(true);
		info.states.depthStencil.depthWriteEnable(true);
		info.states.depthStencil.depthCompareOp(vk::CompareOp::LessOrEqual);
		info.states.depthStencil.depthBoundsTestEnable(false);
		info.states.depthStencil.back().failOp(vk::StencilOp::Keep);
		info.states.depthStencil.back().passOp(vk::StencilOp::Keep);
		info.states.depthStencil.back().compareOp(vk::CompareOp::Allways);
		info.states.depthStencil.stencilTestEnable(false);
		info.states.depthStencil.front(depthStencilState.back());

		info.multisample.pSampleMask(nullptr);
		info.multisample.rasterizationSamples(vk::SampleCountFlagBits::e1);

		pipeline_.reset(new vpp::GraphicsPipeline(device(), info));

		//vertex buffer
		vk::BufferCreateInfo bufInfo;
		bufInfo.size(sizeof(float) * gVertices.size());
		bufInfo.usage(vk::BufferUsageFlagBits::VertexBuffer);

		vpp::MemoryAllocator allocator;
		vertexBuffer_ = vpp::Buffer(allocator, bufInfo, vpp::MemoryPropertyFlagBits::HostVisisble);
		allocator.allocate();

		auto map = vertexBuffer_.memoryMap();
		if(map.ptr())
		{
			std::memcpy(map.ptr(), gVertices.data(), sizeof(float) * gVertices.size());
		}

		//builds renderers with overriden buildCommandBuffer function
		initRenderers();
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
int main()
{
	{
		App app;
		gApp = &app;

	    app.hinstance = GetModuleHandle(nullptr);
	    initWindow(app);

	    vpp::Instance instance({{VK_KHR_SURFACE_EXTENSION_NAME,VK_KHR_WIN32_SURFACE_EXTENSION_NAME}});
	    vpp::Win32Surface surface(instance.vkInstance(), app.hinstance, app.window);

		auto phdev = instance.enumeratrPhysicalDevices()[0];
		auto queues = surface.supportedQueueFamilies();

		std::vector<Vk::queueFamilyProperties> queueProps;
		vk::getPhysicalDeviceQueueFamilyProperties(phdev, queueProps);

		std::uint32_t queueFamily = 99;

		for(auto& queue : queues)
		{
			if(queueProps[i].queueFlags() & vk::QueueFlagBits::Graphics)
			{
				queueFamily = queue;
				break;
			}
		}

		if(queueFamily == 99)
		{
			throw std::runtime_error("unable to get present & graphical queue");
		}

		std::vector<const char*> devExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

		float priorities[1] = {0.0};
		std::vector<vk::DeviceQueueCreateInfo> queueInfos({queueFamily, 1, priorities});

		vk::DeviceCreateInfo devInfo;
		devInfo.enabledExtensionCount(devExtensions.size());
		devInfo.ppEnabledExtensionNames(devExtensions.data());
		devInfo.queueCreateInfoCount(queueInfos.size());
		devInfo.pQueueCreateInfos(queueInfos.data());

		vpp::Device device(instance, phdev, devInfo);

	    vpp::SwapChain swapChain(device, surface, {app.width, app.height});
		MyRenderer renderer(swapChain);

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
