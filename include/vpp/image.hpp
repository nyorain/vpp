// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/handles.hpp> // vpp::ImageHandle, vpp::ImageView
#include <vpp/memoryResource.hpp> // vpp::MemoryResource

namespace vpp {

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
	///   * (4): Will pass ownership of the allocated memory span which must
	///     be bound to the image.
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
	Image(const Device&, vk::Image, DeviceMemory&, vk::DeviceSize memOffset);

	/// Creates the image without any bound memory.
	/// You have to call the ensureMemory function later on to
	/// make sure memory is bound to the image.
	Image(InitData&, const Device&, const vk::ImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	Image(InitData&, const Device&, vk::Image, vk::ImageTiling,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

	Image(Image&& rhs) noexcept = default;
	Image& operator=(Image&& rhs) noexcept = default;

	/// When the two-step deferred constructor was used, this function
	/// will allocate the memory for this resource.
	void init(InitData& data);
};

/// Combines a vulkan image and an imageView for it.
/// Can be e.g. used for textures or framebuffer attachments.
/// See also ViewableImageCreateInfo for default initializers.
class ViewableImage {
public:
	using InitData = Image::InitData;

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
	ViewableImage(Image&&, vk::ImageViewCreateInfo);
	ViewableImage(Image&&, ImageView&&);

	/// Creates (or transfer ownership of) an image but doesn't bind
	/// memory to it or create the view. You have to call
	/// init with the ViewCreateInfo before it can be used in any way.
	ViewableImage(InitData&, const Device&, const vk::ImageCreateInfo&,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	ViewableImage(InitData&, Image&&);

	~ViewableImage() = default;

	ViewableImage(ViewableImage&&) noexcept = default;
	ViewableImage& operator=(ViewableImage&&) noexcept = default;

	/// If the Viewable image was constructed with a deferred constructor,
	/// this will finish the initialization process.
	/// Will assure that the image has bound memory and create the image view.
	/// Will automatically set the image of the CreateImageViewInfo.
	void init(InitData&, vk::ImageViewCreateInfo);

	const auto& image() const { return image_; }
	const auto& imageView() const { return imageView_; }
	const vk::ImageView& vkImageView() const { return imageView_; }
	const vk::Image& vkImage() const { return image_; }

	const Device& device() const { return image_.device(); }
	auto vkDevice() const { return device().vkDevice(); }
	auto vkInstance() const { return device().vkInstance(); }
	auto vkPhysicalDevice() const { return device().vkPhysicalDevice(); }

protected:
	Image image_;
	ImageView imageView_;
};

} // namespace vpp
