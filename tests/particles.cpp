#include "particles.hpp"

#include <random>
#include <type_traits>
#include <cassert>

static_assert(sizeof(float) == 4, "fail");
static_assert(std::is_standard_layout<nytl::Vec3f>::value, "fail2");
static_assert(sizeof(nytl::Vec3f) == sizeof(float) * 3, "fail3");
static_assert(sizeof(nytl::Vec4f) == sizeof(float) * 4, "fail3");

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

	allocator_.allocate();

	writeDescriptorSets();
	writeParticleBuffer();
	writeGraphicsUBO();
	buildComputeBuffer();

	lastUpdate_ = Clock::now();

	assert(particles_.size() == count);
	assert((particlesBuffer_.memoryEntry().offset() % device().properties().limits().minStorageBufferOffsetAlignment()) == 0);
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

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::Graphics, graphicsPipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::Graphics,
		graphicsPipeline_.vkPipelineLayout(), 0, 1, &gd, 0, nullptr);
	vk::cmdBindVertexBuffers(cmdBuffer, 0, 1, &buf, offsets);
	vk::cmdDraw(cmdBuffer, particles_.size(), 1, 0, 0);
}

std::vector<vk::ClearValue> ParticleSystem::clearValues() const
{
	std::vector<vk::ClearValue> ret(2);
	ret[0].color(std::array<float, 4>{{1.f, 1.f, 1.f, 1.f}});
	ret[1].depthStencil({1.f, 0});
	return ret;
}

void ParticleSystem::beforeRender(vk::CommandBuffer cmdBuffer) const
{
	vk::BufferMemoryBarrier bufferBarrier;
	bufferBarrier.srcAccessMask(vk::AccessFlagBits::ShaderWrite);
	bufferBarrier.dstAccessMask(vk::AccessFlagBits::VertexAttributeRead);
	bufferBarrier.buffer(particlesBuffer_.vkBuffer());
	bufferBarrier.offset(0);
	bufferBarrier.size(sizeof(Particle) * particles_.size());
	bufferBarrier.srcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
	bufferBarrier.dstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);

	vk::cmdPipelineBarrier(
		cmdBuffer,
		vk::PipelineStageFlagBits::AllCommands,
		vk::PipelineStageFlagBits::TopOfPipe,
		{},
		0, nullptr,
		1, &bufferBarrier,
		0, nullptr);
}

void ParticleSystem::initGraphicsPipeline()
{
	//vertexBufferLayout
	vertexBufferLayout_ = {
		{
			vk::Format::R32G32B32A32Sfloat, //position
			vk::Format::R32G32B32A32Sfloat, //velocity
			vk::Format::R32G32B32A32Sfloat //color
		},
	0}; //at binding 0

	vpp::GraphicsPipeline::CreateInfo info;
	info.descriptorSetLayouts = {&graphicsDescriptorSetLayout_};
	info.vertexBufferLayouts = {&vertexBufferLayout_};
	info.dynamicStates = {vk::DynamicState::Viewport, vk::DynamicState::Scissor};
	info.renderPass = app_.renderPass.vkRenderPass();

	info.shader.init(device());
	info.shader.addStage({"vert.spv", vk::ShaderStageFlagBits::Vertex});
	info.shader.addStage({"frag.spv", vk::ShaderStageFlagBits::Fragment});

	info.states = vpp::GraphicsPipeline::StatesCreateInfo{vk::Viewport{0, 0, 900, 900, 0.f, 1.f}};
	info.states.rasterization.cullMode(vk::CullModeFlagBits::None);
	info.states.inputAssembly.topology(vk::PrimitiveTopology::PointList);

	graphicsPipeline_ = vpp::GraphicsPipeline(device(), info);
}
void ParticleSystem::initComputePipeline()
{
	vpp::ComputePipeline::CreateInfo info;
	info.descriptorSetLayouts = {&computeDescriptorSetLayout_};
	info.shader.init(device(), {"comp.spv", vk::ShaderStageFlagBits::Compute});

	computePipeline_ = vpp::ComputePipeline(device(), info);
}

void ParticleSystem::initDescriptors()
{
	//init pool
	vk::DescriptorPoolSize typeCounts[2] {};
	typeCounts[0].type(vk::DescriptorType::UniformBuffer);
	typeCounts[0].descriptorCount(2);

	typeCounts[1].type(vk::DescriptorType::StorageBuffer);
	typeCounts[1].descriptorCount(1);

	vk::DescriptorPoolCreateInfo descriptorPoolInfo;
	descriptorPoolInfo.poolSizeCount(2);
	descriptorPoolInfo.pPoolSizes(typeCounts);
	descriptorPoolInfo.maxSets(2);

	vk::createDescriptorPool(vkDevice(), &descriptorPoolInfo, nullptr, &descriptorPool_);


	//graphics set
	std::vector<vpp::DescriptorBinding> gfxBindings =
	{
		{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Vertex} //contains matrices
	};

	graphicsDescriptorSetLayout_ = vpp::DescriptorSetLayout(device(), gfxBindings);
	graphicsDescriptorSet_ = vpp::DescriptorSet(graphicsDescriptorSetLayout_, descriptorPool_);


	//computeSet
	/*
	std::vector<vpp::DescriptorBinding> compBindings =
	{
		{vk::DescriptorType::StorageBuffer, vk::ShaderStageFlagBits::Compute}, //the particles
		{vk::DescriptorType::UniformBuffer, vk::ShaderStageFlagBits::Compute} //delta time, mouse pos
	};
	*/

	std::vector<vpp::DescriptorBinding> compBindings =
	{
		{vk::DescriptorType::StorageBuffer, vk::ShaderStageFlagBits::Compute} //the particles
	};

	computeDescriptorSetLayout_ = vpp::DescriptorSetLayout(device(), compBindings);
	computeDescriptorSet_ = vpp::DescriptorSet(computeDescriptorSetLayout_, descriptorPool_);
}

void ParticleSystem::initDescriptorBuffers()
{
	//graphics
	vk::BufferCreateInfo gfxInfo;
	gfxInfo.size(sizeof(nytl::Mat4f) * 2); //viewMatrix, perspectiveMatrix
	gfxInfo.usage(vk::BufferUsageFlagBits::UniformBuffer);

	graphicsUBO_ = vpp::Buffer(allocator_, gfxInfo, vk::MemoryPropertyFlagBits::HostVisible);

	//compute
	vk::BufferCreateInfo compInfo;
	compInfo.size(sizeof(nytl::Vec2f) + sizeof(std::uint32_t)); //mouse pos.x, mouse pos.y, delta time
	compInfo.usage(vk::BufferUsageFlagBits::UniformBuffer);

	computeUBO_ = vpp::Buffer(allocator_, compInfo, vk::MemoryPropertyFlagBits::HostVisible);
}

void ParticleSystem::initParticles()
{
	std::mt19937 rgen;
	rgen.seed(time(nullptr));
	std::uniform_real_distribution<float> distr(-1.f, 1.f);

	for(auto& particle : particles_)
	{
		/*
		particle.position.x = distr(rgen);
		particle.position.y = distr(rgen);
		particle.position.z = 0.f;
		*/

		particle.position = nytl::Vec4f{0.f, 0.f, 0.f, 1.f};
		particle.velocity = nytl::Vec4f{0.f, 0.f, 0.f, 0.f};
		particle.color = nytl::Vec4f(0.0, 0.0, 1.0, 1.0);
	}
}
void ParticleSystem::initParticleBuffer()
{
	vk::BufferCreateInfo bufInfo;
	bufInfo.size(sizeof(Particle) * particles_.size());
	bufInfo.usage(vk::BufferUsageFlagBits::VertexBuffer | vk::BufferUsageFlagBits::StorageBuffer);

	particlesBuffer_ = vpp::Buffer(allocator_, bufInfo, vk::MemoryPropertyFlagBits::HostVisible);
}

void ParticleSystem::writeParticleBuffer()
{
	auto map = particlesBuffer_.memoryMap();
	std::memcpy(map.ptr(), particles_.data(), sizeof(Particle) * particles_.size());
}

void ParticleSystem::buildComputeBuffer()
{
	//command pool
	vk::CommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.queueFamilyIndex(0);
	cmdPoolInfo.flags({});

	vk::createCommandPool(vkDevice(), &cmdPoolInfo, nullptr, &commandPool_);

	//commandBuffer
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool(commandPool_);
	allocInfo.level(vk::CommandBufferLevel::Primary);
	allocInfo.commandBufferCount(1);

	vk::allocateCommandBuffers(vkDevice(), &allocInfo, &computeBuffer_);

	//build computeBuffer
	vk::CommandBufferBeginInfo cmdBufInfo;
	vk::beginCommandBuffer(computeBuffer_, cmdBufInfo);

	auto cd = computeDescriptorSet_.vkDescriptorSet();

	vk::cmdBindPipeline(computeBuffer_, vk::PipelineBindPoint::Compute, computePipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::Compute,
		computePipeline_.vkPipelineLayout(), 0, 1, &cd, 0, nullptr);
	vk::cmdDispatch(computeBuffer_, particles_.size() / 16, 1, 1);

	vk::endCommandBuffer(computeBuffer_);
}

void ParticleSystem::updateUBOs(const nytl::Vec2ui& mousePos)
{
	//compute
	auto now = Clock::now();
	std::uint32_t delta = 1;
	lastUpdate_ = now;

	nytl::Vec2f normMousePos = 2 * (mousePos / nytl::Vec2f(app_.width, app_.height)) - 1;

	auto map = computeUBO_.memoryMap();
	std::memcpy(map.ptr(), &delta, sizeof(std::uint32_t));
	std::memcpy(map.ptr() + sizeof(std::uint32_t), &normMousePos, sizeof(nytl::Vec2f));
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
	graphicsDescriptorSet_.writeBuffers(0, {{graphicsUBO_.vkBuffer(), 0, sizeof(nytl::Mat4f) * 2}}, vk::DescriptorType::UniformBuffer);

	computeDescriptorSet_.writeBuffers(0, {{particlesBuffer_.vkBuffer(), 0, sizeof(Particle) * particles_.size()}}, vk::DescriptorType::StorageBuffer);
	//computeDescriptorSet_.writeBuffers(1, {{computeUBO_.vkBuffer(), 0, sizeof(float) * 2 + sizeof(std::uint32_t)}}, vk::DescriptorType::UniformBuffer);
}

void ParticleSystem::compute()
{
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount(1);
	submitInfo.pCommandBuffers(&computeBuffer_);

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
