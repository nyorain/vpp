#include <vpp/buffer.hpp>
#include <vpp/provider.hpp>
#include <vpp/transfer.hpp>
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

	//total size of the update (just data without offsets)
	auto totalSize = 0u;
	for(auto& entry : data) totalSize += entry.size;

	//mappable?
	//map the memory and directly copy it into it
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

		if(!map.coherent()) map.flushRanges();
		return std::make_unique<FinishedWork<void>>();
	}

	//use the vkCmdBufferUpdate command for small data with a matching size
	//XXX: bad conditions, big memory can be split?
	//XXX: optimize: copy parts without offset in between at once
	//XXX: decide on offsets and general complexity compared to a transferBuffer
	else if((memoryEntry().allocation().size <= 65536) && (totalSize % 4 == 0))
	{
		//use the update buffer command
		//TODO: correct queue
		auto cmdBuffer = device().commandBufferProvider().get(0);
		auto vkcmdb = cmdBuffer.vkCommandBuffer();

		//the update data will be stored (memcpyd) here since it must be available when
		//then command is called on device (AFTER recording)
		std::vector<std::uint32_t> cpydata(totalSize / 4);

		vk::CommandBufferBeginInfo info;
		vk::beginCommandBuffer(vkcmdb, &info);

		auto bufferOffset = 0u; //current buffer offset to copy the data to
		auto cpyOffset = 0u; //current offset in the copy buffer
		for(auto& entry : data)
		{
			bufferOffset += entry.offset;

			auto size = entry.size;
			auto data8 = reinterpret_cast<std::uint8_t*>(cpydata.data()) + cpyOffset;
			std::memcpy(data8, entry.data, size);

			auto data = reinterpret_cast<const std::uint32_t*>(data8);
			vk::cmdUpdateBuffer(vkcmdb, vkBuffer(), bufferOffset, size, data);

			bufferOffset += size;
			cpyOffset += size;
		}

		vk::endCommandBuffer(vkcmdb);

		//return a custom work implementation based on commandWork
		struct WorkImpl : public CommandWork<void>
		{
			std::vector<std::uint32_t> data_; //make sure the data remains valid until finish

			WorkImpl(CommandBuffer&& cmdBuffer, std::vector<std::uint32_t>&& data)
				: CommandWork(std::move(cmdBuffer)), data_(std::move(data)) {}

			virtual void finish() override { data_ = {}; }
		};

		return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(cpydata));
	}
	else
	{
		//update the buffer manually via transferHeap
		//TODO: implement
		auto cmdBuffer = device().commandBufferProvider().get(0);
		auto vkcmdb = cmdBuffer.vkCommandBuffer();

		auto uploadBuffer = device().transferManager().buffer(memoryEntry().allocation().size);
		uploadBuffer.buffer().fill(data); //will map and copy it

		///TODO: correct buffer copy region
		vk::BufferCopy region(0, 0, memoryEntry().allocation().size);
		vk::cmdCopyBuffer(vkcmdb, uploadBuffer.vkBuffer(), vkBuffer(), 1, &region);

		//return a custom work implementation
		struct WorkImpl : public CommandWork<void>
		{
			TransferRange uploadRange_; //reserve the bufferRange until the command is finished

			WorkImpl(CommandBuffer&& buffer, TransferRange&& range)
				: CommandWork(std::move(buffer)), uploadRange_(std::move(range)) {}

			virtual void finish() override { uploadRange_ = {}; }
		};

		return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(uploadBuffer));
	}
}

std::unique_ptr<Work<std::uint8_t&>> Buffer::retrieve() const
{

}

void Buffer::assureMemory() const
{
	memoryEntry_.allocate();
}

}
