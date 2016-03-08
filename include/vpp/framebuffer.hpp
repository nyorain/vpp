#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp
{

///Vulkan Framebuffer.
class Framebuffer : public Resource
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
		vk::ImageTiling tiling = vk::ImageTiling::Optimal;
		vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1;
		vk::MemoryPropertyFlags imageMemoryFlags = {};
	};

	struct CreateInfo
	{
		vk::Extent2D size;
		vk::RenderPass renderPass;
		std::vector<AttachmentInfo> attachments;
		std::vector<vk::ImageView> externalAttachments;
	};

	struct Buffer
	{
		Image image;
		vk::ImageView imageView;
	};

	//convinience attachment infos
	static AttachmentInfo defaultDepthAttachment;
	static AttachmentInfo defaultColorAttachment;

protected:
	vk::Framebuffer framebuffer_ {};
	vk::Extent2D size_;
	std::vector<Buffer> buffers_;

protected:
	void init(const Device& dev, const CreateInfo& info);
	void init(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initBuffers(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initFramebuffer(const CreateInfo& info);

	void destroy();

public:
	Framebuffer() = default;
	Framebuffer(const Device& dev, const CreateInfo& info);
	Framebuffer(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	~Framebuffer();

	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	void swap(Framebuffer& other) noexcept;

	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }
	const vk::Extent2D& size() const { return size_; }
};

}
