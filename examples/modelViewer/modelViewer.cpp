#include <example.hpp>

#include <nytl/vec.hpp>
#include <nytl/stringParam.hpp>
#include <nytl/convert.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>


//specialization for aiVec -> nytlVec
namespace nytl
{

template<> struct Converter<nytl::Vec3f, aiVector3D>
	{ static nytl::Vec3f call(const aiVector3D& vec) { return {vec.x, vec.y, vec.z}; } };

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

std::vector<Mesh> loadMeshes(const vpp::Device& dev, const nytl::StringParam& filename)
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
		ret.push_back(std::move(mmesh));
	}

	return ret;
}

int main(int argc, char** argv)
{
	std::string filename = "model.obj";
	initApp(app, [&](){ return std::make_unique<>(); });
	mainLoop(app, [&](){
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(app.window, &pos);

		auto vec = nytl::Vec2ui(pos.x, pos.y);
		vec = nytl::clamp(vec, nytl::Vec2ui(0, 0), nytl::Vec2ui(app.width, app.height));

		system.update(vec);
	});
}
