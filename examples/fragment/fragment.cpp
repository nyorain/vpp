#include <example.hpp>

#include <vpp/provider.hpp>
#include <vpp/utility/file.hpp>
#include <vpp/defs.hpp>
#include <vpp/buffer.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/computePipeline.hpp>

#include <nytl/nytl.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h> //srsly microsoft? "near" and "far" macros? WTF man hoooly shit

#include <cassert>
#include <cstring>
#include <chrono>
using Clock = std::chrono::high_resolution_clock;

class FragmentData : public vpp::Resource
{
public:
	FragmentData(App& app);
	~FragmentData();

	void update(const nytl::Vec2ui& mousePos);

protected:
	void init();
	void initPipeline();
	void initDescriptors();
	void initDescriptorBuffers();
	void writeDescriptorSets();
	void updateUBO(const nytl::Vec2ui& mousePos);

protected:
	friend class FragmentRenderer;

	App& app_;

	vpp::GraphicsPipeline pipeline_;

	vk::DescriptorPool descriptorPool_;
	vpp::DescriptorSetLayout descriptorSetLayout_;
	vpp::DescriptorSet descriptorSet_;
	vpp::Buffer ubo_;

	Clock::time_point lastUpdate_;
	bool initialized_ = false;
};

class FragmentRenderer : public vpp::RendererBuilder
{
public:
	FragmentRenderer(FragmentData& data);
	~FragmentRenderer() = default;

	virtual void init(vpp::SwapChainRenderer&) override;
	virtual void build(unsigned int, const vpp::RenderPassInstance& instance) override;
	virtual std::vector<vk::ClearValue> clearValues(unsigned int) override;

	FragmentData& data() const { return *data_; }

protected:
	FragmentData* data_;
};

//FragmentData
FragmentData::FragmentData(App& app) : app_(app)
{
}

FragmentData::~FragmentData()
{
	//TODO
}

void FragmentData::init()
{
	if(initialized_) return;
	Resource::init(app_.context.device());

	initDescriptors();
	initDescriptorBuffers();
	initPipeline();

	ubo_.assureMemory();
	writeDescriptorSets();
	lastUpdate_ = Clock::now();

	initialized_ = true;
}

void FragmentData::initPipeline()
{
	vpp::GraphicsPipeline::CreateInfo info;
	info.descriptorSetLayouts = {descriptorSetLayout_};
	info.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};
	info.renderPass = app_.renderPass.vkRenderPass();

	info.shader = vpp::ShaderProgram(device());
	info.shader.stage("fragment.vert.spv", {vk::ShaderStageBits::vertex});
	info.shader.stage("fragment.frag.spv", {vk::ShaderStageBits::fragment});

	info.states = {vk::Viewport{0, 0, 1, 1, 0.f, 1.f}};
	info.states.rasterization.cullMode = vk::CullModeBits::none;
	info.states.inputAssembly.topology = vk::PrimitiveTopology::triangleList;

	pipeline_ = vpp::GraphicsPipeline(device(), info);
}

void FragmentData::initDescriptors()
{
	//init pool
	vk::DescriptorPoolSize typeCounts[2] {};
	typeCounts[0].type = vk::DescriptorType::uniformBuffer;
	typeCounts[0].descriptorCount = 2;

	vk::DescriptorPoolCreateInfo descriptorPoolInfo;
	descriptorPoolInfo.poolSizeCount = 1;
	descriptorPoolInfo.pPoolSizes = typeCounts;
	descriptorPoolInfo.maxSets = 1;

	descriptorPool_ = vk::createDescriptorPool(vkDevice(), descriptorPoolInfo);

	//graphics set
	std::vector<vpp::DescriptorBinding> gfxBindings =
	{
		{vk::DescriptorType::uniformBuffer, vk::ShaderStageBits::fragment} //fragment stuff
	};

	descriptorSetLayout_ = vpp::DescriptorSetLayout(device(), gfxBindings);
	descriptorSet_ = vpp::DescriptorSet(descriptorSetLayout_, descriptorPool_);
}

void FragmentData::initDescriptorBuffers()
{
	vk::BufferCreateInfo gfxInfo;
	gfxInfo.size = sizeof(float) * 5;
	gfxInfo.usage = vk::BufferUsageBits::uniformBuffer;

	ubo_ = vpp::Buffer(device(), gfxInfo, vk::MemoryPropertyBits::hostVisible);
}

void FragmentData::updateUBO(const nytl::Vec2ui& mousePos)
{
	static float time = 0.f;

	auto now = Clock::now();
	float delta = std::chrono::duration_cast<std::chrono::duration<float,
		std::ratio<1, 1>>>(now - lastUpdate_).count();
	lastUpdate_ = now;

	nytl::Vec2f normMousePos = 2 * (mousePos / nytl::Vec2f(app_.width, app_.height)) - 1;
	nytl::Vec2f res(app_.width, app_.height);

	time += delta;

	auto map = ubo_.memoryMap();
	std::memcpy(map.ptr(), &res, sizeof(float) * 2);
	std::memcpy(map.ptr() + 2 * sizeof(float), &normMousePos, sizeof(float) * 2);
	std::memcpy(map.ptr() + 4 * sizeof(float), &time, sizeof(float));
}

void FragmentData::writeDescriptorSets()
{
	//write buffers
	vpp::DescriptorSetUpdate update(descriptorSet_);
	update.uniform({{ubo_.vkBuffer(), 0, sizeof(nytl::Mat4f) * 2}});
}

void FragmentData::update(const nytl::Vec2ui& mousePos)
{
	updateUBO(mousePos);
}

//FragmentRenderer
FragmentRenderer::FragmentRenderer(FragmentData& data) : data_(&data)
{
}

void FragmentRenderer::build(unsigned int, const vpp::RenderPassInstance& instance)
{
	auto cmdBuffer = instance.vkCommandBuffer();
	VkDeviceSize offsets[1] = { 0 };

	auto gd = data().descriptorSet_.vkDescriptorSet();

	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics, data().pipeline_);
	vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
		data().pipeline_.vkPipelineLayout(), 0, {gd}, {});
	vk::cmdDraw(cmdBuffer, 6, 1, 0, 0);
}

std::vector<vk::ClearValue> FragmentRenderer::clearValues(unsigned int)
{
	std::vector<vk::ClearValue> ret(2, vk::ClearValue{});
	ret[0].color = {{0.f, 0.f, 0.f, 1.f}};
	ret[1].depthStencil = {1.f, 0};
	return ret;
}

void FragmentRenderer::init(vpp::SwapChainRenderer& renderer)
{
	data().init();
	renderer.record();
}

int main(int argc, char** argv)
{
	App app;
	FragmentData data(app);
	initApp(app, [&](){ return std::make_unique<FragmentRenderer>(data); });
	mainLoop(app, [&](){
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(app.window, &pos);

		auto vec = nytl::Vec2ui(pos.x, pos.y);
		vec = nytl::clamp(vec, nytl::Vec2ui(0, 0), nytl::Vec2ui(app.width, app.height));

		data.update(vec);
	});
}
