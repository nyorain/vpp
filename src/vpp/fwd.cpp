#include <vpp/vulkan/enums.hpp>

namespace vpp
{

namespace fwd
{

//from context.hpp
extern const auto defaultDebugFlags = vk::DebugReportBitsEXT::error | vk::DebugReportBitsEXT::warning;

//from commandBuffer.hpp
extern const auto commandBufferLevelPrimary = vk::CommandBufferLevel::primary;

//pipeline.hpp
extern const auto allShaderStages = vk::ShaderStageBits::all;

} //fwd

} //vpp
