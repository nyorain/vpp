#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/backend/win32.hpp>
#include <vpp/context.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/renderer.hpp>
#include <vpp/graphicsPipeline.hpp>

#include <windows.h>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>

//renderer
class MyRenderer : public vpp::Renderer
{
protected:
	std::unique_ptr<vpp::GraphicsPipeline> pipeline_;

	virtual void buildRenderer(vk::CommandBuffer cmdBuffer) const override
	{
		pipeline_->renderCommands(cmdBuffer);
	};

public:
	MyRenderer(const vpp::SwapChain& swapChainp)
	{
		swapChain_ = &swapChainp;

		initCommandPool();
		initRenderPass();

		pipeline_.reset(new vpp::GraphicsPipeline(swapChain().context(), vkRenderPass()));

		initRenderers();
	}
};

//
struct App
{
    HINSTANCE hinstance;
    HWND window;

    unsigned int width = 800;
    unsigned int height = 500;

    vpp::SwapChain* swapChain = nullptr;
    vpp::Context* context = nullptr;
    vpp::Surface* surface = nullptr;
	vpp::Renderer* renderer = nullptr;

	vk::Queue queue {};
};

App* gApp;

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

	    vpp::Context context;
	    vpp::Win32Surface surface(context.vkInstance(), app.hinstance, app.window);
	    vpp::SwapChain swapChain(context, surface, {app.width, app.height});
		MyRenderer renderer(swapChain);

	    app.context = &context;
	    app.surface = &surface;
	    app.swapChain = &swapChain;
		app.renderer = &renderer;

		app.queue = context.queues()[0].queue;

		std::cout << "setup complete.\n";

	    mainLoop(app);

		std::cout << "main loop exited sucessful.\n";
	}

	std::cout << "exiting.\n";

    return 1;
}
