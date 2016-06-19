#include <atomic>

class Referenced
{
protected:
	mutable std::atomic<unsigned int> referenceCount_ {0};

protected:
	Referenced() = default;
	virtual ~Referenced() = default;

public:
	auto ref() const { return ++referenceCount_; }
	auto unref() const
	{
		auto cpy = referenceCount_.load();

		while(true)
		{
			if(std::atomic_compare_exchange_weak(&referenceCount_, &cpy, cpy - 1)) break;
			cpy = referenceCount_.load(); //reload if failed
		}

		//cpy should NEVER be 0 but who knows...
		if(cpy <= 1) delete this;
		return cpy > 0 ? cpy - 1 : 0;
	}
	auto unrefNodelete() const { return --referenceCount_; }

	unsigned int referenceCount() const { return referenceCount_.load(); }
};

class ShaderModule : public Referenced
{
public:

protected:
	vk::ShaderModule module;
};
