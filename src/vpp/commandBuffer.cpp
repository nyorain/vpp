#include <vpp/commandBuffer.hpp>
#include <vpp/vk.hpp>

namespace vpp
{

//CommandBuffer
CommandBuffer::CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool)
	: commandBuffer_(buffer), commandPool_(&pool)
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
		vk::freeCommandBuffers(vkDevice(), commandPool().vkCommandPool(), {commandBuffer_});
		commandBuffer_ = {};
	}

	commandPool_ = {};
}

//CommandPool
CommandPool::CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags)
	: Resource(dev)
{
	vk::CommandPoolCreateInfo info;
	info.flags = flags;
	info.queueFamilyIndex = qfam;

	commandPool_ = vk::createCommandPool(vkDevice(), info);
}
CommandPool::~CommandPool()
{
	destroy();
}

CommandPool::CommandPool(CommandPool&& other) noexcept
{
	this->swap(other);
}
CommandPool& CommandPool::operator=(CommandPool&& other) noexcept
{
	destroy();
	this->swap(other);
	return *this;
}

void CommandPool::swap(CommandPool& other) noexcept
{
	using std::swap;

	swap(commandPool_, other.commandPool_);
	swap(device_, other.device_);
}
void CommandPool::destroy()
{
	if(commandPool_)
	{
		vk::destroyCommandPool(vkDevice(), commandPool_, nullptr);
	}

	commandPool_ = {};
}

std::vector<CommandBuffer> CommandPool::allocate(std::size_t count, vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkCommandPool();
	info.level = lvl;
	info.commandBufferCount = count;

	std::vector<vk::CommandBuffer> buffers(count);
	vk::allocateCommandBuffers(vkDevice(), info, *buffers.data());

	std::vector<CommandBuffer> ret;
	ret.reserve(count);

	for(auto& buf : buffers)
		ret.emplace_back(buf, *this);

	return ret;
}

CommandBuffer CommandPool::allocate(vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkCommandPool();
	info.level = lvl;
	info.commandBufferCount = 1;

	vk::CommandBuffer buffer;
	vk::allocateCommandBuffers(vkDevice(), info, buffer);

	return {buffer, *this};
}

void CommandPool::reset(vk::CommandPoolResetFlags flags) const
{
	vk::resetCommandPool(vkDevice(), vkCommandPool(), flags);
}

}
