struct ExecutionBatch {
	Fence fence {};
	bool finished {};
};

struct ExecutionState : public nytl::NonCopyable {
	/// Returns whether the associated command buffers were submitted to the device.
	bool submitted();

	/// Returns whether the associated command buffers have finished their
	/// execution on the device.
	bool completed();

	/// Makes sure the associated command buffers are submitted to the device.
	/// Will have no effect if they were already submitted.
	void submit();

	/// Waits until execution of the associated command buffers has finished.
	/// Has no effect and returns immediatly if they already have finished.
	void wait();


	std::shared_ptr<ExecutionBatch> batch;
};

class SubmitManager {
	/// Adds the given command buffer to the pending ones.
	/// Will not be submitted immediatly to the device.
	/// The ExecutionState parameter can be given if the state of
	/// the commands should be monitored.
	void add(SubmitInfo, ExecutionState* state = {});

	/// Submits all pending command buffers for the given queue.
	/// Will lock all queues while submitting, so calling this owning any
	/// queue will result in a deadlock.
	void submit(vk::Queue queue);

protected:
	std::vector<std::pair<SubmitInfo, ExecutionState*>> pending_;
};
