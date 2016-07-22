#include <example.hpp> //defnitions for initApp and mainLoop as well as the App declaration

#include <vpp/provider.hpp>
#include <vpp/utility/file.hpp>
#include <vpp/buffer.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/computePipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>

#include <nytl/vec.hpp>
#include <nytl/mat.hpp>
#include <nytl/transform.hpp>

#include <random>
#include <type_traits>
#include <cassert>
#include <cstring>
#include <chrono>
using Clock = std::chrono::high_resolution_clock;

//types
struct Particle
{
	nytl::Vec2f position;
	nytl::Vec2f velocity;
	nytl::Vec2f color;
};

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

class ParticleSystem : public vpp::Resource
{
protected:
	App& app_;
	std::vector<Particle> particles_;
	vpp::Buffer particlesBuffer_;

	vpp::VertexBufferLayout vertexBufferLayout_;
	vpp::Pipeline computePipeline_;
	vpp::Pipeline graphicsPipeline_;

	vpp::PipelineLayout graphicsLayout_;
	vpp::PipelineLayout computeLayout_;


	vpp::CommandBuffer computeBuffer_;
	vpp::DescriptorPool descriptorPool_;

	vpp::DescriptorSetLayout computeDescriptorSetLayout_;
	vpp::DescriptorSet computeDescriptorSet_;
	vpp::Buffer computeUBO_;

	vpp::DescriptorSetLayout graphicsDescriptorSetLayout_;
	vpp::DescriptorSet graphicsDescriptorSet_;
	vpp::Buffer graphicsUBO_;

	std::size_t count_;
	bool initialized_ = false;

	//ubo values
	nytl::Mat4f viewMatrix_; //camera
	Clock::time_point lastUpdate_;

protected:
	void init();
	void buildComputeBuffer();

	void initGraphicsPipeline();
	void initComputePipeline();

	void initDescriptors();
	void initDescriptorBuffers();
	void initParticles();
	void initParticleBuffer();

	void writeDescriptorSets();
	void writeParticleBuffer();
	void writeGraphicsUBO();

public:
	ParticleSystem(App& app, std::size_t count);
	~ParticleSystem();

	void update(const nytl::Vec2ui& mousePos);
	friend class ParticleRenderer;
};

class ParticleRenderer : public vpp::RendererBuilder
{
public:
	ParticleRenderer(ParticleSystem& sys);
	~ParticleRenderer();

	void init(vpp::SwapChainRenderer& renderer) override;
	void build(unsigned int, const vpp::RenderPassInstance&) override;
	std::vector<vk::ClearValue> clearValues(unsigned int) override;
	void beforeRender(vk::CommandBuffer cmdBuffer) override;
	AdditionalSemaphores submit(unsigned int id) override;

	ParticleSystem& ps() const { return *system_; }

protected:
	ParticleSystem* system_;
	vk::Semaphore computeSemaphore_;

	std::vector<vk::PipelineStageFlags> waitMasks_;
	std::vector<vk::CommandBuffer> buffers_;
	std::vector<vk::Semaphore> waitSemaphores_;
	std::vector<vk::Semaphore> signalSemaphores_;
};

//Renderer
ParticleRenderer::ParticleRenderer(ParticleSystem& sys) : system_(&sys)
{
	auto& dev = ps().app_.context.device();
	computeSemaphore_ = vk::createSemaphore(dev, {});
}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::init(vpp::SwapChainRenderer& renderer)
{
	if(!ps().initialized_)ps().init();
	ps().initialized_ = true;
	renderer.record();
	ps().particles_.clear();
}

void ParticleRenderer::build(unsigned int id, const vpp::RenderPassInstance& instance)
{
	auto cmdBuffer = instance.vkCommandBuffer();
	VkDeviceSize offsets[1] = { 0 };

	auto gd = ps().graphicsDescriptorSet_.vkHandle();
	auto buf = ps().particlesBuffer_.vkHandle();

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics,
		ps().graphicsPipeline_);
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
		ps().graphicsLayout_, 0, {gd}, {});
	vk::cmdBindVertexBuffers(cmdBuffer, 0, {buf}, offsets);
	vk::cmdDraw(cmdBuffer, ps().count_, 1, 0, 0);
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
	bufferBarrier.buffer = ps().particlesBuffer_;
	bufferBarrier.offset = 0;
	bufferBarrier.size = sizeof(Particle) * ps().count_;
	bufferBarrier.srcQueueFamilyIndex = vk::queueFamilyIgnored;
	bufferBarrier.dstQueueFamilyIndex = vk::queueFamilyIgnored;

	// vk::BufferMemoryBarrier uniformBarrier;
	// bufferBarrier.srcAccessMask = vk::AccessBits::hostWrite;
	// bufferBarrier.dstAccessMask = vk::AccessBits::uniformRead;
	// bufferBarrier.buffer = ps().graphicsUBO_;
	// bufferBarrier.offset = 0;
	// bufferBarrier.size = ps().graphicsUBO_.size();
	// bufferBarrier.srcQueueFamilyIndex = vk::queueFamilyIgnored;
	// bufferBarrier.dstQueueFamilyIndex = vk::queueFamilyIgnored;

	// vk::cmdPipelineBarrier(cmdBuffer, vk::PipelineStageBits::topOfPipe,
	// 	vk::PipelineStageBits::bottomOfPipe, {}, {}, {bufferBarrier, uniformBarrier}, {});

	vk::cmdPipelineBarrier(cmdBuffer, vk::PipelineStageBits::topOfPipe,
		vk::PipelineStageBits::bottomOfPipe, {}, {}, {bufferBarrier}, {});
}

ParticleRenderer::AdditionalSemaphores ParticleRenderer::submit(unsigned int id)
{
	auto& dev = ps().app_.context.device();

	//submit command buffer
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &ps().computeBuffer_.vkHandle();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &computeSemaphore_;

	dev.submitManager().add(*ps().app_.context.graphicsComputeQueue(), submitInfo);

	return {{computeSemaphore_, vk::PipelineStageBits::allCommands}};
	//return {};
}

//ParticleSystem
ParticleSystem::ParticleSystem(App& app, std::size_t count)
	: Resource(), app_(app)
{
	count_ = count;
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::init()
{
	Resource::init(app_.context.device());
	particles_.resize(count_);

	initDescriptors();
	initDescriptorBuffers();
	initParticles();
	initParticleBuffer();

	initGraphicsPipeline();
	initComputePipeline();

	//allocator_.allocate();
	device().deviceAllocator().allocate();

	writeDescriptorSets();
	writeParticleBuffer();
	writeGraphicsUBO();

	buildComputeBuffer();

	lastUpdate_ = Clock::now();
}

void ParticleSystem::initGraphicsPipeline()
{
	//vertexBufferLayout
	vertexBufferLayout_ = {
		{
			vk::Format::r32g32Sfloat, //position
			vk::Format::r32g32Sfloat, //velocity
			vk::Format::r32g32Sfloat //color (only r and g values)
		},
	0}; //at binding 0

	graphicsLayout_ = {device(), {graphicsDescriptorSetLayout_}};

	vpp::PipelineCache cache(device());
	if(vpp::fileExists("gfxCache")) cache = {device(), "gfxCache"};

	vpp::GraphicsPipelineBuilder builder(device(), app_.renderPass);
	builder.vertexBufferLayouts = {vertexBufferLayout_};
	builder.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};
	builder.layout = graphicsLayout_;

	builder.shader.stage("particles.vert.spv", {vk::ShaderStageBits::vertex});
	builder.shader.stage("particles.frag.spv", {vk::ShaderStageBits::fragment});

	builder.states.blendAttachments[0].blendEnable = true;
	builder.states.blendAttachments[0].colorBlendOp = vk::BlendOp::add;
	builder.states.blendAttachments[0].srcColorBlendFactor = vk::BlendFactor::srcAlpha;
	builder.states.blendAttachments[0].dstColorBlendFactor = vk::BlendFactor::oneMinusSrcAlpha;
	builder.states.blendAttachments[0].srcAlphaBlendFactor = vk::BlendFactor::one;
	builder.states.blendAttachments[0].dstAlphaBlendFactor = vk::BlendFactor::zero;
	builder.states.blendAttachments[0].alphaBlendOp = vk::BlendOp::add;

	builder.states.rasterization.cullMode = vk::CullModeBits::none;
	builder.states.inputAssembly.topology = vk::PrimitiveTopology::pointList;

	graphicsPipeline_ = builder.build(cache);
	vpp::save(cache, "gfxCache");
}
void ParticleSystem::initComputePipeline()
{
	computeLayout_ = {device(), {computeDescriptorSetLayout_}};

	vpp::PipelineCache cache(device());
	if(vpp::fileExists("compCache")) cache = {device(), "compCache"};

	vpp::ComputePipelineBuilder builder;
	builder.shaderStage = {device(), "particles.comp.spv", {vk::ShaderStageBits::compute}};
	builder.layout = computeLayout_;

	computePipeline_ = builder.build(cache);
	vpp::save(cache, "compCache");
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

	descriptorPool_ = {device(), descriptorPoolInfo};

	//graphics set
	auto gfxBindings = {
		vpp::descriptorBinding(vk::DescriptorType::uniformBuffer, vk::ShaderStageBits::vertex, 0)
	};

	graphicsDescriptorSetLayout_ = {device(), gfxBindings};
	graphicsDescriptorSet_ = {graphicsDescriptorSetLayout_, descriptorPool_};

	//computeSet
	auto compBindings = {
		vpp::descriptorBinding(vk::DescriptorType::storageBuffer, vk::ShaderStageBits::compute, 0),
		vpp::descriptorBinding(vk::DescriptorType::uniformBuffer, vk::ShaderStageBits::compute, 1)
	};

	computeDescriptorSetLayout_ = {device(), compBindings};
	computeDescriptorSet_ = {computeDescriptorSetLayout_, descriptorPool_};
}

void ParticleSystem::initDescriptorBuffers()
{
	//graphics
	vk::BufferCreateInfo gfxInfo;
	gfxInfo.size = sizeof(nytl::Mat4f) * 2; //viewMatrix, perspectiveMatrix
	gfxInfo.usage = vk::BufferUsageBits::uniformBuffer;

	graphicsUBO_ = {device(), gfxInfo, vk::MemoryPropertyBits::hostVisible};

	//compute
	vk::BufferCreateInfo compInfo;
	compInfo.size = sizeof(float) * 5; //mouse pos(vec2f), speed, deltaTime, attract
	compInfo.usage = vk::BufferUsageBits::uniformBuffer;

	computeUBO_ = {device(), compInfo, vk::MemoryPropertyBits::hostVisible};
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
		particle.color = nytl::Vec2f(1., 1.);
	}
}
void ParticleSystem::initParticleBuffer()
{
	vk::BufferCreateInfo bufInfo;
	bufInfo.size = sizeof(Particle) * count_;
	bufInfo.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::storageBuffer
		| vk::BufferUsageBits::transferDst;

	particlesBuffer_ = {device(), bufInfo, 1};
}

void ParticleSystem::writeParticleBuffer()
{
	vpp::BufferUpdate update(particlesBuffer_, vpp::BufferAlign::std430);
	update.add(particles_);

	//fill430(particlesBuffer_, particles_);
}

void ParticleSystem::buildComputeBuffer()
{
	computeBuffer_ = device().commandProvider().get(0);

	//build computeBuffer
	vk::CommandBufferBeginInfo cmdBufInfo;
	vk::beginCommandBuffer(computeBuffer_, cmdBufInfo);

	auto cd = computeDescriptorSet_.vkHandle();

	vk::BufferMemoryBarrier uniformBarrier;
	uniformBarrier.srcAccessMask = vk::AccessBits::hostWrite;
	uniformBarrier.dstAccessMask = vk::AccessBits::uniformRead;
	uniformBarrier.buffer = computeUBO_;
	uniformBarrier.offset = 0;
	uniformBarrier.size = 0x14; //no idea. debug layer.
	uniformBarrier.srcQueueFamilyIndex = vk::queueFamilyIgnored;
	uniformBarrier.dstQueueFamilyIndex = vk::queueFamilyIgnored;

	vk::cmdPipelineBarrier(computeBuffer_, vk::PipelineStageBits::topOfPipe,
		vk::PipelineStageBits::bottomOfPipe, {}, {}, {uniformBarrier}, {});

	vk::cmdBindPipeline(computeBuffer_, vk::PipelineBindPoint::compute, computePipeline_);
	vk::cmdBindDescriptorSets(computeBuffer_, vk::PipelineBindPoint::compute,
		computeLayout_, 0, {cd}, {});
	vk::cmdDispatch(computeBuffer_, count_ / 16, 1, 1);

	vk::endCommandBuffer(computeBuffer_);
}

void ParticleSystem::writeGraphicsUBO()
{
	auto pMat = nytl::perspective3(45.f, 900.f / 900.f, 0.1f, 100.f);
	auto vMat = nytl::identityMat<4, float>();
	auto map = graphicsUBO_.memoryMap();

	vpp::BufferUpdate update(graphicsUBO_, vpp::BufferAlign::std140);
	update.add(pMat, vMat);
}

void ParticleSystem::writeDescriptorSets()
{
	vpp::DescriptorSetUpdate gfx(graphicsDescriptorSet_);
	gfx.uniform({{graphicsUBO_, 0, sizeof(nytl::Mat4f) * 2}});

	vpp::DescriptorSetUpdate comp(computeDescriptorSet_);
	comp.storage({{particlesBuffer_, 0, sizeof(Particle) * count_}});
	comp.uniform({{computeUBO_, 0, sizeof(float) * 5}});

	vpp::apply({gfx, comp});
}

void ParticleSystem::ParticleSystem::update(const nytl::Vec2ui& mousePos)
{
	//compute
	auto now = Clock::now();
	float delta = std::chrono::duration_cast<std::chrono::duration<
		float, std::ratio<1, 1>>>(now - lastUpdate_).count();
	lastUpdate_ = now;

	float speed = 15.f;
	float attract = (GetKeyState(VK_LBUTTON) < 0);

	nytl::Vec2f normMousePos = 2 * (mousePos / nytl::Vec2f(app_.width, app_.height)) - 1;

	auto map = computeUBO_.memoryMap();
	std::memcpy(map.ptr(), &delta, sizeof(float));
	std::memcpy(map.ptr() + sizeof(float), &speed, sizeof(float));
	std::memcpy(map.ptr() + sizeof(float) * 2, &attract, sizeof(float));
	std::memcpy(map.ptr() + sizeof(float) * 3, &normMousePos, sizeof(nytl::Vec2f));
	if(!map.coherent())map.flush();
}

int main(int argc, char** argv)
{
	auto count = 500;
	if(argc > 1) count = std::stoi(argv[1]);

	App app;
	ParticleSystem system(app, count * 1024);
	initApp(app, [&](){ return std::make_unique<ParticleRenderer>(system); });
	mainLoop(app, [&](){
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(app.window, &pos);

		auto vec = nytl::Vec2ui(pos.x, pos.y);
		vec = nytl::clamp(vec, nytl::Vec2ui(0, 0), nytl::Vec2ui(app.width, app.height));

		system.update(vec);
	});
}
