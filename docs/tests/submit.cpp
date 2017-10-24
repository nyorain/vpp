#include "init.hpp"
#include <vpp/submit.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/queue.hpp>
#include <vpp/sync.hpp>

#include <thread>
#include <chrono>

TEST(submit) {
	auto& dev = *globals.device;
	auto& queue = *dev.queue(vk::QueueBits::graphics);
	vpp::QueueSubmitter submitter(queue);

	// Basic tests, assuring the information/submit/add/wait
	// funcitonality works

	EXPECT(submitter.current(), 1u);
	EXPECT(submitter.pending(), 0u);
	EXPECT(submitter.submitted(1u), false);
	EXPECT(submitter.completed(1u), false);

	vpp::Event event = {dev};

	auto cmdBuf = dev.commandAllocator().get(queue.family());
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

TEST(wait) {
	auto& dev = *globals.device;
	auto& submitter = dev.queueSubmitter();
	auto& queue = submitter.queue();

	// we record 2 commands buffer:
	// - cmdBuf1 waits on event1 to become signaled
	// - cmdBuf2 just signals event2
	// - there is a semaphore dependency to execute cmdBuf2 after
	//   cmdBuf1 has completed
	// - after submitting, we can start everything (leading finally
	//   to the settings on event2 by setting event1 from the device)
	// - we test that QueueSubmitter submits everything the way and order
	//   we expect it to, allowing such things

	vpp::Event event1 = {dev};
	vpp::Event event2 = {dev};

	auto cmdBuf1 = dev.commandAllocator().get(queue.family());
	vk::beginCommandBuffer(cmdBuf1, {});
	vk::cmdWaitEvents(cmdBuf1, {event1}, vk::PipelineStageBits::host,
		vk::PipelineStageBits::topOfPipe, {}, {}, {});
	vk::endCommandBuffer(cmdBuf1);

	vpp::Semaphore semaphore = {dev};

	auto submitInfo = vk::SubmitInfo {
		{}, {}, {},
		1, &cmdBuf1.vkHandle(),
		1, &semaphore.vkHandle()
	};

	auto id = submitter.add(submitInfo);

	auto cmdBuf2 = dev.commandAllocator().get(queue.family());
	vk::beginCommandBuffer(cmdBuf2, {});
	vk::cmdSetEvent(cmdBuf2, event2, vk::PipelineStageBits::allCommands);
	vk::endCommandBuffer(cmdBuf2);

	const vk::PipelineStageFlags stage = vk::PipelineStageBits::topOfPipe;
	submitInfo = {
		1, &semaphore.vkHandle(), &stage,
		1, &cmdBuf2.vkHandle()	
	};

	EXPECT(submitter.add(submitInfo), id);
	EXPECT(submitter.current(), id);
	EXPECT(submitter.submitted(id), false);
	EXPECT(submitter.completed(id), false);

	EXPECT(vk::getEventStatus(dev, event1), vk::Result::eventReset);
	EXPECT(vk::getEventStatus(dev, event2), vk::Result::eventReset);

	submitter.submit(id);
	EXPECT(submitter.current(), id + 1);
	EXPECT(submitter.submitted(id), true);
	EXPECT(submitter.completed(id), false);

	EXPECT(vk::getEventStatus(dev, event1), vk::Result::eventReset);
	EXPECT(vk::getEventStatus(dev, event2), vk::Result::eventReset);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	EXPECT(vk::getEventStatus(dev, event1), vk::Result::eventReset);
	EXPECT(vk::getEventStatus(dev, event2), vk::Result::eventReset);

	vk::setEvent(dev, event1);
	EXPECT(vk::getEventStatus(dev, event1), vk::Result::eventSet);

	EXPECT(submitter.submitted(id), true);
	submitter.wait(id);
	EXPECT(submitter.completed(id), true);

	EXPECT(vk::getEventStatus(dev, event1), vk::Result::eventSet);
	EXPECT(vk::getEventStatus(dev, event2), vk::Result::eventSet);
}