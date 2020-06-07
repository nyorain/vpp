// Copyright (c) 2016-2020 Jan Kelling
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#include <vpp/shader.hpp>
#include <vpp/vk.hpp>
#include <vpp/util/file.hpp>

#include <string>
#include <fstream>

namespace vpp {

vk::ShaderModule loadShaderModule(vk::Device dev, std::string_view filename) {
	auto code = readFile(filename, true);
	if(code.size() % 4) {
		std::string msg =  "vpp::loadShaderModule: invalid spirv shader file '";
		msg += filename;
		msg += "', filesize isn't a multiple of 4";
		throw std::runtime_error(msg);
	}

	auto ptr = reinterpret_cast<const std::uint32_t*>(code.data());
	return loadShaderModule(dev, {ptr, ptr + (code.size() / 4)});
}

vk::ShaderModule loadShaderModule(vk::Device dev,
		nytl::Span<const std::uint32_t> code) {
	vk::ShaderModuleCreateInfo info;
	info.codeSize = code.size() * 4;
	info.pCode = code.data();
	return vk::createShaderModule(dev, info);
}

// ShaderModule
ShaderModule::ShaderModule(const Device& dev, vk::ShaderModule module) :
	ResourceHandle(dev, module) {
}
ShaderModule::ShaderModule(const Device& dev, std::string_view filename) :
	ResourceHandle(dev, loadShaderModule(dev, filename)) {
}

ShaderModule::ShaderModule(const Device& dev,
	nytl::Span<const std::uint32_t> code) :
		ResourceHandle(dev, loadShaderModule(dev, code)) {
}

ShaderModule::~ShaderModule() {
	if(vkHandle()) {
		vk::destroyShaderModule(device(), vkHandle());
	}
}

vk::ObjectType ShaderModule::vkObjectType() {
	return vk::ObjectType::shaderModule;
}

// ShaderProgram
ShaderProgram::ShaderProgram(nytl::Span<const StageInfo> infos) {
	stages_.reserve(infos.size());
	for(auto& i : infos) {
		stage(i);
	}
}

void ShaderProgram::stage(vk::ShaderModule module, vk::ShaderStageBits stage) {
	for(auto& s : stages_) {
		if(s.stage == stage) {
			s.module = module;
			return;
		}
	}

	stages_.push_back({{}, stage, module, u8"main"});
}

void ShaderProgram::stage(const vk::PipelineShaderStageCreateInfo& info) {
	for(auto& s : stages_) {
		if(s.stage == info.stage) {
			s = info;
			return;
		}
	}

	stages_.push_back(info);
}

void ShaderProgram::stage(const StageInfo& info) {
	for(auto& s : stages_) {
		if(s.stage == info.stage) {
			s.pName = info.entry;
			s.flags = info.flags;
			s.module = info.module;
			s.pSpecializationInfo = info.specialization;
			return;
		}
	}

	stages_.push_back({info.flags, info.stage, info.module, info.entry,
		info.specialization});
}

vk::PipelineShaderStageCreateInfo* ShaderProgram::stage(vk::ShaderStageBits stage) {
	for(auto& s : stages_) {
		if(s.stage == stage) {
			return &s;
		}
	}

	return nullptr;
}

const vk::PipelineShaderStageCreateInfo* ShaderProgram::stage(
		vk::ShaderStageBits stage) const {
	for(auto& s : stages_) {
		if(s.stage == stage) {
			return &s;
		}
	}

	return nullptr;
}

} // namespace vpp
