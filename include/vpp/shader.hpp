// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vpp/fwd.hpp>
#include <vpp/resource.hpp> // vpp::ResourceHandle
#include <vpp/util/span.hpp> // nytl::Span
#include <string_view> // std::string_view
#include <vector> // std::vector

namespace vpp {

/// RAII wrapper around a vulkan ShaderModule.
/// Can be created from a file holding the spirv binary or directly from the spirv code.
class ShaderModule : public ResourceHandle<vk::ShaderModule> {
public:
	ShaderModule() = default;

	/// \throws std::runtime_error if the given file cannot be read or has
	/// an invalid size.
	ShaderModule(const Device& dev, std::string_view file);
	ShaderModule(const Device& dev, nytl::Span<const uint32_t> bytes);
	~ShaderModule();

	ShaderModule(ShaderModule&& rhs) noexcept { swap(*this, rhs); }
	ShaderModule& operator=(ShaderModule&& rhs) noexcept { swap(*this, rhs); return *this; }
};

/// ShaderProgram with multiple stages for graphic pipelines.
/// Can be used to group multiple shader modules together to full programs.
/// Does not own the references shader modules, therefore they have to remain valid
/// at least until the ShaderProgram is destructed.
/// Note that this class is not a Resource class since it only logically groups
/// shader stages togehter and not manages them in any way.
class ShaderProgram {
public:
	/// Describes a single shader stage of a program.
	/// Lighter version of vk::PipelineShaderStageCreateInfo with sane defaults.
	/// Module and stage must always be set.
	struct StageInfo {
		vk::ShaderModule module;
		vk::ShaderStageBits stage;
		const vk::SpecializationInfo* specialization = nullptr;
		const char* entry = u8"main";
		vk::PipelineShaderStageCreateFlags flags = {};
	};

public:
	ShaderProgram(nytl::Span<const StageInfo> stages = {});
	~ShaderProgram() = default;

	ShaderProgram(const ShaderProgram& rhs) = default;
	ShaderProgram& operator=(const ShaderProgram& rhs) = default;

	/// Returns the given shader stage if there is any, nullptr otherwise.
	/// If not null, the returned pointer is guaranteed to be valid until the object
	/// is destructed or a new shader stage is added.
	vk::PipelineShaderStageCreateInfo* stage(vk::ShaderStageBits stage);
	const vk::PipelineShaderStageCreateInfo* stage(vk::ShaderStageBits stage) const;

	/// Changes or adds a new shader stage.
	/// The passed shader modules must be valid until the ShaderProgram is destructed.
	/// Note that this might invalidate returned stages as well as previously used
	/// vkStageInfos data poitners.
	void stage(vk::ShaderModule, vk::ShaderStageBits stage);
	void stage(const vk::PipelineShaderStageCreateInfo&);
	void stage(const StageInfo&);

	/// Returns a vector with the stage infos that can be used for pipeline creation.
	const std::vector<vk::PipelineShaderStageCreateInfo>& vkStageInfos() const { return stages_; }

protected:
	std::vector<vk::PipelineShaderStageCreateInfo> stages_;
};

/// Can be used to create a shader module from a spirv binary file.
/// Note that the code size must be a multiple of 4 bytes.
/// Throws std::runtime_error if the file cannot be read or has an invalid size.
/// Note that the returned object is a plain handle and must be manually destroyed.
/// If possible, prefer to use the wrapper class ShaderModule.
vk::ShaderModule loadShaderModule(vk::Device dev, std::string_view filename);

/// Can be used to create a shader module form the raw spirv code.
/// Note that the returned object is a plain handle and must be manually destroyed.
/// If possible, prefer to use the wrapper class ShaderModule.
vk::ShaderModule loadShaderModule(vk::Device dev, nytl::Span<const uint32_t> code);

} // namespace vpp
