#include "mesh.hpp"

#include <nytl/vec.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct Vertex
{
	nytl::Vec3f position_;
	nytl::Vec3f normal_;
	nytl::Vec2f textureCoords_;
};

struct MeshInfo
{
	std::vector<Vertex> vertices_;
	std::vector<std::size_t> indices_;
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

	auto meshinfos = impl_->load(info.filename);
	ret.reserve(meshinfos.size());

	for(auto& meshinfo : meshinfos)
	{
		vpp::Buffer vertices(allocator);
		auto map = vertices.memoryMap();

		std::size_t offset = 0;
		for(auto data : info.vertexData)
		{

		}

		vpp::Buffer indices(allocator);
		map = indices.memoryMap();

		Mesh mesh(vertices, indices, meshInfo.indices_.size());
		ret.push_back(mesh);
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
	}

	return init(*scene);
}

std::vector<MeshInfo> MeshLoader::Impl::init(const aiScene& scene)
{
	std::vector<MeshInfo> ret;
	ret.resize(scene.numMeshes);

	for(std::size_t i(0); i < scene.numMeshes; ++i)
	{
		ret[i] = init(scene.mMeshes[i], scene);
	}

	return ret;
}

MeshInfo MeshLoader::Impl::init(const aiMesh& mesh, const aiScene& scene)
{
	MeshInfo ret;
	ret.materialIndex_ = mesh.mMaterialIndex;
	ret.vertices_.resize(mesh.mNumVertices);

	for(std::size_t i(0); i < mesh.numVertices; ++i)
	{
		auto& vertex = ret.vertices_[i];

		vertex.position = nytlVec(mesh.mVertices[i]);
		vertex.normals = nytlVec(mesh.mNormals[i]);

		if(mesh.HasTextureCoords(0))
		{
			vertex.textureCoords = nytlVec(mesh.mTextureCoords[0][i]);
		}
	}

	ret.indices_.resize(mesh.numFaces * 3);
	for(std::size_t i(0); i < mesh.numFaces; ++i)
	{
		auto& face = mesh.mFaces[i];

		ret.indices_[i * 3] = face.mIndices[0];
		ret.indices_[i * 3 + 1] = face.mIndices[1];
		ret.indices_[i * 3 + 2] = face.mIndices[2];
	}

	return ret;
}
