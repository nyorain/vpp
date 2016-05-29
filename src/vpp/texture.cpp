#include <vpp/texture.hpp>

namespace vpp
{

Texture2D::Texture2D(const Device& dev, const CreateInfo& info, const ImageData& data)
{
	{
		DeviceMemoryAllocator allocator(dev);
		initMemoryLess(allocator, data);
	}
	initMemoryResources(info);
}
Texture2D::Texture2D(DeviceMemoryAllocator& alloc, const CreateInfo& info, const ImageData& data)
{
	initMemoryLess(alloc, data);
	alloc.allocate();

	initMemoryResources(info);
}
Texture2D::Texture2D(Image&& image, const CreateInfo& info)
{

}
Texture2D::~Texture2D()
{

}

void Texture2D::initMemoryLess(DeviceMemoryAllocator& alloc, const ImageData& data)
{

}
void Texture2D::initMemoryResources(const CreateInfo& info)
{

}

}
