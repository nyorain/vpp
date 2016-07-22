#include <vpp/buffer.hpp>
#include <vpp/vk.hpp>
#include <vpp/provider.hpp>
#include <vpp/transfer.hpp>
#include <vpp/transferWork.hpp>
#include <vpp/utility/debug.hpp>

#include <utility>
#include <cstring>

namespace vpp
{

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, vk::MemoryPropertyFlags mflags)
{
	vkHandle() = vk::createBuffer(dev, info);
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits = dev.memoryTypeBits(mflags, reqs.memoryTypeBits);
	dev.deviceAllocator().request(vkHandle(), reqs, info.usage, memoryEntry_);
}

Buffer::Buffer(const Device& dev, const vk::BufferCreateInfo& info, std::uint32_t memoryTypeBits)
{
	vkHandle() = vk::createBuffer(dev, info);
	auto reqs = vk::getBufferMemoryRequirements(dev, vkHandle());

	reqs.memoryTypeBits &= memoryTypeBits;
	dev.deviceAllocator().request(vkHandle(), reqs, info.usage, memoryEntry_);
}

Buffer::~Buffer()
{
	if(vkHandle()) vk::destroyBuffer(device(), vkHandle());
}

DataWorkPtr retrieve(const Buffer& buf, vk::DeviceSize offset, vk::DeviceSize size)
{
	VPP_DEBUG_CHECK(vpp::retrive(buffer),
	{
		if(!buf.memoryEntry().allocated())
		{
			VPP_DEBUG_OUTPUT("Buffer has no memory. Undefined will be data retrived. "
				"Calling assureMemory() and returning the undefined data");
			buf.assureMemory();
		}
	});

	if(size == vk::wholeSize) size = buf.memoryEntry().size() - offset;

	//retrieve by mapping
	if(buf.mappable())
	{
		return std::make_unique<MappableDownloadWork>(buf.memoryMap());
	}
	else
	{
		//use transfer buffer
		const Queue* queue;
		auto qFam = transferQueueFamily(buf.device(), &queue);
		auto cmdBuffer = buf.device().commandProvider().get(qFam);
		auto downloadBuffer = buf.device().transferManager().buffer(size);

		vk::BufferCopy region {offset, 0, size};

		vk::beginCommandBuffer(cmdBuffer, {});
		vk::cmdCopyBuffer(cmdBuffer, buf, downloadBuffer.buffer(), {region});
		vk::endCommandBuffer(cmdBuffer);

		return std::make_unique<DownloadWork>(std::move(cmdBuffer), *queue,
			std::move(downloadBuffer));
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
	else
	{
		const Queue* queue;
		auto qFam = transferQueueFamily(device(), &queue);
		auto cmdBuffer = device().commandProvider().get(qFam);
		copies_.push_back({0, 0, 0});

		if(direct)
		{
			data_.resize(buffer.size());
			direct_ = true;
			work_ = std::make_unique<CommandWork<void>>(std::move(cmdBuffer), *queue);
		}
		else
		{
			auto uploadBuffer = device().transferManager().buffer(buffer.size());
			map_ = uploadBuffer.buffer().memoryMap();
			work_ = std::make_unique<UploadWork>(std::move(cmdBuffer), *queue,
				std::move(uploadBuffer));
		}
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
	VPP_DEBUG_CHECK(vpp::BufferUpdate::checkCopies,
	{
		if(bufferOffset_ > buffer().size()) VPP_DEBUG_OUTPUT("Buffer write overflow.");
	});

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
	if(!direct_ && !map_.coherent()) map_.flush();

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
