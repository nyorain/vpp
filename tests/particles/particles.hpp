#pragma once

#include <vpp/vpp.hpp>
#include <vpp/defs.hpp>
#include <nytl/nytl.hpp>
#include <windows.h> //srsly microsoft? "near" and "far" macros? WTF man hoooly shit

#include <chrono>
using Clock = std::chrono::high_resolution_clock;

class ParticleSystem;

struct App
{
    HINSTANCE hinstance = nullptr;
    HWND window = nullptr;

    unsigned int width = 900;
    unsigned int height = 900;

    vpp::Context* context = nullptr;
	vpp::RenderPass renderPass {};
	vpp::SwapChainRenderer* renderer = nullptr;
	vpp::SwapChainRenderer::CreateInfo rendererInfo {};

	ParticleSystem* particleSystem;

	const vpp::Queue* presentQueue {};
	const vpp::Queue* computeQueue {};
};

//global app
extern App* gApp;


struct Particle
{
	nytl::Vec4f position;
	nytl::Vec4f velocity;
	nytl::Vec4f color;
};

class ParticleSystem : public vpp::Resource
{
protected:
	App& app_;
	vpp::DeviceMemoryAllocator allocator_;

	std::vector<Particle> particles_;
	vpp::Buffer particlesBuffer_;

	vpp::VertexBufferLayout vertexBufferLayout_;
	vpp::ComputePipeline computePipeline_;
	vpp::GraphicsPipeline graphicsPipeline_;

	vk::CommandBuffer computeBuffer_;

	vk::DescriptorPool descriptorPool_;

	vpp::DescriptorSetLayout computeDescriptorSetLayout_;
	vpp::DescriptorSet computeDescriptorSet_;
	vpp::Buffer computeUBO_;

	vpp::DescriptorSetLayout graphicsDescriptorSetLayout_;
	vpp::DescriptorSet graphicsDescriptorSet_;
	vpp::Buffer graphicsUBO_;

	//ubo values
	nytl::Mat4f viewMatrix_; //camera
	Clock::time_point lastUpdate_;

protected:
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

	void updateUBOs(const nytl::Vec2ui& mousePos);
	void compute();

public:
	ParticleSystem(App& app, std::size_t count);
	~ParticleSystem();

	void update(const nytl::Vec2ui& mousePos);
	friend class ParticleRenderer;
};

class ParticleRenderer : public vpp::RendererBuilder
{
protected:
	App* app_;
	vk::Semaphore computeSemaphore_;

public:
	ParticleRenderer(App& app);
	~ParticleRenderer();

	void init(vpp::SwapChainRenderer& renderer) override;
	void build(unsigned int, const vpp::RenderPassInstance&) override;
	std::vector<vk::ClearValue> clearValues(unsigned int) override;
	void beforeRender(vk::CommandBuffer cmdBuffer) override;
	vk::SubmitInfo submit(vk::CommandBuffer cmd, vk::Semaphore wait, vk::Semaphore signal) override;

	ParticleSystem& ps() const { return *app_->particleSystem; }
};

//utility
void initRenderPass(App& app);
