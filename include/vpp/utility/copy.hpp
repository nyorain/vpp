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

}
