#include <vpp/commandBuffer.hpp>

namespace vpp
{

//CommandBuffer
CommandBuffer::CommandBuffer(vk::CommandBuffer buffer, const CommandPool* pool)
	: commandBuffer_(buffer), commandPool_(pool)
{
}

CommandBuffer::~CommandBuffer()
{
	destroy();
}

CommandBuffer::CommandBuffer(CommandBuffer&& other) noexcept
{
	this->swap(other);
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

void CommandBuffer::swap(CommandBuffer& other) noexcept
{
	using std::swap;
	swap(commandPool_, other.commandPool_);
	swap(commandBuffer_, other.commandBuffer_);
}

void CommandBuffer::destroy()
{
	if(vkCommandBuffer())
	{
		vk::freeCommandBuffers(vkDevice(), commandPool().vkCommandPool(), 1, &commandBuffer_);
		commandBuffer_ = {};
	}

	commandPool_ = {};
}

//SetupCommandBuffer
SetupCommandBuffer::SetupCommandBuffer(CommandBuffer&& buffer) : commandBuffer_(std::move(buffer))
{
	//set command buffer into recording state
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags(vk::CommandBufferUsageFlagBits::OneTimeSubmit);
	vk::beginCommandBuffer(vkCommandBuffer(), &beginInfo);
}

SetupCommandBuffer::~SetupCommandBuffer()
{
	//end recording state
	vk::endCommandBuffer(vkCommandBuffer());

	//XXX: SUBMIT
}

//CommandPool
CommandPool::~CommandPool()
{

}

}
