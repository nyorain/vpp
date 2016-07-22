#include <example.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>

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

//mesh loading
struct Vertex
{
	nytl::Vec3f position;
	nytl::Vec2f texCoords;
	nytl::Vec3f normal;
};

struct MeshInfo
{
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
	unsigned int materialIndex;
};

struct Mesh
{
	MeshInfo info;
	vpp::Buffer buffer;
};

struct Material
{
	std::string name;

	nytl::Vec4f ambient;
	nytl::Vec4f diffuse;
	nytl::Vec4f specular;
	float opacity;

	vpp::DescriptorSet descriptorSet;
	vpp::ViewableImage texture;
	vpp::Buffer uniformData;
};

struct Scene
{
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
};

struct ModelData
{
	App& app;
	std::string filename;
	Scene scene;

	vpp::PipelineLayout pipelineLayout;
	vpp::Pipeline pipeline;
	vpp::DescriptorSetLayout descriptorSetLayout;
	vpp::DescriptorSet descriptorSet;
	vpp::DescriptorPool descriptorPool;
	vpp::Sampler sampler;
	vpp::ViewableImage texture;
	vpp::Buffer uniformBuffer;
};

class ModelRenderer : public vpp::RendererBuilder
{
public:
	ModelRenderer(ModelData& data) : data_(data) {}

	void build(unsigned int, const vpp::RenderPassInstance& ini) override;
	void init(vpp::SwapChainRenderer& renderer);
	std::vector<vk::ClearValue> clearValues(unsigned int) override;

protected:
	ModelData& data_;
};

//implementation
Scene loadScene(const vpp::Device& dev, const nytl::StringParam& filename)
{
	Scene ret;

	auto flags =
		aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;

	Assimp::Importer importer;
	auto scene = importer.ReadFile(filename, flags);
	if(!scene)
	{
		//error
		std::cerr << "assimp ReadFile failed for file " << filename << "\n";
		return ret;
	}

	ret.meshes.reserve(scene->mNumMeshes);

	for(auto& mesh : nytl::makeRange(*scene->mMeshes, scene->mNumMeshes))
	{
		MeshInfo meshinfo;

		meshinfo.materialIndex = mesh->mMaterialIndex;
		meshinfo.vertices.resize(mesh->mNumVertices);

		for(std::size_t i(0); i < mesh->mNumVertices; ++i)
		{
			auto& vertex = meshinfo.vertices[i];

			vertex.position = nytl::convert(mesh->mVertices[i]);
			vertex.normal = nytl::convert(mesh->mNormals[i]);

			if(mesh->HasTextureCoords(0))
			{
				vertex.texCoords = nytl::convert(mesh->mTextureCoords[0][i]);
			}
		}

		meshinfo.indices.resize(mesh->mNumFaces * 3);
		for(std::size_t i(0); i < mesh->mNumFaces; ++i)
		{
			auto& face = mesh->mFaces[i];

			meshinfo.indices[i * 3] = face.mIndices[0];
			meshinfo.indices[i * 3 + 1] = face.mIndices[1];
			meshinfo.indices[i * 3 + 2] = face.mIndices[2];
		}

		//buffer data
		auto verticesSize = meshinfo.vertices.size() * sizeof(Vertex);
		auto indicesSize = meshinfo.indices.size() * sizeof(std::uint32_t);

		std::vector<std::uint8_t> bufferData(verticesSize + indicesSize);
		std::size_t offset = 0;

		//first all vertices then all indices
		std::memcpy(bufferData.data(), meshinfo.vertices.data(), verticesSize);
		std::memcpy(bufferData.data() + verticesSize, meshinfo.indices.data(), indicesSize);

		//create the buffer
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.size = offset;
		bufferInfo.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::indexBuffer;

		vpp::Buffer buffer(dev, bufferInfo, vk::MemoryPropertyBits::deviceLocal);
		vpp::fill140(buffer, vpp::raw(bufferData));

		//mesh
		Mesh mmesh {std::move(meshinfo), std::move(buffer)};
		ret.meshes.push_back(std::move(mmesh));
	}

	//load the materials
	ret.materials.reserve(scene->mNumMeshes);
	for(auto& material : nytl::makeRange(*scene->mMaterials, scene->mNumMaterials))
	{
		ret.materials.emplace_back();
		auto& mmat = ret.materials.back();

		aiString aistr;
		material->Get(AI_MATKEY_NAME, aistr);
		mmat.name = aistr.C_Str();

		aiColor4D color;
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mmat.ambient = nytl::convert(color);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mmat.diffuse = nytl::convert(color);
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mmat.specular = nytl::convert(color);
		material->Get(AI_MATKEY_OPACITY, mmat.opacity);

		if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &aistr);
			//load texture
		}
	}

	return ret;
}

void initModelData(const vpp::Device& dev, const std::string& modelFile, ModelData& data)
{
	static bool initialized = false;
	if(initialized) return;

	initialized = true;
	data.scene = loadMeshes(dev, modelFile);

	{
		// int width, height, comp;
		// auto imgdata = stbi_load("image.png", &width, &height, &comp, 4);
		// if(!imgdata) throw std::runtime_error("Failed to load image image.png");

		vk::Extent3D extent;
		extent.width = width;
		extent.height = height;
		extent.depth = 1;

		auto info = vpp::ViewableImage::defaultColor2D();
		info.imgInfo.extent = extent;
		info.imgInfo.format = vk::Format::r8g8b8a8Unorm;
		info.viewInfo.format = vk::Format::r8g8b8a8Unorm;
		info.imgInfo.usage = vk::ImageUsageBits::sampled;

		//or this
		info.imgInfo.tiling = vk::ImageTiling::optimal;
		info.imgInfo.usage |= vk::ImageUsageBits::transferDst;

		data.texture = {dev, info};

		vpp::fill(data.texture.image(), *imgdata, vk::Format::r8g8b8a8Unorm,
			vk::ImageLayout::undefined, extent, {vk::ImageAspectBits::color, 0, 0})->finish();

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

		data.descriptorPool = {dev, info};

		//set
		data.descriptorSetLayout = {dev,
			{vpp::descriptorBinding(vk::DescriptorType::combinedImageSampler,
			vk::ShaderStageBits::fragment)}};

		data.descriptorSet = vpp::DescriptorSet(layout, data.descriptorPool);

		//write
		vpp::DescriptorSetUpdate update(data.descriptorSet);
		update.imageSampler({{data.sampler, data.texture.vkImageView(), vk::ImageLayout::general}});
	}

	//pipeline
	{
		data.pipelineLayout = {dev, {data.descriptorSetLayout}};

		vpp::GraphicsPipelineBuilder builder(dev, data.app.renderPass);
		builder.layout = data.pipelineLayout;
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

		data.pipeline = builder.build();
	}
}

void ModelRenderer::init(vpp::SwapChainRenderer& renderer)
{
	initModelData(renderer.device(), data_.filename, data_);
	renderer.record();
}

void ModelRenderer::build(unsigned int id, const vpp::RenderPassInstance& ini)
{
	auto cmdBuffer = ini.vkCommandBuffer();
	vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics,
		data_.pipeline);

	for(auto& mesh : data_.meshes)
	{
		// vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
		// 	data_.pipelineLayout, 0, {data_.descriptorSet}, {});
		// vk::cmdDraw(cmdBuffer, 6, 1, 0, 0);
	}
}

std::vector<vk::ClearValue> ModelRenderer::clearValues(unsigned int id)
{
	std::vector<vk::ClearValue> ret(2, vk::ClearValue{});
	ret[0].color = {{0.f, 0.f, 0.f, 1.0f}};
	ret[1].depthStencil = {1.f, 0};
	return ret;
}

int main(int argc, char** argv)
{
	std::string filename = "model.obj";

	App app;
	ModelData data {app, filename};
	initApp(app, [&](){ return std::make_unique<ModelRenderer>(data); });
	mainLoop(app, [&](){ });
}
