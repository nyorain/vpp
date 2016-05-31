#include "particles.hpp"

#include <random>
#include <type_traits>
#include <cassert>
#include <cstring>

App* gApp;

//ParticleSystem
ParticleSystem::ParticleSystem(App& app, std::size_t count)
	: Resource(app.context->device()), app_(app), allocator_(app.context->device())
{
	particles_.resize(count);

	initDescriptors();
	initDescriptorBuffers();
	initParticles();
	initParticleBuffer();

	initGraphicsPipeline();
	initComputePipeline();

	//allocator_.allocate();
	device().memoryAllocator().allocate();

	writeDescriptorSets();
	writeParticleBuffer();
	writeGraphicsUBO();
	buildComputeBuffer();

	lastUpdate_ = Clock::now();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::build(const vpp::RenderPassInstance& instance) const
{
	auto cmdBuffer = instance.vkCommandBuffer();
	VkDeviceSize offsets[1] = { 0 };

	auto gd = graphicsDescriptorSet_.vkDescriptorSet();
	auto buf = particlesBuffer_.vkBuffer();

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics, graphicsPipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
		graphicsPipeline_.vkPipelineLayout(), 0, 1, &gd, 0, nullptr);
	vk::cmdBindVertexBuffers(cmdBuffer, 0, 1, &buf, offsets);
	vk::cmdDraw(cmdBuffer, particles_.size(), 1, 0, 0);
}

std::vector<vk::ClearValue> ParticleSystem::clearValues() const
{
	std::vector<vk::ClearValue> ret(2);
	ret[0].color = {{0.f, 0.f, 0.f, 1.f}};
	ret[1].depthStencil = {1.f, 0};
	return ret;
}

void ParticleSystem::beforeRender(vk::CommandBuffer cmdBuffer) const
{
	vk::BufferMemoryBarrier bufferBarrier;
	bufferBarrier.srcAccessMask = vk::AccessBits::shaderWrite;
	bufferBarrier.dstAccessMask = vk::AccessBits::vertexAttributeRead;
	bufferBarrier.buffer = particlesBuffer_.vkBuffer();
	bufferBarrier.offset = 0;
	bufferBarrier.size = sizeof(Particle) * particles_.size();
	bufferBarrier.srcQueueFamilyIndex = vk::queueFamilyIgnored;
	bufferBarrier.dstQueueFamilyIndex = vk::queueFamilyIgnored;

	vk::cmdPipelineBarrier(cmdBuffer, vk::PipelineStageBits::allCommands,
		vk::PipelineStageBits::topOfPipe, {}, 0, nullptr, 1, &bufferBarrier, 0, nullptr);
}

void ParticleSystem::initGraphicsPipeline()
{
	//vertexBufferLayout
	vertexBufferLayout_ = {
		{
			vk::Format::r32g32b32a32Sfloat, //position
			vk::Format::r32g32b32a32Sfloat, //velocity
			vk::Format::r32g32b32a32Sfloat //color
		},
	0}; //at binding 0

	vpp::GraphicsPipeline::CreateInfo info;
	info.descriptorSetLayouts = {&graphicsDescriptorSetLayout_};
	info.vertexBufferLayouts = {&vertexBufferLayout_};
	info.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};
	info.renderPass = app_.renderPass.vkRenderPass();

	info.shader = vpp::ShaderProgram(device());
	info.shader.addStage({"particles.vert.spv", vk::ShaderStageBits::vertex});
	info.shader.addStage({"particles.frag.spv", vk::ShaderStageBits::fragment});

	info.states = vpp::GraphicsPipeline::StatesCreateInfo{vk::Viewport{0, 0, 900, 900, 0.f, 1.f}};
	info.states.rasterization.cullMode = vk::CullModeBits::none;
	info.states.inputAssembly.topology = vk::PrimitiveTopology::pointList;

	graphicsPipeline_ = vpp::GraphicsPipeline(device(), info);
}
void ParticleSystem::initComputePipeline()
{
	vpp::ComputePipeline::CreateInfo info;
	info.descriptorSetLayouts = {&computeDescriptorSetLayout_};
	info.shader = vpp::ShaderStage(device(), {"particles.comp.spv", vk::ShaderStageBits::compute});

	computePipeline_ = vpp::ComputePipeline(device(), info);
}

void ParticleSystem::initDescriptors()
{
	//init pool
	vk::DescriptorPoolSize typeCounts[2] {};
	typeCounts[0].type = vk::DescriptorType::uniformBuffer;
	typeCounts[0].descriptorCount = 2;

	typeCounts[1].type = vk::DescriptorType::storageBuffer;
	typeCounts[1].descriptorCount = 1;

	vk::DescriptorPoolCreateInfo descriptorPoolInfo;
	descriptorPoolInfo.poolSizeCount = 2;
	descriptorPoolInfo.pPoolSizes = typeCounts;
	descriptorPoolInfo.maxSets = 2;

	vk::createDescriptorPool(vkDevice(), &descriptorPoolInfo, nullptr, &descriptorPool_);


	//graphics set
	std::vector<vpp::DescriptorBinding> gfxBindings =
	{
		{vk::DescriptorType::uniformBuffer, vk::ShaderStageBits::vertex} //contains matrices
	};

	graphicsDescriptorSetLayout_ = vpp::DescriptorSetLayout(device(), gfxBindings);
	graphicsDescriptorSet_ = vpp::DescriptorSet(graphicsDescriptorSetLayout_, descriptorPool_);


	//computeSet
	std::vector<vpp::DescriptorBinding> compBindings =
	{
		{vk::DescriptorType::storageBuffer, vk::ShaderStageBits::compute}, //the particles
		{vk::DescriptorType::uniformBuffer, vk::ShaderStageBits::compute} //delta time, mouse pos
	};

	computeDescriptorSetLayout_ = vpp::DescriptorSetLayout(device(), compBindings);
	computeDescriptorSet_ = vpp::DescriptorSet(computeDescriptorSetLayout_, descriptorPool_);
}

void ParticleSystem::initDescriptorBuffers()
{
	//graphics
	vk::BufferCreateInfo gfxInfo;
	gfxInfo.size = sizeof(nytl::Mat4f) * 2; //viewMatrix, perspectiveMatrix
	gfxInfo.usage = vk::BufferUsageBits::uniformBuffer;

	graphicsUBO_ = vpp::Buffer(device(), gfxInfo, vk::MemoryPropertyBits::hostVisible);

	//compute
	vk::BufferCreateInfo compInfo;
	compInfo.size = sizeof(float) * 5; //mouse pos(vec2f), speed, deltaTime, attract
	compInfo.usage = vk::BufferUsageBits::uniformBuffer;

	computeUBO_ = vpp::Buffer(device(), compInfo, vk::MemoryPropertyBits::hostVisible);
}

void ParticleSystem::initParticles()
{
	std::mt19937 rgen;
	rgen.seed(time(nullptr));
	std::uniform_real_distribution<float> distr(-0.5f, 0.5f);

	for(auto& particle : particles_)
	{
		particle.position.x = distr(rgen);
		particle.position.y = distr(rgen);
		particle.position.z = 0.f;
		particle.position.w = 1.f;

		particle.velocity = nytl::Vec4f{0.f, 0.f, 0.f, 0.f};
		particle.color = nytl::Vec4f(0.0, 0.0, 1.0, 1.0);
	}
}
void ParticleSystem::initParticleBuffer()
{
	vk::BufferCreateInfo bufInfo;
	bufInfo.size = sizeof(Particle) * particles_.size();
	bufInfo.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::storageBuffer;

	particlesBuffer_ = vpp::Buffer(device(), bufInfo, vk::MemoryPropertyBits::deviceLocal);
}

void ParticleSystem::writeParticleBuffer()
{
	// auto map = particlesBuffer_.memoryMap();
	// std::memcpy(map.ptr(), particles_.data(), sizeof(Particle) * particles_.size());
	auto work = particlesBuffer_.fill({particles_});
	work->finish();
}

void ParticleSystem::buildComputeBuffer()
{
	//command pool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex = 0;
	cmdPoolInfo.flags = {};

	vk::createCommandPool(vkDevice(), &cmdPoolInfo, nullptr, &commandPool_);

	//commandBuffer
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool_;
	allocInfo.level = vk::CommandBufferLevel::primary;
	allocInfo.commandBufferCount = 1;

	vk::allocateCommandBuffers(vkDevice(), &allocInfo, &computeBuffer_);

	//build computeBuffer
	vk::CommandBufferBeginInfo cmdBufInfo;
	vk::beginCommandBuffer(computeBuffer_, &cmdBufInfo);

	auto cd = computeDescriptorSet_.vkDescriptorSet();

	vk::cmdBindPipeline(computeBuffer_, vk::PipelineBindPoint::compute, computePipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::compute,
		computePipeline_.vkPipelineLayout(), 0, 1, &cd, 0, nullptr);
	vk::cmdDispatch(computeBuffer_, particles_.size() / 16, 1, 1);

	vk::endCommandBuffer(computeBuffer_);
}

void ParticleSystem::updateUBOs(const nytl::Vec2ui& mousePos)
{
	//compute
	auto now = Clock::now();
	float delta = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(now - lastUpdate_).count();
	lastUpdate_ = now;

	float speed = 15.f;
	float attract = (GetKeyState(VK_LBUTTON) < 0);

	nytl::Vec2f normMousePos = 2 * (mousePos / nytl::Vec2f(app_.width, app_.height)) - 1;

	auto map = computeUBO_.memoryMap();
	std::memcpy(map.ptr(), &delta, sizeof(float));
	std::memcpy(map.ptr() + sizeof(float), &speed, sizeof(float));
	std::memcpy(map.ptr() + sizeof(float) * 2, &attract, sizeof(float));
	std::memcpy(map.ptr() + sizeof(float) * 3, &normMousePos, sizeof(nytl::Vec2f));
}

void ParticleSystem::writeGraphicsUBO()
{
	auto pMat = nytl::perspective3(45.f, 900.f / 900.f, 0.1f, 100.f);
	auto vMat = nytl::identityMat<4, float>();

	auto map = graphicsUBO_.memoryMap();
	std::memcpy(map.ptr(), &pMat, sizeof(nytl::Mat4f));
	std::memcpy(map.ptr() + sizeof(nytl::Mat4f), &vMat, sizeof(nytl::Mat4f));
}

void ParticleSystem::writeDescriptorSets()
{
	//write buffers
	// graphicsDescriptorSet_.writeBuffers(0, {{graphicsUBO_.vkBuffer(), 0, sizeof(nytl::Mat4f) * 2}}, vk::DescriptorType::UniformBuffer);

	// computeDescriptorSet_.writeBuffers(0, {{particlesBuffer_.vkBuffer(), 0, sizeof(Particle) * particles_.size()}}, vk::DescriptorType::StorageBuffer);
	// computeDescriptorSet_.writeBuffers(1, {{computeUBO_.vkBuffer(), 0, sizeof(float) * 5}}, vk::DescriptorType::UniformBuffer);
}

void ParticleSystem::compute()
{
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &computeBuffer_;

	vk::queueSubmit(app_.computeQueue, 1, &submitInfo, 0);
	vk::queueWaitIdle(app_.computeQueue);
}

void ParticleSystem::ParticleSystem::update(const nytl::Vec2ui& mousePos)
{
	updateUBOs(mousePos);
	compute();
}


//utility
void initRenderPass(App& app)
{
	auto& dev = app.context->device();
	auto& swapChain = app.context->swapChain();

	vk::AttachmentDescription attachments[2] {};

	//color from swapchain
	attachments[0].format = swapChain.format();
	attachments[0].samples = vk::SampleCountBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::clear;
	attachments[0].storeOp = vk::AttachmentStoreOp::store;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachments[0].initialLayout = vk::ImageLayout::colorAttachmentOptimal;
	attachments[0].finalLayout = vk::ImageLayout::colorAttachmentOptimal;

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
	attachments[1].initialLayout = vk::ImageLayout::depthStencilAttachmentOptimal;
	attachments[1].finalLayout = vk::ImageLayout::depthStencilAttachmentOptimal;

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
