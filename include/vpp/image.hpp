#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>

namespace vpp
{

class Image : public Resource
{
protected:
	vk::Image image_ {};
	DeviceMemory::Entry memoryEntry_ {};

public:
	Image() = default;
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	Image(DeviceMemoryAllocator& allctr, const vk::ImageCreateInfo& info,
		vk::MemoryPropertyFlags mflags = {});
	~Image();

	const DeviceMemory::Entry& memoryEntry() const { return memoryEntry_; }
	vk::Image vkImage() const { return image_; }
	MemoryMap memoryMap() const;
};

};
