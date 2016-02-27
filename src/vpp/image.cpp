#include <vpp/image.hpp>
#include <utility>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	vk::MemoryRequirements reqs;
	vk::createImage(vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(vkDevice(), image_, &reqs);

	auto type = device().memoryType(reqs.memoryTypeBits(), mflags);
	if(type == -1)
	{
		throw std::runtime_error("vpp::Image: no matching deviceMemoryType");
	}

	auto memory = std::make_shared<DeviceMemory>(dev, reqs.size(), type);

	auto alloc = memory->alloc(reqs.size(), reqs.alignment());
	memoryEntry_ = DeviceMemory::Entry(memory, alloc);

	vk::bindImageMemory(vkDevice(), image_, memory->vkDeviceMemory(), alloc.offset);
}

Image::Image(DeviceMemoryAllocator& allctr, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
	: Resource(allctr.device())
{
	vk::MemoryRequirements reqs;
	vk::createImage(vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(vkDevice(), image_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(reqs.memoryTypeBits(), mflags));
	allctr.request(image_, reqs, info.tiling(), memoryEntry_);
}

Image::Image(Image&& other)
{
	Resource::create(other.device());

	std::swap(memoryEntry_, other.memoryEntry_);
	std::swap(image_, other.image_);
}

Image& Image::operator=(Image&& other)
{
	destroy();
	Resource::create(other.device());

	std::swap(memoryEntry_, other.memoryEntry_);
	std::swap(image_, other.image_);

	return *this;
}

Image::~Image()
{
	destroy();
}

void Image::destroy()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage(), nullptr);

	memoryEntry_ = {};
	image_ = {};
}

MemoryMap Image::memoryMap() const
{
	return MemoryMap(memoryEntry_);
}

}
