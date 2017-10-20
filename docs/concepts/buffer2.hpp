// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/memory.hpp>
#include <vpp/allocator.hpp>
#include <variant>

namespace vpp {

// to fwd
struct ViewableImageCreateInfo;

// general header

/// Tag that initializes resources in a deferred way.
/// Usually better for performance but you have to call an additional
/// function when you use them.
struct DeferredTag {};
constexpr DeferredTag deferred {};

// memory resource header

/// Represents a resource that is bound to memory and keeps track
/// of this memory. Does not support sparse bindings.
class MemoryResource {
public:
	MemoryResource() = default;

	MemoryResource(MemoryResource&& rhs) noexcept = default;
	MemoryResource& operator=(MemoryResource&& rhs) noexcept = default;

	/// Checks if this memory resource was initialized yet and if not it 
	/// will be initialized.
	void ensureMemory();

	/// Returns whether this resource is bound to hostVisible memory.
	bool mappable() const;

	/// Creates/Updates the memoryMap of the memory this resource is
	/// bound to and returns a view. Must be bound to hostVisible memory,
	/// i.e. mappable() must be true.
	MemoryMapView memoryMap(vk::DeviceSize offset = 0u, 
		vk::DeviceSize size = vk::wholeSize) const;

	/// Returns the size this resource takes up in memory.
	/// Note that this might differ from the real size of the resource
	/// (e.g. buffer or image data size) due to metadata.
	vk::DeviceSize memorySize() const;
	const MemoryEntry& memoryEntry() const;

protected:
	MemoryEntry memoryEntry_;
};

// buffer header


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
	///   * (3,4): a valid (non-empty) MemoryEntry: Will pass ownership
	///     of the memory entry, must be associated with the image.
	///     Only the constructor that newly creates the image will bind the 
	///     memory to the image. Both constructors will make sure
	///     that the memory entry is allocated (and not pending).
	///   * (5,6): use a custom DeviceMemory object. The memory MUST
	///     have enough free space, will throw otherwise. You might
	///     consider using the memoryEntry constructor in this case
	///     (since you probably have to check for a possible allocation range 
	///     anyways).
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

	Image(const Device&, vk::Image, const MemoryEntry&);
	Image(const Device&, const vk::ImageCreateInfo&, const MemoryEntry&);

	Image(const Device&, const vk::ImageCreateInfo&, vpp::DeviceMemory&);
	Image(const Device&, vk::Image, vpp::DeviceMemory&);

	/// Creates the image without any bound memory.
	/// You have to call the ensureMemory function later on to
	/// make sure memory is bound to the image.
	Image(DeferredTag, const Device&, const vk::ImageCreateInfo&, 
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});
	Image(DeferredTag, const Device&, vk::Image, vk::ImageTiling,
		unsigned int memBits = ~0u, vpp::DeviceMemoryAllocator* = {});

	Image(Image&& rhs) noexcept = default;
	Image& operator=(Image&& rhs) noexcept = default;

	/// To be called when the buffer was initialized with
	/// a deferred constructor. Will make sure the buffer
	/// has bound memory.
	void init() { ensureMemory(); }
};

/// RAII wrapper around a vulkan image view.
class ImageView : public ResourceHandle<vk::ImageView> {
public:
	ImageView() = default;
	ImageView(const Device& dev, const vk::ImageViewCreateInfo& info);
	ImageView(const Device& dev, vk::ImageView imageView);
	~ImageView();

	ImageView(ImageView&& lhs) noexcept { swap(*this, lhs); }
	ImageView& operator=(ImageView lhs) noexcept { 
		swap(*this, lhs); 
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
	ViewableImage(DeferredTag, Image);
	ViewableImage(DeferredTag, const Device&, const vk::ImageCreateInfo&, 
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