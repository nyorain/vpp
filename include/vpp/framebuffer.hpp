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
		vk::ImageAspectFlags aspects;
		vk::ComponentMapping components {};
		bool mappable = 0;
		unsigned int mipLevels = 1;
		unsigned int arrayLayers = 1;
		vk::ImageTiling tiling = vk::ImageTiling::Optimal;
		vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1;
		vk::MemoryPropertyFlags imageMemoryFlags = {};
		vk::Extent2D size;
	};

	//convinience attachment info instances
	static CreateInfo defaultDepthAttachment;
	static CreateInfo defaultColorAttachment;

protected:
	Image image_;
	vk::ImageView imageView_ {};

protected:
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

	void initMemoryLess(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initMemoryResources(const CreateInfo& info);

	const Image& image() const { return image_; }
	vk::ImageView vkImageView() const { return imageView_; }
};

///Vulkan Framebuffer.
class Framebuffer : public Resource
{
public:
	struct CreateInfo
	{
		vk::RenderPass renderPass {};
		vk::Extent2D size {};
		std::vector<FramebufferAttachment::CreateInfo> attachments;
	};

public:
	static std::vector<FramebufferAttachment::CreateInfo>
		parseRenderPass(const RenderPass& renderPass);

protected:
	vk::Framebuffer framebuffer_ {};
	std::vector<FramebufferAttachment> attachments_;
	CreateInfo info_;

protected:
	void initAttachments(DeviceMemoryAllocator& allocator);
	void initFramebuffer(const std::map<unsigned int, vk::ImageView>& extAttachments);

	void destroy();

public:
	Framebuffer() = default;
	Framebuffer(const Device& dev, const CreateInfo& info);
	Framebuffer(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	~Framebuffer();

	Framebuffer(Framebuffer&& other) noexcept;
	Framebuffer& operator=(Framebuffer&& other) noexcept;

	void swap(Framebuffer& other) noexcept;

	void initMemoryLess(DeviceMemoryAllocator& allocator, const CreateInfo& info);
	void initMemoryResources(const std::map<unsigned int, vk::ImageView>& extAttachments = {});

	vk::Framebuffer vkFramebuffer() const { return framebuffer_; }
	const vk::Extent2D& size() const { return info_.size; }
	const std::vector<FramebufferAttachment>& attachments() const { return attachments_; }
	const CreateInfo& info() const { return info_; }
};

}
