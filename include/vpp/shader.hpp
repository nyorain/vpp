#pragma once

#include <vpp/vk.hpp>

namespace vpp
{

vk::ShaderModule
loadShader(vk::Device dev, const std::string& file, vk::ShaderStageFlagBits stage);

vk::ShaderModule
loadShaderGLSL(vk::Device dev, const std::string& file, vk::ShaderStageFlagBits stage);

}
