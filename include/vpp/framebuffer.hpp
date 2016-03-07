#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp
{

///Vulkan framebuffer
class Framebuffer : public Resource
{
public:
	struct CreateInfo
	{
		std::vector<vk::ImageView> attachements;
	};

protected:
	vk::Framebuffer framebuffer_ {};
	vk::Extent2D size_;

public:
	Framebuffer() = default;
	Framebuffer(const Device& dev, const CreateInfo& info);

	const vk::Extent2D& size() const { return size_; }
	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }
};

///FrameBuffer with needed images.
class OwnedFramebuffer : public Framebuffer
{
public:
	struct AttachmentInfo
	{
		vk::Format format;
		vk::ImageUsageFlags usage;
		vk::ImageAspectFlagBits aspects;
		bool mappable = 0;
		unsigned int mipLevels = 1;
		unsigned int arrayLayers = 1;
	};

	struct CreateInfo
	{
		std::vector<AttachmentInfo> attachments_;
		std::vector<vk::ImageView> extraAttachments_;
	};

	struct Buffer
	{
		Image image_;
		vk::ImageView imageView_;
	};

	//convinience attachment infos
	static AttachmentInfo defaultDepthAttachment;
	static AttachmentInfo defaultColorAttachment;

protected:
	std::vector<Buffer> buffers_;
};

}
