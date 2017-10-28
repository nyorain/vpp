#include "init.hpp"

#include <vpp/formats.hpp>
#include <vpp/framebuffer.hpp>
#include <vpp/renderPass.hpp>
#include <vpp/image.hpp>

// we create a renderpass and a framebuffer for it (as simple
// as possible)
TEST(framebuffer) {
	auto& dev = *globals.device;
	const auto size = vk::Extent3D {420, 693};

	using VICI = vpp::ViewableImageCreateInfo;
	auto colorUsage = vk::ImageUsageBits::colorAttachment;
	auto colorInfo = VICI::color(dev, size, colorUsage).value();
	auto depthInfo = VICI::depth(dev, size).value();
	vpp::ViewableImage color(dev, colorInfo);
	vpp::ViewableImage depth(dev, depthInfo);

	vk::AttachmentDescription attachments[2] {{{},
			colorInfo.img.format, vk::SampleCountBits::e1,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::AttachmentLoadOp::dontCare,
			vk::AttachmentStoreOp::dontCare,
			vk::ImageLayout::undefined,
			vk::ImageLayout::colorAttachmentOptimal,
		}, {{},
			depthInfo.img.format, vk::SampleCountBits::e1,
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