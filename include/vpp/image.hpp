#pragma once

#include <vpp/fwd.hpp>
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
	Image() = default;
	Image(const Device& dev, const vk::ImageCreateInfo& info, vk::MemoryPropertyFlags mflags = {});
	~Image();

	Image(Image&& other) noexcept;
	Image& operator=(Image&& other) noexcept;

	///Creates the Image for two-step-initiazation.
	void create(const Device& dev, const vk::ImageCreateInfo& inf, vk::MemoryPropertyFlags flgs = {});

	///Fills the image with the given data.
	///Expects that the image was either created with the host visible memory flag or
	///with the transfer dst flag.
	WorkPtr fill(const std::uint8_t& data, std::size_t size, vk::Format format,
		const vk::Extent3D& extent) const;

	///Asynchronously retrieves the image data and returns the work handle.
	DataWorkPtr retrieve() const;

	const vk::Image& vkImage() const { return image_; }

	operator vk::Image() const { return vkImage(); }
	friend void swap(Image& a, Image& b) noexcept;

protected:
	vk::Image image_ {};
};

WorkPtr changeLayout(const Device& dev, vk::Image img, vk::ImageLayout ol, vk::ImageLayout nl,
	vk::ImageAspectFlags aspect);

inline WorkPtr changeLayout(const Image& img, vk::ImageLayout ol, vk::ImageLayout nl,
	vk::ImageAspectFlags aspect) { return changeLayout(img.device(), img, ol, nl, aspect); }

///Viewable image, can be e.g. used as framebuffer attachment.
class ViewableImage : public ResourceReference<ViewableImage>
{
public:
	struct CreateInfo;

	///\{
	///Some useful default create infos.
	///Can e.g. be used for framebuffers or texures.
	///Note that some properties like e.g. the size must be set manually afterwards.
	static CreateInfo defaultDepth2D();
	static CreateInfo defaultColor2D();
	///\}

public:
	ViewableImage() = default;
	ViewableImage(const Device& dev, const CreateInfo& info);
	~ViewableImage();

	ViewableImage(ViewableImage&& other) noexcept;
	ViewableImage& operator=(ViewableImage&& other) noexcept;

	void create(const Device& dev, const vk::ImageCreateInfo& img, vk::MemoryPropertyFlags flgs = {});
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

///TODO:
///Vulkan image view.
class ImageView : public Resource
{
public:
protected:
	vk::ImageView imageView_ {};
};

};
