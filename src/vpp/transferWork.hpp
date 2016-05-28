#pragma once

#include <vpp/fwd.hpp>
#include <vpp/work.hpp>
#include <vpp/transfer.hpp>

namespace vpp
{

///Utility template base class for all transfer work implementations using a TransferRange.
template<typename T>
class TransferWork : public CommandWork<T>
{
public:
	TransferWork(CommandBuffer&& cmdBuf, TransferRange&& range)
		: CommandWork<T>(std::move(cmdBuf)), transferRange_(std::move(range)) {}

protected:
	TransferRange transferRange_;
};

///Download work implementation.
class DownloadWork : public TransferWork<std::uint8_t&>
{
public:
	using TransferWork::TransferWork;
	virtual std::uint8_t& data() override
	{
		finish();
		if(!bufferData_) bufferData_ = transferRange_.buffer().retrieve();
		return bufferData_->data();
	}

protected:
	std::unique_ptr<Work<std::uint8_t&>> bufferData_;
};

///Upload work implementation.
class UploadWork : public TransferWork<void>
{
public:
	using TransferWork::TransferWork;
};

}
