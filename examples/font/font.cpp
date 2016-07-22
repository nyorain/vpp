#include <example.hpp>

#include <vpp/image.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/vk.hpp>

#define DDF_FONT_IMPLEMENTATION
#include "ddf_font.h"

constexpr char printString[] = "HelloWorldThisFontHasNoSpacesqq";
constexpr auto stringSize = sizeof(printString) - 1;

constexpr auto xpos = 00;
constexpr auto ypos = 200;

struct Vertex
{
	float x, y, u, v;
};

class TextureData : public vpp::Resource
{
public:
	TextureData(App& app) : app_(app) {}
	~TextureData() {}

	void init()
	{
		if(initialized_) return;
		vpp::Resource::init(app_.context.device());
		ddf_font font;

		//image
		{
			if(!ddf_font_create(&font, "font.ddf"))
			{
				printf("ddf_font error %d", ddf_get_last_error());
				exit(-1);
			}

			auto width = font.texture_width;
			auto height = font.texture_height;
			const uint8_t* data = font.texture_data;

			std::cout << width << " -- " << height << "\n";

			std::vector<uint8_t> data2(width * height * 4, 255);
			for(auto h = 0u; h < height; ++h)
			{
				for(auto w = 0u; w < width; ++w)
				{
					auto pos = h * width + w;
					std::memcpy(data2.data() + pos * 4, data + pos * 3, 3);
					// std::cout << static_cast<uint32_t>(*data2.data() + pos * 4) << "\n";
				}
			}

			//assert(comp = 4); //assert rgba
			vk::Extent3D extent;
			extent.width = width;
			extent.height = height;
			extent.depth = 1;

			//texture
			auto info = vpp::ViewableImage::defaultColor2D();
			info.imgInfo.extent = extent;
			info.imgInfo.tiling = vk::ImageTiling::optimal;
			info.imgInfo.format = vk::Format::r8g8b8a8Unorm;
			info.imgInfo.usage = vk::ImageUsageBits::transferDst | vk::ImageUsageBits::sampled;
			info.imgInfo.initialLayout = vk::ImageLayout::undefined;
			info.viewInfo.format = vk::Format::r8g8b8a8Unorm;
			texture_ = {device(), info};

			fill(texture_.image(), *data2.data(), vk::Format::r8g8b8a8Unorm,
				vk::ImageLayout::undefined, extent, {vk::ImageAspectBits::color, 0, 0})->finish();

			//sampler
			vk::SamplerCreateInfo samplerInfo;
			samplerInfo.magFilter = vk::Filter::linear;
			samplerInfo.minFilter = vk::Filter::linear;
			samplerInfo.mipmapMode = vk::SamplerMipmapMode::linear;
			samplerInfo.addressModeU = vk::SamplerAddressMode::clampToEdge;
			samplerInfo.addressModeV = vk::SamplerAddressMode::clampToEdge;
			samplerInfo.addressModeW = vk::SamplerAddressMode::clampToEdge;
			samplerInfo.mipLodBias = 0;
			samplerInfo.anisotropyEnable = false;
			samplerInfo.maxAnisotropy = 0;
			samplerInfo.compareEnable = false;
			samplerInfo.compareOp = {};
			samplerInfo.minLod = 0.0;
			samplerInfo.maxLod = 0.0;
			samplerInfo.borderColor = vk::BorderColor::floatOpaqueWhite;
			samplerInfo.unnormalizedCoordinates = false;
			sampler_ = vk::createSampler(device(), samplerInfo);
		}

		//buffer
		{
			vk::BufferCreateInfo info;
			info.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::transferDst;
			info.size = sizeof(float) * 4 * stringSize * 6; //6 verts per char
			vertexBuffer_ = {device(), info};
			vertexBuffer_.assureMemory();

			vpp::BufferUpdate update(vertexBuffer_, vpp::BufferAlign::std140);
			float xoff = xpos;
			for(auto i = 0u; i < stringSize; ++i)
			{
				auto& c = printString[i];

				// auto charData = &sdf_spacing[(c - 32) * 8];
				const auto& charData = *ddf_get_glyph_properties(&font, c);
				if(!&charData) continue;

				float xstart = xoff;
				float ystart = ypos + charData.offset_y;
				float ustart = charData.texcoord_x;
				float vstart = charData.texcoord_y;
				float xsize = charData.charwidth * 3;
				float ysize = charData.charheight * 3;
				float xsizeuv = charData.texwidth;
				float ysizeuv = charData.texheight;

				Vertex vertex = {xstart, ystart, ustart, vstart};
				update.add(vpp::raw(vertex)); //1

				vertex.x += xsize;
				vertex.u += xsizeuv;
				update.add(vpp::raw(vertex)); //2

				vertex.y += ysize;
				vertex.v += ysizeuv;
				update.add(vpp::raw(vertex)); //3
				update.add(vpp::raw(vertex)); //4

				vertex.x -= xsize;
				vertex.u -= xsizeuv;
				update.add(vpp::raw(vertex)); //5

				vertex.y -= ysize;
				vertex.v -= ysizeuv;
				update.add(vpp::raw(vertex)); //6

				xoff = xstart + 2.5 * charData.advance; //chardata.advance
			}
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
			builder.vertexBufferLayouts = {{{vk::Format::r32g32Sfloat, vk::Format::r32g32Sfloat}}};

			builder.states.blendAttachments[0].blendEnable = true;
			builder.states.blendAttachments[0].colorBlendOp = vk::BlendOp::add;
			builder.states.blendAttachments[0].srcColorBlendFactor = vk::BlendFactor::srcAlpha;
			builder.states.blendAttachments[0].dstColorBlendFactor =
				vk::BlendFactor::oneMinusSrcAlpha;
			builder.states.blendAttachments[0].srcAlphaBlendFactor = vk::BlendFactor::one;
			builder.states.blendAttachments[0].dstAlphaBlendFactor = vk::BlendFactor::zero;
			builder.states.blendAttachments[0].alphaBlendOp = vk::BlendOp::add;

			builder.shader.stage("font.vert.spv", {vk::ShaderStageBits::vertex});
			builder.shader.stage("font.frag.spv", {vk::ShaderStageBits::fragment});

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
	vpp::Buffer vertexBuffer_;
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
		vk::cmdBindVertexBuffers(cmdBuffer, 0, {data().vertexBuffer_}, {0});
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
			data().pipelineLayout_, 0, {data().descriptorSet_}, {});
		vk::cmdDraw(cmdBuffer, stringSize * 6, 1, 0, 0);
	}
	std::vector<vk::ClearValue> clearValues(unsigned int) override
	{
		std::vector<vk::ClearValue> ret(2, vk::ClearValue{});
		ret[0].color = {{1.f, 1.f, 1.f, 1.0f}};
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
