#include "particles.hpp"
#include <vpp/provider.hpp>
#include <vpp/utility/file.hpp>

#include <random>
#include <type_traits>
#include <cassert>
#include <cstring>

App* gApp;

namespace vpp
{

template<std::size_t R, std::size_t C, typename V> struct VulkanType<nytl::Mat<R, C, V>>
{
	static constexpr auto type = vpp::ShaderType::matrix;
	static constexpr auto major = R;
	static constexpr auto minor = C;
	static constexpr auto transpose = true;
};

template<> struct VulkanType<nytl::Vec2f> { static constexpr auto type = vpp::ShaderType::vec2; };
template<> struct VulkanType<nytl::Vec3f> { static constexpr auto type = vpp::ShaderType::vec3; };
template<> struct VulkanType<nytl::Vec4f> { static constexpr auto type = vpp::ShaderType::vec4; };

template<> struct VulkanType<Particle>
{
	static constexpr auto type = vpp::ShaderType::structure;
	static constexpr auto members = std::make_tuple(
		&Particle::position,
		&Particle::velocity,
		&Particle::color);
};

}

//Renderer
ParticleRenderer::ParticleRenderer(App& app) : app_(&app)
{
	auto& dev = app.context->device();
	computeSemaphore_ = vk::createSemaphore(dev, {});
}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::init(vpp::SwapChainRenderer& renderer)
{
	std::cout << "init\n";
	renderer.record();
	ps().particles_.clear();
}

void ParticleRenderer::build(unsigned int id, const vpp::RenderPassInstance& instance)
{
	std::cout << "building: " << id << "\n";
	auto cmdBuffer = instance.vkCommandBuffer();
	VkDeviceSize offsets[1] = { 0 };

	auto gd = ps().graphicsDescriptorSet_.vkDescriptorSet();
	auto buf1 = ps().particlesBuffer1_.vkBuffer();
	auto buf2 = ps().particlesBuffer2_.vkBuffer();
	auto buf3 = ps().particlesBuffer3_.vkBuffer();
	auto buf4 = ps().particlesBuffer4_.vkBuffer();

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics,
		ps().graphicsPipeline_.vkPipeline());
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
		ps().graphicsPipeline_.vkPipelineLayout(), 0, {gd}, {});

	auto size = ps().count_ / 4;
	vk::cmdBindVertexBuffers(cmdBuffer, 0, {buf1}, offsets);
	vk::cmdDraw(cmdBuffer, size * 4, 1, 0, 0);

	vk::cmdBindVertexBuffers(cmdBuffer, 0, {buf2}, offsets);
	vk::cmdDraw(cmdBuffer, size, 1, 0, 0);

	vk::cmdBindVertexBuffers(cmdBuffer, 0, {buf3}, offsets);
	vk::cmdDraw(cmdBuffer, size, 1, 0, 0);

	vk::cmdBindVertexBuffers(cmdBuffer, 0, {buf4}, offsets);
	vk::cmdDraw(cmdBuffer, size, 1, 0, 0);
}

std::vector<vk::ClearValue> ParticleRenderer::clearValues(unsigned int)
{
	std::vector<vk::ClearValue> ret(2, vk::ClearValue{});
	ret[0].color = {{0.f, 0.f, 0.f, 0.5f}};
	ret[1].depthStencil = {1.f, 0};
	return ret;
}

void ParticleRenderer::beforeRender(vk::CommandBuffer cmdBuffer)
{
	vk::BufferMemoryBarrier bufferBarrier;
	bufferBarrier.srcAccessMask = vk::AccessBits::shaderWrite | vk::AccessBits::shaderRead;
	bufferBarrier.dstAccessMask = vk::AccessBits::vertexAttributeRead;
	bufferBarrier.buffer = ps().particlesBuffer1_.vkBuffer();
	bufferBarrier.offset = 0;
	bufferBarrier.size = sizeof(Particle) * ps().particles_.size() / 4;
	bufferBarrier.srcQueueFamilyIndex = vk::queueFamilyIgnored;
	bufferBarrier.dstQueueFamilyIndex = vk::queueFamilyIgnored;

	vk::cmdPipelineBarrier(cmdBuffer, vk::PipelineStageBits::computeShader,
		vk::PipelineStageBits::vertexShader, {}, {}, {&bufferBarrier}, {});
}

ParticleRenderer::AdditionalSemaphores ParticleRenderer::submit(unsigned int id)
{
	auto& dev = app_->context->device();

	//submit command buffer
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &ps().computeBuffer_;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &computeSemaphore_;

	dev.submitManager().add(*app_->context->computeQueue(), submitInfo);

	return {{computeSemaphore_, vk::PipelineStageBits::allCommands}};
	//return {};
}

//ParticleSystem
ParticleSystem::ParticleSystem(App& app, std::size_t count)
	: Resource(app.context->device()), app_(app), allocator_(app.context->device())
{
	particles_.resize(count);
	count_ = count;

	initDescriptors();
	initDescriptorBuffers();
	initParticles();
	initParticleBuffer();

	initComputePipeline();
	initGraphicsPipeline();

	//allocator_.allocate();
	device().deviceAllocator().allocate();

	writeDescriptorSets();
	writeParticleBuffer();
	writeGraphicsUBO();

	buildComputeBuffer();

	lastUpdate_ = Clock::now();
}

ParticleSystem::~ParticleSystem()
{
}


void ParticleSystem::initGraphicsPipeline()
{
	//vertexBufferLayout
	vertexBufferLayout_ = {
		{
			vk::Format::r32g32Sfloat, //position
			vk::Format::r32g32Sfloat, //velocity
			vk::Format::r32g32b32a32Sfloat //color
		},
	0}; //at binding 0

	vk::PipelineCache cache;
	if(vpp::fileExists("gfxCache")) cache = loadPipelineCache(device(), "gfxCache");
	else cache = vk::createPipelineCache(device(), {});

	vpp::GraphicsPipeline::CreateInfo info;
	info.descriptorSetLayouts = {graphicsDescriptorSetLayout_};
	info.vertexBufferLayouts = {vertexBufferLayout_};
	info.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};
	info.renderPass = app_.renderPass.vkRenderPass();
	info.cache = cache;

	info.shader = vpp::ShaderProgram(device());
	info.shader.stage("particles.vert.spv", {vk::ShaderStageBits::vertex});
	info.shader.stage("particles.frag.spv", {vk::ShaderStageBits::fragment});

	info.states = vpp::GraphicsPipeline::States(vk::Viewport{0, 0, 900, 900, 0.f, 1.f});
	info.states.rasterization.cullMode = vk::CullModeBits::none;
	info.states.inputAssembly.topology = vk::PrimitiveTopology::pointList;

	nytl::Timer timer;
	graphicsPipeline_ = vpp::GraphicsPipeline(device(), info);
	std::cout << timer.elapsedTime().milliseconds() << " to create the gfx pipeline.\n";

	vpp::savePipelineCache(device(), cache, "gfxCache");
	vk::destroyPipelineCache(device(), cache);
}
void ParticleSystem::initComputePipeline()
{
	vk::PipelineCache cache;
	if(vpp::fileExists("compCache")) cache = loadPipelineCache(device(), "compCache");
	else cache = vk::createPipelineCache(device(), {});

	vpp::ComputePipeline::CreateInfo info;
	info.descriptorSetLayouts = {&computeDescriptorSetLayout_};
	info.shader = vpp::ShaderStage(device(), "particles.comp.spv", {vk::ShaderStageBits::compute});
	info.cache = cache;

	nytl::Timer timer;
	computePipeline_ = vpp::ComputePipeline(device(), info);
	std::cout << timer.elapsedTime().milliseconds() << " to create the compute pipeline.\n";

	vpp::savePipelineCache(device(), cache, "compCache");
	vk::destroyPipelineCache(device(), cache);
}

void ParticleSystem::initDescriptors()
{
	//init pool
	vk::DescriptorPoolSize typeCounts[2] {};
	typeCounts[0].type = vk::DescriptorType::uniformBuffer;
	typeCounts[0].descriptorCount = 5;

	typeCounts[1].type = vk::DescriptorType::storageBuffer;
	typeCounts[1].descriptorCount = 4;

	vk::DescriptorPoolCreateInfo descriptorPoolInfo;
	descriptorPoolInfo.poolSizeCount = 2;
	descriptorPoolInfo.pPoolSizes = typeCounts;
	descriptorPoolInfo.maxSets = 5;

	descriptorPool_ = vk::createDescriptorPool(vkDevice(), descriptorPoolInfo);


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

	computeDescriptorSet1_ = vpp::DescriptorSet(computeDescriptorSetLayout_, descriptorPool_);
	computeDescriptorSet2_ = vpp::DescriptorSet(computeDescriptorSetLayout_, descriptorPool_);
	computeDescriptorSet3_ = vpp::DescriptorSet(computeDescriptorSetLayout_, descriptorPool_);
	computeDescriptorSet4_ = vpp::DescriptorSet(computeDescriptorSetLayout_, descriptorPool_);
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
	std::uniform_real_distribution<float> distr(-0.85f, 0.85f);

	for(auto& particle : particles_)
	{
		particle.position.x = distr(rgen);
		particle.position.y = distr(rgen);

		particle.velocity = nytl::Vec2f{0.f, 0.f};
		particle.color = nytl::Vec4f(0.0, 0.0, 1.0, 1.0);
	}
}
void ParticleSystem::initParticleBuffer()
{
	vk::BufferCreateInfo bufInfo;
	bufInfo.size = sizeof(Particle) * particles_.size() / 4;
	bufInfo.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::storageBuffer | vk::BufferUsageBits::transferDst;

	particlesBuffer1_ = vpp::Buffer(device(), bufInfo, 1);
	particlesBuffer2_ = vpp::Buffer(device(), bufInfo, 1);
	particlesBuffer3_ = vpp::Buffer(device(), bufInfo, 1);
	particlesBuffer4_ = vpp::Buffer(device(), bufInfo, 1);
}

void ParticleSystem::writeParticleBuffer()
{
	// particlesBuffer1_.assureMemory();
	// std::cout << "deviceLocal: " << (particlesBuffer_.memoryEntry().memory()->propertyFlags() & vk::MemoryPropertyBits::deviceLocal) << "\n";
	// std::cout << "size: " << particlesBuffer_.size() << "\n";
	// auto map = particlesBuffer_.memoryMap();
	// std::memcpy(map.ptr(), particles_.data(), sizeof(Particle) * particles_.size());
	auto size = particles_.size() / 4;
	// auto work = particlesBuffer1_.fill({particles_.data(), sizeof(Particle) * size});
	// auto work2 = particlesBuffer2_.fill({particles_.data() + size, sizeof(Particle) * size});
	// auto work3 = particlesBuffer3_.fill({particles_.data() + 2 * size, sizeof(Particle) * size});
	// auto work4 = particlesBuffer4_.fill({particles_.data() + 3 * size, sizeof(Particle) * size});
	//
	// //std::cout << "finish...\n";
	// work->finish();
	//  work2->finish();
	//  work3->finish();
	//  work4->finish();

	std::vector<Particle> p1(particles_.begin(), particles_.begin() + size);
	std::vector<Particle> p2(particles_.begin() + size, particles_.begin() + 2 * size);
	std::vector<Particle> p3(particles_.begin() + 2 * size, particles_.begin() + 3 * size);
	std::vector<Particle> p4(particles_.begin() + 3 * size, particles_.begin() + 4 * size);

	vpp::BufferUpdate u1(particlesBuffer1_);
	u1.add(p1);

	vpp::BufferUpdate u2(particlesBuffer2_);
	u2.add(p2);

	vpp::BufferUpdate u3(particlesBuffer3_);
	u3.add(p3);

	vpp::BufferUpdate u4(particlesBuffer4_);
	u4.add(p4);

	//particlesBuffer1_.fill140(particles_);
}

void ParticleSystem::buildComputeBuffer()
{
	computeBuffer_ = device().commandProvider().get(0);

	//build computeBuffer
	vk::CommandBufferBeginInfo cmdBufInfo;
	vk::beginCommandBuffer(computeBuffer_, cmdBufInfo);

	auto cd1 = computeDescriptorSet1_.vkDescriptorSet();
	auto cd2 = computeDescriptorSet2_.vkDescriptorSet();
	auto cd3 = computeDescriptorSet3_.vkDescriptorSet();
	auto cd4 = computeDescriptorSet4_.vkDescriptorSet();

	vk::cmdBindPipeline(computeBuffer_, vk::PipelineBindPoint::compute, computePipeline_.vkPipeline());

	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::compute,
		computePipeline_.vkPipelineLayout(), 0, {cd1}, {});
	vk::cmdDispatch(computeBuffer_, particles_.size() / 64, 1, 1);

	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::compute,
		computePipeline_.vkPipelineLayout(), 0, {cd2}, {});
	vk::cmdDispatch(computeBuffer_, particles_.size() / 64, 1, 1);

	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::compute,
		computePipeline_.vkPipelineLayout(), 0, {cd3}, {});
	vk::cmdDispatch(computeBuffer_, particles_.size() / 64, 1, 1);

	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::compute,
		computePipeline_.vkPipelineLayout(), 0, {cd4}, {});
	vk::cmdDispatch(computeBuffer_, particles_.size() / 64, 1, 1);

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
	// std::memcpy(map.ptr(), &pMat, sizeof(nytl::Mat4f));
	// std::memcpy(map.ptr() + sizeof(nytl::Mat4f), &vMat, sizeof(nytl::Mat4f));

	vpp::BufferUpdate update(graphicsUBO_);
	update.add(pMat, vMat);
}

void ParticleSystem::writeDescriptorSets()
{
	vpp::DescriptorSetUpdate gfx(graphicsDescriptorSet_);
	gfx.uniform({{graphicsUBO_, 0, sizeof(nytl::Mat4f) * 2}});

	vpp::DescriptorSetUpdate comp1(computeDescriptorSet1_);
	comp1.storage({{particlesBuffer1_, 0, sizeof(Particle) * particles_.size() / 4}});
	comp1.uniform({{computeUBO_, 0, sizeof(float) * 5}});

	vpp::DescriptorSetUpdate comp2(computeDescriptorSet2_);
	comp2.storage({{particlesBuffer2_, 0, sizeof(Particle) * particles_.size() / 4}});
	comp2.uniform({{computeUBO_, 0, sizeof(float) * 5}});

	vpp::DescriptorSetUpdate comp3(computeDescriptorSet3_);
	comp3.storage({{particlesBuffer3_, 0, sizeof(Particle) * particles_.size() / 4}});
	comp3.uniform({{computeUBO_, 0, sizeof(float) * 5}});

	vpp::DescriptorSetUpdate comp4(computeDescriptorSet4_);
	comp4.storage({{particlesBuffer4_, 0, sizeof(Particle) * particles_.size() / 4}});
	comp4.uniform({{computeUBO_, 0, sizeof(float) * 5}});

	vpp::apply({gfx, comp1, comp2, comp3, comp4});
}

void ParticleSystem::compute()
{
	/*
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &computeBuffer_;

	vk::queueSubmit(app_.computeQueue, {submitInfo}, 0);
	vk::queueWaitIdle(app_.computeQueue);
	*/
}

void ParticleSystem::ParticleSystem::update(const nytl::Vec2ui& mousePos)
{
	updateUBOs(mousePos);
	//compute();
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
