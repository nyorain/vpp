//just ideas how the api should work

#include <vpp/vpp.hpp>

std::vector<float> vertices =
{
	{{1.0f,  0.6f, 0.0f}, { 1.0f, 0.0f, 0.0f}},
	{{-1.0f,  1.0f, 0.0f}, { 0.0f, 1.0f, 0.0f}},
	{{0.0f, -1.0f, 0.0f}, { 0.0f, 0.0f, 1.0f}}
};

int main()
{
	vpp::ContextSettings settings = { /* ... */ };
	
	auto window = initWindow();
	auto context = vpp::Win32Context(settings, hinstance, window);

	vpp::VertexBufferLayout vbLayout({vpp::VertexBufferLayout::Point3fColor3f});
	vpp::DescriptorSetLayout dsLayout(context.device(), {vk::DescriptorType::UniformBuffer});

	vpp::GraphicsPipeline::CreateInfo createInfo;
	createInfo.renderPass = context.swapChain().vkRenderPass();
	createInfo.vertexBufferLayouts = vblayout;
	createInfo.descriptorSetLayouts = dsLayout;
	createInfo.shaderProgram = vpp::ShaderProgram({
			{vk::ShaderStageFlags::Vertex, "vert.sprv"}, 
			{vk::ShaderStageFlags::Fragment, "frag.sprv"}
		});

	vpp::GraphicsPipeline pipeline(context.device(), createInfo);

	//buffer, descriptors
	vpp::Buffer vertexBuffer(context.device(), vertices);
	vpp::Buffer uniformBuffer(context.device(), someTransformMatrix);

	vpp::DescriptorSet descriptorSet(dsLayout);
	static_cast<vpp::BufferDescriptor>(descriptorSet[0]).write(uniformBuffer); //#1
	descriptorSet.writeBuffers(0, {uniformBuffer}) //#2; 

	//later
	pipeline.drawCommands(commandBuffer, {vertexBuffer}, {descriptorSet});
}
