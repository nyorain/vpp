#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/allocator.hpp>

#include <memory>

namespace vpp
{

///Represinting a vulkan image on a device and having its own memory allocation bound to it.
class Image : public Resource
{
public:
	Image() = default;
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	~Image();

	Image(Image&& other) noexcept;
	Image& operator=(Image&& other) noexcept;

	///Assures that there is device memory associated with this buffer.
	///Will be implicitly called on member functions that require it.
	void assureMemory() const;

	///Returns a vulkan memory map guard. Should only be called when buffer was created on a
	///host visible device memory heap and if the device memory was allocated.
	MemoryMapView memoryMap() const;

	const DeviceMemoryAllocator::Entry& memoryEntry() const { return *memoryEntry_; }
	vk::Image vkImage() const { return image_; }

	friend void swap(Image& a, Image& b) noexcept;

protected:
	void destroy();

protected:
	vk::Image image_ {};
	std::unique_ptr<MemoryEntry> memoryEntry_ {};
};

};
