#pragma once

#include <vpp/fwd.hpp>
#include <vpp/work.hpp>
#include <vpp/transfer.hpp>

namespace vpp
{

//Those classes are used internally (buffer and image fill/retrieve) but might also be useful
//for custom upload/download work implementations

///Utility template base class for all transfer work implementations using a TransferRange.
template<typename T>
class TransferWork : public CommandWork<T>
{
public:
	TransferWork(CommandBuffer&& cmdBuf, TransferRange&& range)
		: CommandWork<T>(std::move(cmdBuf)), transferRange_(std::move(range)) {}

public:
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
		downloadWork_ = retrieve(transferRange_.buffer());
		return downloadWork_->data();
	}

public:
	DataWorkPtr downloadWork_;
};

///Upload work implementation.
class UploadWork : public TransferWork<void>
{
public:
	using TransferWork::TransferWork;
};

}
