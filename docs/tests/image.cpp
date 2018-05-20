#include "init.hpp"
#include "bugged.hpp"
#include <vpp/image.hpp>
#include <vpp/imageOps.hpp>

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

	// NOTE: they should not be used like this.
	// the multiple submissions should at least be grouped together...
	vpp::changeLayout(img1,
		vk::ImageLayout::undefined, vk::PipelineStageBits::topOfPipe, {},
		vk::ImageLayout::transferDstOptimal, vk::PipelineStageBits::transfer,
		vk::AccessBits::transferWrite,
		{vk::ImageAspectBits::color, 0, 1, 0, 1}, dev.queueSubmitter());

	auto work = vpp::fillStaging(img1, vk::Format::r8g8b8a8Unorm,
		vk::ImageLayout::transferDstOptimal, size, data,
		{vk::ImageAspectBits::color});
	auto workm = std::move(work);
	workm.wait();
	workm.submit();
	workm.finish();

	vpp::fillMap(img2, vk::Format::r8Unorm, size,
		{data.data(), size.width * size.height},
		{vk::ImageAspectBits::color});

	auto work1 = vpp::changeLayout(img1,
		vk::ImageLayout::transferDstOptimal, vk::PipelineStageBits::transfer,
		vk::AccessBits::transferWrite,
		vk::ImageLayout::transferSrcOptimal, vk::PipelineStageBits::transfer,
		vk::AccessBits::transferRead,
		{vk::ImageAspectBits::color, 0, 1, 0, 1}, dev.queueSubmitter());
	auto work2 = std::move(work1);
	work2.finish();

	auto dataWork1 = vpp::retrieveStaging(img1,
		vk::Format::r8g8b8a8Unorm, vk::ImageLayout::transferSrcOptimal,
		size, {vk::ImageAspectBits::color, 0, 0});
	auto dataWork2 = std::move(dataWork1);

	auto data1 = dataWork2.data();
	EXPECT(data1.size(), size.width * size.height * 4u);
	EXPECT((unsigned int) data1[0], 0x00u);
	EXPECT((unsigned int) data1[1], 0x01u);
	EXPECT((unsigned int) data1[2], 0x02u);
	EXPECT((unsigned int) data1[3], 0x03u);
	for(auto i = 4u; i < data1.size(); ++i) {
		EXPECT((unsigned int) data1[i], 0xffu);
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
