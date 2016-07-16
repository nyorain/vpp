#include <example.hpp>

#include <vpp/image.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/vk.hpp>

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
			int width, height, comp;
			auto data = stbi_load("image.png", &width, &height, &comp, 4);
			if(!data) throw std::runtime_error("Failed to load image image.png");

			vk::Extent3D extent;
			extent.width = width;
			extent.height = height;
			extent.depth = 1;

			auto info = vpp::ViewableImage::defaultColor2D();
			info.imgInfo.extent = extent;
			info.imgInfo.format = vk::Format::r8g8b8a8Unorm;
			info.viewInfo.format = vk::Format::r8g8b8a8Unorm;
			info.imgInfo.usage = vk::ImageUsageBits::sampled;

			//either this
			// info.imgInfo.tiling = vk::ImageTiling::linear;
			// info.memoryFlags = vk::MemoryPropertyBits::hostVisible;

			//or this
			info.imgInfo.tiling = vk::ImageTiling::optimal;
			info.imgInfo.usage |= vk::ImageUsageBits::transferDst;

			texture_ = {device(), info};

			fill(texture_.image(), *data, vk::Format::r8g8b8a8Unorm, vk::ImageLayout::undefined,
				extent, {vk::ImageAspectBits::color, 0, 0})->finish();

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
			samplerInfo.maxLod = 0.25;
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

			descriptorPool_ = {device(), info};

			//set
			layout = {device(), {vpp::descriptorBinding(vk::DescriptorType::combinedImageSampler,
				vk::ShaderStageBits::fragment)}};

			descriptorSet_ = vpp::DescriptorSet(layout, descriptorPool_);

			//write
			vpp::DescriptorSetUpdate update(descriptorSet_);
			update.imageSampler({{sampler_, texture_.vkImageView(), vk::ImageLayout::general}});
		}

		//pipeline
		{
			pipelineLayout_ = {device(), {layout}};

			vpp::GraphicsPipelineBuilder builder(device(), app_.renderPass);
			builder.layout = pipelineLayout_;
			builder.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};

			builder.shader.stage("texture.vert.spv", {vk::ShaderStageBits::vertex});
			builder.shader.stage("texture.frag.spv", {vk::ShaderStageBits::fragment});

			builder.states.blendAttachments[0].blendEnable = true;
			builder.states.blendAttachments[0].colorBlendOp = vk::BlendOp::add;
			builder.states.blendAttachments[0].srcColorBlendFactor = vk::BlendFactor::srcAlpha;
			builder.states.blendAttachments[0].dstColorBlendFactor =
				vk::BlendFactor::oneMinusSrcAlpha;
			builder.states.blendAttachments[0].srcAlphaBlendFactor = vk::BlendFactor::one;
			builder.states.blendAttachments[0].dstAlphaBlendFactor = vk::BlendFactor::zero;
			builder.states.blendAttachments[0].alphaBlendOp = vk::BlendOp::add;

			builder.states.rasterization.cullMode = vk::CullModeBits::none;
			builder.states.inputAssembly.topology = vk::PrimitiveTopology::triangleList;

			pipeline_ = builder.build();
		}


		initialized_ = true;
	}

protected:
	friend class TextureRenderer;

	App& app_;
	vpp::ViewableImage texture_;
	vpp::Pipeline pipeline_;
	vpp::PipelineLayout pipelineLayout_;
	vpp::DescriptorSet descriptorSet_;
	vpp::DescriptorPool descriptorPool_;
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
			data().pipeline_);
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
			data().pipelineLayout_, 0, {data().descriptorSet_}, {});
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
