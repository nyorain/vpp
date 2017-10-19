#include "init.hpp"
#include "bugged.hpp"
#include <vpp/image.hpp>

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

	imgInfo.usage = vk::ImageUsageBits::sampled;
	imgInfo.format = vk::Format::r8Uint;
	imgInfo.tiling = vk::ImageTiling::linear;
	auto bits = dev.memoryTypeBits(vk::MemoryPropertyBits::hostVisible);
	vpp::Image img2 = {dev, imgInfo, bits};

	std::vector<std::uint8_t> data(4 * size.width * size.height, 0xffu);
	data[0] = 0x00;
	data[1] = 0x01;
	data[2] = 0x02;
	data[3] = 0x03;

	auto layout1 = vk::ImageLayout::undefined;
	vpp::WorkManager mgr;
	mgr.add(vpp::fill(img1, *data.data(), 
		vk::Format::b8g8r8a8Unorm,
		layout1, size,
		{vk::ImageAspectBits::color, 0, 0}, {}, false));

	auto layout2 = vk::ImageLayout::undefined;
	mgr.add(vpp::fill(img2, *data.data(), 
		vk::Format::r8Unorm,
		layout2, size,
		{vk::ImageAspectBits::color, 0, 0}));

	mgr.add(vpp::changeLayout(dev, img2, 
		layout2, vk::PipelineStageBits::allCommands, 
		vk::ImageLayout::shaderReadOnlyOptimal,
		vk::PipelineStageBits::allCommands, 
		{vk::ImageAspectBits::color, 0, 1, 0, 1}));
	layout2 = vk::ImageLayout::transferSrcOptimal;
	mgr.finish();

	auto dataWork1 = vpp::retrieve(img1, layout1, 
		vk::Format::b8g8r8a8Unorm, size,
		{vk::ImageAspectBits::color, 0, 0}, {}, false);
	auto dataWork2 = vpp::retrieve(img2, layout2,
		vk::Format::r8Unorm, size,
		{vk::ImageAspectBits::color, 0, 0});

	mgr.finish();
	auto data1 = dataWork1->data();
	EXPECT(data1.size(), size.width * size.height * 4u);
	EXPECT((unsigned int) data1[0], 0x00u);
	EXPECT((unsigned int) data1[1], 0x01u);
	EXPECT((unsigned int) data1[2], 0x02u);
	EXPECT((unsigned int) data1[3], 0x03u);
	for(auto i = 4u; i < data1.size(); ++i) {
		EXPECT((unsigned int) data1[i], 0xffu);
	}

	auto data2 = dataWork2->data();
	EXPECT(data2.size(), size.width * size.height * 1u);
	EXPECT((unsigned int) data2[0], 0x00u);
	EXPECT((unsigned int) data2[1], 0x01u);
	EXPECT((unsigned int) data2[2], 0x02u);
	EXPECT((unsigned int) data2[3], 0x03u);
	for(auto i = 4u; i < data2.size(); ++i) {
		EXPECT((unsigned int) data2[i], 0xffu);
	}
}