#include "clouds.hpp"

App* gApp = nullptr;

//FragmentRenderer
FragmentRenderer::FragmentRenderer(App& app)
	: Resource(app.context->device()), app_(app)
{
	initDescriptors();
	initDescriptorBuffers();
	initPipeline();

	//it is used to write the descriptor set
	ubo_.assureMemory();

	writeDescriptorSets();

	lastUpdate_ = Clock::now();
}

FragmentRenderer::~FragmentRenderer()
{
}

void FragmentRenderer::build(const vpp::RenderPassInstance& instance) const
{
	auto cmdBuffer = instance.vkCommandBuffer();
	VkDeviceSize offsets[1] = { 0 };

	auto gd = descriptorSet_.vkDescriptorSet();

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, pipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
		pipeline_.vkPipelineLayout(), 0, 1, &gd, 0, nullptr);
	vk::cmdDraw(cmdBuffer, 6, 1, 0, 0);
}

std::vector<vk::ClearValue> FragmentRenderer::clearValues() const
{
	std::vector<vk::ClearValue> ret(2);
	ret[0].color(std::array<float, 4>{{0.f, 0.f, 0.f, 1.f}});
	ret[1].depthStencil({1.f, 0});
	return ret;
}

void FragmentRenderer::initPipeline()
{
	vpp::GraphicsPipeline::CreateInfo info;
	info.descriptorSetLayouts = {&descriptorSetLayout_};
	info.dynamicStates = {};
	info.renderPass = app_.renderPass.vkRenderPass();

	info.shader = vpp::ShaderProgram(device());
	info.shader.addStage({"clouds.vert.spv", vk::ShaderStageFlagBits::Vertex});
	info.shader.addStage({"clouds.frag.spv", vk::ShaderStageFlagBits::Fragment});

	info.states = vpp::GraphicsPipeline::StatesCreateInfo{vk::Viewport{0, 0, 900, 900, 0.f, 1.f}};
	info.states.rasterization.cullMode(vk::CullModeFlagBits::None);
	info.states.inputAssembly.topology(vk::PrimitiveTopology::TriangleList);

	pipeline_ = vpp::GraphicsPipeline(device(), info);
}

void FragmentRenderer::initDescriptors()
{
	//init pool
	vk::DescriptorPoolSize typeCounts[2] {};
	typeCounts[0].type(vk::DescriptorType::UniformBuffer);
	typeCounts[0].descriptorCount(2);

	vk::DescriptorPoolCreateInfo descriptorPoolInfo;
	descriptorPoolInfo.poolSizeCount(1);
	descriptorPoolInfo.pPoolSizes(typeCounts);
	descriptorPoolInfo.maxSets(1);

	vk::createDescriptorPool(vkDevice(), &descriptorPoolInfo, nullptr, &descriptorPool_);

	//graphics set
	std::vector<vpp::DescriptorBinding> gfxBindings =
	{
		{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Fragment} //fragment stuff
	};

	descriptorSetLayout_ = vpp::DescriptorSetLayout(device(), gfxBindings);
	descriptorSet_ = vpp::DescriptorSet(descriptorSetLayout_, descriptorPool_);
}

void FragmentRenderer::initDescriptorBuffers()
{
	vk::BufferCreateInfo gfxInfo;
	gfxInfo.size(sizeof(float)); //viewMatrix, perspectiveMatrix
	gfxInfo.usage(vk::BufferUsageFlagBits::UniformBuffer);

	ubo_ = vpp::Buffer(device(), gfxInfo, vk::MemoryPropertyFlagBits::HostVisible);
}

void FragmentRenderer::updateUBO(const nytl::Vec2ui& mousePos)
{
	//compute
	static float time = 0.f;

	auto now = Clock::now();
	float delta = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(now - lastUpdate_).count();
	lastUpdate_ = now;

	float speed = 15.f;
	float attract = (GetKeyState(VK_LBUTTON) < 0);

	nytl::Vec2f normMousePos = 2 * (mousePos / nytl::Vec2f(app_.width, app_.height)) - 1;

	time += delta;

	auto map = ubo_.memoryMap();
	std::memcpy(map.ptr(), &time, sizeof(float));
}

void FragmentRenderer::writeDescriptorSets()
{
	//write buffers
	descriptorSet_.writeBuffers(0, {{ubo_.vkBuffer(), 0, sizeof(nytl::Mat4f) * 2}},
		vk::DescriptorType::UniformBuffer);
}

void FragmentRenderer::update(const nytl::Vec2ui& mousePos)
{
	updateUBO(mousePos);
}

//utility
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

/*
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
*/

	//only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint(vk::PipelineBindPoint::Graphics);
	subpass.flags({});
	subpass.inputAttachmentCount(0);
	subpass.pInputAttachments(nullptr);
	subpass.colorAttachmentCount(1);
	subpass.pColorAttachments(&colorReference);
	subpass.pResolveAttachments(nullptr);
	subpass.pDepthStencilAttachment(nullptr); //XXX
	subpass.preserveAttachmentCount(0);
	subpass.pPreserveAttachments(nullptr);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount(1); //XXX
	renderPassInfo.pAttachments(attachments);
	renderPassInfo.subpassCount(1);
	renderPassInfo.pSubpasses(&subpass);
	renderPassInfo.dependencyCount(0);
	renderPassInfo.pDependencies(nullptr);

	app.renderPass = vpp::RenderPass(dev, renderPassInfo);
}
