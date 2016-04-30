#pragma once

#include <vpp/vpp.hpp>
#include <nytl/nytl.hpp>
#include <windows.h> //srsly microsoft? "near" and "far" macros? WTF man hoooly shit

#include <chrono>
using Clock = std::chrono::high_resolution_clock;

class Renderer;

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

	Renderer* gRenderer {};

	vk::Queue presentQueue {};
};

//global app
extern App* gApp;

class Renderer : public vpp::RendererBuilder, public vpp::Resource
{
protected:
	App& app_;

	vpp::GraphicsPipeline pipeline_;
	vpp::GraphicsPipeline postPipeline_;

	vk::DescriptorPool descriptorPool_;

	vpp::DescriptorSetLayout descriptorSetLayout_;
	vpp::DescriptorSetLayout postDescriptorSetLayout_;

	//holds 1 uniform buffer with object color and time
	vpp::DescriptorSet descriptorSet_;

	//holds 1 uniform buffer with godrays combine vars, 1 stencil image, 1 color attachment
	vpp::DescriptorSet postDescriptorSet_;

	vpp::Buffer ubo_;
	vpp::Buffer postUbo_;
	vpp::Buffer vertexBuffer_;

	vk::ImageView stencilView_;
	vk::Sampler stencilSampler_;

	//ubo values
	Clock::time_point lastUpdate_;

protected:
	void initPipeline();
	void initDescriptors();
	void initDescriptorBuffers();
	void initVertexBuffer();
	void initStencilTexture();

	void writeDescriptorSets();
	void writeVertexBuffer();

	void updateUBO(const nytl::Vec2ui& mousePos);

public:
	Renderer(App& app);
	~Renderer();

	void update(const nytl::Vec2ui& mousePos);

	virtual void build(const vpp::RenderPassInstance& instance) const override;
	virtual std::vector<vk::ClearValue> clearValues() const override;

	virtual void init(const vpp::SwapChainRenderer& renderer) override;
};

//utility
vk::SamplerCreateInfo defaultSamplerInfo();
void initRenderPass(App& app);
