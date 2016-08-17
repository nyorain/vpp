#include "model.hpp"
#include <vpp/pipeline.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <example.hpp>

#include <nytl/vec.hpp>
#include <nytl/stringParam.hpp>
#include <nytl/convert.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#define STBI_IMPLEMENTATION
#include <stb_image.h>


//specialization for aiVec <-> nytlVec
namespace nytl
{

template<> struct Converter<aiVector2D, Vec2f>
	{ static aiVector2D call(const Vec2f& vec) { return {vec.x, vec.y}; } };

template<> struct Converter<aiVector3D, Vec3f>
	{ static aiVector3D call(const Vec3f& vec) { return {vec.x, vec.y, vec.z}; } };

template<> struct Converter<Vec2f, aiVector2D>
	{ static Vec2f call(const aiVector2D& vec) { return {vec.x, vec.y}; } };

template<> struct Converter<Vec3f, aiVector3D>
	{ static Vec3f call(const aiVector3D& vec) { return {vec.x, vec.y, vec.z}; } };

template<> struct Converter<Vec4f, aiColor4D>
	{ static Vec4f call(const aiColor4D& col) { return {col.r, col.g, col.b, col.a}; } };
}

namespace vpp
{

template<> struct VulkanType<MaterialData> : public VulkanTypeStruct<true>
{
	static constexpr auto members = std::make_tuple(
		&MaterialData::colors,
		&MaterialData::shininess,
		&MaterialData::texture);
};

template<> struct VulkanType<MaterialColors> : public VulkanTypeStruct<false>
{
	static constexpr auto members = std::make_tuple(
		&MaterialColors::ambient,
		&MaterialColors::diffuse,
		&MaterialColors::specular);
};

}


class ModelRenderer : public vpp::RendererBuilder
{
public:
	ModelRenderer(ModelData& data) : data_(data) {}

	void build(unsigned int, const vpp::RenderPassInstance& ini) override;
	void init(vpp::SwapChainRenderer& renderer) override;
	void frame(unsigned int id) override;
	std::vector<vk::ClearValue> clearValues(unsigned int) override;

protected:
	ModelData& data_;
};

//implementation
//TODO: both load/init functions can be really improved. Use vpp::WorkManager.
vpp::WorkManager loadScene(const vpp::Device& dev, ModelData& modeldata)
{
	Scene& scene = modeldata.scene;

	auto flags =
		aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;

	Assimp::Importer importer;
	auto aiscene = importer.ReadFile(modeldata.filename, flags);
	if(!aiscene)
		throw std::runtime_error("assimp ReadFile failed for file " + modeldata.filename);

	modeldata.scene.meshes.reserve(aiscene->mNumMeshes);
	vpp::WorkManager workManager;

	for(auto& mesh : nytl::makeRange(*aiscene->mMeshes, aiscene->mNumMeshes))
	{
		Mesh mmesh;

		mmesh.materialIndex = mesh->mMaterialIndex;
		mmesh.vertices.resize(mesh->mNumVertices);

		for(std::size_t i(0); i < mesh->mNumVertices; ++i)
		{
			auto& vertex = mmesh.vertices[i];

			vertex.position = nytl::convert(mesh->mVertices[i]);
			vertex.normal = nytl::convert(mesh->mNormals[i]);

			//why have assimp tex coords 3 components?
			if(mesh->HasTextureCoords(0))
				vertex.texCoords = nytl::convert<nytl::Vec3f>(mesh->mTextureCoords[0][i]);
		}

		mmesh.indices.resize(mesh->mNumFaces * 3);
		for(std::size_t i(0); i < mesh->mNumFaces; ++i)
		{
			auto& face = mesh->mFaces[i];

			mmesh.indices[i * 3] = face.mIndices[0];
			mmesh.indices[i * 3 + 1] = face.mIndices[1];
			mmesh.indices[i * 3 + 2] = face.mIndices[2];
		}

		//buffer data
		auto verticesSize = mmesh.vertices.size() * sizeof(Vertex);
		auto indicesSize = mmesh.indices.size() * sizeof(std::uint32_t);

		std::vector<std::uint8_t> bufferData(verticesSize + indicesSize);
		std::size_t offset = 0;

		//first all vertices then all indices
		std::memcpy(bufferData.data(), mmesh.vertices.data(), verticesSize);
		std::memcpy(bufferData.data() + verticesSize, mmesh.indices.data(), indicesSize);

		//create the buffer
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.size = offset;
		bufferInfo.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::indexBuffer;

		mmesh.buffer = {dev, bufferInfo, vk::MemoryPropertyBits::deviceLocal};
		workManager.add(vpp::fill140(mmesh.buffer, vpp::raw(bufferData)));

		//mesh
		modeldata.scene.meshes.push_back(std::move(mmesh));
	}

	//vulkan descriptor pool for materials
	//pool
	vk::DescriptorPoolSize typeCounts[2];
	typeCounts[0].type = vk::DescriptorType::uniformBuffer;
	typeCounts[0].descriptorCount = aiscene->mNumMaterials;

	typeCounts[1].type = vk::DescriptorType::combinedImageSampler;
	typeCounts[1].descriptorCount = aiscene->mNumMaterials;

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = typeCounts;
	poolInfo.maxSets = aiscene->mNumMaterials;

	modeldata.descriptorPool = {dev, poolInfo};

	//uniform buffer info
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.usage = vk::BufferUsageBits::uniformBuffer;
	// bufferInfo.size = vpp::neededBufferSize140(MaterialData{});
	constexpr auto size = vpp::neededBufferSize140<MaterialData>();
	// static_assert(size == 56, "TEST");
	static_assert(size == 56, "TEST");
	bufferInfo.size = size;

	//load the materials
	modeldata.scene.materials.reserve(aiscene->mNumMaterials);
	for(auto& material : nytl::makeRange(*aiscene->mMaterials, aiscene->mNumMaterials))
	{
		modeldata.scene.materials.emplace_back();
		auto& mmat = modeldata.scene.materials.back();

		aiString aistr;
		material->Get(AI_MATKEY_NAME, aistr);
		mmat.name = aistr.C_Str();

		aiColor4D color;
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mmat.data.colors.ambient = nytl::convert(color);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mmat.data.colors.diffuse = nytl::convert(color);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mmat.data.colors.specular = nytl::convert(color);
		// material->Get(AI_MATKEY_OPACITY, mmat.opacity);
		material->Get(AI_MATKEY_SHININESS, mmat.data.shininess);

		//load texture
		if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			mmat.data.texture = true;
			int width, height, comp;
			const std::uint8_t* imgdata;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &aistr);
			if(aistr.data[0] == '*') //embedded texture
			{
				auto id = std::stoi(aistr.data + 1);
				if(aiscene->mNumTextures <= id)
				{
					auto msg = std::string("Invalid texture id ") + (aistr.data + 1);
					throw std::runtime_error(msg);
				}

				auto tex = aiscene->mTextures[id];
				auto tdata = reinterpret_cast<std::uint8_t*>(tex->pcData);
				if(tex->mHeight)
				{
					width = tex->mWidth;
					height = tex->mHeight;
					imgdata = tdata;
				}
				else
				{
					imgdata = stbi_load_from_memory(tdata, tex->mWidth, &width,
						&height, &comp, 4);
					if(!imgdata)
					{
						auto msg = std::string("Failed to load texture ") + aistr.C_Str();
						throw std::runtime_error(msg);
					}
				}
			}
			else
			{
				auto imgdata = stbi_load(aistr.C_Str(), &width, &height, &comp, 4);
				if(!imgdata)
				{
					auto msg = std::string("Failed to load texture ") + aistr.C_Str();
					throw std::runtime_error(msg);
				}
			}

			vk::Extent3D extent;
			extent.width = width;
			extent.height = height;
			extent.depth = 1;

			auto info = vpp::ViewableImage::defaultColor2D();
			info.imgInfo.extent = extent;
			info.imgInfo.format = vk::Format::r8g8b8a8Unorm;
			info.viewInfo.format = vk::Format::r8g8b8a8Unorm;
			info.imgInfo.usage = vk::ImageUsageBits::sampled;
			info.imgInfo.tiling = vk::ImageTiling::optimal;
			info.imgInfo.usage |= vk::ImageUsageBits::transferDst;

			mmat.diffuseMap = {dev, info};

			auto& img = mmat.diffuseMap.image();
			workManager.add(vpp::fill(img, *imgdata, vk::Format::r8g8b8a8Unorm,
				vk::ImageLayout::undefined, extent, {vk::ImageAspectBits::color, 0, 0}));
		}

		//vulkan resources
		mmat.descriptorSet = {modeldata.descriptorSetLayout, modeldata.descriptorPool};
		mmat.ubo = {dev, bufferInfo};

		workManager.add(vpp::fill140(mmat.ubo, mmat.data));

		vpp::DescriptorSetUpdate update(mmat.descriptorSet);
		update.uniform({{mmat.ubo, 0ul, bufferInfo.size}});
		update.image({{{}, mmat.diffuseMap.vkImageView(), vk::ImageLayout::shaderReadOnlyOptimal}});
	}
}

void initModelData(const vpp::Device& dev, ModelData& data)
{
	static bool initialized = false;
	if(initialized) return;

	initialized = true;

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
	data.sampler = {dev, samplerInfo};

	//descriptorSetLayout
	using vpp::descriptorBinding;
	constexpr auto fragmentStage = vk::ShaderStageBits::fragment;
	constexpr auto vertexStage = vk::ShaderStageBits::fragment;
	constexpr auto fragVertStages = fragmentStage | vertexStage;

	//first set
	//ubo for transform
	//ubo for scene data (light)
	//ubo for material
	//texture
	data.descriptorSetLayout = {dev, {
		descriptorBinding(vk::DescriptorType::uniformBuffer, vertexStage),
		descriptorBinding(vk::DescriptorType::uniformBuffer, fragmentStage),
		descriptorBinding(vk::DescriptorType::uniformBuffer, fragmentStage),
		descriptorBinding(vk::DescriptorType::combinedImageSampler, fragmentStage)
	}};

	//vertexBufferLayout
	vpp::VertexBufferLayout vertexBufferLayout = {
		{
			vk::Format::r32g32b32Sfloat, //position
			vk::Format::r32g32b32Sfloat, //normal
			vk::Format::r32g32Sfloat //uv
		},
	0}; //at binding 0

	//pipeline
	data.pipelineLayout = {dev, {data.descriptorSetLayout}};

	vpp::GraphicsPipelineBuilder builder(dev, data.app.renderPass);
	builder.layout = data.pipelineLayout;
	builder.dynamicStates = {vk::DynamicState::viewport, vk::DynamicState::scissor};
	builder.vertexBufferLayouts = {vertexBufferLayout};

	builder.shader.stage("texture.vert.spv", {vk::ShaderStageBits::vertex});
	builder.shader.stage("texture.frag.spv", {vk::ShaderStageBits::fragment});

	auto& ba = builder.states.blendAttachments;
	ba[0].blendEnable = true;
	ba[0].colorBlendOp = vk::BlendOp::add;
	ba[0].srcColorBlendFactor = vk::BlendFactor::srcAlpha;
	ba[0].dstColorBlendFactor = vk::BlendFactor::oneMinusSrcAlpha;
	ba[0].srcAlphaBlendFactor = vk::BlendFactor::one;
	ba[0].dstAlphaBlendFactor = vk::BlendFactor::zero;
	ba[0].alphaBlendOp = vk::BlendOp::add;

	builder.states.rasterization.cullMode = vk::CullModeBits::none;
	builder.states.inputAssembly.topology = vk::PrimitiveTopology::triangleList;

	data.pipeline = builder.build();

	//init scene data
	const nytl::Vec3f lightPos = {0.f, 3.f, 5.f};
	const nytl::Vec3f lightDir = {0.f, -1.f, -0.5f};
	const nytl::Vec3f lightCol = {1.f, 1.f, 1.f};

	vk::BufferCreateInfo bufferInfo;
	bufferInfo.usage = vk::BufferUsageBits::uniformBuffer;
	bufferInfo.size = vpp::neededBufferSize140(lightPos, lightDir, lightCol);
	data.sceneBuffer = {dev, bufferInfo};

	vpp::WorkManager workManager;
	workManager.add(vpp::fill140(data.sceneBuffer, lightPos, lightDir, lightCol));
	workManager.add(loadScene(dev, data));
}

void ModelRenderer::init(vpp::SwapChainRenderer& renderer)
{
	initModelData(renderer.device(), data_);
	renderer.record();
}

void ModelRenderer::build(unsigned int id, const vpp::RenderPassInstance& ini)
{
	auto cmdBuffer = ini.vkCommandBuffer();
	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics,
		data_.pipeline);

	for(auto& mesh : data_.scene.meshes)
	{
		auto indOffset = mesh.vertices.size() * sizeof(Vertex);
		vk::cmdBindVertexBuffers(cmdBuffer, 0, {mesh.buffer}, {{0}});
		vk::cmdBindIndexBuffer(cmdBuffer, mesh.buffer, indOffset, vk::IndexType::uint32);

		auto& mat = data_.scene.materials[mesh.materialIndex];

		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
			data_.pipelineLayout, 0, {mat.descriptorSet}, {});
		vk::cmdDrawIndexed(cmdBuffer, mesh.indices.size(), 1, 0, 0, 0);
	}
}

std::vector<vk::ClearValue> ModelRenderer::clearValues(unsigned int id)
{
	std::vector<vk::ClearValue> ret(2, vk::ClearValue{});
	ret[0].color = {{0.f, 0.f, 0.f, 1.0f}};
	ret[1].depthStencil = {1.f, 0};
	return ret;
}

void ModelRenderer::frame(unsigned int id)
{

}

int main(int argc, char** argv)
{
	std::string filename = "model.obj";

	App app;
	ModelData data {app, filename};
	initApp(app, [&](){ return std::make_unique<ModelRenderer>(data); });
	mainLoop(app, [&](){ });
}
