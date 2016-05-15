#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/resource.hpp>
#include <vpp/allocator.hpp>

#include <memory>

namespace vpp
{

///Represinting a vulkan image on a device and having its own memory allocation bound to it.
class Image : public ResourceReference<Image>
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

	///Fills the image with the given data.
	///Expects that the image was either created with the host visible memory flag or
	///with the transfer dst flag.
	void fill(const std::uint8_t& data, std::size_t size, vk::Format format,
		const vk::Extent3D& extent) const;

	const MemoryEntry& memoryEntry() const { return memoryEntry_; }
	const vk::Image& vkImage() const { return image_; }

	const MemoryEntry& resourceRef() const { return memoryEntry(); }
	friend void swap(Image& a, Image& b) noexcept;

protected:
	void destroy();

protected:
	vk::Image image_ {};
	MemoryEntry memoryEntry_ {};
};

///Viewable image, can be e.g. used as framebuffer attachment.
class ViewableImage : public ResourceReference<ViewableImage>
{
public:
	struct CreateInfo
	{
		vk::ImageCreateInfo imageInfo {};
		vk::ImageViewCreateInfo viewInfo {};
		vk::MemoryPropertyFlags imageMemoryFlags = {};
	};

	//convinience attachment info instances
	static CreateInfo defaultDepth2D;
	static CreateInfo defaultColor2D;

public:
	ViewableImage() = default;
	ViewableImage(const Device& dev, const CreateInfo& info);
	~ViewableImage();

	ViewableImage(ViewableImage&& other) noexcept;
	ViewableImage& operator=(ViewableImage&& other) noexcept;

	void initMemoryLess(const Device& dev, const vk::ImageCreateInfo& info,
		vk::MemoryPropertyFlags imageMemFlags = {});
	void initMemoryResources(vk::ImageViewCreateInfo info);

	const Image& image() const { return image_; }
	vk::ImageView vkImageView() const { return imageView_; }
	vk::Image vkImage() const { return image_.vkImage(); }

	void destroy();
	const Image& resourceRef() const { return image_; }
	friend void swap(ViewableImage& a, ViewableImage& b) noexcept;

protected:
	Image image_;
	vk::ImageView imageView_ {};
};

};
