#include "init.hpp"
#include "bugged.hpp"
#include <vpp/buffer.hpp>

// this should not give any warnings in the debug layer
// used to make sure move/copy initialization works correctly
// and does not leak resources
TEST(copy) {
	auto& dev = *globals.device;

	vk::BufferCreateInfo bufInfo;
	bufInfo.size = 1024;
	bufInfo.usage = vk::BufferUsageBits::uniformBuffer;
	auto buffer = vpp::Buffer(dev, bufInfo);
	buffer = {dev, bufInfo};
	buffer = {dev, bufInfo};
	buffer = {dev, bufInfo};
}
