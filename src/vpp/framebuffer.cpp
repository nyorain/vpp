#include <vpp/framebuffer.hpp>
#include <vpp/memory.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/utility/range.hpp>

namespace vpp
{

//static convinience attachment instances
FramebufferAttachment::CreateInfo FramebufferAttachment::defaultDepthAttachment
{
	vk::Format::D16UnormS8Uint,
 	vk::ImageUsageFlagBits::DepthStencilAttachment | vk::ImageUsageFlagBits::TransferSrc,
	vk::ImageAspectFlagBits::Depth | vk::ImageAspectFlagBits::Stencil
};

FramebufferAttachment::CreateInfo FramebufferAttachment::defaultColorAttachment
{
	vk::Format::B8G8R8A8Unorm,
	vk::ImageUsageFlagBits::ColorAttachment,
	vk::ImageAspectFlagBits::Color,
	{
		vk::ComponentSwizzle::R,
		vk::ComponentSwizzle::G,
		vk::ComponentSwizzle::B,
		vk::ComponentSwizzle::A
	}
};

//attachment
FramebufferAttachment::FramebufferAttachment(const Device& dev, const CreateInfo& info)
	: Resource(dev)
{
	{
		DeviceMemoryAllocator allocator(dev);
		initImage(allocator, info);
	}

	initView(info);
}

FramebufferAttachment::FramebufferAttachment(DeviceMemoryAllocator& allocator, const CreateInfo& info)
	: Resource(allocator.device())
{
	initImage(allocator, info);
	allocator.allocate();

	initView(info);
}

FramebufferAttachment::~FramebufferAttachment()
{
	destroy();
}

FramebufferAttachment::FramebufferAttachment(FramebufferAttachment&& other) noexcept
{
	this->swap(other);
}

FramebufferAttachment& FramebufferAttachment::operator=(FramebufferAttachment&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

void FramebufferAttachment::swap(FramebufferAttachment& other) noexcept
{
	using std::swap;

	swap(image_, other.image_);
	swap(imageView_, other.imageView_);
	swap(device_, other.device_);
}

void FramebufferAttachment::destroy()
{
	if(vkImageView()) vk::destroyImageView(vkDevice(), vkImageView(), nullptr);

	image_ = {};
	Resource::destroy();
}

void FramebufferAttachment::initMemoryLess(DeviceMemoryAllocator& allocator,
	const CreateInfo& info)
{
	Resource::init(allocator.device());
	initImage(allocator, info);
}

void FramebufferAttachment::initMemoryResources(const CreateInfo& info)
{
	initView(info);
}

void FramebufferAttachment::initImage(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{
	vk::ImageCreateInfo imageInfo;
	imageInfo.imageType(vk::ImageType::e2D);
	imageInfo.format(info.format);
	imageInfo.extent({info.size.width(), info.size.height(), 1});
	imageInfo.mipLevels(info.mipLevels);
	imageInfo.arrayLayers(info.arrayLayers);
	imageInfo.samples(info.samples);
	imageInfo.tiling(info.tiling);
	imageInfo.usage(info.usage);
	imageInfo.flags({});

	image_ = Image(allocator, imageInfo, info.imageMemoryFlags);
}

void FramebufferAttachment::initView(const CreateInfo& info)
{
	vk::ImageSubresourceRange subrange;
	subrange.aspectMask(info.aspects);
	subrange.baseMipLevel(0);
	subrange.levelCount(info.mipLevels);
	subrange.baseArrayLayer(0);
	subrange.layerCount(info.arrayLayers);

	vk::ImageViewCreateInfo imageViewInfo;
	imageViewInfo.viewType(vk::ImageViewType::e2D);
	imageViewInfo.format(info.format);
	imageViewInfo.image(image_.vkImage());
	imageViewInfo.flags({});
	imageViewInfo.subresourceRange(subrange);
	imageViewInfo.components(info.components);

	vk::createImageView(vkDevice(), &imageViewInfo, nullptr, &imageView_);
}


//Framebuffer
Framebuffer::Framebuffer(const Device& dev, const CreateInfo& info)
	: Resource(dev), info_(info)
{
	DeviceMemoryAllocator allocator(dev);

	initAttachments(allocator);
	initFramebuffer({});
}

Framebuffer::Framebuffer(DeviceMemoryAllocator& allocator, const CreateInfo& info)
	 : Resource(allocator.device()), info_(info)
{
	initAttachments(allocator);
	initFramebuffer({});
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
	swap(other.attachments_, attachments_);
	swap(other.info_, info_);
}

void Framebuffer::destroy()
{
	if(vkFramebuffer()) vk::destroyFramebuffer(vkDevice(), vkFramebuffer(), nullptr);

	attachments_.clear();
	framebuffer_ = {};
	info_ = {};

	Resource::destroy();
}

void Framebuffer::initMemoryLess(DeviceMemoryAllocator& allocator, const CreateInfo& info)
{
	Resource::init(allocator.device());
	info_ = info;

	attachments_.reserve(info.attachments.size());
	for(auto& attachmentinfo : info_.attachments)
	{
		attachmentinfo.size = size();

		attachments_.emplace_back();
		attachments_.back().initMemoryLess(allocator, attachmentinfo);
	}
}

void Framebuffer::initMemoryResources(const std::map<unsigned int, vk::ImageView>& extAttachments)
{
	for(std::size_t i(0); i < attachments_.size(); ++i)
	{
		attachments_[i].initMemoryResources(info_.attachments[i]);
	}

	initFramebuffer(extAttachments);
}

void Framebuffer::initAttachments(DeviceMemoryAllocator& allocator)
{
	//memory-less init
	attachments_.reserve(info_.attachments.size());
	for(auto& attachmentinfo : info_.attachments)
	{
		attachmentinfo.size = size();

		attachments_.emplace_back();
		attachments_.back().initMemoryLess(allocator, attachmentinfo);
	}

	//allocate and init memory resources
	allocator.allocate();
	for(std::size_t i(0); i < attachments_.size(); ++i)
	{
		attachments_[i].initMemoryResources(info_.attachments[i]);
	}
}

void Framebuffer::initFramebuffer(const std::map<unsigned int, vk::ImageView>& extAttachments)
{
	//attachments
	std::vector<vk::ImageView> attachments;
	attachments.resize(attachments_.size() + extAttachments.size());

	for(auto& extView : extAttachments)
	{
		if(extView.first > attachments.size())
			throw std::logic_error("bpp::Framebuffer: invalid external Attachment id given");

		attachments[extView.first] = extView.second;
	}

	std::size_t currentID = 0;
	for(auto& buf : attachments_)
	{
		while(attachments[currentID]) ++currentID;

		attachments[currentID] = buf.vkImageView();
		++currentID;
	}

	//createinfo
	vk::FramebufferCreateInfo createInfo;
	createInfo.renderPass(info_.renderPass);
	createInfo.attachmentCount(attachments.size());
	createInfo.pAttachments(attachments.data());
	createInfo.width(size().width());
	createInfo.height(size().height());
	createInfo.layers(1);

	vk::createFramebuffer(vkDevice(), &createInfo, nullptr, &framebuffer_);
}

//static utility
std::vector<FramebufferAttachment::CreateInfo> Framebuffer::parseRenderPass(const RenderPass& rp)
{
	std::vector<FramebufferAttachment::CreateInfo> ret;
	ret.reserve(rp.attachments().size());

	for(std::size_t i(0); i < rp.attachments().size(); ++i)
	{
		FramebufferAttachment::CreateInfo fbaInfo;
		fbaInfo.format = rp.attachments()[i].format();
		fbaInfo.usage = {};
		fbaInfo.aspects = {};

		for(auto& sub : rp.subpasses())
		{
			if(sub.pDepthStencilAttachment() && sub.pDepthStencilAttachment()->attachment() == i)
			{
				fbaInfo.usage |= vk::ImageUsageFlagBits::DepthStencilAttachment;
				fbaInfo.aspects |= vk::ImageAspectFlagBits::Depth | vk::ImageAspectFlagBits::Stencil;
			}

			for(auto& ref : makeRange(sub.pInputAttachments(), sub.inputAttachmentCount()))
			{
				if(ref.attachment() == i)
				{
					fbaInfo.usage |= vk::ImageUsageFlagBits::InputAttachment;
					fbaInfo.aspects |= vk::ImageAspectFlagBits::Depth;
				}
			}

			for(auto& ref : makeRange(sub.pColorAttachments(), sub.colorAttachmentCount()))
			{
				if(ref.attachment() == i)
				{
					fbaInfo.usage |= vk::ImageUsageFlagBits::ColorAttachment;
					fbaInfo.aspects |= vk::ImageAspectFlagBits::Color;
				}
			}

		}

		ret.push_back(fbaInfo);
	}

	return ret;
}


}
