#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

#include "strings.hpp"
#include "enums.hpp"

namespace vk
{

//exception
class VulkanError : public std::runtime_error
{
public:
	VulkanError(vk::Result err, const std::string& msg = "") : std::runtime_error(msg), error(err) {}
	const vk::Result error;
};

namespace call
{

//VK_RESULT to string
inline std::string resultErrorMsg(vk::Result result)
{
	return enumString(result);
}

//throw
inline vk::Result checkResultThrow(vk::Result result, const char* function)
{
	if(result == vk::Result::Success)
		return result;

	auto msg = resultErrorMsg(result);
	auto ecode = static_cast<unsigned int>(result);
	const std::string err =
        "Vulkan Error Code " + std::to_string(ecode) + ", " + msg + "\nin function " + function;

    std::cerr << err << std::endl;
	throw VulkanError(result, err);
}

//warn
inline vk::Result checkResultWarn(vk::Result result, const char* function)
{
	if(result != vk::Result::Success)
	{
		auto msg = resultErrorMsg(result);
		auto ecode = static_cast<unsigned int>(result);
		const std::string err =
			"Vulkan Error Code " + std::to_string(ecode) + ", " + msg + "\nin function " + function;

		std::cerr << err << std::endl;
	}

	return result;
}

} //namespace call

} //namespace vpp

//macro for current context validation
#if defined(__GNUC__) || defined(__clang__)
 #define VPP_FUNC_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
 #define VPP_FUNC_NAME __FUNCTION__
#else
 #define VPP_FUNC_NAME __func__
#endif //FUNCNAME


#define VPP_CALL_W(x) \
	::vk::call::checkResultWarn(static_cast<vk::Result>(x), VPP_FUNC_NAME)

#define VPP_CALL_T(x) \
	::vk::call::checkResultThrow(static_cast<vk::Result>(x), VPP_FUNC_NAME)

#define VPP_CALL(x) VPP_CALL_T(x)
