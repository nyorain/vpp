#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

namespace vpp
{

//exception
class vulkan_error : public std::runtime_error
{
public:
	vulkan_error(const std::string& msg = "") : std::runtime_error(msg) {}
};

//VK_RESULT to string
inline std::string resultErrorMsg(VkResult result)
{
	const static std::map<int, std::string> msgs =
	{
		{VK_NOT_READY, "VK_NOT_READY"},
		{VK_TIMEOUT, "VK_TIMEOUT"},
		{VK_EVENT_SET , "VK_EVENT_SET"},
		{VK_ERROR_INITIALIZATION_FAILED, "VK_ERROR_INITIALIZATION_FAILED"},
		{VK_ERROR_DEVICE_LOST, "VK_ERROR_DEVICE_LOST"},
		{VK_ERROR_MEMORY_MAP_FAILED, "VK_ERROR_MEMORY_MAP_FAILED"},
		{VK_ERROR_LAYER_NOT_PRESENT, "VK_ERROR_LAYER_NOT_PRESENT"},
		{VK_ERROR_EXTENSION_NOT_PRESENT, "VK_ERROR_EXTENSION_NOT_PRESENT"},
		{VK_ERROR_INCOMPATIBLE_DRIVER, "VK_ERROR_INCOMPATIBLE_DRIVER"}
	};

	auto it = msgs.find(result);
	if(it == msgs.cend())
		return "unkown error";

	return it->second;
}

//throw
inline bool checkResultThrow(VkResult result, const char* function, const char* call,
    const char* file, unsigned int line)
{
	if(result == VK_SUCCESS)
		return 1;

	auto msg = resultErrorMsg(result);
	const std::string err =
        "Vulkan Error Code " + std::to_string(result) + ", " + msg +
		"\n\tfunction " + function +
		"\n\tcalled " + call +
        "\n\tin file " + file +
        "\n\tline " + std::to_string(line);

    std::cerr << err << std::endl;
	throw vulkan_error(err);

	return 0;
}

//warn
inline bool checkResultWarn(VkResult result, const char* function, const char* call,
    const char* file, unsigned int line)
{
	if(result == VK_SUCCESS)
		return 1;

	auto msg = resultErrorMsg(result);
    const std::string err =
        "Vulkan Error Code " + std::to_string(result) + ", " + msg +
		"\n\tfunction " + function +
		"\n\tcalled " + call +
        "\n\tin file " + file +
        "\n\tline " + std::to_string(line);

    std::cerr << err << std::endl;

	return 0;
}

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
	::vpp::checkResultWarn(static_cast<VkResult>(x), VPP_FUNC_NAME, #x,  __FILE__, __LINE__)

#define VPP_CALL_T(x) \
	::vpp::checkResultThrow(static_cast<VkResult>(x), VPP_FUNC_NAME, #x, __FILE__, __LINE__)

#define VPP_CALL(x) VPP_CALL_T(x)
