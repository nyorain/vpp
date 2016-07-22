#pragma once

#include <vpp/backend/win32.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/transfer.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/renderer.hpp>
#include <vpp/image.hpp>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>
#include <functional>
#include <memory>

struct App
{
    HINSTANCE hinstance = nullptr;
    HWND window = nullptr;

    unsigned int width = 1400;
    unsigned int height = 900;

    vpp::Context context;
	vpp::RenderPass renderPass {};
	vpp::SwapChainRenderer renderer;
	vpp::SwapChainRenderer::CreateInfo rendererInfo {};

	std::function<std::unique_ptr<vpp::RendererBuilder>()> func;
	bool initialized = false;
};

App* gApp;

void toggleFullscreen(HWND hwnd)
{
	static DWORD savedStyle;
	static DWORD savedExstyle;
	static bool fullscreen = false;

	if(!fullscreen)
	{
		savedStyle = ::GetWindowLong(hwnd, GWL_STYLE);
		savedExstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);

		MONITORINFO monitorinfo;
		monitorinfo.cbSize = sizeof(monitorinfo);
	    ::GetMonitorInfo(::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST),
			&monitorinfo);

		auto& rect = monitorinfo.rcMonitor;
		rect.right -= rect.left;
		rect.bottom -= rect.top;

		::SetWindowLong(hwnd, GWL_STYLE, (savedStyle | WS_POPUP) &
			~(WS_OVERLAPPEDWINDOW));
		::SetWindowLong(hwnd, GWL_EXSTYLE, (savedExstyle | WS_EX_TOPMOST) &
			~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
		::SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom,
			SWP_NOOWNERZORDER |	SWP_ASYNCWINDOWPOS | SWP_FRAMECHANGED |
			SWP_NOZORDER | SWP_NOACTIVATE);

		fullscreen = true;
	}
	else
	{
		SetWindowLong(hwnd, GWL_STYLE, savedStyle);
		SetWindowLong(hwnd, GWL_EXSTYLE, savedExstyle);
		::SetWindowPos(hwnd, nullptr, 100, 100, 1200, 800, SWP_ASYNCWINDOWPOS | SWP_FRAMECHANGED);
		fullscreen = false;
	}
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
		case WM_ERASEBKGND:
		{
			//dont erase it to avoid flickering
			break;
		}

		case WM_KEYDOWN:
		{
			switch(wparam)
			{
				case VK_ESCAPE:
				{
					PostQuitMessage(0);
					DestroyWindow(hwnd);
					break;
				}

				case VK_F11:
				{
					toggleFullscreen(hwnd);
					break;
				}
			}
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
			if(gApp && gApp->initialized)
			{
				std::cout << "resizing to " << LOWORD(lparam) << "," << HIWORD(lparam) << "\n";
				gApp->context.swapChain().resize({LOWORD(lparam), HIWORD(lparam)});

				auto& swapChain = gApp->context.swapChain();
				auto& info = gApp->rendererInfo;
				gApp->renderer = {};
				gApp->renderer = vpp::SwapChainRenderer(swapChain, info, gApp->func());

				gApp->renderer.renderBlock();
				//render(*gApp);

				gApp->width = LOWORD(lparam);
				gApp->height = HIWORD(lparam);
			}

			break;
		}

        default:
            return DefWindowProc(hwnd, message, wparam, lparam);
    }

	return 0;
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
    wndClass.hbrBackground = nullptr;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = name.c_str();
    wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&wndClass))
    {
        throw std::runtime_error("Failed to register window class");
    }

	auto flags = WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_OVERLAPPEDWINDOW;
    app.window = CreateWindowEx(0, name.c_str(), name.c_str(), flags, CW_USEDEFAULT,
        CW_USEDEFAULT, app.width, app.height, nullptr, nullptr, app.hinstance, nullptr);

    if(!app.window)
    {
        throw std::runtime_error("Failed to create window");
    }

	COLORREF RRR = RGB(255, 0, 255);
	SetLayeredWindowAttributes(app.window, RRR, (BYTE)0, LWA_COLORKEY);

    ShowWindow(app.window, SW_SHOW);
    SetForegroundWindow(app.window);
    SetFocus(app.window);
}

//
void mainLoop(App& app, const std::function<void()>& update)
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

		app.renderer.renderBlock();
		update();

		++frames;
		if(clock::now() - point > std::chrono::seconds(1))
		{
			std::cout << frames << " fps\n";
			point = clock::now();
			frames = 0u;
		}
    }
}

void initRenderPass(App& app)
{
	auto& dev = app.context.device();
	auto& swapChain = app.context.swapChain();

	vk::AttachmentDescription attachments[2] {};

	//color from swapchain
	attachments[0].format = swapChain.format();
	attachments[0].samples = vk::SampleCountBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::clear;
	attachments[0].storeOp = vk::AttachmentStoreOp::store;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachments[0].initialLayout = vk::ImageLayout::undefined;
	attachments[0].finalLayout = vk::ImageLayout::presentSrcKHR;

	vk::AttachmentReference colorReference;
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::colorAttachmentOptimal;

	//depth from own depth stencil
	attachments[1].format = vk::Format::d16UnormS8Uint;
	attachments[1].samples = vk::SampleCountBits::e1;
	attachments[1].loadOp = vk::AttachmentLoadOp::clear;
	attachments[1].storeOp = vk::AttachmentStoreOp::store;
	attachments[1].stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachments[1].stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachments[1].initialLayout = vk::ImageLayout::undefined;
	attachments[1].finalLayout = vk::ImageLayout::undefined;
	// attachments[1].initialLayout = vk::ImageLayout::depthStencilAttachmentOptimal;
	// attachments[1].finalLayout = vk::ImageLayout::depthStencilAttachmentOptimal;

	vk::AttachmentReference depthReference;
	depthReference.attachment = 1;
	depthReference.layout = vk::ImageLayout::depthStencilAttachmentOptimal;

	//only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::graphics;
	subpass.flags = {};
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorReference;
	subpass.pResolveAttachments = nullptr;
	subpass.pDepthStencilAttachment = &depthReference;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments = attachments;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies = nullptr;

	app.renderPass = vpp::RenderPass(dev, renderPassInfo);
}

//
void initApp(App& app, const std::function<std::unique_ptr<vpp::RendererBuilder>()>& func)
{
	std::uint32_t computeQF; //queueFamily
	gApp = &app;
	app.func = func;

    app.hinstance = GetModuleHandle(nullptr);
    initWindow(app);

	app.context = vpp::createContext(app.window, {app.width, app.height});

	initRenderPass(app);

	app.rendererInfo.queueFamily = app.context.graphicsComputeQueue()->family();
	app.rendererInfo.renderPass = app.renderPass;
	app.rendererInfo.attachments = {{vpp::ViewableImage::defaultDepth2D()}};
	app.context.device().transferManager().shrink();

	app.renderer = vpp::SwapChainRenderer(app.context.swapChain(), app.rendererInfo, func());

	app.initialized = true;
}
