#include <vpp/image.hpp>

namespace vpp
{

Image::Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
	 : Resource(dev)
{
	vk::MemoryRequirements reqs;
	vk::createImage(vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(vkDevice(), image_, &reqs);

	auto type = device().memoryType(reqs.memoryTypeBits(), mflags);
	auto memory = std::make_shared<DeviceMemory>(dev, type, reqs.size());

	auto alloc = memory.alloc(reqs.allocationSize(), reqs.alignment());
	memoryEntry_ = DeviceMemory::Entry(std::move(memory), alloc);
}

Image::Image(DeviceMemoryAllocator& allctr, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags)
	: Resource(allctr.device())
{
	vk::MemoryRequirements reqs;
	vk::createImage(vkDevice(), &info, nullptr, &image_);
	vk::getImageMemoryRequirements(vkDevice(), image_, &reqs);

	reqs.memoryTypeBits(device().memoryTypeBits(reqs.memoryTypeBits(), mflags));
	allctr.request(image_, rqes, info.tiling(), memoryEntry_);
}

Image::~Image()
{
	if(vkImage()) vk::destroyImage(vkDevice(), vkImage(), nullptr);
}

MemoryMap Image::memoryMap() const
{
	return MemoryMap(memoryEntry_);
}

}
