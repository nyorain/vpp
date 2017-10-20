// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/image.hpp> // vpp::Image

namespace vpp {

/// Combines a vulkan image and an imageView for it.
/// Can be e.g. used for textures or framebuffer attachments.
/// See also ViewableImageCreateInfo for default initializers.
class ViewableImage : public ResourceReference<ViewableImage> {
public:
	ViewableImage() = default;

	/// Constructs new (or transfers ownership of existent) image and
	/// view. Immediately binds memory to the image.
	ViewableImage(Image, ImageView);
	ViewableImage(Image, const vk::ImageViewCreateInfo&);
	ViewableImage(const Device&, const vk::ImageCreateInfo&, 
		const vk::ImageViewCreateInfo&, 
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	ViewableImage(const Device& dev, const ViewableImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

	/// Creates (or transfer ownership of) an image but doesn't bind
	/// memory to it or create the view. You have to call
	/// init with the ViewCreateInfo before it can be used in any way.
	ViewableImage(DeferTag, Image);
	ViewableImage(DeferTag, const Device&, const vk::ImageCreateInfo&, 
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

	~ViewableImage();

	ViewableImage(ViewableImage&& rhs) noexcept { swap(*this, rhs); }
	ViewableImage& operator=(ViewableImage rhs) noexcept { 
		swap(*this, rhs); 
		return *this; 
	}

	/// If the Viewable image was constructed with a deferred constructor.
	/// This will assure the image has bound memory and create the image view.
	void init(const vk::ImageViewCreateInfo& info);

	const Image& image() const { return image_; }
	vk::ImageView vkImageView() const { return imageView_; }
	vk::Image vkImage() const { return image(); }

	const Image& resourceRef() const { return image_; }
	friend void swap(ViewableImage& a, ViewableImage& b) noexcept;

protected:
	Image image_;
	ImageView imageView_;
};

} // namespace vpp