#include "init.hpp"
#include "bugged.hpp"

#include <vpp/shader.hpp>
#include <vpp/pipeline.hpp>
#include <vpp/pipelineInfo.hpp>
#include <vpp/renderPass.hpp>

uint32_t dummy_vert_spv[] = {
	119734787, 65536, 524289, 62, 0, 131089, 1, 393227, 1, 1280527431, 1685353262,
	808793134, 0, 196622, 0, 1, 524303, 0, 4, 1852399981, 0, 13, 37, 55, 196611, 2,
	450, 589828, 1096764487, 1935622738, 1918988389, 1600484449, 1684105331,
	1868526181, 1667590754, 29556, 589828, 1096764487, 1935622738, 1768186216,
	1818191726, 1969712737, 1600481121, 1882206772, 7037793, 262149, 4, 1852399981,
	0, 393221, 11, 1348430951, 1700164197, 2019914866, 0, 393222, 11, 0,
	1348430951, 1953067887, 7237481, 458758, 11, 1, 1348430951, 1953393007,
	1702521171, 0, 458758, 11, 2, 1130327143, 1148217708, 1635021673, 6644590,
	458758, 11, 3, 1130327143, 1147956341, 1635021673, 6644590, 196613, 13, 0,
	262149, 17, 1970037078, 101, 262150, 17, 0, 7565168, 327686, 17, 1, 1869377379,
	114, 393221, 37, 1449094247, 1702130277, 1684949368, 30821, 327685, 40,
	1701080681, 1818386808, 101, 327685, 55, 1131705711, 1919904879, 0, 327685, 58,
	1701080681, 1818386808, 101, 327752, 11, 0, 11, 0, 327752, 11, 1, 11, 1,
	327752, 11, 2, 11, 3, 327752, 11, 3, 11, 4, 196679, 11, 2, 262215, 37, 11, 42,
	262215, 55, 30, 0, 131091, 2, 196641, 3, 2, 196630, 6, 32, 262167, 7, 6, 4,
	262165, 8, 32, 0, 262187, 8, 9, 1, 262172, 10, 6, 9, 393246, 11, 7, 6, 10, 10,
	262176, 12, 3, 11, 262203, 12, 13, 3, 262165, 14, 32, 1, 262187, 14, 15, 0,
	262167, 16, 6, 2, 262174, 17, 16, 7, 262187, 8, 18, 3, 262172, 19, 17, 18,
	262187, 6, 20, 3208642560, 327724, 16, 21, 20, 20, 262187, 6, 22, 1063675494,
	262187, 6, 23, 1050253722, 262187, 6, 24, 1065353216, 458796, 7, 25, 22, 23,
	23, 24, 327724, 17, 26, 21, 25, 262187, 6, 27, 0, 262187, 6, 28, 1061158912,
	327724, 16, 29, 27, 28, 458796, 7, 30, 23, 22, 23, 24, 327724, 17, 31, 29, 30,
	327724, 16, 32, 28, 20, 458796, 7, 33, 23, 23, 22, 24, 327724, 17, 34, 32, 33,
	393260, 19, 35, 26, 31, 34, 262176, 36, 1, 14, 262203, 36, 37, 1, 262176, 39,
	7, 19, 262176, 41, 7, 16, 262176, 47, 3, 7, 262187, 6, 49, 3212836864, 262176,
	50, 3, 6, 262203, 47, 55, 3, 262187, 14, 57, 1, 262176, 59, 7, 7, 327734, 2, 4,
	0, 3, 131320, 5, 262203, 39, 40, 7, 262203, 39, 58, 7, 262205, 14, 38, 37,
	196670, 40, 35, 393281, 41, 42, 40, 38, 15, 262205, 16, 43, 42, 327761, 6, 44,
	43, 0, 327761, 6, 45, 43, 1, 458832, 7, 46, 44, 45, 27, 24, 327745, 47, 48, 13,
	15, 196670, 48, 46, 393281, 50, 51, 13, 15, 9, 262205, 6, 52, 51, 327813, 6,
	53, 52, 49, 393281, 50, 54, 13, 15, 9, 196670, 54, 53, 262205, 14, 56, 37,
	196670, 58, 35, 393281, 59, 60, 58, 56, 57, 262205, 7, 61, 60, 196670, 55, 61,
	65789, 65592
};

uint32_t dummy_frag_spv[] = {
	119734787, 65536, 524289, 13, 0, 131089, 1, 393227, 1, 1280527431, 1685353262,
	808793134, 0, 196622, 0, 1, 458767, 4, 4, 1852399981, 0, 9, 11, 196624, 4, 7,
	196611, 2, 450, 589828, 1096764487, 1935622738, 1918988389, 1600484449,
	1684105331, 1868526181, 1667590754, 29556, 589828, 1096764487, 1935622738,
	1768186216, 1818191726, 1969712737, 1600481121, 1882206772, 7037793, 262149, 4,
	1852399981, 0, 393221, 9, 1182037359, 1130848626, 1919904879, 0, 262149, 11,
	1866690153, 7499628, 262215, 9, 30, 0, 262215, 11, 30, 0, 131091, 2, 196641, 3,
	2, 196630, 6, 32, 262167, 7, 6, 4, 262176, 8, 3, 7, 262203, 8, 9, 3, 262176,
	10, 1, 7, 262203, 10, 11, 1, 327734, 2, 4, 0, 3, 131320, 5, 262205, 7, 12, 11,
	196670, 9, 12, 65789, 65592
};

vpp::RenderPass createRenderPass(const vpp::Device& dev, vk::Format format) {
	vk::AttachmentDescription attachment {};

	attachment.format = format;
	attachment.samples = vk::SampleCountBits::e1;
	attachment.loadOp = vk::AttachmentLoadOp::clear;
	attachment.storeOp = vk::AttachmentStoreOp::store;
	attachment.stencilLoadOp = vk::AttachmentLoadOp::dontCare;
	attachment.stencilStoreOp = vk::AttachmentStoreOp::dontCare;
	attachment.initialLayout = vk::ImageLayout::undefined;
	attachment.finalLayout = vk::ImageLayout::presentSrcKHR;

	vk::AttachmentReference colorReference;
	colorReference.attachment = 0;
	colorReference.layout = vk::ImageLayout::colorAttachmentOptimal;

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::graphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorReference;

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &attachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	return {dev, renderPassInfo};
}

TEST(defaultCreate) {
	auto& dev = *globals.device;

	auto rp = createRenderPass(dev, vk::Format::b8g8r8a8Unorm);
	auto layout = vpp::PipelineLayout(dev, {});

	auto vert = vpp::ShaderModule(dev, dummy_vert_spv);
	auto frag = vpp::ShaderModule(dev, dummy_frag_spv);

	auto info = vpp::GraphicsPipelineInfo(rp, layout, {{
		{vert, vk::ShaderStageBits::vertex},
		{frag, vk::ShaderStageBits::fragment}
	}});

	constexpr auto cacheName = "graphicsPipelineCache.bin";
	vpp::PipelineCache cache(dev, cacheName);

	vk::Pipeline vkPipeline;
	vk::createGraphicsPipelines(dev, cache, 1, info.info(), nullptr,
		vkPipeline);

	vpp::save(cache, cacheName);
	auto raii = vpp::Pipeline(dev, vkPipeline);
}

