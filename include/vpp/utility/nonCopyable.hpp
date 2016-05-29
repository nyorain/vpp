#pragma once

namespace vpp
{

///Utility base for classes that cannot be copied but moved.
class NonCopyable
{
private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

public:
	NonCopyable() = default;
	~NonCopyable() = default;

	NonCopyable(NonCopyable&& other) noexcept = default;
	NonCopyable& operator=(NonCopyable&& other) noexcept = default;
};

///Utility base class that cannot copied nor moved.
class NonMoveable
{
private:
	NonMoveable(const NonMoveable&) = delete;
	NonMoveable& operator =(const NonMoveable&) = delete;
	NonMoveable(NonMoveable&&) = delete;
	NonMoveable& operator=(NonMoveable&&) = delete;

protected:
	NonMoveable() noexcept = default;
	~NonMoveable() noexcept = default;
};

}
