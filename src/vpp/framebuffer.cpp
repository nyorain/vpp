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
	swap(other.buffers_, buffers_);
}

void Framebuffer::init(const Device& dev, const CreateInfo& info)
{
	destroy();
	Resource::init(dev);

	size_ = info.size;

	{
		DeviceMemoryAllocator allocator(device());
		initBuffers(allocator, info);
	}

	initFramebuffer(info);
}

void Framebuffer::init(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{
	destroy();
	Resource::init(allocator.device());

	size_ = info.size;
	initBuffers(allocator, info);
	initFramebuffer(info);
}

void Framebuffer::destroy()
{
	if(vkFramebuffer()) vk::destroyFramebuffer(vkDevice(), vkFramebuffer(), nullptr);
	for(auto& buf : buffers_) vk::destroyImageView(vkDevice(), buf.imageView, nullptr);

	buffers_.clear();
	framebuffer_ = {};
	size_.vkHandle() = {};

	Resource::destroy();
}

void Framebuffer::initBuffers(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{
	buffers_.reserve(info.attachments.size());
	for(auto& attachment : info.attachments)
	{
		Buffer buf;

		//image
		vk::ImageCreateInfo imageInfo;
		imageInfo.imageType(vk::ImageType::e2D);
		imageInfo.format(attachment.format);
		imageInfo.extent({size().width(), size().height(), 1});
		imageInfo.mipLevels(attachment.mipLevels);
		imageInfo.arrayLayers(attachment.arrayLayers);
		imageInfo.samples(attachment.samples);
		imageInfo.tiling(attachment.tiling);
		imageInfo.usage(attachment.usage);
		imageInfo.flags({});

		buf.image = Image(allocator, imageInfo, attachment.imageMemoryFlags);

		//imageView
		vk::ImageSubresourceRange subrange;
		subrange.aspectMask(attachment.aspects);
		subrange.baseMipLevel(0);
		subrange.levelCount(attachment.mipLevels);
		subrange.baseArrayLayer(0);
		subrange.layerCount(attachment.arrayLayers);

		vk::ImageViewCreateInfo imageViewInfo;
		imageViewInfo.viewType(vk::ImageViewType::e2D);
		imageViewInfo.format(attachment.format);
		imageViewInfo.image(buf.image.vkImage());
		imageViewInfo.flags({});
		imageViewInfo.subresourceRange(subrange);

		vk::createImageView(vkDevice(), &imageViewInfo, nullptr, &buf.imageView);

		//add
		buffers_.push_back(std::move(buf));
	}
}

void Framebuffer::initFramebuffer(const CreateInfo& info)
{
	//attachments
	std::vector<vk::ImageView> attachments;
	attachments.reserve(buffers_.size() + info.externalAttachments.size());

	for(auto& buf : buffers_) attachments.push_back(buf.imageView);
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
