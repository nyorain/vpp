#include <vpp/buffer.hpp>
#include <vpp/provider.hpp>
#include <utility>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	vk::MemoryRequirements reqs;
	vk::createBuffer(dev.vkDevice(), &info, nullptr, &buffer_);
	vk::getBufferMemoryRequirements(dev.vkDevice(), buffer_, &reqs);

	reqs.memoryTypeBits(dev.memoryTypeBits(mflags, reqs.memoryTypeBits()));
	dev.deviceMemoryAllocator().request(buffer_, reqs, memoryEntry_);
}

Buffer::Buffer(Buffer&& other) noexcept
{
	swap(*this, other);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	destroy();
	swap(*this, other);
	return *this;
}

Buffer::~Buffer()
{
	destroy();
}

void swap(Buffer& a, Buffer& b) noexcept
{
	using std::swap;

	swap(b.memoryEntry_, a.memoryEntry_);
	swap(b.buffer_, a.buffer_);
}

void Buffer::destroy()
{
	if(vkBuffer()) vk::destroyBuffer(vkDevice(), vkBuffer(), nullptr);

	memoryEntry_ = {};
	buffer_ = {};
}

MemoryMapView Buffer::memoryMap() const
{
	assureMemory();
	return memoryEntry().map();
}

//todo
std::unique_ptr<Work<void>> Buffer::fill(const std::vector<BufferData>& data) const
{
	//TODO: check for overflow (too much data given?)
	//when to use vkCmdUpdateBuffer (multiple times if needed) and when vkCmdCopyBuffer?
	//detailed checks for vkcmdupdatebuffer reqs? alignment, multiple of 4...

	assureMemory();

	//mappable?
	if(memoryEntry().memory()->propertyFlags() & vk::MemoryPropertyFlagBits::HostVisible)
	{
		//directly map it
		auto map = memoryEntry().map();

		std::size_t offset = 0;
		for(auto& dataEntry : data) {
			offset += dataEntry.offset;
			std::memcpy(map.ptr() + offset, dataEntry.data, dataEntry.size);
			offset += dataEntry.size;
		}

		//flushMemoryRanges?
		return std::make_unique<FinishedWork<void>>();
	}
	else if(memoryEntry().allocation().size <= 65536)
	{
		//use the update buffer command
		//TODO: correct queue
		auto cmdBuffer = device().commandBufferProvider().get(0);
		auto vkcmdb = cmdBuffer.vkCommandBuffer();

		vk::CommandBufferBeginInfo info;
		vk::beginCommandBuffer(vkcmdb, &info);

		auto offset = 0u;
		for(auto& entry : data)
		{
			auto size = entry.size;
			auto data = reinterpret_cast<const std::uint32_t*>(entry.data);
			vk::cmdUpdateBuffer(vkcmdb, vkBuffer(), offset, size, data);
			offset += size;
		}

		vk::endCommandBuffer(vkcmdb);

		struct WorkImpl : public CommandWork<void>
		{
			std::vector<std::uint8_t> data_;
			CommandBuffer cmdBuffer_;
			vk::CommandBuffer vkCommandBuffer_; //need valid pointer for vk::SubmitInfo

			WorkImpl(CommandBuffer&& cmdBuffer) : cmdBuffer_(std::move(cmdBuffer))
			{
				vkCommandBuffer_ = cmdBuffer_.vkCommandBuffer();

				vk::SubmitInfo submitInfo;
				submitInfo.commandBufferCount(1);
				submitInfo.pCommandBuffers(&vkCommandBuffer_);

				auto queueptr = cmdBuffer_.device().queue(vk::QueueFlagBits::Graphics);
				CommandWork::operator=(cmdBuffer_.device().submitManager().add(queue, submitInfo));
			}

			virtual void finish() override
			{
				cmdBuffer_ = {};
				vkCommandBuffer_ = {};
				data_ = {};
			}
		};

		return std::make_unique<WorkImpl>(std::move(cmdBuffer));
	}
	// else
	// {
	// 	//update the buffer manually via transferHeap
	// 	struct WorkImpl : public Work<void>
	// 	{
	// 	public:
	// 		vk::CommmandBuffer cmdBuffer_;
	// 		TransferManager::BufferRange uploadRange_;
	//
	// 	public:
	// 		WorkImpl(Buffer& buffer)
	// 		{
	//
	// 		}
	// 	};
	//
	// 	//TODO: implement
	// 	auto cmdBuffer = device().commandProvider().alloc();
	// 	auto uploadBuffer = device().uploadBuffer(memoryEntry().allocation().size);
	//
	// 	vk::BufferCopy region(0, 0, memoryEntry().allocation().size);
	// 	vk::cmdCopyBuffer(cmdBuffer, uploadBuffer.vkBuffer(), vkBuffer(), 1, &region);
	//
	// 	return std::make_unique<WorkImpl>();
	// }
}

void Buffer::assureMemory() const
{
	memoryEntry_.allocate();
}

}
