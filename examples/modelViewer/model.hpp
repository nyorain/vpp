#pragma once

#include <vpp/buffer.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/image.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/framebuffer.hpp>

#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

struct App;

struct SizedBuffer : public vpp::Buffer
{
	using vpp::Buffer::Buffer;
	vk::DeviceSize bufferSize;
};

///Represents one vertex of a mesh and all data it could possibly hold.
struct Vertex
{
	nytl::Vec3f position;
	nytl::Vec3f normal;
	nytl::Vec2f texCoords;
};

///Represents a mesh, i.e. a set of vertices that all are drawn using the same material.
struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<std::uint32_t> indices;
	unsigned int materialIndex {};

	//we use two buffers per mesh, since one is deviceLocal and the other one hostVisible
	SizedBuffer buffer; //verts, indcs
	SizedBuffer ubo; //combined (view, model) transform matrix
	vpp::DescriptorSet descriptorSet;
};

struct MaterialColors
{
	nytl::Vec4f ambient;
	nytl::Vec4f diffuse;
	nytl::Vec4f specular;
};

struct DirLight
{
	nytl::Vec3f dir;
	MaterialColors colors;
};

struct PointLight
{
	nytl::Vec3f pos;
	MaterialColors colors;
};

///Host representation of the device material/ubo
struct MaterialData
{
	MaterialColors colors;
	float shininess;
	bool texture;
};

struct Material
{
	std::string name;
	vpp::ViewableImage diffuseMap;
	MaterialData data;
	SizedBuffer ubo; //holds the material data
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
	vpp::DescriptorPool descriptorPool;
	vpp::Sampler sampler;
	nytl::Vec3f viewPos;

	struct
	{
		vpp::Pipeline pipeline;
		vpp::PipelineLayout pipelineLayout;
		vpp::DescriptorSetLayout descriptorSetLayout;
		vpp::RenderPass renderPass;
		vpp::CommandBuffer commandBuffer;
		vpp::Framebuffer framebuffer;
		vpp::DescriptorSet descriptorSet;
		vk::Semaphore semaphore;

		unsigned int width = 1024;
		unsigned int height = 1024;
	} offscreen;

	struct
	{
		vpp::Pipeline pipeline;
		vpp::PipelineLayout pipelineLayout;
		vpp::DescriptorSetLayout descriptorSetLayout;
		vpp::DescriptorSet descriptorSet;
	} quad;

	//holds light and view position/dir
	//also holds the transform matrix for the shadow map
	SizedBuffer sceneBuffer;
};
