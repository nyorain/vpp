#include "mesh.hpp"

#include <nytl/vec.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

namespace vpp
{

struct Vertex
{
	nytl::Vec3f position;
	nytl::Vec3f normal;
	nytl::Vec2f textureCoords;
};

struct MeshInfo
{
	std::vector<Vertex> vertices_;
	std::vector<std::uint32_t> indices_;
	unsigned int materialIndex_;
};

class MeshLoader::Impl : public Resource
{
public:
	Assimp::Importer importer_;

public:
	using Resource::Resource;

	std::vector<MeshInfo> init(const std::string& filename);
	std::vector<MeshInfo> init(const aiScene& scene);
	MeshInfo init(const aiMesh& mesh, const aiScene& scene);
};

nytl::Vec3f nytlVec(const aiVector3D& vec){ return {vec.x, vec.y, vec.z}; }

//MeshLoader
MeshLoader::MeshLoader(const Device& dev) : Resource(dev)
{
	impl_.reset(new Impl());
}

MeshLoader::~MeshLoader()
{
}

std::vector<Mesh> MeshLoader::load(const std::string& file, const std::vector<VertexData>& vertexData)
{
	std::vector<Mesh> ret;
	vpp::DeviceMemoryAllocator allocator(device());

	auto meshinfos = impl_->init(file);
	ret.reserve(meshinfos.size());

	for(auto& meshinfo : meshinfos)
	{
		std::vector<std::uint8_t> vertexBufferData;

		std::size_t offset = 0;
		for(auto& vert : meshinfo.vertices_)
		{
			for(auto type : vertexData)
			{
				float* data = nullptr;
				std::size_t size = 0;

				if(type == VertexData::position)
				{
					data = vert.position.data();
					size = 3 * sizeof(float);
				}
				else if(type == VertexData::normal)
				{
					data = vert.normal.data();
					size = 3 * sizeof(float);
				}
				else if(type == VertexData::textureCoords)
				{
					data = vert.textureCoords.data();
					size = 2 * sizeof(float);
				}

				std::memcpy(vertexBufferData.data() + offset, data, size);
				offset += size;
			}
		}

		//vertices
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.size(offset);
		bufferInfo.usage(vk::BufferUsageFlagBits::VertexBuffer);

		vpp::Buffer vertices(allocator, bufferInfo, vk::MemoryPropertyFlagBits::HostVisible);
		auto map = vertices.memoryMap();

		std::memcpy(map.ptr(), vertexBufferData.data(), offset);

		//indices
		bufferInfo.size(meshinfo.indices_.size() * sizeof(std::uint32_t));
		bufferInfo.usage(vk::BufferUsageFlagBits::IndexBuffer);

		vpp::Buffer indices(allocator, bufferInfo, vk::MemoryPropertyFlagBits::HostVisible);
		map = indices.memoryMap();

		std::memcpy(map.ptr(), meshinfo.indices_.data(), bufferInfo.size());

		//mesh
		Mesh mesh{std::move(vertices), std::move(indices), meshinfo.indices_.size()};
		ret.push_back(std::move(mesh));
	}

	return ret;
}

//MeshLoaderImpl
std::vector<MeshInfo> MeshLoader::Impl::init(const std::string& filename)
{
	auto flags =
		aiProcess_FlipWindingOrder | aiProcess_Triangulate | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;

	auto scene = importer_.ReadFile(filename.c_str(), flags);
	if(!scene)
	{
		//error
		std::cerr << "assimp ReadFile failed for file " << filename << "\n";
		return {};
	}

	return init(*scene);
}

std::vector<MeshInfo> MeshLoader::Impl::init(const aiScene& scene)
{
	std::vector<MeshInfo> ret;
	ret.resize(scene.mNumMeshes);

	for(std::size_t i(0); i < scene.mNumMeshes; ++i)
	{
		ret[i] = init(*scene.mMeshes[i], scene);
	}

	return ret;
}

MeshInfo MeshLoader::Impl::init(const aiMesh& mesh, const aiScene& scene)
{
	MeshInfo ret;
	ret.materialIndex_ = mesh.mMaterialIndex;
	ret.vertices_.resize(mesh.mNumVertices);

	for(std::size_t i(0); i < mesh.mNumVertices; ++i)
	{
		auto& vertex = ret.vertices_[i];

		vertex.position = nytlVec(mesh.mVertices[i]);
		vertex.normal = nytlVec(mesh.mNormals[i]);

		if(mesh.HasTextureCoords(0))
		{
			vertex.textureCoords = nytlVec(mesh.mTextureCoords[0][i]);
		}
	}

	ret.indices_.resize(mesh.mNumFaces * 3);
	for(std::size_t i(0); i < mesh.mNumFaces; ++i)
	{
		auto& face = mesh.mFaces[i];

		ret.indices_[i * 3] = face.mIndices[0];
		ret.indices_[i * 3 + 1] = face.mIndices[1];
		ret.indices_[i * 3 + 2] = face.mIndices[2];
	}

	return ret;
}

}
