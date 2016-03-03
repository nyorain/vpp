#pragma once

#include <vpp/device.hpp>
#include <vpp/buffer.hpp>
#include <vpp/resource.hpp>

namespace vpp
{

class Mesh;

/*
class Light
{
public:
	enum class Type
	{
		point,
		directional
	};

};

class Scene
{
protected:
	std::vector<Mesh> meshes_;
	std::vector<Light> lights_;
};

class Material
{
protected:
	std::vector<std::shared_ptr<Texture>> textures_;
	nytl::Vec4f diffuseColor_;
	nytl::Vec4f
};
*/

class Mesh
{
public:
	vpp::Buffer vertexBuffer_;
	vpp::Buffer indexBuffer_;
	std::size_t indexCount_;

public:
	const vpp::Buffer& vertexBuffer() const { return vertexBuffer_; };
	const vpp::Buffer& indexBuffer() const { return indexBuffer_; };
	std::size_t indexCount() const { return indexCount_; }
};

class MeshLoader : public Resource
{
public:
	enum class VertexData
	{
		position,
		color,
		normal,
		textureCoords,
		binormal,
		tangent
	};

protected:
	class Impl;
	std::unique_ptr<Impl> impl_;

public:
	MeshLoader(const Device& device);
	~MeshLoader();

	std::vector<Mesh> load(const std::string& file, const std::vector<VertexData>& vertexData);
};

}
