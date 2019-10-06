#include "init.hpp"

#include <vpp/formats.hpp>
#include <vpp/handles.hpp>
#include <vpp/image.hpp>

// we create a renderpass and a framebuffer for it (as simple
// as possible)
TEST(framebuffer) {
	auto& dev = *globals.device;
	const auto size = vk::Extent2D {420, 693};
	const auto colorFormat = vk::Format::r8g8b8a8Srgb;

	vpp::ViewableImageCreateInfo vic(colorFormat, vk::ImageAspectBits::color,
		size, vk::ImageUsageBits::colorAttachment);
	EXPECT(vpp::supported(dev, vic.img), true);
	vpp::ViewableImage color(dev.devMemAllocator(), vic);

	vic.img.usage = vk::ImageUsageBits::depthStencilAttachment;
	vic.img.format = vpp::findSupported(dev, {{
		vk::Format::d32Sfloat,
		vk::Format::d16Unorm,
		vk::Format::d24UnormS8Uint,
		vk::Format::d32SfloatS8Uint,
		vk::Format::x8D24UnormPack32
	}}, vic.img);
	vic.view.format = vic.img.format;
	vic.view.subresourceRange = {vk::ImageAspectBits::depth, 0, 1, 0, 1};
	vpp::ViewableImage depth(dev.devMemAllocator(), vic);

	vk::AttachmentDescription attachments[2] {{{},
			colorFormat, vk::SampleCountBits::e1,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::ImageLayout::undefined,
			vk::ImageLayout::colorAttachmentOptimal,
		}, {{},
			vic.img.format, vk::SampleCountBits::e1,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::ImageLayout::undefined,
			vk::ImageLayout::depthStencilAttachmentOptimal,
		}
	};

	vk::AttachmentReference colorRef {
		0, vk::ImageLayout::colorAttachmentOptimal
	};

	vk::AttachmentReference depthRef {
		1, vk::ImageLayout::depthStencilAttachmentOptimal
	};

	vk::SubpassDescription subpass {{},
		vk::PipelineBindPoint::graphics, 0, {},
		1, &colorRef, 0, &depthRef
	};

	vk::RenderPassCreateInfo rpInfo;
	vpp::RenderPass renderPass {dev, {{},
		2, attachments,
		1, &subpass,
	}};

	auto fbAttachments = {color.vkImageView(), depth.vkImageView()};
	vk::FramebufferCreateInfo info {
		{}, renderPass, 2, fbAttachments.begin(), size.width, size.height, 1
	};
	auto fb = vpp::Framebuffer(dev, info);
	auto moved = std::move(fb);
}
