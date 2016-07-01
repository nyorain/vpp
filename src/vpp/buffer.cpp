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

///TODO: split up in smaller parts
WorkPtr Buffer::fill(const std::vector<BufferData>& data, bool prefdirect) const
{
	//TODO: check for overflow (too much data given?)
	//when to use vkCmdUpdateBuffer (multiple times if needed) and when vkCmdCopyBuffer?
	//detailed checks for vkcmdupdatebuffer reqs? alignment, multiple of 4...

	assureMemory();

	//mappable?
	//map the memory and directly copy it into it
	if(memoryEntry().memory()->propertyFlags() & vk::MemoryPropertyBits::hostVisible)
	{
		//directly map it
		auto map = memoryEntry().map();

		std::size_t offset = 0;
		for(auto& dataEntry : data)
		{
			offset += dataEntry.offset;
			std::memcpy(map.ptr() + offset, dataEntry.data, dataEntry.size);
			offset += dataEntry.size;
		}

		if(!map.coherent()) map.flushRanges();
		return std::make_unique<FinishedWork<void>>();
	}

	//use transfer buffer or direct update command
	else
	{
		//analyze the given data to check with method is appropriate
		bool directUpdate = true;

		//total size of the update (just data without offsets)
		//and check if size and offset % 4 applies (needs to)
		auto totalSize = 0u;

		{
			auto currentSize = 0u;
			auto dstOffset = 0u;
			for(auto& entry : data)
			{
				dstOffset += entry.offset;
				if(entry.offset != 0)
				{
					if(currentSize > 65536 || dstOffset % 4 || currentSize % 4) directUpdate = false;
					currentSize = 0;
				}
				currentSize += entry.size;
				totalSize += entry.size;
				dstOffset += entry.size;
			}

			//check for last segment
			if(currentSize > 65536 || dstOffset % 4 || currentSize % 4) directUpdate = false;
		}

		//TODO: correct queue
		auto cmdBuffer = device().commandProvider().get(0);
		auto vkcmdb = cmdBuffer.vkCommandBuffer();

		vk::CommandBufferBeginInfo info;
		vk::beginCommandBuffer(vkcmdb, info);

		//use the vkCmdBufferUpdate command for small data with a matching size and offset
		//only when requirements matched (directUpdate) and direct is preferred (prefdirect)
		if(directUpdate && prefdirect)
		{
			//use the update buffer command
			//the update data will be stored (memcpyd) here since it must be available when
			//then command is called on device (AFTER recording)
			std::vector<std::uint32_t> cpydata(totalSize / 4);

			auto bufferOffset = 0u; //current buffer offset to copy the data to
			auto cpyOffset = 0u; //current offset in the copy buffer
			for(auto& entry : data)
			{
				bufferOffset += entry.offset;

				auto size = entry.size;
				auto data8 = reinterpret_cast<std::uint8_t*>(cpydata.data()) + cpyOffset;
				std::memcpy(data8, entry.data, size);

				auto data = reinterpret_cast<const std::uint32_t*>(data8);
				vk::cmdUpdateBuffer(vkcmdb, vkBuffer(), bufferOffset, size, *data);

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

				virtual void finish() override { CommandWork::finish(); data_ = {}; }
			};

			return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(cpydata));
		}
		else
		{
			//data without gaps to fill transfer buffer
			auto datacopy = data;
			for(auto& entry : datacopy) entry.offset = 0;

			auto uploadBuffer = device().transferManager().buffer(totalSize);
			uploadBuffer.buffer().fill(datacopy); //will map and copy it

			//fill buffer copy regions
			//always just one copy region for continous data segments
			std::vector<vk::BufferCopy> regions;
			auto currSize = 0u;
			auto currDstStart = 0u;
			auto currSrcStart = 0u;
			auto dstOffset = 0u;
			auto srcOffset = 0u;
			for(auto& entry : data)
			{
				//if data is not continous fill a copy region struct if needed
				if(entry.offset != 0)
				{
					dstOffset += entry.offset;

					if(currSize > 0)
					{
						vk::BufferCopy region{currSrcStart, currDstStart, currSize};
						regions.push_back(region);
						currSize = 0;
					}

					currDstStart = dstOffset;
					currSrcStart = srcOffset;
				}

				currSize += entry.size;
				srcOffset += entry.size;
				dstOffset += entry.size;
			}

			//final region for last data segment
			if(currSize > 0)
			{
				vk::BufferCopy region{currSrcStart, currDstStart, currSize};
				regions.push_back(region);
			}

			//record cpy command for filled regions
			auto uploadBuf = uploadBuffer.vkBuffer();
			vk::cmdCopyBuffer(vkcmdb, uploadBuf, vkBuffer(), regions);

			vk::endCommandBuffer(vkcmdb);

			//return a custom work implementation
			struct WorkImpl : public CommandWork<void>
			{
				TransferRange uploadRange_; //reserve the bufferRange until the command is finished

				WorkImpl(CommandBuffer&& buffer, TransferRange&& range)
					: CommandWork(std::move(buffer)), uploadRange_(std::move(range)) { queue(); }

				virtual void finish() override { CommandWork::finish(); uploadRange_ = {}; }
			};

			return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(uploadBuffer));
		}
	}
}

DataWorkPtr Buffer::retrieve() const
{
	//needed here? or throw if not yet allocated? there cannot be data to retrieved if the
	//memory is allocated by this call...
	assureMemory();

	//retrieve by mapping
	if(memoryEntry().memory()->propertyFlags() & vk::MemoryPropertyBits::hostVisible)
	{
		//mappable
		//XXX TODO: alternative to use extra host buffer: just use the mapped range as return data
		//would have to ensure that the buffer stays alive until data is retrieved.

		auto map = memoryEntry().map();
		std::vector<std::uint8_t> buffer(memoryEntry().size()); //buffer to fill the data with
		std::memcpy(buffer.data(), map.ptr(), map.size());

		struct WorkImpl : public FinishedWork<std::uint8_t&>
		{
			std::vector<std::uint8_t> buffer_;

			WorkImpl(std::vector<std::uint8_t>&& buffer) : buffer_(std::move(buffer)) {}
			virtual std::uint8_t& data() override { return *buffer_.data(); }
		};

		return std::make_unique<WorkImpl>(std::move(buffer));
	}

	//retrive by download buffer
	else
	{
		//use transfer buffer
		auto cmdBuffer = device().commandProvider().get(0);
		auto vkcmdb = cmdBuffer.vkCommandBuffer();

		//transfer buffer
		auto downloadBuffer = device().transferManager().buffer(memoryEntry().size());

		struct WorkImpl : public CommandWork<std::uint8_t&>
		{
			TransferRange downloadRange_;
			std::unique_ptr<Work<std::uint8_t&>> bufferData_;

			WorkImpl(CommandBuffer&& cmdBuf, TransferRange&& range)
				: CommandWork(std::move(cmdBuf)), downloadRange_(std::move(range)) {}

			virtual std::uint8_t& data() override
			{
				finish();
				bufferData_ = downloadRange_.buffer().retrieve();
				return bufferData_->data();
			}
		};

		return std::make_unique<WorkImpl>(std::move(cmdBuffer), std::move(downloadBuffer));
	}
}

//BufferFill
BufferUpdate::BufferUpdate(const Buffer& buffer, Align align, bool direct)
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
}

void BufferUpdate::write(const void* ptr, std::size_t size)
{
	if(direct_ && copies_.back().size + size > 65536)
	{
		auto delta = 65536 - copies_.back().size;
		copies_.back().size = 65536;
		copies_.push_back({internalOffset_ + delta, bufferOffset_ + delta, -delta});
	}

	std::memcpy(&data(), ptr, size);
	bufferOffset_ += size;
	internalOffset_ += size;
	if(!buffer().mappable()) copies_.back().size += size;
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

//utilty
unsigned int align(ShaderType type)
{
	switch(type)
	{
		case ShaderType::scalar: return 4;
		case ShaderType::scalar_64:
		case ShaderType::mat2:
		case ShaderType::vec2: return 8;
		case ShaderType::mat3:
		case ShaderType::mat4:
		case ShaderType::vec3:
		case ShaderType::vec4:
		case ShaderType::vec2_64: return 16;
		case ShaderType::vec3_64:
		case ShaderType::vec4_64: return 32;
		default: return 0;
	}
}

unsigned int align(const Range<ShaderType> types)
{
	auto max = 0u;
	for(auto& t : types)
		if(align(t) > max) max = align(t);
	return std::max(max, 16u);
}

}
