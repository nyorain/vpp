#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vulkan/span.hpp>
#include <vpp/vulkan/structs.hpp>

namespace vpp {

/// Information about an image to be created.
struct ImageUseCase {
	ImageUseCase() = default;
	ImageUseCase(const vk::ImageCreateInfo&);

	vk::ImageTiling tiling;
	vk::ImageUsageFlags usage;
	vk::SampleCountFlags samples;
	vk::Extent3D size; // max size relevant
};

/// Information about a buffer to be created.
struct BufferUseCase {
	BufferUseCase() = default;
	BufferUseCase(const vk::BufferCreateInfo&);

	vk::BufferUsageFlags flags;
	vk::DeviceSize size; // max size relevant
};

/// Returns whether the given format is valid for the given use
/// case. 
bool supported(vk::Format, const ImageUseCase&);
bool supported(vk::Format, const BufferUseCase&);

/// Selects the first format from the list that supports the given
/// use case.
vk::Format findSupported(nytl::Span<const vk::Format>, const ImageUseCase&);
vk::Format findSupported(nytl::Span<const vk::Format>, const BufferUseCase&);

/// Combines vk::ImageCreateInfo and vk::ImageViewCreatInfo and
/// offers default initializers.
struct ViewableImageCreateInfo {
	vk::ImageCreateInfo img {}; // info to create the image
	vk::ImageViewCreateInfo view {}; // info to create the view

	/// Default color image info. 
	/// Uses the defaults seen as default arguments.
	/// Allowed to pass 0 as as third size parameter, will automatically
	/// be corrected to 1.
	static ViewableImageCreateInfo color(const vk::Extent3D& size,
		vk::ImageTiling = vk::ImageTiling::optimal,
		vk::ImageUsageFlags = 
			vk::ImageUsageBits::transferDst |
			vk::ImageUsageBits::sampled,
		vk::Format = vk::Format::r8g8b8a8Unorm,
		vk::SampleCountBits = vk::SampleCountBits::e1);

	/// Default depth viewable image create info that automatically
	/// chooses a correct format.
	/// Allowed to pass 0 as as third size parameter, will automatically
	/// be corrected to 1.
	static ViewableImageCreateInfo depth(const Device& dev,
		const vk::Extent3D& size, 
		vk::ImageUsageFlags = 
			vk::ImageUsageBits::depthStencilAttachment | 
			vk::ImageUsageBits::sampled,
		nytl::Span<const vk::Format> possibleFormats = {
			vk::Format::d32Sfloat,
			vk::Format::d32SfloatS8Uint,
			vk::Format::d24UnormS8Uint,
			vk::Format::d16UnormS8Uint,
			vk::Format::d16Unorm
		}, vk::ImageTiling = vk::ImageTiling::optimal,
		vk::SampleCountBits = vk::SampleCountBits::e1); 

	// TODO: depthStencil, stencil, additional color formats (other priorities)
	// TODO: add memBits to structure? to allow hostVisible/hostCoherent default
	//  initializers?
};

} // namespace vpp