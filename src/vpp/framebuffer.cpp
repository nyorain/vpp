#include <vpp/framebuffer.hpp>

namespace vpp
{

//static convinience attachment instances
Framebuffer::AttachmentInfo Framebuffer::defaultDepthAttachment {};
Framebuffer::AttachmentInfo Framebuffer::defaultColorAttachment {};

//Framebuffer
Framebuffer::Framebuffer(const Device& dev, const CreateInfo& info)
{

}
Framebuffer::Framebuffer(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{

}
Framebuffer::~Framebuffer()
{
	destroy();
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
	this->swap(other);
}
Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

void Framebuffer::swap(Framebuffer& other) noexcept
{
	using std::swap;

	swap(other.device_, device_);
	swap(other.framebuffer_, framebuffer_);
	swap(other.size_, size_);
	swap(other.attachments_, attachments_);
}

void Framebuffer::init(const Device& dev, const CreateInfo& info)
{
	destroy();
	Resource::init(dev);

	size_ = info.size;

	{
		DeviceMemoryAllocator allocator(device());
		initAttachments(allocator, info);
	}

	initFramebuffer(info);
}

void Framebuffer::init(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{
	destroy();
	Resource::init(allocator.device());

	size_ = info.size;
	initAttachments(allocator, info);
	initFramebuffer(info);
}

void Framebuffer::destroy()
{
	if(vkFramebuffer()) vk::destroyFramebuffer(vkDevice(), vkFramebuffer(), nullptr);
	for(auto& buf : attachments_) vk::destroyImageView(vkDevice(), buf.imageView, nullptr);

	attachments_.clear();
	framebuffer_ = {};
	size_.vkHandle() = {};

	Resource::destroy();
}

void Framebuffer::initAttachments(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{
	attachments_.reserve(info.attachments.size());
	for(auto& attachmentinfo : info.attachments)
	{
		Attachment attachment;

		//image
		vk::ImageCreateInfo imageInfo;
		imageInfo.imageType(vk::ImageType::e2D);
		imageInfo.format(attachmentinfo.format);
		imageInfo.extent({size().width(), size().height(), 1});
		imageInfo.mipLevels(attachmentinfo.mipLevels);
		imageInfo.arrayLayers(attachmentinfo.arrayLayers);
		imageInfo.samples(attachmentinfo.samples);
		imageInfo.tiling(attachmentinfo.tiling);
		imageInfo.usage(attachmentinfo.usage);
		imageInfo.flags({});

		attachment.image = Image(allocator, imageInfo, attachmentinfo.imageMemoryFlags);

		//imageView
		vk::ImageSubresourceRange subrange;
		subrange.aspectMask(attachmentinfo.aspects);
		subrange.baseMipLevel(0);
		subrange.levelCount(attachmentinfo.mipLevels);
		subrange.baseArrayLayer(0);
		subrange.layerCount(attachmentinfo.arrayLayers);

		vk::ImageViewCreateInfo imageViewInfo;
		imageViewInfo.viewType(vk::ImageViewType::e2D);
		imageViewInfo.format(attachmentinfo.format);
		imageViewInfo.image(attachment.image.vkImage());
		imageViewInfo.flags({});
		imageViewInfo.subresourceRange(subrange);

		vk::createImageView(vkDevice(), &imageViewInfo, nullptr, &attachment.imageView);

		//add
		attachments_.push_back(std::move(attachment));
	}
}

void Framebuffer::initFramebuffer(const CreateInfo& info)
{
	//attachments
	std::vector<vk::ImageView> attachments;
	attachments.reserve(attachments_.size() + info.externalAttachments.size());

	for(auto& buf : attachments_) attachments.push_back(buf.imageView);
	for(auto& view : info.externalAttachments) attachments.push_back(view);

	//createinfo
	vk::FramebufferCreateInfo createInfo;
	createInfo.renderPass(info.renderPass);
	createInfo.attachmentCount(attachments.size());
	createInfo.pAttachments(attachments.data());
	createInfo.width(size().width());
	createInfo.height(size().height());
	createInfo.layers(1);

	vk::createFramebuffer(vkDevice(), &createInfo, nullptr, &framebuffer_);
}

}
