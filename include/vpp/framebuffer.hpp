#pragma once

#include <vpp/vk.hpp>
#include <vpp/fwd.hpp>
#include <vpp/device.hpp>
#include <vpp/resource.hpp>
#include <vpp/image.hpp>

namespace vpp
{

//Framebuffer attachment holding image, imageView and format.
class FramebufferAttachment : public Resource
{
public:
	struct CreateInfo
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

	//convinience attachment info instances
	static CreateInfo defaultDepthAttachment;
	static CreateInfo defaultColorAttachment;

protected:
	Image image_;
	vk::ImageView imageView_ {};

protected:
	void init(const Device& dev, const CreateInfo& info);

	void initImage(const CreateInfo& info);
	void initImage(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initView(const CreateInfo& info);

	void destroy();

public:
	FramebufferAttachment() = default;
	FramebufferAttachment(const Device& dev, const CreateInfo& info);
	FramebufferAttachment(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	~FramebufferAttachment();

	FramebufferAttachment(FramebufferAttachment&& other) noexcept;
	FramebufferAttachment& operator=(FramebufferAttachment&& other) noexcept;

	void swap(FramebufferAttachment& other) noexcept;

	void initMemoryLess(DeviceMemoryAllocator& allocator& allocator, const CreateInfo& info);
	void initMemoryResource();

	const Image& image() const { return image_; }
	vk::ImageView vkImageView() const { return imageView_; }
};

///Vulkan Framebuffer.
class Framebuffer : public Resource
{
public:
	struct CreateInfo
	{
		vk::RenderPass renderPass;
		vk::Extent2D size;
		std::vector<FramebufferAttachment::CreateInfo> attachments;
		std::vector<std::pair<unsigned int, vk::ImageView>> externalAttachments;
	};

	struct RenderPassCreateInfo
	{
		const Renderpass& renderPass;
		vk::Extent2D size;
		std::vector<std::pair<unsigned int, vk::ImageView>> externalAttachments;
	};

protected:
	vk::Framebuffer framebuffer_ {};
	vk::Extent2D size_;
	std::vector<FramebufferAttachment> attachments_;
	CreateInfo createInfo_;

protected:
	void init(const Device& dev, const CreateInfo& info);
	void init(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initAttachments(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initFramebuffer(const CreateInfo& info);

	void destroy();

public:
	Framebuffer() = default;
	Framebuffer(const Device& dev, const CreateInfo& info);
	Framebuffer(const Device& dev, const RenderPassCreateInfo& info);
	Framebuffer(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	Framebuffer(DeviceMemoryAllocator& allocator, const RenderPassCreateInfo& info);
	~Framebuffer();

	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	void swap(Framebuffer& other) noexcept;

	void initMemoryLess(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initMemoryLess(DeviceMemoryAllocator& allocator, const RenderPassCreateInfo& info);
	void initMemoryResource();

	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }
	const vk::Extent2D& size() const { return size_; }
	const std::vector<FramebufferAttachment>& attachments() const { return attachments_; }
	const CreateInfo& createInfo() const { return createInfo_; }
};

}
