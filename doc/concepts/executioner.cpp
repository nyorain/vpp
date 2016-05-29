//collect work in a manager, way more efficient
CommandManager executioner;

executioner.add(image.fill());
executioner.add(image.changeLayout());
executioner.add(buffer.fill());
executioner.add(image.copy());

//submits the pending work to the gpu
executioner.submit();

//makes sure all work (and direct following operations) have been executed
executioner.finish();

//execute work individually
auto work = image.data();
work->submit();
work->finish();

//alternative
work->submitFinish();

auto data = work.data();


//interfaces concept
class Work
{
public:
	enum class State
	{
		pending, //was not yet submitted
		submitted, //was submitted, gpu is (or will be) working on it
		executed, //gpu has finished execution, finish operation not done yet
		finished //work was completley finished
	};

public:
	virtual ~Work();

	virtual vk::SubmitInfo submitInfo() const;
	virtual void wasSubmitted(); //signals that it was externally submitted
	virtual void submit(); //will return (almost) immediatly usually
	virtual void finish(); //will block until the opertion has completed (it it hasnt)
	virtual State state();
};

class CommandManger
{
protected:
	std::vector<std::unique_ptr<Work>> todo_;

public:
	Work& add(std::unique_ptr<Work> work);
	void submit();
	void finish();
};
