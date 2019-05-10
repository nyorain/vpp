#include "init.hpp"
#include "bugged.hpp"
#include <vpp/image.hpp>
#include <vpp/imageOps.hpp>
#include <vpp/submit.hpp>
#include <vpp/handles.hpp>
#include <vpp/commandAllocator.hpp>
#include <vpp/queue.hpp>
#include <vpp/sharedBuffer.hpp>

TEST(image) {
	auto& dev = *globals.device;
	auto size = vk::Extent3D {20u, 20u, 1u};

	vk::ImageCreateInfo imgInfo = {
		{},
		vk::ImageType::e2d,
		vk::Format::b8g8r8a8Unorm,
		size,
		1, 1,
		vk::SampleCountBits::e1,
		vk::ImageTiling::optimal,
		vk::ImageUsageBits::transferDst | vk::ImageUsageBits::transferSrc,
		vk::SharingMode::exclusive,
		0, nullptr, vk::ImageLayout::undefined
	};

	vpp::Image img1 = {dev, imgInfo};
	auto moved = std::move(img1);
	img1 = std::move(moved);

	imgInfo.usage = vk::ImageUsageBits::sampled;
	imgInfo.format = vk::Format::r8Uint;
	imgInfo.tiling = vk::ImageTiling::linear;
	imgInfo.initialLayout = vk::ImageLayout::preinitialized;
	auto bits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
	vpp::Image img2 = {dev, imgInfo, bits};

	std::vector<std::byte> data(4 * size.width * size.height, std::byte{0xffu});
	data[0] = std::byte {0x00};
	data[1] = std::byte {0x01};
	data[2] = std::byte {0x02};
	data[3] = std::byte {0x03};

	auto& qs = dev.queueSubmitter();
	auto& queue = qs.queue();
	{
		auto cb = dev.commandAllocator().get(queue.family());
		vk::beginCommandBuffer(cb, {});

		vk::ImageMemoryBarrier barrier;
		barrier.image = img1;
		barrier.oldLayout = vk::ImageLayout::undefined;
		barrier.newLayout = vk::ImageLayout::transferDstOptimal;
		barrier.dstAccessMask = vk::AccessBits::transferWrite;
		barrier.subresourceRange = {vk::ImageAspectBits::color, 0, 1, 0, 1};
		vk::cmdPipelineBarrier(cb, vk::PipelineStageBits::topOfPipe,
			vk::PipelineStageBits::transfer, {}, {}, {}, {{barrier}});

		auto stage = vpp::fillStaging(cb, img1, vk::Format::r8g8b8a8Unorm,
			vk::ImageLayout::transferDstOptimal, size, data,
			{vk::ImageAspectBits::color});

		vk::endCommandBuffer(cb);
		qs.wait(qs.add(cb));
	}

	vpp::fillMap(img2, vk::Format::r8Unorm, size,
		{data.data(), size.width * size.height},
		{vk::ImageAspectBits::color});

	{
		auto cb = dev.commandAllocator().get(queue.family());
		vk::beginCommandBuffer(cb, {});

		vk::ImageMemoryBarrier barrier;
		barrier.image = img1;
		barrier.oldLayout = vk::ImageLayout::transferDstOptimal;
		barrier.newLayout = vk::ImageLayout::transferSrcOptimal;
		barrier.srcAccessMask = vk::AccessBits::transferWrite;
		barrier.dstAccessMask = vk::AccessBits::transferRead;
		barrier.subresourceRange = {vk::ImageAspectBits::color, 0, 1, 0, 1};
		vk::cmdPipelineBarrier(cb, vk::PipelineStageBits::transfer,
			vk::PipelineStageBits::transfer, {}, {}, {}, {{barrier}});

		auto stage = vpp::retrieveStaging(cb, img1, vk::Format::r8g8b8a8Unorm,
			vk::ImageLayout::transferSrcOptimal, size,
			{vk::ImageAspectBits::color});

		vk::endCommandBuffer(cb);
		qs.wait(qs.add(cb));

		auto map = stage.memoryMap();
		auto data = map.span();

		EXPECT(data.size(), size.width * size.height * 4u);
		EXPECT((unsigned int) data[0], 0x00u);
		EXPECT((unsigned int) data[1], 0x01u);
		EXPECT((unsigned int) data[2], 0x02u);
		EXPECT((unsigned int) data[3], 0x03u);
		for(auto i = 4u; i < data.size(); ++i) {
			EXPECT((unsigned int) data[i], 0xffu);
		}
	}

	auto data2 = vpp::retrieveMap(img2, vk::Format::r8Unorm, size,
		{vk::ImageAspectBits::color});
	EXPECT(data2.size(), size.width * size.height * 1u);
	EXPECT((unsigned int) data2[0], 0x00u);
	EXPECT((unsigned int) data2[1], 0x01u);
	EXPECT((unsigned int) data2[2], 0x02u);
	EXPECT((unsigned int) data2[3], 0x03u);
	for(auto i = 4u; i < data2.size(); ++i) {
		EXPECT((unsigned int) data2[i], 0xffu);
	}
}

TEST(moving) {
	auto& dev = *globals.device;
	auto size = vk::Extent3D {20u, 20u, 1u};

	vk::ImageCreateInfo imgInfo = {
		{},
		vk::ImageType::e2d,
		vk::Format::b8g8r8a8Unorm,
		size,
		1, 1,
		vk::SampleCountBits::e1,
		vk::ImageTiling::optimal,
		vk::ImageUsageBits::transferDst | vk::ImageUsageBits::transferSrc,
		vk::SharingMode::exclusive,
		0, nullptr, vk::ImageLayout::undefined
	};

	vpp::Image img1 = {dev, imgInfo};
	img1 = {};
}
