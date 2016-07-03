#include <vpp/buffer.hpp>
#include <vpp/vk.hpp>
#include <vpp/provider.hpp>
#include <vpp/transfer.hpp>
#include <vpp/transferWork.hpp>

#include <utility>
#include <cstring>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	buffer_ = vk::createBuffer(dev.vkDevice(), info);
	auto reqs = vk::getBufferMemoryRequirements(dev.vkDevice(), buffer_);

	reqs.memoryTypeBits = dev.memoryTypeBits(mflags, reqs.memoryTypeBits);
	dev.memoryAllocator().request(buffer_, reqs, info.usage, memoryEntry_);
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, std::uint32_t memoryTypeBits)
{
	buffer_ = vk::createBuffer(dev.vkDevice(), info);
	auto reqs = vk::getBufferMemoryRequirements(dev.vkDevice(), buffer_);

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.memoryAllocator().request(buffer_, reqs, info.usage, memoryEntry_);
}

Buffer::Buffer(Buffer&& other) noexcept
{
	swap(*this, other);
}

Buffer& Buffer::operator=(Buffer other) noexcept
{
	swap(*this, other);
	return *this;
}

Buffer::~Buffer()
{
	if(vkBuffer()) vk::destroyBuffer(vkDevice(), vkBuffer(), nullptr);
}

void swap(Buffer& a, Buffer& b) noexcept
{
	using std::swap;

	swap(a.memoryEntry_, b.memoryEntry_);
	swap(a.buffer_, b.buffer_);
}

DataWorkPtr retrieve(const Buffer& buf, vk::DeviceSize offset, vk::DeviceSize size)
{
	//needed here? or throw if not yet allocated? there cannot be data to retrieved if the
	//memory is allocated by this call...
	buf.assureMemory();
	if(size == vk::wholeSize) size = buf.memoryEntry().size() - offset;

	//retrieve by mapping
	if(buf.mappable())
	{
		//mappable
		//XXX TODO: alternative to use extra host buffer: just use the mapped range as return data
		//would have to ensure that the buffer stays alive until data is retrieved.
		//has to be done anyway...

		auto map = buf.memoryEntry().map();
		std::vector<std::uint8_t> buffer(size); //buffer to fill the data with
		std::memcpy(buffer.data(), map.ptr(), map.size());

		struct WorkImpl : public FinishedWork<std::uint8_t&>
		{
			std::vector<std::uint8_t> buffer_;

			WorkImpl(std::vector<std::uint8_t>&& buffer) : buffer_(std::move(buffer)) {}
			virtual std::uint8_t& data() override { return *buffer_.data(); }
		};

		return std::make_unique<WorkImpl>(std::move(buffer));
	}
	else
	{
		//use transfer buffer
		auto cmdBuffer = buf.device().commandProvider().get(0);
		auto downloadBuffer = buf.device().transferManager().buffer(size);

		vk::BufferCopy region {offset, 0, size};

		vk::beginCommandBuffer(cmdBuffer, {});
		vk::cmdCopyBuffer(cmdBuffer, buf, downloadBuffer.buffer(), {region});
		vk::endCommandBuffer(cmdBuffer);

		struct WorkImpl : public CommandWork<std::uint8_t&>
		{
			TransferRange downloadRange_;
			std::unique_ptr<Work<std::uint8_t&>> bufferData_;

			WorkImpl(CommandBuffer&& cmdBuf, TransferRange&& range)
				: CommandWork(std::move(cmdBuf)), downloadRange_(std::move(range)) {}

			virtual std::uint8_t& data() override
			{
				finish();
				bufferData_ = retrieve(downloadRange_.buffer());
				return bufferData_->data();
			}
		};

		return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(downloadBuffer));
	}
}

//BufferFill
BufferUpdate::BufferUpdate(const Buffer& buffer, BufferAlign align, bool direct)
	: buffer_(&buffer), align_(align)
{
	buffer.assureMemory();
	if(buffer.mappable())
	{
		map_ = buffer.memoryMap();
		work_ = std::make_unique<FinishedWork<void>>();
	}
	else if(direct)
	{
		auto cmdBuffer = device().commandProvider().get(0);
		data_.resize(buffer.size());
		work_ = std::make_unique<CommandWork<void>>(std::move(cmdBuffer));
		direct_ = true;

		copies_.push_back({0, 0, 0});
	}
	else
	{
		auto uploadBuffer = device().transferManager().buffer(buffer.size());
		auto cmdBuffer = device().commandProvider().get(0);
		map_ = uploadBuffer.buffer().memoryMap();
		work_ = std::make_unique<UploadWork>(std::move(cmdBuffer), std::move(uploadBuffer));

		copies_.push_back({0, 0, 0});
	}
}

BufferUpdate::~BufferUpdate()
{
	if(work_) apply()->finish();
}

void BufferUpdate::offset(std::size_t size, bool update)
{
	bufferOffset_ += size;
	if(update)
	{
		std::memset(&data(), 0, size);
		internalOffset_ += size;
	}
	else if(!buffer().mappable())
	{
		if(!copies_.back().size)
		{
			copies_.back().srcOffset = internalOffset_;
			copies_.back().dstOffset = bufferOffset_;
		}
		else
		{
			copies_.push_back({internalOffset_, bufferOffset_, 0});
		}
	}

	checkCopies();
}

void BufferUpdate::align(std::size_t align)
{
	if(!align) return;
	auto old = bufferOffset_;
	bufferOffset_ = std::ceil(bufferOffset_ / double(align)) * align;
	internalOffset_ += bufferOffset_ - old;

	if(!buffer().mappable() && !copies_.back().size)
	{
		copies_.back().srcOffset += bufferOffset_ - old;
		copies_.back().dstOffset += bufferOffset_ - old;
	}

	checkCopies();
}

void BufferUpdate::write(const void* ptr, std::size_t size)
{
	std::memcpy(&data(), ptr, size);
	bufferOffset_ += size;
	internalOffset_ += size;
	if(!buffer().mappable()) copies_.back().size += size;
	checkCopies();
}

void BufferUpdate::checkCopies()
{
	while(direct_ && copies_.back().size > 65536)
	{
		auto delta = copies_.back().size - 65536;
		copies_.back().size = 65536;
		copies_.push_back({internalOffset_ - delta, bufferOffset_ - delta, delta});
	}
}

std::uint8_t& BufferUpdate::data()
{
	if(!direct_ && buffer().mappable()) return *(map_.ptr() + bufferOffset_);
	else if(!direct_) return *(map_.ptr() + internalOffset_);
	else return data_[internalOffset_];
}

WorkPtr BufferUpdate::apply()
{
	if(!direct_ && !map_.coherent()) map_.flushRanges();

	auto uploadWork = dynamic_cast<UploadWork*>(work_.get()); //transfer
	auto commandWork = dynamic_cast<CommandWork<void>*>(work_.get()); //direct
	if(uploadWork)
	{
		auto& cmdBuf = uploadWork->cmdBuffer_;
		auto& transferRange = uploadWork->transferRange_;

		vk::beginCommandBuffer(cmdBuf, {});
		for(auto& update : copies_)
			vk::cmdCopyBuffer(cmdBuf, transferRange.buffer(), buffer(), {update});
		vk::endCommandBuffer(cmdBuf);
	}
	else if(commandWork)
	{
		auto& cmdBuf = commandWork->cmdBuffer_;

		vk::beginCommandBuffer(cmdBuf, {});
		for(auto& upd : copies_)
			vk::cmdUpdateBuffer(cmdBuf, buffer(), upd.dstOffset, upd.size, data_[upd.srcOffset]);
		vk::endCommandBuffer(cmdBuf);
	}

	data_ = {};
	map_ = {};
	copies_ = {};

	return std::move(work_);
}

void BufferUpdate::alignUniform()
{
	align(device().properties().limits.minUniformBufferOffsetAlignment);
}

void BufferUpdate::alignStorage()
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

void BufferUpdate::alignTexel()
{
	align(device().properties().limits.minTexelBufferOffsetAlignment);
}

}
