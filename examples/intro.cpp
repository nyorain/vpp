// A basic example for using vpp.
// To not pull in a window library dependency, this just works on windows (for now).
// Will use vpp to set up a vulkan rendering context (instance, device, swapchain) and
// then also render a simple triangle on it
// Note how this example has way less codes than the plain c vulkan hello world example
// while not losing any really revelant parts.

#define VK_USE_PLATFORM_WIN32_KHR

// Include the basic vpp headers needed in this example
// Most of them provide actually quite extensive documentation.
#include <vpp/instance.hpp>
#include <vpp/device.hpp>
#include <vpp/swapchain.hpp>
#include <vpp/renderer.hpp>
#include <vpp/buffer.hpp>
#include <vpp/bufferOps.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/vk.hpp>

#include <array>


// some shader spirv forward declarations we need later on
extern std::uint32_t vertexSpirv[];
extern std::uint32_t fragmentSpirv[];

// the needed vulkan extensions
const char* iniExtensions[] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
const char* devExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

// some functions for the rather uninteresting parts
class Window createWindow(vk::Instance instance);
vpp::RenderPass createRenderPass(const vpp::Swapchain& swapchain);

int main()
{
	// first, create a vulkan instance
	vk::ApplicationInfo appInfo ("vpp-intro", 1, "vpp", 1, VK_API_VERSION_1_0);

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledExtensionCount = 2;
	instanceInfo.enabledExtensionCount = iniExtensions;

	vpp::Instance instance(instanceInfo);

	// this function will create a winapi window wrapper and also create a surface for it
	// this should usually be done by a cross platform window abstraction
	auto window = createWindow();

	// now create a device for the instance and surface
	// note how vpp will automatically select a suited physical device and query
	// queue families to create basic-needs queues with this constructor.
	// We also retrieve the present queue to present on our surface from this
	// constructor
	const vpp::Queue* presentQueue;
	vpp::Device device(instance, window.vkSurface, presentQueue);

	// now we can create a vulkan swapchain
	// again, we just use the fast way that choses quite sane defaults for us but note
	// that the class offers many convininient configuration possibilities
	vpp::Swapchain swapchain(device, window.vkSurface);

	// to render the triangle we also need to create a render pass
	vpp::RenderPass renderPass = createRenderPass(swapchain);

	// to render onto the created swapchain we can use vpp::SwapchainRenderer
	// the class implements the default framebuffer and commandbuffer handling
	// we simply implement the vpp::RendererBuilder interface that will be used
	// to build the render command buffers
	vpp::SwapchainRenderer renderer(device, ...);

	// we also create the graphics pipeline that will render our triangle as well
	// as the buffer to hold our vertices

	// first load the shader modules and create the shader program for our pipeline
	vpp::ShaderModule vertexShader(device, vertexSpirv);
	vpp::ShaderModule fragmentShader(device, fragmentSpirv);

	vpp::ShaderProgram shaderStages(device, {
		{vertexShader, vk::ShaderStageBits::vertex},
		{fragmentShader, vk::ShaderStageBits::fragment}
	});

	{
		vk::GraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.stagesCount = shaderStages.vkStageInfos().size();
		pipelineInfo.pStages = shaderStages.vkStageInfos().data();

		vk::PipelineVertexInputStateCreateInfo vertexInfo;

		vk::PipelineInputAssemblyStateCreateInfo assemblyInfo;

		vk::PipelineRasterizationStateCreateInfo rasterizationInfo;

	}

	vpp::Pipeline

	// note how vpp takes care of buffer allocation (in an efficient way, even when used
	// for multiple resources)
	constexpr auto size = 3 * 2 * 4; // 3 vertices, vec2 with 4 bytes components
	vpp::Buffer vertexBuffer(device, {{}, size, vk::BufferUsageBits::vertexBuffer});

	// out vertex data (only the positions)
	constexpr std::array<std::array<float, 2>, 3> vertexData = {
		{0.5f, 0.25f}, // top
		{0.25f, 0.75f}, // left
		{0.75f, 0.75f} // right
	};

	// we did not create the buffer explicitly on host visisble memory
	// vpp can now be used to fill the vertex buffer with data in the most efficient way
	// we fill it using std140 layout (since thats the default for vertex buffers)
	// note how vpp automatically unpacks the std::array
	vpp::fill140(vertexBuffer, vertexData);
}
