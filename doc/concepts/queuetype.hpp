Device::Queue* queue(const Device& dev, vk::QueueFlags flags);
Device::Queue* queue(const Device& dev, const QueueType& op);

struct QueueType
{
	struct Logicop
	{
		//outer node
		vk::QueueFlags flags;

		//inner node
		Logicop sub1, sub2;
		int op;
	};

	Logicop op_;
};
