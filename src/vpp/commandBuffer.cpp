#include <vpp/commandBuffer.hpp>

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
		vk::freeCommandBuffers(vkDevice(), commandPool().vkCommandPool(), 1, &commandBuffer_);
		commandBuffer_ = {};
	}

	commandPool_ = {};
}

//SetupCommandBuffer
SetupCommandBuffer::SetupCommandBuffer(CommandBuffer&& buffer) : commandBuffer_(std::move(buffer))
{
	entry_ = std::make_shared<CommandManagerEntry>();

	//set command buffer into recording state
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags(vk::CommandBufferUsageFlagBits::OneTimeSubmit);
	vk::beginCommandBuffer(vkCommandBuffer(), &beginInfo);
}

SetupCommandBuffer::~SetupCommandBuffer()
{
	//end recording state
	vk::endCommandBuffer(vkCommandBuffer());

	//submit
	vk::SubmitInfo info;
	auto queue = device().queue(commandBuffer().commandPool().queueFamily())->queue;

	device().commandManager().execute(vkCommandBuffer(), queue, info, entry_);
}

//CommandPool
CommandPool::CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags)
	: Resource(dev)
{
	vk::CommandPoolCreateInfo info;
	info.flags(flags);
	info.queueFamilyIndex(qfam);

	vk::createCommandPool(vkDevice(), &info, nullptr, &commandPool_);
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
	info.commandPool(vkCommandPool());
	info.level(lvl);
	info.commandBufferCount(count);

	std::vector<vk::CommandBuffer> buffers(count);
	vk::allocateCommandBuffers(vkDevice(), &info, buffers.data());

	std::vector<CommandBuffer> ret;
	ret.reserve(count);

	for(auto& buf : buffers)
		ret.emplace_back(buf, *this);

	return ret;
}

CommandBuffer CommandPool::allocate(vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool(vkCommandPool());
	info.level(lvl);
	info.commandBufferCount(1);

	vk::CommandBuffer buffer;
	vk::allocateCommandBuffers(vkDevice(), &info, &buffer);

	return {buffer, *this};
}

void CommandPool::reset(vk::CommandPoolResetFlags flags) const
{
	vk::resetCommandPool(vkDevice(), vkCommandPool(), flags);
}

//CommandManager
CommandExecutionState CommandManager::execute(vk::CommandBuffer buffer, vk::Queue queue,
	vk::SubmitInfo info)
{
	buffers_.emplace_back(std::make_shared<CommandManagerEntry>());
	buffers_.back()->buffer = buffer;

	//TODO: submit!
}

void CommandManager::execute(vk::CommandBuffer buffer, vk::Queue queue, vk::SubmitInfo info,
	const std::shared_ptr<CommandManagerEntry>& entry)
{
	buffers_.push_back(entry);
	buffers_.back()->buffer = buffer;
	buffers_.back()->destroy = true;

	//TODO: submit!
}

void CommandManager::wait()
{
	std::vector<vk::Fence> fences(buffers_.size());
	for(auto& buf : buffers_)
	{
		if(buf->fence) fences.push_back(buf->fence);
	}

	//TODO
	while(vk::waitForFences(vkDevice(), fences.size(),
		fences.data(), 1, -1) == vk::Result::Timeout);

	for(auto& buf : buffers_)
	{
		if(buf->destroy)
		{
			//vk::freeCommandBuffers(vkDevice(), buf, [pool?]);
		}

		buf->buffer = {};
		buf->fence = {};
	}

	buffers_.clear();
}

}
