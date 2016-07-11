#include <example.hpp>

#include <vpp/defs.hpp>
#include <vpp/image.hpp>
#include <vpp/graphicsPipeline.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class TextureData : public vpp::Resource
{
public:
	TextureData(App& app) : app_(app) {}
	~TextureData() {}

	void init()
	{
		if(initialized_) return;
		vpp::Resource::init(app_.context.device());

		//image
		{
			//load image
			int width, height, comp;
			auto data = stbi_load("image.png", &width, &height, &comp, 4);

			//assert(comp = 4); //assert rgba
			vk::Extent3D extent;
			extent.width = width;
			extent.height = height;
			extent.depth = 1;

			//texture
			auto info = vpp::ViewableImage::defaultColor2D();
			info.imgInfo.extent = extent;
			info.imgInfo.tiling = vk::ImageTiling::linear;
			info.imgInfo.format = vk::Format::r8g8b8a8Unorm;
			info.viewInfo.format = vk::Format::r8g8b8a8Unorm;
			info.memoryFlags = vk::MemoryPropertyBits::hostVisible;
			texture_ = {device(), info};

			std::cout << texture_.image().size() << "\n";
			std::cout << width * height * comp << "\n";

			fill(texture_.image(), *data, vk::ImageLayout::undefined, extent,
				{vk::ImageAspectBits::color, 0, 0, 1}, width * height * comp);

			//sampler
			vk::SamplerCreateInfo samplerInfo;
			samplerInfo.magFilter = vk::Filter::linear;
			samplerInfo.minFilter = vk::Filter::linear;
			samplerInfo.mipmapMode = vk::SamplerMipmapMode::nearest;
			samplerInfo.addressModeU = vk::SamplerAddressMode::repeat;
			samplerInfo.addressModeV = vk::SamplerAddressMode::repeat;
			samplerInfo.addressModeW = vk::SamplerAddressMode::repeat;
			samplerInfo.mipLodBias = 0;
			samplerInfo.anisotropyEnable = true;
			samplerInfo.maxAnisotropy = 16;
			samplerInfo.compareEnable = false;
			samplerInfo.compareOp = {};
			samplerInfo.minLod = 0;
			samplerInfo.maxLod = 0;
			samplerInfo.borderColor = vk::BorderColor::floatTransparentBlack;
			samplerInfo.unnormalizedCoordinates = false;
			sampler_ = vk::createSampler(device(), samplerInfo);
		}

		//descriptor
		vpp::DescriptorSetLayout layout;
		{
			//pool
			vk::DescriptorPoolSize typeCounts[1];
			typeCounts[0].type = vk::DescriptorType::combinedImageSampler;
			typeCounts[0].descriptorCount = 1;

			vk::DescriptorPoolCreateInfo info;
			info.poolSizeCount = 1;
			info.pPoolSizes = typeCounts;
			info.maxSets = 1;

			descriptorPool_ = vk::createDescriptorPool(device(), info);

			//set
			layout = {device(), {{vk::DescriptorType::combinedImageSampler,
				vk::ShaderStageBits::fragment}}};

			descriptorSet_ = vpp::DescriptorSet(layout, descriptorPool_);

			//write
			vpp::DescriptorSetUpdate update(descriptorSet_);
			update.combinedSampler({{sampler_, texture_.vkImageView(), vk::ImageLayout::general}});
		}

		//pipeline
		{
			vpp::GraphicsPipeline::CreateInfo info;
			info.descriptorSetLayouts = {layout};
			info.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};
			info.renderPass = app_.renderPass.vkRenderPass();

			info.shader = vpp::ShaderProgram(device());
			info.shader.stage("texture.vert.spv", {vk::ShaderStageBits::vertex});
			info.shader.stage("texture.frag.spv", {vk::ShaderStageBits::fragment});
			
			info.states = {{0, 0, 900, 900}};
			info.states.rasterization.cullMode = vk::CullModeBits::none;
			info.states.inputAssembly.topology = vk::PrimitiveTopology::triangleList;

			pipeline_ = {device(), info};
		}


		initialized_ = true;
	}

protected:
	friend class TextureRenderer;

	App& app_;
	vpp::ViewableImage texture_;
	vpp::GraphicsPipeline pipeline_;
	vpp::DescriptorSet descriptorSet_;
	vk::DescriptorPool descriptorPool_;
	vk::Sampler sampler_;
	bool initialized_ = false;
};

class TextureRenderer : public vpp::RendererBuilder
{
public:
	TextureRenderer(TextureData& xdata) : data_(xdata) {}
	~TextureRenderer() = default;

	TextureData& data() { return data_; }

	void init(vpp::SwapChainRenderer& renderer) override
	{
		data_.init();
		renderer.record();
	}
	void build(unsigned int, const vpp::RenderPassInstance& instance) override
	{
		auto cmdBuffer = instance.vkCommandBuffer();
		vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics,
			data().pipeline_.vkPipeline());
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
			data().pipeline_.vkPipelineLayout(), 0, {data().descriptorSet_}, {});
		vk::cmdDraw(cmdBuffer, 6, 1, 0, 0);
	}
	std::vector<vk::ClearValue> clearValues(unsigned int) override
	{
		std::vector<vk::ClearValue> ret(2, vk::ClearValue{});
		ret[0].color = {{0.f, 0.f, 0.f, 1.0f}};
		ret[1].depthStencil = {1.f, 0};
		return ret;
	}

protected:
	TextureData& data_;
};

int main(int argc, char** argv)
{
	App app;
	TextureData data(app);
	initApp(app, [&](){ return std::make_unique<TextureRenderer>(data); });
	mainLoop(app, [](){});
}
