#include <example.hpp>

#include <nytl/vec.hpp>
#include <nytl/stringParam.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>


//utility
//to be patched into nytl
template<typename T, typename O>
struct Converter;

template<typename T, typename O>
auto convert(const O& other) -> decltype(Converter<T, O>::call(other))
{
	return Converter<T, O>::call(other);
}

template<typename O>
class AutoCastable
{
public:
	template<typename T>
	operator T() const { return convert<T>(*data_); }

public:
	const O* data_;
};

//int a = convert(3.f);
//int b = convert(&a); //not allowed
template<typename O>
AutoCastable<O> convert(const O& other) { return {&other}; }

//specialization for aiVec -> nytlVec
template<> struct Converter<nytl::Vec3f, aiVector3D>
	{ static nytl::Vec3f call(const aiVector3D& vec) { return {vec.x, vec.y, vec.z}; } };

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

class Mesh
{
public:

protected:
	vpp::Buffer buffer_;
};

std::vector<Mesh> loadMeshes(const nytl::StringParam& filename)
{
	std::vector<Mesh> ret;
	std::vector<MeshInfo> meshinfos;

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

	meshinfos.reserve(scene->mNumMeshes);
	ret.reserve(scene->mNumMeshes);

	for(auto& mesh : nytl::makeRange(*scene->mMeshes, scene->mNumMeshes))
	{
		meshinfos.emplace_back();
		auto& meshinfo = meshinfos.back();

		meshinfo.materialIndex = mesh.mMaterialIndex;
		meshinfo.vertices.resize(mesh.mNumVertices);

		for(std::size_t i(0); i < mesh.mNumVertices; ++i)
		{
			auto& vertex = meshinfo.vertices[i];

			vertex.position = convert(mesh.mVertices[i]);
			vertex.normal = convert(mesh.mNormals[i]);

			if(mesh.HasTextureCoords(0))
			{
				vertex.texCoords = convert(mesh.mTextureCoords[0][i]);
			}
		}

		meshinfo.indices.resize(mesh.mNumFaces * 3);
		for(std::size_t i(0); i < mesh.mNumFaces; ++i)
		{
			auto& face = mesh.mFaces[i];

			meshinfo.indices[i * 3] = face.mIndices[0];
			meshinfo.indices[i * 3 + 1] = face.mIndices[1];
			meshinfo.indices[i * 3 + 2] = face.mIndices[2];
		}

		//buffer data
		std::vector<std::uint8_t> bufferData;
		std::size_t offset = 0;

		//create the buffer
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.size = offset;
		bufferInfo.usage = vk::BufferUsageBits::vertexBuffer | vk::BufferUsageBits::indexBuffer;

		vpp::Buffer buffer(allocator, bufferInfo, vk::MemoryPropertyBits::deviceLocal);
		vpp::fill140(buffer, vpp::raw(bufferData));

		//mesh
		Mesh mesh{std::move(vertices), std::move(indices), meshinfo.indices_.size()};
		ret.push_back(std::move(mesh));
	}


	for(auto& meshinfo : meshinfos)
	{

		for(auto& vert : meshinfo.vertices)
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
	}

	return ret;
}
