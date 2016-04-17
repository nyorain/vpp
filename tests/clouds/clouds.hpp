#pragma once

#include <vpp/vpp.hpp>
#include <nytl/nytl.hpp>
#include <windows.h> //srsly microsoft? "near" and "far" macros? WTF man hoooly shit

#include <chrono>
using Clock = std::chrono::high_resolution_clock;

class FragmentRenderer;

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

	FragmentRenderer* fragmentRenderer {};

	vk::Queue presentQueue {};
};

//global app
extern App* gApp;

class FragmentRenderer : public vpp::RendererBuilder, public vpp::Resource
{
protected:
	App& app_;

	vpp::GraphicsPipeline pipeline_;

	vk::DescriptorPool descriptorPool_;
	vpp::DescriptorSetLayout descriptorSetLayout_;
	vpp::DescriptorSet descriptorSet_;
	vpp::Buffer ubo_;

	//ubo values
	Clock::time_point lastUpdate_;

protected:
	void initPipeline();
	void initDescriptors();
	void initDescriptorBuffers();

	void writeDescriptorSets();

	void updateUBO(const nytl::Vec2ui& mousePos);

public:
	FragmentRenderer(App& app);
	~FragmentRenderer();

	void update(const nytl::Vec2ui& mousePos);

	virtual void build(const vpp::RenderPassInstance& instance) const override;
	virtual std::vector<vk::ClearValue> clearValues() const override;
};

//utility
void initRenderPass(App& app);
