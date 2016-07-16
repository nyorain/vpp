//Basic exmample on how to use vpp

int main()
{

//Let vpp query the backend and create a context for it.
//Normally you would pass some createInfo as 2nd param wich describes e.g. which
//queues/extensions/layers and
//debug information you want, we just use the defaults here.
//In this case we e.g. create a win32 context by using the win32 createContext overload.
vpp::Context context = vpp::createContext(hwnd, {});

//Context is basically just a all-in-one wrapper for the objects needed to render with vulkan
//If you want fine-grained control over these objects, the Context class is not what you want.
auto& dev = context->device();
auto& ini = context->instance();
auto& swapChain = context->swapChain();
auto& surface = constext->surface();

//create a render pass matching your needs
auto renderPass = createRenderPass();

//To render somethin now, you could e.g. use the vpp::SwapChainRenderer which requires you to
//build your render command buffers, but manages all the rest for you.
class MyRendererBuilder : public vpp::RendererBuilder
{
	//Build the render commands for the given render pass instance.
	void build(unsigned int, const vpp::RenderPassInstance& renderini) const override
	{
		auto cmdBuffer = renderini.vkCommandBuffer();

		vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, <some pipeline>);
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics, <some descriptors>);
		vk::cmdBindVertexBuffers(cmdBuffer, <some vertex buffers>);
		vk::cmdDraw(cmdBuffer, <some draw params>);
	}

	//Clear the color attachment in black and the depth attachment with value 1.
	std::vector<vk::ClearValues> clearValues(unsigned int) const override
	{
		std::vector<vk::ClearValue> ret(2);
		ret[0].color(std::array<float, 4>{{0.f, 0.f, 0.f, 1.f}});
		ret[1].depthStencil({1.f, 0});
		return ret;
	}
} builderObject;

//The SwapChain renderer can be easily constructed and used with the builder implemention from above.
//We simply pass him the swapChain to render on, the builder implementation, the created render pass
//and the vulkan present queue the context automatically creates.
vpp::SwapChainRenderer renderer(swapChain, builderObject, {renderPass, context->presentQueue()});

//Main loop.
while(handleBackendEvents()) renderer.renderBlock();

}
