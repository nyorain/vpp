// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memoryResource.hpp> // vpp::MemoryResource

namespace vpp {

// image header
/// RAII vk::Image wrapper.
/// Does not handle any additional functionality
class ImageHandle : public ResourceHandle<vk::Image> {
public:
	ImageHandle() = default;
	ImageHandle(const Device&, const vk::ImageCreateInfo&);
	ImageHandle(const Device&, vk::Image);
	~ImageHandle();

	ImageHandle(ImageHandle&& rhs) noexcept { swap(*this, rhs); }
	auto& operator=(ImageHandle rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

class Image : public ImageHandle, public MemoryResource {
public:
	Image() = default;

	/// The various types of constructors:
	/// - Transfer ownership vs create
	///   * (1,3,5,7): ImageCreateInfo parameter: create a new image
	///   * (2,4,6,8): vk::Image parameter: transfer ownerhip of existent image
	/// - Allocate mechanism. You can either use/pass
	///   * (1,2): using a DeviceMemoryAllocator, if none is given using
	///     the default threadlocal allocator of the given device.
	///     Guarantees that the memory is allocated on a memory type
	///     contained in memBits (e.g. to assure it's allocated
	///     on hostVisible memory).
	///   * (3): allocate on a specific DeviceMemory object.
	///     Will throw std::runtime_error if the DeviceMemory fails
	///     to allocate enough memory. The DeviceMemory must
	///     be allocated on a type that is supported for the
	///     created image (the vulkan spec gives some guarantess there).
	///   * (4): Will pass ownership of the memory entry which must be
	///     in allocated state and bound to the image.
	/// - Deferred? See the vpp doc for deferred initialization
	///   * (1-6) bind the image immediately to memory. For (1,2) this
	///     means to immediately allocate memory, which might result
	///     in a vkAllocateMemory call
	///   * (7,8) does not bind the image to memory, only when
	///     ensureMemory is called. Already issues a reserving request
	///     to the DeviceMemoryAllocator, might result in less
	///     memory allocations made if multiple resources are created deferred.
	/// For constructors that receive an already existent image but
	/// allocate and bind the memory for you, you have to pass the images
	/// tiling to allow the constructor to keep any granularity
	/// requirements.
	Image(const Device&, const vk::ImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	Image(const Device&, vk::Image, vk::ImageTiling,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

	Image(const Device&, const vk::ImageCreateInfo&, DeviceMemory&);
	Image(const Device&, vk::Image, MemoryEntry&&);

	/// Creates the image without any bound memory.
	/// You have to call the ensureMemory function later on to
	/// make sure memory is bound to the image.
	Image(DeferTag, const Device&, const vk::ImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	Image(DeferTag, const Device&, vk::Image, vk::ImageTiling,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

	Image(Image&& rhs) noexcept = default;
	Image& operator=(Image&& rhs) noexcept = default;

	/// To be called when the image was initialized with
	/// a deferred constructor. Will make sure the image
	/// has bound memory.
	void init() { ensureMemory(); }
};

/// RAII wrapper around a vulkan image view.
class ImageView : public ResourceHandle<vk::ImageView> {
public:
	ImageView() = default;
	ImageView(const Device&, const vk::ImageViewCreateInfo&);
	ImageView(const Device&, vk::ImageView);
	~ImageView();

	ImageView(ImageView&& rhs) noexcept { swap(*this, rhs); }
	ImageView& operator=(ImageView rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

/// Combines a vulkan image and an imageView for it.
/// Can be e.g. used for textures or framebuffer attachments.
/// See also ViewableImageCreateInfo for default initializers.
class ViewableImage : public ResourceReference<ViewableImage> {
public:
	ViewableImage() = default;

	/// Constructs new (or transfers ownership of existent) image and
	/// view. Immediately binds memory to the image.
	/// The image member can be left empty (nullHandle) in all
	/// passed ImageViewCreateInfos, it will be set automatically.
	ViewableImage(const Device&, const vk::ImageCreateInfo&,
		const vk::ImageViewCreateInfo&, unsigned int memBits = ~0u,
		vpp::DeviceMemoryAllocator* = {});
	ViewableImage(const Device& dev, const ViewableImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	ViewableImage(Image&&, const vk::ImageViewCreateInfo&);
	ViewableImage(Image&&, ImageView&&);

	/// Creates (or transfer ownership of) an image but doesn't bind
	/// memory to it or create the view. You have to call
	/// init with the ViewCreateInfo before it can be used in any way.
	ViewableImage(DeferTag, const Device&, const vk::ImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	ViewableImage(DeferTag, Image&&);

	~ViewableImage() = default;

	ViewableImage(ViewableImage&&) noexcept = default;
	ViewableImage& operator=(ViewableImage&&) noexcept = default;

	/// If the Viewable image was constructed with a deferred constructor.
	/// This will assure the image has bound memory and create the image view.
	/// Will automatically set the image of the createInfo.
	void init(vk::ImageViewCreateInfo);

	const auto& image() const { return image_; }
	const auto& imageView() const { return imageView_; }
	const vk::ImageView& vkImageView() const { return imageView_; }
	const vk::Image& vkImage() const { return image_; }

	const Image& resourceRef() const { return image_; }

protected:
	Image image_;
	ImageView imageView_;
};

/// RAII wrapper for a vulkan sampler.
class Sampler : public ResourceHandle<vk::Sampler> {
public:
	Sampler() = default;
	Sampler(const Device&, const vk::SamplerCreateInfo&);
	Sampler(const Device&, vk::Sampler);
	~Sampler();

	Sampler(Sampler&& rhs) noexcept { swap(*this, rhs); }
	Sampler& operator=(Sampler rhs) noexcept {
		swap(*this, rhs);
		return *this;
	}
};

} // namespace vpp
