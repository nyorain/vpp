#include "model.hpp"
#include <vpp/pipeline.hpp>
#include <vpp/graphicsPipeline.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/vk.hpp>
#include <example.hpp>

#include <nytl/vec.hpp>
#include <nytl/stringParam.hpp>
#include <nytl/convert.hpp>
#include <nytl/transform.hpp>
#include <nytl/time.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wmisleading-indentation"
#pragma GCC diagnostic warning "-Wunused-variable"
#endif

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

//prototype. end of file
vpp::RenderPass initOffscreenRenderPass(const vpp::Device& dev);

class ModelRenderer : public vpp::RendererBuilder
{
public:
	ModelRenderer(ModelData& data) : data_(data) {}

	void build(unsigned int, const vpp::RenderPassInstance& ini) override;
	void init(vpp::SwapChainRenderer& renderer) override;
	void frame(unsigned int id) override;
	AdditionalSemaphores submit(unsigned int) override;
	std::vector<vk::ClearValue> clearValues(unsigned int) override;

protected:
	ModelData& data_;
};

//implementation
//TODO: both load/init functions can be really improved. Use vpp::WorkManager.
vpp::WorkManager loadScene(const vpp::Device& dev, ModelData& modeldata)
{
	auto flags =
		// aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices |
		// aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;

		aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace;

	Assimp::Importer importer;
	auto aiscene = importer.ReadFile(modeldata.filename, flags);
	if(!aiscene)
		throw std::runtime_error("assimp ReadFile failed for file " + modeldata.filename);

	//one extra uniform descriptor for offscreen rendering
	//one extra combined image sampler descriptor for quad
	vk::DescriptorPoolSize typeCounts[2];
	typeCounts[0].type = vk::DescriptorType::uniformBuffer;
	typeCounts[0].descriptorCount = aiscene->mNumMeshes * 3 + 1;

	typeCounts[1].type = vk::DescriptorType::combinedImageSampler;
	typeCounts[1].descriptorCount = 2 * aiscene->mNumMeshes + 1;

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = typeCounts;
	poolInfo.maxSets = aiscene->mNumMaterials + 2;

	modeldata.descriptorPool = {dev, poolInfo};

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

			// std::cout << vertex.position << "\n";
			// std::cout << vertex.normal << "\n";

			//why have assimp tex coords 3 components?
			if(mesh->HasTextureCoords(0))
				vertex.texCoords = nytl::convert<nytl::Vec3f>(mesh->mTextureCoords[0][i]);
		}

		// std::cout << "incs \n";

		mmesh.indices.resize(mesh->mNumFaces * 3);
		for(std::size_t i(0); i < mesh->mNumFaces; ++i)
		{
			auto& face = mesh->mFaces[i];

			mmesh.indices[i * 3] = face.mIndices[0];
			mmesh.indices[i * 3 + 1] = face.mIndices[1];
			mmesh.indices[i * 3 + 2] = face.mIndices[2];

			// std::cout << mmesh.indices[i * 3] << "\n";
			// std::cout << mmesh.indices[i * 3 + 1] << "\n";
			// std::cout << mmesh.indices[i * 3 + 2] << "\n";
		}

		//buffer data
		auto verticesSize = mmesh.vertices.size() * sizeof(Vertex);
		auto indicesSize = mmesh.indices.size() * sizeof(std::uint32_t);

		std::vector<std::uint8_t> bufferData(verticesSize + indicesSize);
		if(bufferData.empty()) continue;

		//first all vertices then all indices
		std::memcpy(bufferData.data(), mmesh.vertices.data(), verticesSize);
		std::memcpy(bufferData.data() + verticesSize, mmesh.indices.data(), indicesSize);

		//create the buffer
		//vertex index
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.size = bufferData.size();
		bufferInfo.usage =
			vk::BufferUsageBits::vertexBuffer |
			vk::BufferUsageBits::indexBuffer |
			vk::BufferUsageBits::transferDst;
		mmesh.buffer = {dev, bufferInfo, vk::MemoryPropertyBits::deviceLocal};
		mmesh.buffer.bufferSize = bufferInfo.size;
		workManager.add(vpp::fill140(mmesh.buffer, vpp::raw(bufferData)));

		//transform ubo
		bufferInfo.size = sizeof(nytl::Mat4f);
		bufferInfo.usage = vk::BufferUsageBits::uniformBuffer;
		mmesh.ubo = {dev, bufferInfo, vk::MemoryPropertyBits::hostVisible};
		mmesh.ubo.bufferSize = bufferInfo.size;

		// auto cameraMat = nytl::perspective3(110.f, 900.f / 900.f, 0.01f, 100.f);
		// auto cameraMat = nytl::identityMat<4, float>();
		// workManager.add(vpp::fill140(mmesh.ubo, cameraMat));

		mmesh.descriptorSet = {modeldata.descriptorSetLayout, modeldata.descriptorPool};

		//mesh
		modeldata.scene.meshes.push_back(std::move(mmesh));
	}

	//vulkan descriptor pool for materials
	//pool

	//uniform buffer info
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.usage = vk::BufferUsageBits::uniformBuffer | vk::BufferUsageBits::transferDst;
	bufferInfo.size = vpp::neededBufferSize140<MaterialData>();

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
				auto id = static_cast<unsigned int>(std::stoi(aistr.data + 1));
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
		mmat.ubo = {dev, bufferInfo};
		mmat.ubo.bufferSize = bufferInfo.size;

		mmat.ubo.assureMemory();
		workManager.add(vpp::fill140(mmat.ubo, mmat.data));
	}

	//update mesh descriptors
	for(auto& mesh : modeldata.scene.meshes)
	{
		vpp::DescriptorSetUpdate update(mesh.descriptorSet);

		update.uniform({{mesh.ubo, 0ul, mesh.ubo.bufferSize}}); //transform ubo

		auto& mat = modeldata.scene.materials[mesh.materialIndex];
		update.uniform({{modeldata.sceneBuffer, 0ul, modeldata.sceneBuffer.bufferSize}});
		update.uniform({{mat.ubo, 0ul, mat.ubo.bufferSize}});

		if(mat.data.texture)
		{
			update.imageSampler({{{}, mat.diffuseMap.vkImageView(),
				vk::ImageLayout::shaderReadOnlyOptimal}});
		}

		auto imageView = modeldata.offscreen.framebuffer.attachments()[0].vkImageView();
		update.imageSampler({{{}, imageView, vk::ImageLayout::shaderReadOnlyOptimal}}, 4);
	}

	return workManager;
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
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::linear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::clampToBorder;
	samplerInfo.addressModeV = vk::SamplerAddressMode::clampToBorder;
	samplerInfo.addressModeW = vk::SamplerAddressMode::clampToBorder;
	samplerInfo.mipLodBias = 0;
	samplerInfo.anisotropyEnable = true;
	samplerInfo.maxAnisotropy = 0;
	samplerInfo.compareEnable = false;
	samplerInfo.compareOp = {};
	samplerInfo.minLod = 0;
	samplerInfo.maxLod = 1;
	samplerInfo.borderColor = vk::BorderColor::floatOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = false;
	data.sampler = {dev, samplerInfo};

	//descriptorSetLayout
	using vpp::descriptorBinding;
	constexpr auto fragmentStage = vk::ShaderStageBits::fragment;
	constexpr auto vertexStage = vk::ShaderStageBits::vertex;
	constexpr auto fragVertStages = fragmentStage | vertexStage;

	//first set
	//ubo for transform
	//ubo for scene data (light)
	//ubo for material
	//texture
	data.descriptorSetLayout = {dev, {
		descriptorBinding(vk::DescriptorType::uniformBuffer, vertexStage),
		descriptorBinding(vk::DescriptorType::uniformBuffer, fragVertStages),
		descriptorBinding(vk::DescriptorType::uniformBuffer, fragmentStage),
		descriptorBinding(vk::DescriptorType::combinedImageSampler, fragmentStage, -1, 1,
			&data.sampler.vkHandle()),
		descriptorBinding(vk::DescriptorType::combinedImageSampler, fragmentStage, -1, 1,
			&data.sampler.vkHandle())
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

	builder.shader.stage("model.vert.spv", {vk::ShaderStageBits::vertex});
	builder.shader.stage("model.frag.spv", {vk::ShaderStageBits::fragment});

	auto& ba = builder.states.blendAttachments;
	ba[0].blendEnable = false;
	ba[0].colorBlendOp = vk::BlendOp::add;
	ba[0].srcColorBlendFactor = vk::BlendFactor::srcAlpha;
	ba[0].dstColorBlendFactor = vk::BlendFactor::oneMinusSrcAlpha;
	ba[0].srcAlphaBlendFactor = vk::BlendFactor::one;
	ba[0].dstAlphaBlendFactor = vk::BlendFactor::zero;
	ba[0].alphaBlendOp = vk::BlendOp::add;

	builder.states.depthStencil.depthTestEnable = true;
	builder.states.depthStencil.depthWriteEnable = true;
	builder.states.rasterization.cullMode = vk::CullModeBits::none;

	data.pipeline = builder.build();

	//init scene data
	const nytl::Vec3f viewPos = {0.f, 0.f, 0.f};
	const nytl::Vec3f lightPos = {0.f, 20.f, 0.f};
	const nytl::Vec3f lightDir = {1.f, -1.f, 0.f};
	const nytl::Vec3f lightCol = {1.f, 1.f, 1.f};

	vk::BufferCreateInfo bufferInfo;
	bufferInfo.usage = vk::BufferUsageBits::uniformBuffer;

	//3 light vars, shadowMap transformMatrix
	{
		using namespace nytl;
		bufferInfo.size = vpp::neededBufferSize140<Vec3f, Vec3f, Vec3f, Mat4f>();
	}

	//transformMatrix for shadow map
	// float ratio = data.offscreen.width / data.offscreen.height;

	auto proj = nytl::ortho3(-10.f, 10.f, -10.f, 10.f, 1.f, 8.f);
	// auto proj = nytl::perspective3(45.f, ratio, 0.1f, 100.f);
	// auto view = nytl::lookAt(lightPos, lightPos + lightDir, {0.f, 1.f, 0.f});
	// auto view = nytl::lookAt(lightPos, lightPos + lightDir, nytl::cross(lightDir, nytl::Vec3f{0.f, 0.f, 1.f}));

	// auto up = nytl::cross(lightDir, nytl::Vec3f{0.f, 0.f, 1.f});
	auto view = nytl::lookAt({-1.f, 4.f, -1.f}, nytl::Vec3f{0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
	auto mat = proj * view;

	std::cout << mat << "\n";
	std::cout << proj << "\n";
	std::cout << view << "\n";

	data.sceneBuffer = {dev, bufferInfo, vk::MemoryPropertyBits::hostVisible};
	data.sceneBuffer.bufferSize = bufferInfo.size;
	data.sceneBuffer.assureMemory();
	// auto offscreenTransformOffset = bufferInfo.size - sizeof(nytl::Mat4f);

	// need to be done before init the scene
	data.offscreen.renderPass = initOffscreenRenderPass(dev);
	data.offscreen.framebuffer = {dev, data.offscreen.renderPass,
		{data.offscreen.width, data.offscreen.height}, {vpp::ViewableImage::defaultDepth2D()}};


	{
		vpp::WorkManager workManager;
		// workManager.add(vpp::fill140(data.sceneBuffer, viewPos, lightPos, lightCol));
		workManager.add(vpp::fill140(data.sceneBuffer, viewPos, lightDir, lightCol,
			mat));
		workManager.add(loadScene(dev, data));
	}



	//quad pipeline stuff since similiar to normal scene one
	data.quad.descriptorSetLayout = {dev, {
		descriptorBinding(vk::DescriptorType::combinedImageSampler, fragmentStage, 0, 1,
			&data.sampler.vkHandle())
	}};

	data.quad.pipelineLayout = {dev, {data.quad.descriptorSetLayout}};
	data.quad.descriptorSet = {data.quad.descriptorSetLayout, data.descriptorPool};

	builder.layout = data.quad.pipelineLayout;
	builder.vertexBufferLayouts = {};
	builder.shader.stage("modelQuad.vert.spv", {vk::ShaderStageBits::vertex});
	builder.shader.stage("modelQuad.frag.spv", {vk::ShaderStageBits::fragment});
	data.quad.pipeline = builder.build();




	//offscreen renderPass& pipeline
	vk::Viewport viewport;
	viewport.width = data.offscreen.width;
	viewport.height = data.offscreen.height;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	vk::Rect2D scissor;
	scissor.extent = {data.offscreen.width, data.offscreen.height};
	scissor.offset = {0, 0};

	data.offscreen.semaphore = vk::createSemaphore(dev, {});
	data.offscreen.descriptorSetLayout = {dev, {
		descriptorBinding(vk::DescriptorType::uniformBuffer, vertexStage),
	}};

	//vertexBufferLayout
	data.offscreen.descriptorSet = {data.offscreen.descriptorSetLayout, data.descriptorPool};
	data.offscreen.pipelineLayout = {dev, {data.offscreen.descriptorSetLayout}};

	{
		vpp::DescriptorSetUpdate update(data.offscreen.descriptorSet);
		// update.uniform({{data.sceneBuffer, offscreenTransformOffset, sizeof(nytl::Mat4f)}});
		update.uniform({{data.sceneBuffer, 0, data.sceneBuffer.bufferSize}});
	}

	//pipeline
	{
		vpp::GraphicsPipelineBuilder b2(dev, data.offscreen.renderPass);
		b2.dynamicStates = {};
		b2.layout = data.offscreen.pipelineLayout;
		b2.vertexBufferLayouts = {vertexBufferLayout};

		// b2.states.viewport.pViewports = &viewport;
		// b2.states.viewport.pScissors = &scissor;
		b2.states.viewports.push_back(viewport);
		b2.states.scissors.push_back(scissor);
		b2.states.blendAttachments.clear();

		b2.shader.stage("modelOffscreen.vert.spv", {vk::ShaderStageBits::vertex});
		b2.shader.stage("modelOffscreen.frag.spv", {vk::ShaderStageBits::fragment});

		data.offscreen.pipeline = b2.build();
	}

	// auto qf = dev.queue(vk::QueueBits::graphics)->family();
	auto qf = dev.queues()[0]->family();
	data.offscreen.commandBuffer = dev.commandProvider().get(qf);

	//record the commandBuffer
	{
		auto& cmd = data.offscreen.commandBuffer;
		vk::beginCommandBuffer(cmd, {});

		vk::ClearValue clearValue {};
		clearValue.depthStencil = {1.f, 0};

		vk::RenderPassBeginInfo rpbInfo;
		rpbInfo.renderPass = data.offscreen.renderPass;
		rpbInfo.framebuffer = data.offscreen.framebuffer;
		rpbInfo.clearValueCount = 1;
		rpbInfo.pClearValues = &clearValue;
		rpbInfo.renderArea = {{0, 0}, {data.offscreen.width, data.offscreen.height}};
		vk::cmdBeginRenderPass(cmd, rpbInfo, vk::SubpassContents::eInline);

		vk::cmdBindPipeline(cmd, vk::PipelineBindPoint::graphics,
			data.offscreen.pipeline);
		vk::cmdBindDescriptorSets(cmd, vk::PipelineBindPoint::graphics,
			data.offscreen.pipelineLayout, 0, {data.offscreen.descriptorSet}, {});

		for(auto& mesh : data.scene.meshes)
		{
			auto indOffset = mesh.vertices.size() * sizeof(Vertex);
			vk::cmdBindVertexBuffers(cmd, 0, {mesh.buffer}, {{0}});
			vk::cmdBindIndexBuffer(cmd, mesh.buffer, indOffset, vk::IndexType::uint32);
			vk::cmdDrawIndexed(cmd, mesh.indices.size(), 1, 0, 0, 0);
		}

		vk::cmdEndRenderPass(cmd);
		vk::endCommandBuffer(cmd);
	}




	//init stuff for (test-) drawing the shadow map
	{
		vpp::DescriptorSetUpdate update(data.quad.descriptorSet);
		auto imgView = data.offscreen.framebuffer.attachments()[0].vkImageView();
		// auto imgView = data.app.renderer.staticAttachments()[0].vkImageView();
		update.imageSampler({{{}, imgView, vk::ImageLayout::shaderReadOnlyOptimal}});
		update.apply();
	}

}

void ModelRenderer::init(vpp::SwapChainRenderer& renderer)
{
	static bool i = false;
	if(!i)
	{
		i = true;
		initModelData(renderer.device(), data_);
	}

	// {
	// 	vpp::DescriptorSetUpdate update(data_.quad.descriptorSet);
	// 	// auto imgView = data.offscreen.framebuffer.attachments()[0].vkImageView();
	// 	auto imgView = data_.app.renderer.staticAttachments()[0].vkImageView();
	// 	update.imageSampler({{data_.sampler, imgView, vk::ImageLayout::general}});
	// }

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

		// auto& mat = data_.scene.materials[mesh.materialIndex];
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
			data_.pipelineLayout, 0, {mesh.descriptorSet}, {});
		vk::cmdDrawIndexed(cmdBuffer, mesh.indices.size(), 1, 0, 0, 0);
	}

	//draw quad
	{
		vk::cmdBindPipeline(cmdBuffer, vk::PipelineBindPoint::graphics,
			data_.quad.pipeline);
		vk::cmdBindDescriptorSets(cmdBuffer, vk::PipelineBindPoint::graphics,
			data_.quad.pipelineLayout, 0, {data_.quad.descriptorSet}, {});
		vk::cmdDraw(cmdBuffer, 6, 1, 0, 0);
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

ModelRenderer::AdditionalSemaphores ModelRenderer::submit(unsigned int)
{
	auto& dev = data_.app.context.device();
	auto* queue = dev.queues()[0].get();

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &data_.offscreen.commandBuffer.vkHandle();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &data_.offscreen.semaphore;
	dev.submitManager().add(*queue, submitInfo);
	dev.submitManager().submit();

	return {{data_.offscreen.semaphore, vk::PipelineStageBits::topOfPipe}};
}

int main(int argc, char** argv)
{
	std::string filename = "model.obj";

	App app;
	ModelData data {app, filename};
	initApp(app, [&](){ return std::make_unique<ModelRenderer>(data); });

	nytl::Vec2i mpos;
	float yaw = -90.f;
	float pitch  = 0.f;
	nytl::Vec3f cameraFront = {0.f, 0.f, -1.f};
	bool firstMouse = true;
	app.onMove = [&](const nytl::Vec2i& vec) {
		if(firstMouse)
		{
			mpos = vec;
			firstMouse = false;
			return;
		}

		auto delta = vec - mpos;
		mpos = vec;

		yaw += delta.x * 0.5;
		pitch += delta.y * 0.5;

		if(pitch > 89.0f) pitch = 89.0f;
		if(pitch < -89.0f) pitch = -89.0f;

		nytl::Vec3f front;
		front.x = cos(nytl::radians(yaw)) * cos(nytl::radians(pitch));
		front.y = sin(nytl::radians(pitch));
		front.z = sin(nytl::radians(yaw)) * cos(nytl::radians(pitch));
		cameraFront = nytl::normalize(front);
	};

	// app.onKeyPress = [&](unsigned int key) {
	// 	//key C
	// 	if(key == 0x43)
	// 	{
	// 		// auto width = app.context.swapChain().size().width;
	// 		// auto height = app.context.swapChain().size().height;
	//
	// 		auto width = data.offscreen.width;
	// 		auto height = data.offscreen.height;
	//
	// 		std::vector<std::uint8_t> uintData(width * height);
	//
	// 		// auto work = vpp::retrieve(app.renderer.staticAttachments()[0].image(),
	// 		auto work = vpp::retrieve(data.offscreen.framebuffer.attachments()[0].image(),
	// 			vk::ImageLayout::shaderReadOnlyOptimal, vk::Format::d32Sfloat,
	// 			{width, height, 1},
	// 			{vk::ImageAspectBits::depth, 0, 0});
	//
	// 		auto& floatData = reinterpret_cast<float&>(work->data());
	// 		for(auto i = 0u; i < uintData.size(); ++i)
	// 			uintData[i] = 255. * linearize((&floatData)[i]);
	//
	// 		stbi_write_png("depthBuffer.png", width, height, 1,
	// 			uintData.data(), width);
	// 	}
	// };

	nytl::Timer timer;
	float totalTime;
	mainLoop(app, [&](){
		auto time = timer.elapsedTime().microseconds();
		timer.reset();
		auto timeFac = time / 100000.;
		totalTime += time / 5000000.;

		nytl::Vec3f delta;

		//wsadqe
		auto xaxis = nytl::normalize(nytl::cross(cameraFront, nytl::Vec3f{0.f, 1.f, 0.f}));
		auto yaxis = nytl::normalize(nytl::cross(cameraFront, xaxis));
		if((1 << 16) & ::GetAsyncKeyState(0x57)) delta += timeFac * cameraFront;
		if((1 << 16) & ::GetAsyncKeyState(0x53)) delta -= timeFac * cameraFront;

		if((1 << 16) & ::GetAsyncKeyState(0x41)) delta -= timeFac * xaxis;
		if((1 << 16) & ::GetAsyncKeyState(0x44)) delta += timeFac * xaxis;

		if((1 << 16) & ::GetAsyncKeyState(0x51)) delta += timeFac * yaxis;
		if((1 << 16) & ::GetAsyncKeyState(0x45)) delta -= timeFac * yaxis;

		data.viewPos += delta;

		// auto viewMat = nytl::identityMat<4, float>();
		// nytl::translate(viewMat, data.viewPos);
		// nytl::rotate(viewMat, {0.f, 1.f, 0.f});
		// viewMat.invert();

		auto modelMat = nytl::identityMat<4, float>();
		// nytl::translate(modelMat, {0.f, 2.f, -5.f});

		auto viewMat = nytl::lookAt(data.viewPos, data.viewPos + cameraFront, {0.f, 1.f, 0.f});
		auto cameraMat = nytl::perspective3(45.f, 1400.f / 900.f, 0.1f, 100.f);
		auto mat = cameraMat * viewMat * modelMat;

		// std::cout << viewMat << "\n";
		// std::cout << cameraMat << "\n";
		// std::cout << mat << "\n";

		const nytl::Vec3f lightPos {0.f, 20.f, 0.f};
		const nytl::Vec3f lightDir {-1.f, -1.f, -1.f};
		// const nytl::Vec3f lightDir {std::sin(totalTime), -1.f, std::cos(totalTime)};
		// const nytl::Vec3f lightCol {1.f, std::sin(totalTime), 1.f};
		const nytl::Vec3f lightCol {1.f, 1.f, 1.f};

		vpp::WorkManager manager;
		manager.add(vpp::fill140(data.sceneBuffer, data.viewPos, lightDir, lightCol));
		for(auto& mesh : data.scene.meshes)
		{
			manager.add(vpp::fill140(mesh.ubo, mat));
		}
	});

	vk::destroySemaphore(app.context.device(), data.offscreen.semaphore);
}

vpp::RenderPass initOffscreenRenderPass(const vpp::Device& dev)
{
	vk::AttachmentDescription attachments[1] {};

	//depth
	attachments[0].format = vk::Format::d32Sfloat;
	attachments[0].samples = vk::SampleCountBits::e1;
	attachments[0].loadOp = vk::AttachmentLoadOp::clear;
	attachments[0].storeOp = vk::AttachmentStoreOp::store;
	attachments[0].stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachments[0].stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachments[0].initialLayout = vk::ImageLayout::undefined;
	attachments[0].finalLayout = vk::ImageLayout::shaderReadOnlyOptimal;

	vk::AttachmentReference depthReference;
	depthReference.attachment = 0;
	depthReference.layout = vk::ImageLayout::depthStencilAttachmentOptimal;

	//only subpass
	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::graphics;
	subpass.flags = {};
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	subpass.colorAttachmentCount = 0;
	subpass.pColorAttachments = nullptr;
	subpass.pResolveAttachments = nullptr;
	subpass.pDepthStencilAttachment = &depthReference;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = attachments;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies = nullptr;

	return {dev, renderPassInfo};
}
