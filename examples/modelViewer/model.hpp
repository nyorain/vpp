#pragma once

#include <vpp/buffer.hpp>
#include <vpp/descriptor.hpp>
#include <vpp/image.hpp>
#include <vpp/pipeline.hpp>

#include <nytl/vec.hpp>
#include <nytl/mat.hpp>

struct App;

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
	vpp::Buffer buffer; //verts, indcs, combined (view, model) transform matrix
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

///Host representation of the gpu material/ubo
struct MaterialData
{
	MaterialColors colors;
	float shininess;
	bool texture;
};

struct Material
{
	std::string name;
	vpp::DescriptorSet descriptorSet;
	vpp::ViewableImage diffuseMap;
	MaterialData data;
	vpp::Buffer ubo; //holds the material data
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

	//holds light and view position
	vpp::Buffer sceneBuffer;
};
