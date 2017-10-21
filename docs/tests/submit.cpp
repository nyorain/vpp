#include "init.hpp"
#include <vpp/submit.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/queue.hpp>
#include <vpp/sync.hpp>

TEST(submit) {
	auto& dev = *globals.device;
	auto& queue = *dev.queue(vk::QueueBits::graphics);
	vpp::QueueSubmitter submitter(queue);

	EXPECT(submitter.current(), 1u);
	EXPECT(submitter.pending(), 0u);
	EXPECT(submitter.submitted(1u), false);
	EXPECT(submitter.completed(1u), false);

	vpp::Event event = {dev};

	auto cmdBuf = dev.commandProvider().get(queue.family());
	vk::beginCommandBuffer(cmdBuf, {});
	vk::cmdSetEvent(cmdBuf, event, vk::PipelineStageBits::allCommands);
	vk::endCommandBuffer(cmdBuf);

	EXPECT(submitter.add({{}, {}, {}, 1, &cmdBuf.vkHandle(), {}, {}}), 1u);
	EXPECT(submitter.current(), 1u);
	EXPECT(submitter.pending(), 1u);
	EXPECT(submitter.submitted(1u), false);
	EXPECT(submitter.completed(1u), false);

	submitter.submit(1u);
	EXPECT(submitter.current(), 2u);
	EXPECT(submitter.pending(), 0u);
	EXPECT(submitter.submitted(1u), true);

	submitter.wait(1u);
	EXPECT(submitter.pending(), 0u);
	EXPECT(submitter.submitted(1u), true);
	EXPECT(submitter.completed(1u), true);
	EXPECT(vk::getEventStatus(dev, event), vk::Result::eventSet);

	submitter.update();
	EXPECT(submitter.current(), 2u);
	EXPECT(submitter.pending(), 0u);
	EXPECT(submitter.submitted(1u), true);
	EXPECT(submitter.completed(1u), true);
}