#pragma once

#include <vpp/fwd.hpp>
#include <vpp/vk.hpp>
#include <vpp/resource.hpp>
#include <vpp/submit.hpp>

#include <memory>

namespace vpp
{

///Work base class.
///The Work classes abstract possible gpu work do be done. They are therefore not executed
///automatically, but usually batched together for performance gains.
class WorkBase
{
public:
	enum class State
	{
		none, //invalid state
		pending, //was not yet submitted but queued
		submitted, //was submitted, gpu is (or will be) working on it
		executed, //gpu has finished execution, finish operation not done yet
		finished //work was completley finished
	};

public:
	virtual ~WorkBase() = default;

	virtual void submit() = 0; //will return (almost) immediatly (i.e. not wait for completion)
	virtual State state() = 0; //returns the current state of the work
	virtual void wait() = 0; //waits until the command buffer is fullly executed
	virtual void finish() = 0; //will block until the opertion has completed (if it hasnt)
};

///Abstract base class representing (possibly async) work which might return data of type R.
template <typename R>
class Work : public WorkBase
{
public:
	virtual R data() = 0; //will implicitly wait for work finish (if needed)
};

///Work specialization for void without the possibility to retrn data.
template<>
class Work<void> : public WorkBase
{
};

///Work implementation that can be used (for void or derived from for other types) to signal that
///there is no work to be done. This may be used e.g. in cases where functions might require
///additional gpu work and therefore return a work pointer, but in some cases there is no extra
///work to be done and then a FinsishedWork object (or derivate) can be used.
template<typename R>
class FinishedWork : public Work<R>
{
public:
	virtual ~FinishedWork() = default;

	virtual void submit() override {}
	virtual void finish() override {}
	virtual void wait() override {}
	virtual WorkBase::State state() override { return WorkBase::State::finished; }
};

///Class that implements the work interface for command buffers and gpu submissions.
template<typename R>
class CommandWork : public Work<R>
{
public:
	CommandWork(CommandExecutionState&& state);

	virtual void submit() override;
	virtual void finish() override;
	virtual void wait() override;
	virtual WorkBase::State state() override;

protected:
	CommandExecutionState executionState_;
	WorkBase::State state_;
};

///Manages (i.e. submits and waits) for multiple work objects.
class WorkManager : public Resource
{
public:
	WorkManager() = default;
	WorkManager(const Device& dev, std::size_t submitThreshold = 100);

	WorkBase& add(std::unique_ptr<WorkBase> work);
	void submit();
	void finish();

protected:
	std::size_t submitThreshold_;
	std::vector<std::unique_ptr<WorkBase>> todo_;
};

//commandWork implementation
#include <vpp/bits/work.inl>

}
