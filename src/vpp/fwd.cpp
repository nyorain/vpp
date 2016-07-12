#include <vpp/vulkan/enums.hpp>
#include <vpp/config.hpp>

namespace vpp
{

namespace fwd
{

//from context.hpp
#ifdef VPP_DEBUG
 extern const vk::DebugReportFlagsEXT defaultDebugFlags = vk::DebugReportBitsEXT::error |
 	vk::DebugReportBitsEXT::warning;
#else
 extern const vk::DebugReportFlagsEXT defaultDebugFlags = {};
#endif

//from commandBuffer.hpp
extern const auto commandBufferLevelPrimary = vk::CommandBufferLevel::primary;

//pipeline.hpp
extern const auto allShaderStages = vk::ShaderStageBits::all;

} //fwd

} //vpp
