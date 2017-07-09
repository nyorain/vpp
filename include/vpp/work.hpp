// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/submit.hpp>
#include <vpp/commandBuffer.hpp>
#include <vpp/util/span.hpp>
#include <vpp/util/log.hpp>

#include <memory> // std::unique_ptr
#include <stdexcept> // std::exception

namespace vpp {

/// Work base class.
/// The Work classes abstract (possibly asynchronous) work do be done.
/// Using a Work object, one can monitor and influence the state of the associated work.
/// Implementations should finish the work in their destructor (but catch exceptions).
/// Therefore destruction of a WorkBase object might block if finish or wait
/// was not called during its lifetime.
/// A Work object first must be submitted (i.e. started), then one can wait upon the
/// operation to finish (i.e. wait for the executed state). After that, implementations
/// have the possibility to execute finishing operations like freeing used resources
/// or postprocess retrieved data (finished state).
class WorkBase {
public:
	enum class State {
		none, // invalid state
		pending, // was not yet submitted but queued
		submitted, // was submitted, gpu is (or will be) working on it
		executed, // finished execution, finish operation not done yet
		finished // work was completley finished
	};

public:
	virtual ~WorkBase() = default;

	virtual void submit() = 0; // will not wait for completion of the work
	virtual void wait() = 0; // waits until the work if completed
	virtual void finish() = 0; // will block until the work has completed and finish it
	virtual State state() = 0; // returns the current state of the work

	bool pending() { return state() == State::pending; }
	bool submitted() { return static_cast<unsigned int>(state()) > 1; }
	bool executed() { return static_cast<unsigned int>(state()) > 2; }
	bool finished() { return static_cast<unsigned int>(state()) > 3; }
};

/// Abstract base class representing (possibly async) work which might return data of type R.
/// Used for example to fetch data from the device (e.g. read a buffer or image).
template <typename R>
class Work : public WorkBase {
public:
	/// Will wait for the work to finish and the return the associated data.
	/// Note that if the returned data references any additional non-owned data, it is
	/// only guaranteed to stay valid as long as the Work objects exists.
	/// So if this function is called on a temporary object, one must copy the returned data
	/// if it is not owned (e.g. a vector or unique ptr) before the work object is destructed.
	/// Implementations are allowed to move the returned data here, i.e. this function
	/// might only return valid data the first time it is called.
	/// If so, the work implementation has to set the own state to invalid after
	/// the data was retrieved and reset all associated state and resources.
	virtual R data() = 0;
};

/// Work specialization for void without the possibility to return data.
/// Used e.g. for data-less device operations such as command buffer submissions or uploading
/// data (e.g. for a buffer or image) to the device.
/// This class should be preferred over using WorkBase directly
template<> class Work<void> : public WorkBase {};

// Convinience typedefs
using WorkPtr = std::unique_ptr<Work<void>>;
using DataWorkPtr = std::unique_ptr<Work<nytl::Span<const uint8_t>>>;

/// Work implementation that can be used (for void or derived from for other types) to signal that
/// there is no work to be done. This may be used e.g. in cases where functions might require
/// additional gpu work and therefore return a work pointer, but in some cases there is no extra
/// work to be done and then a FinsishedWork object (or derivate) can be used.
/// Note that this type is abstract if 'R' is not void, then the function to return data
/// from Work<R> must be implemented.
template<typename R>
class FinishedWork : public Work<R> {
public:
	virtual ~FinishedWork() = default;

	virtual void submit() override {}
	virtual void finish() override {}
	virtual void wait() override {}
	virtual WorkBase::State state() override { return WorkBase::State::finished; }
};

/// Implements the work interface for command buffers and device submissions.
/// Submitting the work means submitting it for the device.
/// Waiting for it menas waiting for an associated fence.
/// Finishing the work has by default not additional effect over wait other than freeing
/// the owned CommandBuffer, but might be used by further implementations.
template<typename R>
class CommandWork : public Work<R> {
public:
	CommandWork() = default;
	CommandWork(CommandBuffer&&, const vpp::Queue&);
	~CommandWork();

	virtual void submit() override;
	virtual void finish() override;
	virtual void wait() override;
	virtual WorkBase::State state() override;

	CommandBuffer& commandBuffer() { return cmdBuffer_; }
	const CommandBuffer& commandBuffer() const { return cmdBuffer_; }
	CommandExecutionState& executionState() { return executionState_; }
	const CommandExecutionState& executionState() const { return executionState_; }

protected:
	CommandBuffer cmdBuffer_;
	CommandExecutionState executionState_;
	WorkBase::State state_ {WorkBase::State::none};
};

/// Manages (i.e. submits and waits) for multiple work objects.
/// On desctruction this call will automatically finish all owned work objects.
/// The work objects are always finished in the same order that they were added.
/// Can be really useful when postponing multiple work batches together and not expliclity
/// finish them until end of initialization which can result in better performance.
class WorkManager {
public:
	WorkManager() = default;
	~WorkManager();

	WorkManager(WorkManager&& other) = default;
	WorkManager& operator=(WorkManager&& other) = default;

	/// Transfers ownership of the given work objects to the WorkManager.
	/// The WorkManager will add them to the internal list of work to be done.
	void add(std::unique_ptr<WorkBase>&& work);
	void add(std::vector<std::unique_ptr<WorkBase>>&& work);
	void add(WorkManager&& works);

	/// Assures that all owned work objects are submitted.
	void submit();

	/// Finished all owned work objects.
	/// This function might block.
	void finish();

protected:
	std::vector<std::unique_ptr<WorkBase>> todo_;
};

// CommandWork implementation
#include <vpp/bits/work.inl>

} // namespace vpp
