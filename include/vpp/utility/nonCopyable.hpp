//This file is taken from nyorain/nytl nytl/nonCopyable

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
class NonMovable
{
private:
	NonMovable(const NonMovable&) = delete;
	NonMovable& operator =(const NonMovable&) = delete;
	NonMovable(NonMovable&&) = delete;
	NonMovable& operator=(NonMovable&&) = delete;

protected:
	constexpr NonMovable() noexcept = default;
	~NonMovable() noexcept = default;
};

}
