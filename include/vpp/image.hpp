#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp>
#include <vpp/memoryResource.hpp>
#include <vpp/allocator.hpp>
#include <vpp/work.hpp>

#include <memory>

namespace vpp
{


///Representing a vulkan image on a device and having its own memory allocation bound to it.
///The Image class does not store further information like size, type, format or layout.
///All of this must be handled by the application to guarantee the best performance.
class Image : public MemoryResource
{
public:
	using EmptyWorkPtr = std::unique_ptr<Work<void>>;
	using DataWorkPtr = std::unique_ptr<Work<std::uint8_t&>>;

public:
	Image() = default;
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	~Image();

	Image(Image&& other) noexcept;
	Image& operator=(Image&& other) noexcept;

	///Creates the Image for two-step-initiazation.
	void create(const Device& dev, const vk::ImageCreateInfo& inf, vk::MemoryPropertyFlags flgs = {});

	//TODO: some functionality for layouts (storing the current layout reasonable?)
	///Records a image layout change command into the given command buffer.
	void changeLayoutCommand(vk::CommandBuffer cmdBuffer, vk::ImageLayout oldlayout, vk::ImageLayout
		newlayout) const;

	///Queues up work on the device to change the image layout and returns the work handle.
	EmptyWorkPtr changeLayout(vk::ImageLayout oldlayout, vk::ImageLayout newlayout) const;

	///Fills the image with the given data.
	///Expects that the image was either created with the host visible memory flag or
	///with the transfer dst flag.
	EmptyWorkPtr fill(const std::uint8_t& data, std::size_t size, vk::Format format,
		const vk::Extent3D& extent) const;

	///Asynchronously retrieves the image data and returns the work handle.
	DataWorkPtr retrieve() const;

	const vk::Image& vkImage() const { return image_; }
	friend void swap(Image& a, Image& b) noexcept;

protected:
	vk::Image image_ {};
};

///Viewable image, can be e.g. used as framebuffer attachment.
class ViewableImage : public ResourceReference<ViewableImage>
{
public:
	struct CreateInfo
	{
		vk::ImageCreateInfo imageInfo {};
		vk::ImageViewCreateInfo viewInfo {};
		vk::MemoryPropertyFlags memoryFlags = {};
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

	void create(const Device& dev, const vk::ImageCreateInfo& img, vk::MemoryPropertyFlags flgs = {});
	void create(const Device& dev, const CreateInfo& info);
	void init(const vk::ImageViewCreateInfo& info);

	const Image& image() const { return image_; }
	vk::ImageView vkImageView() const { return imageView_; }
	vk::Image vkImage() const { return image_.vkImage(); }

	const Image& resourceRef() const { return image_; }
	friend void swap(ViewableImage& a, ViewableImage& b) noexcept;

protected:
	Image image_;
	vk::ImageView imageView_ {};
};

};
