#include <vpp/commandBuffer.hpp>
#include <vpp/vk.hpp>

namespace vpp
{

//CommandBuffer
CommandBuffer::CommandBuffer(vk::CommandBuffer buffer, const CommandPool& pool)
	: ResourceReferenceHandle(buffer), commandPool_(&pool)
{
}

CommandBuffer::~CommandBuffer()
{
	if(vkHandle())
	{
		//TODO. cannot always be done
		//vk::freeCommandBuffers(vkDevice(), commandPool().vkCommandPool(), {commandBuffer_});
	}
}

void CommandBuffer::swap(CommandBuffer& lhs) noexcept
{
	using std::swap;
	swap(resourceBase(), lhs.resourceBase());
	swap(commandPool_, lhs.commandPool_);
}

//CommandPool
CommandPool::CommandPool(const Device& dev, std::uint32_t qfam, vk::CommandPoolCreateFlags flags)
	: ResourceHandle(dev)
{
	vk::CommandPoolCreateInfo info;
	info.flags = flags;
	info.queueFamilyIndex = qfam;

	handle_ = vk::createCommandPool(device(), info);
}
CommandPool::~CommandPool()
{
	if(vkHandle()) vk::destroyCommandPool(vkDevice(), vkHandle(), nullptr);
}

void CommandPool::swap(CommandPool& lhs) noexcept
{
	using std::swap;
	swap(resourceBase(), lhs.resourceBase());
	swap(flags_, lhs.flags_);
	swap(qFamily_, lhs.qFamily_);
}

std::vector<CommandBuffer> CommandPool::allocate(std::size_t count, vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkHandle();
	info.level = lvl;
	info.commandBufferCount = count;

	std::vector<vk::CommandBuffer> buffers(count);
	vk::allocateCommandBuffers(device(), info, *buffers.data());

	std::vector<CommandBuffer> ret;
	ret.reserve(count);

	for(auto& buf : buffers)
		ret.emplace_back(buf, *this);

	return ret;
}

CommandBuffer CommandPool::allocate(vk::CommandBufferLevel lvl)
{
	vk::CommandBufferAllocateInfo info;
	info.commandPool = vkHandle();
	info.level = lvl;
	info.commandBufferCount = 1;

	vk::CommandBuffer buffer;
	vk::allocateCommandBuffers(vkDevice(), info, buffer);

	return {buffer, *this};
}

void CommandPool::reset(vk::CommandPoolResetFlags flags) const
{
	vk::resetCommandPool(device(), vkHandle(), flags);
}

//CommandProvider
CommandProvider::CommandProvider(const Device& dev) : Resource(dev)
{
	tlsID_ = dev.threadLocalStorage().add();
}

CommandBuffer CommandProvider::get(std::uint32_t family,
	 vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl)
{
	auto ptr = device().threadLocalStorage().get(tlsID_);
	if(!ptr->get())
		ptr->reset(new ValueStorage<std::vector<CommandPool>>());

	auto& pools = static_cast<ValueStorage<std::vector<CommandPool>>*>(ptr->get())->value;
	for(auto& pool : pools) {
		if(pool.queueFamily() == family && pool.flags() == flags) {
			return pool.allocate(lvl);
		}
	}

	 pools.emplace_back(device(), family, flags);
	 return pools.back().allocate(lvl);
}


std::vector<CommandBuffer> CommandProvider::get(std::uint32_t family, unsigned int count,
	vk::CommandPoolCreateFlags flags, vk::CommandBufferLevel lvl)
{
	auto ptr = device().threadLocalStorage().get(tlsID_);
	if(!ptr->get())
		ptr->reset(new ValueStorage<std::vector<CommandPool>>());

	auto& pools = static_cast<ValueStorage<std::vector<CommandPool>>*>(ptr->get())->value;
	for(auto& pool : pools) {
		if(pool.queueFamily() == family && pool.flags() == flags) {
			return pool.allocate(count, lvl);
		}
	}

	 pools.emplace_back(device(), family, flags);
	 return pools.back().allocate(count, lvl);
}

}
