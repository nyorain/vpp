#include "particles.hpp"
#include <vpp/backend/win32.hpp>
#include <vpp/commandBuffer.hpp>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>

//
LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
		case WM_ERASEBKGND:
		{
			//dont erase it to avoid flickering
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
			if(gApp && gApp->context && gApp->renderer)
			{
				std::cout << "resizing to " << LOWORD(lparam) << "," << HIWORD(lparam) << "\n";
				gApp->context->swapChain().resize({LOWORD(lparam), HIWORD(lparam)});

				auto builder = std::make_unique<ParticleRenderer>(*gApp);
				auto& swapChain = gApp->context->swapChain();
				auto& info = gApp->rendererInfo;
				*gApp->renderer = vpp::SwapChainRenderer(swapChain, info, std::move(builder));
				gApp->renderer->render();

				gApp->width = LOWORD(lparam);
				gApp->height = HIWORD(lparam);
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
	app.renderer->renderBlock();

	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(app.window, &pos);

	auto vec = nytl::Vec2ui(pos.x, pos.y);
	vec = nytl::clamp(vec, nytl::Vec2ui(0, 0), nytl::Vec2ui(app.width, app.height));

	app.particleSystem->update(vec);
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
		std::uint32_t computeQF; //queueFamily

		App app;
		gApp = &app;

	    app.hinstance = GetModuleHandle(nullptr);
	    initWindow(app);

		auto context = vpp::createContext(app.window, {});
		app.context = &context;

		initRenderPass(app);

		app.rendererInfo.queueFamily = context.graphicsQueue()->family();
		app.rendererInfo.renderPass = app.renderPass;
		app.rendererInfo.attachments = {{vpp::ViewableImage::defaultDepth2D()}};

		ParticleSystem particleSystem(app, 1024 * 3000);
		app.particleSystem = &particleSystem;

		std::cout << "setup complete0.\n";

		auto builder = std::make_unique<ParticleRenderer>(app);
		vpp::SwapChainRenderer renderer(context.swapChain(), app.rendererInfo, std::move(builder));
		app.renderer = &renderer;

		std::cout << "setup complete1.\n";

		app.presentQueue = &context.presentQueue();
		app.computeQueue = context.computeQueue();

		std::cout << "qf: " << context.computeQueue()->family() << "\n";
		std::cout << "setup complete2.\n";

	    mainLoop(app);

		std::cout << "main loop exited sucessful.\n";

		//would otherwise be implicitly destructed on App::~App call when going out of scope
		//but at that point the vulkan device would have been already destructed
		app.renderPass = {};
	}

	std::cout << "exiting.\n";

    return 1;
}
