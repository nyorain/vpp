#define VK_USE_PLATFORM_WIN32_KHR

#include <vpp/backend/win32.hpp>
#include <vpp/device.hpp>
#include <vpp/swapChain.hpp>
#include <vpp/renderer.hpp>
#include <vpp/shader.hpp>
#include <vpp/buffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/debug.hpp>
#include <vpp/graphicsPipeline.hpp>

#include <nytl/transform.hpp>

#include <windows.h> //srsly microsoft? "near" and "far" macros? WTF man hoooly shit

#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>

template<std::size_t R, std::size_t C, typename P>
nytl::Mat<C, R, P> transpose(const nytl::Mat<R, C, P>& mat)
{
	nytl::Mat<C, R, P> ret;
	for(std::size_t i(0); i < R; ++i)
		ret.col(i) = mat.row(i);

	return ret;
}


static const std::vector<float> gVertices =
/*
{
      -0.5f,-0.5f,-0.5f,    0.f, 1.f, 0.8f,
      -0.5f, 0.5f,-0.5f,    .2f, .3f, 0.5f,
       0.5f, 0.5f, -0.5f,   .8f, .8f, 0.0f,
	   0.5f, -0.5f,-0.5f,   .8f, .8f, 0.0f,

	  -0.5f,-0.5f, 0.5f,    .8f, .8f, 0.0f,
      -0.5f, 0.5f, 0.5f,    .0f, 0.2f, 1.0f,
	   0.5f, 0.5f, 0.5f,	1.f, 0.2f, 0.f,
	   0.5f,-0.5f, 0.5f,	1.f, 0.2f, 0.f,
 };
 */
 {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

std::vector<float> gPlane =
{
	-2.f, 1.f, -2.f, 0.f, 0.f, 1.f,
	-2.f, 1.f, 2.f, 0.f, 0.f, 1.f,
	2.f, 1.f, 2.f, 0.f, 0.f, 1.f,
	-2.f, 1.f, -2.f, 0.f, 0.f, 1.f,
	2.f, 1.f, -2.f, 0.f, 0.f, 1.f,
	2.f, 1.f, 2.f, 0.f, 0.f, 1.f,
};

 static const std::vector<std::uint32_t> gIndices =
 {
	 0, 1, 3, //top
	 1, 2, 3,
	 4, 7, 5, //bottom
	 5, 7, 6,
	 0, 4, 1, //front
	 4, 5, 1,
	 3, 2, 7, //back
	 7, 2, 6,
	 0, 3, 7, //left
	 0, 7, 4,
	 1, 5, 6, //right
	 1, 6, 2
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

    unsigned int width = 900;
    unsigned int height = 900;

    vpp::Context* context = nullptr;
	vpp::RenderPass renderPass {};
	vpp::SwapChainRenderer* renderer = nullptr;
	MyRenderer* myRenderer = nullptr;
	vpp::SwapChainRenderer::CreateInfo rendererInfo {};

	vk::Queue queue {};
};


void initRenderPass(App& app)
{
	auto& dev = app.context->device();
	auto& swapChain = app.context->swapChain();

	vk::AttachmentDescription attachments[2] {};

	//color from swapchain
	attachments[0].format(swapChain.format());
	attachments[0].samples(vk::SampleCountFlagBits::e1);
	attachments[0].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[0].storeOp(vk::AttachmentStoreOp::Store);
	attachments[0].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[0].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[0].initialLayout(vk::ImageLayout::ColorAttachmentOptimal);
	attachments[0].finalLayout(vk::ImageLayout::ColorAttachmentOptimal);

	vk::AttachmentReference colorReference;
	colorReference.attachment(0);
	colorReference.layout(vk::ImageLayout::ColorAttachmentOptimal);

	//depth from own depth stencil
	attachments[1].format(vk::Format::D16UnormS8Uint);
	attachments[1].samples(vk::SampleCountFlagBits::e1);
	attachments[1].loadOp(vk::AttachmentLoadOp::Clear);
	attachments[1].storeOp(vk::AttachmentStoreOp::Store);
	attachments[1].stencilLoadOp(vk::AttachmentLoadOp::DontCare);
	attachments[1].stencilStoreOp(vk::AttachmentStoreOp::DontCare);
	attachments[1].initialLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);
	attachments[1].finalLayout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	vk::AttachmentReference depthReference;
	depthReference.attachment(1);
	depthReference.layout(vk::ImageLayout::DepthStencilAttachmentOptimal);

	//only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint(vk::PipelineBindPoint::Graphics);
	subpass.flags({});
	subpass.inputAttachmentCount(0);
	subpass.pInputAttachments(nullptr);
	subpass.colorAttachmentCount(1);
	subpass.pColorAttachments(&colorReference);
	subpass.pResolveAttachments(nullptr);
	subpass.pDepthStencilAttachment(&depthReference);
	subpass.preserveAttachmentCount(0);
	subpass.pPreserveAttachments(nullptr);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount(2);
	renderPassInfo.pAttachments(attachments);
	renderPassInfo.subpassCount(1);
	renderPassInfo.pSubpasses(&subpass);
	renderPassInfo.dependencyCount(0);
	renderPassInfo.pDependencies(nullptr);

	app.renderPass = vpp::RenderPass(dev, renderPassInfo);
}

App* gApp;

//renderer
class MyRenderer : public vpp::RendererBuilder, public vpp::Resource
{
protected:
	std::unique_ptr<vpp::DeviceMemoryAllocator> allocator_;

	std::unique_ptr<vpp::GraphicsPipeline> pipeline_;
	std::unique_ptr<vpp::Buffer> vertexBuffer_;
	std::unique_ptr<vpp::Buffer> planeVertexBuffer_;
	std::unique_ptr<vpp::Buffer> descriptorBuffer_;
	std::unique_ptr<vpp::Buffer> planeDescriptorBuffer_;
	std::unique_ptr<vpp::Buffer> indexBuffer_;
	std::unique_ptr<vpp::DescriptorSet> descriptorSet_;
	std::unique_ptr<vpp::DescriptorSet> planeDescriptorSet_;
	std::unique_ptr<vpp::DescriptorSet> planeSet_;
	std::unique_ptr<vpp::DescriptorSetLayout> descriptorSetLayout_;

	vpp::VertexBufferLayout vertexBufferLayout_;
	vk::DescriptorPool descriptorPool_;

	nytl::Transform3 transform_;
	nytl::Mat4f viewMatrix_;
	nytl::Mat4f projectionMatrix_;

protected:
	void initVertexBuffer()
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size(sizeof(float) * gVertices.size());
		bufInfo.usage(vk::BufferUsageFlagBits::VertexBuffer);

		vertexBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));

		planeVertexBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));
	}
/*
	void initIndexBuffer()
	{
		vk::BufferCreateInfo bufInfo;
		bufInfo.size(sizeof(std::uint32_t) * gIndices.size());
		bufInfo.usage(vk::BufferUsageFlagBits::IndexBuffer);

		 indexBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));
	}
*/
	void initDescriptorPool()
	{
		vk::DescriptorPoolSize typeCounts[1] {};
		typeCounts[0].type(vk::DescriptorType::UniformBuffer);
		typeCounts[0].descriptorCount(2);

		vk::DescriptorPoolCreateInfo descriptorPoolInfo;
		descriptorPoolInfo.poolSizeCount(1);
		descriptorPoolInfo.pPoolSizes(typeCounts);
		descriptorPoolInfo.maxSets(2);

		vk::createDescriptorPool(vkDevice(), &descriptorPoolInfo, nullptr, &descriptorPool_);
	}

	void initDescriptorSets()
	{
		//descriptorBuffer
		vk::BufferCreateInfo bufInfo;
		bufInfo.size(sizeof(nytl::Mat4f) * 3);
		bufInfo.usage(vk::BufferUsageFlagBits::UniformBuffer);

		descriptorBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));

		planeDescriptorBuffer_.reset(new vpp::Buffer(*allocator_, bufInfo,
			vk::MemoryPropertyFlagBits::HostVisible));

		//descriptor layout
		std::vector<vpp::DescriptorBinding> bindings =
		{
			{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Vertex}
		};

		descriptorSetLayout_.reset(new vpp::DescriptorSetLayout(device(), bindings));

		descriptorSet_.reset(new vpp::DescriptorSet(*descriptorSetLayout_, descriptorPool_));
		planeDescriptorSet_.reset(new vpp::DescriptorSet(*descriptorSetLayout_, descriptorPool_));
	}

	void fillVertexBuffer()
	{
		auto map = vertexBuffer_->memoryMap();
		std::memcpy(map.ptr(), gVertices.data(), sizeof(float) * gVertices.size());
		map.unmap();

		auto map2 = planeVertexBuffer_->memoryMap();
		std::memcpy(map2.ptr(), gPlane.data(), sizeof(float) * gPlane.size());
		map2.unmap();
	}

/*
	void fillIndexBuffer()
	{
		auto map = indexBuffer_->memoryMap();
		std::memcpy(map.ptr(), gIndices.data(), sizeof(std::uint32_t) * gIndices.size());
	}
*/
	void fillDescriptorBuffer()
	{
		auto map = descriptorBuffer_->memoryMap();

		auto size = sizeof(nytl::Mat4f);
		const auto& mat = transform_.transformMatrix();

		auto ptr = static_cast<std::uint8_t*>(map.ptr());
		std::memcpy(ptr, transpose(mat).data(), size);
		std::memcpy(ptr + size, transpose(viewMatrix_).data(), size);
		std::memcpy(ptr + 2 * size, projectionMatrix_.data(), size);

		map.unmap();

		//plane
		auto model = nytl::identityMat<4, float>();
		nytl::rotate(model, {0.f, 1.f, 0.f}, 45.f);

		auto map2 = planeDescriptorBuffer_->memoryMap();
		ptr = static_cast<std::uint8_t*>(map2.ptr());
		std::memcpy(ptr, transpose(model).data(), size);
		std::memcpy(ptr + size, transpose(viewMatrix_).data(), size);
		std::memcpy(ptr + 2 * size, projectionMatrix_.data(), size);

		map2.unmap();
	}

public:
	MyRenderer(App& app) : Resource(app.context->device())
	{
		viewMatrix_ = nytl::identityMat<4>();
		viewMatrix_[2][3] = -3.0;

		projectionMatrix_ = nytl::perspective3(45.f, 900.f / 900.f, 0.1f, 100.f);
		//projectionMatrix_ = nytl::identityMat<4>();

		//std::cout << viewMatrix_ << "\n";
		//std::cout << projectionMatrix_ << "\n";


		allocator_.reset(new vpp::DeviceMemoryAllocator(device()));

		initVertexBuffer();
		//initIndexBuffer();

		initDescriptorPool();
		initDescriptorSets();

		//vertex layout
		vertexBufferLayout_ = {{vk::Format::R32G32B32Sfloat, vk::Format::R32G32B32Sfloat}, 0};

		//info
		vpp::GraphicsPipeline::CreateInfo info;

		//vpp
		info.descriptorSetLayouts = {descriptorSetLayout_.get()};
		info.vertexBufferLayouts = {&vertexBufferLayout_};
		info.dynamicStates = {vk::DynamicState::Viewport, vk::DynamicState::Scissor};
		info.renderPass = app.renderPass.vkRenderPass();

		info.shader.init(device());
		info.shader.addStage({"vert.spv", vk::ShaderStageFlagBits::Vertex});
		info.shader.addStage({"frag.spv", vk::ShaderStageFlagBits::Fragment});

		info.states = vpp::GraphicsPipeline::StatesCreateInfo{vk::Viewport{0, 0, 900, 900, 0.f, 1.f}};
		info.states.rasterization.cullMode(vk::CullModeFlagBits::None); //triangles are not all cc order

		pipeline_.reset(new vpp::GraphicsPipeline(device(), info));

		allocator_.reset(); //destroy it -> allocates

		fillVertexBuffer();
		//fillIndexBuffer();
		fillDescriptorBuffer();

		descriptorSet_->writeBuffers(0,
			{{descriptorBuffer_->vkBuffer(), 0, sizeof(nytl::Mat4f) * 3}});

		planeDescriptorSet_->writeBuffers(0,
			{{planeDescriptorBuffer_->vkBuffer(), 0, sizeof(nytl::Mat4f) * 3}});
	}

	void updateRotation()
	{
		static float angle = 0;
		angle += 0.0002;

		transform_.resetTransform();
		nytl::rotate(transform_.transformMatrix(), {1, 0.4, 1.5}, angle);
		//std::cout << transform_.transformMatrix() << "\n";
		fillDescriptorBuffer();
	}

	virtual void build(const vpp::RenderPassInstance& instance) const override
	{
		auto cmdBuffer = instance.vkCommandBuffer();
		VkDeviceSize offsets[1] = { 0 };

		auto buf = vertexBuffer_->vkBuffer();
		auto vkDesc = descriptorSet_->vkDescriptorSet();

		auto planeBuf = planeVertexBuffer_->vkBuffer();
		auto planeVkDesc = planeDescriptorSet_->vkDescriptorSet();

		vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, pipeline_->vkPipeline());

		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
			pipeline_->vkPipelineLayout(), 0, 1, &vkDesc, 0, nullptr);
		vk::cmdBindVertexBuffers(cmdBuffer, 0, 1, &buf, offsets);
		vk::cmdDraw(cmdBuffer, gVertices.size(), 1, 0, 0);

		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
			pipeline_->vkPipelineLayout(), 0, 1, &planeVkDesc, 0, nullptr);
		vk::cmdBindVertexBuffers(cmdBuffer, 0, 1, &planeBuf, offsets);
		vk::cmdDraw(cmdBuffer, gPlane.size(), 1, 0, 0);
	};

	virtual std::vector<vk::ClearValue> clearValues() const override
	{
		std::vector<vk::ClearValue> ret(2);
		ret[0].color(std::array<float, 4>{{1.f, 1.f, 1.f, 1.f}});
		ret[1].depthStencil({1.f, 0});
		return ret;
	}

};

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
				*gApp->renderer = vpp::SwapChainRenderer(gApp->context->swapChain(), *gApp->myRenderer,
					gApp->rendererInfo);
				gApp->renderer->render();
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
	app.myRenderer->updateRotation();
	app.renderer->render();
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

		vpp::Win32Context context({{900, 900}}, app.window);
		app.context = &context;

		initRenderPass(app);

		MyRenderer myrenderer(app);
		app.myRenderer = &myrenderer;

		app.rendererInfo.queue = context.presentQueue();
		app.rendererInfo.renderPass = &app.renderPass;
		app.rendererInfo.staticAttachments = {vpp::FramebufferAttachment::defaultDepthAttachment};

		vpp::SwapChainRenderer renderer(context.swapChain(), myrenderer, app.rendererInfo);
		app.renderer = &renderer;

		app.queue = context.presentQueue().queue;

		std::cout << "setup complete.\n";

	    mainLoop(app);

		std::cout << "main loop exited sucessful.\n";
	}

	std::cout << "exiting.\n";

    return 1;
}
