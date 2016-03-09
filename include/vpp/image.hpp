#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/memory.hpp>

#include <memory>

namespace vpp
{

class Image : public Resource
{
protected:
	vk::Image image_ {};
	std::unique_ptr<DeviceMemory::Entry> memoryEntry_ {};

protected:
	void destroy();

public:
	Image();
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	Image(DeviceMemoryAllocator& allctr, const vk::ImageCreateInfo& info,
		vk::MemoryPropertyFlags mflags = {});
	~Image();

	Image(Image&& other) noexcept;
	Image& operator=(Image&& other) noexcept;

	void swap(Image& other) noexcept;

	const DeviceMemory::Entry& memoryEntry() const { return *memoryEntry_; }
	vk::Image vkImage() const { return image_; }
	MemoryMap memoryMap() const;
};

};
