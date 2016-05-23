// The MIT License
//
// Copyright © 2016 nyorain
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// The implementation for the Flag tempalte class was taken from nvidias vkcpp project:
// Changes were made to it, which are protected by the above given license.
// link (May 2016): https://github.com/nvpro-pipeline/vkcpp
// license:

// Copyright(c) 2015-2016, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//The specification (vk.xml) itself is protected by the following license:

// Copyright (c) 2015-2016 The Khronos Group Inc.
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and/or associated documentation files (the
// "Materials"), to deal in the Materials without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Materials, and to
// permit persons to whom the Materials are furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Materials.
// 
// THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
// 
// ------------------------------------------------------------------------
// 
// This file, vk.xml, is the Vulkan API Registry. It is a critically important
// and normative part of the Vulkan Specification, including a canonical
// machine-readable definition of the API, parameter and member validation
// language incorporated into the Specification and reference pages, and other
// material which is registered by Khronos, such as tags used by extension and
// layer authors. The only authoritative version of vk.xml is the one
// maintained in the master branch of the Khronos Vulkan Github project.
    
//Automaitcally generated header file for the vulkan api for the nyorain/vpp library
//Conatains all structure and enum declarations as well as command wrappers.
//Do not edit manually.

#pragma once

#include "fwd.hpp"

namespace vk
{

enum class PipelineCacheHeaderVersion : std::int32_t
{
	one = 1,
};

enum class StructureType : std::int32_t
{
	applicationInfo = 0,
	instanceCreateInfo = 1,
	deviceQueueCreateInfo = 2,
	deviceCreateInfo = 3,
	submitInfo = 4,
	memoryAllocateInfo = 5,
	mappedMemoryRange = 6,
	bindSparseInfo = 7,
	fenceCreateInfo = 8,
	semaphoreCreateInfo = 9,
	eventCreateInfo = 10,
	queryPoolCreateInfo = 11,
	bufferCreateInfo = 12,
	bufferViewCreateInfo = 13,
	imageCreateInfo = 14,
	imageViewCreateInfo = 15,
	shaderModuleCreateInfo = 16,
	pipelineCacheCreateInfo = 17,
	pipelineShaderStageCreateInfo = 18,
	pipelineVertexInputStateCreateInfo = 19,
	pipelineInputAssemblyStateCreateInfo = 20,
	pipelineTessellationStateCreateInfo = 21,
	pipelineViewportStateCreateInfo = 22,
	pipelineRasterizationStateCreateInfo = 23,
	pipelineMultisampleStateCreateInfo = 24,
	pipelineDepthStencilStateCreateInfo = 25,
	pipelineColorBlendStateCreateInfo = 26,
	pipelineDynamicStateCreateInfo = 27,
	graphicsPipelineCreateInfo = 28,
	computePipelineCreateInfo = 29,
	pipelineLayoutCreateInfo = 30,
	samplerCreateInfo = 31,
	descriptorSetLayoutCreateInfo = 32,
	descriptorPoolCreateInfo = 33,
	descriptorSetAllocateInfo = 34,
	writeDescriptorSet = 35,
	copyDescriptorSet = 36,
	framebufferCreateInfo = 37,
	renderPassCreateInfo = 38,
	commandPoolCreateInfo = 39,
	commandBufferAllocateInfo = 40,
	commandBufferInheritanceInfo = 41,
	commandBufferBeginInfo = 42,
	renderPassBeginInfo = 43,
	bufferMemoryBarrier = 44,
	imageMemoryBarrier = 45,
	memoryBarrier = 46,
	loaderInstanceCreateInfo = 47,
	loaderDeviceCreateInfo = 48,
	swapchainCreateInfoKHR = 1000001000,
	presentInfoKHR = 1000001001,
	displayModeCreateInfoKHR = 1000002000,
	displaySurfaceCreateInfoKHR = 1000002001,
	displayPresentInfoKHR = 1000003000,
	xlibSurfaceCreateInfoKHR = 1000004000,
	xcbSurfaceCreateInfoKHR = 1000005000,
	waylandSurfaceCreateInfoKHR = 1000006000,
	mirSurfaceCreateInfoKHR = 1000007000,
	androidSurfaceCreateInfoKHR = 1000008000,
	win32SurfaceCreateInfoKHR = 1000009000,
	debugReportCallbackCreateInfoEXT = 1000011000,
	pipelineRasterizationStateRasterizationOrderAMD = 1000018000,
	debugMarkerObjectNameInfoEXT = 1000022000,
	debugMarkerObjectTagInfoEXT = 1000022001,
	debugMarkerMarkerInfoEXT = 1000022002,
};

enum class SystemAllocationScope : std::int32_t
{
	command = 0,
	object = 1,
	cache = 2,
	device = 3,
	instance = 4,
};

enum class InternalAllocationType : std::int32_t
{
	executable = 0,
};

enum class Result : std::int32_t
{
	success = 0,
	notReady = 1,
	timeout = 2,
	eventSet = 3,
	eventReset = 4,
	incomplete = 5,
	errorOutOfHostMemory = -1,
	errorOutOfDeviceMemory = -2,
	errorInitializationFailed = -3,
	errorDeviceLost = -4,
	errorMemoryMapFailed = -5,
	errorLayerNotPresent = -6,
	errorExtensionNotPresent = -7,
	errorFeatureNotPresent = -8,
	errorIncompatibleDriver = -9,
	errorTooManyObjects = -10,
	errorFormatNotSupported = -11,
	errorSurfaceLostKHR = -1000000000,
	errorNativeWindowInUseKHR = -1000000001,
	suboptimalKHR = 1000001003,
	errorOutOfDateKHR = -1000001004,
	errorIncompatibleDisplayKHR = -1000003001,
	errorValidationFailedEXT = -1000011001,
	errorInvalidShaderNV = -1000012000,
	nvExtension1Error = -1000013000,
};

enum class Format : std::int32_t
{
	undefined = 0,
	r4G4UnormPack8 = 1,
	r4G4B4A4UnormPack16 = 2,
	b4G4R4A4UnormPack16 = 3,
	r5G6B5UnormPack16 = 4,
	b5G6R5UnormPack16 = 5,
	r5G5B5A1UnormPack16 = 6,
	b5G5R5A1UnormPack16 = 7,
	a1R5G5B5UnormPack16 = 8,
	r8Unorm = 9,
	r8Snorm = 10,
	r8Uscaled = 11,
	r8Sscaled = 12,
	r8Uint = 13,
	r8Sint = 14,
	r8Srgb = 15,
	r8G8Unorm = 16,
	r8G8Snorm = 17,
	r8G8Uscaled = 18,
	r8G8Sscaled = 19,
	r8G8Uint = 20,
	r8G8Sint = 21,
	r8G8Srgb = 22,
	r8G8B8Unorm = 23,
	r8G8B8Snorm = 24,
	r8G8B8Uscaled = 25,
	r8G8B8Sscaled = 26,
	r8G8B8Uint = 27,
	r8G8B8Sint = 28,
	r8G8B8Srgb = 29,
	b8G8R8Unorm = 30,
	b8G8R8Snorm = 31,
	b8G8R8Uscaled = 32,
	b8G8R8Sscaled = 33,
	b8G8R8Uint = 34,
	b8G8R8Sint = 35,
	b8G8R8Srgb = 36,
	r8G8B8A8Unorm = 37,
	r8G8B8A8Snorm = 38,
	r8G8B8A8Uscaled = 39,
	r8G8B8A8Sscaled = 40,
	r8G8B8A8Uint = 41,
	r8G8B8A8Sint = 42,
	r8G8B8A8Srgb = 43,
	b8G8R8A8Unorm = 44,
	b8G8R8A8Snorm = 45,
	b8G8R8A8Uscaled = 46,
	b8G8R8A8Sscaled = 47,
	b8G8R8A8Uint = 48,
	b8G8R8A8Sint = 49,
	b8G8R8A8Srgb = 50,
	a8B8G8R8UnormPack32 = 51,
	a8B8G8R8SnormPack32 = 52,
	a8B8G8R8UscaledPack32 = 53,
	a8B8G8R8SscaledPack32 = 54,
	a8B8G8R8UintPack32 = 55,
	a8B8G8R8SintPack32 = 56,
	a8B8G8R8SrgbPack32 = 57,
	a2R10G10B10UnormPack32 = 58,
	a2R10G10B10SnormPack32 = 59,
	a2R10G10B10UscaledPack32 = 60,
	a2R10G10B10SscaledPack32 = 61,
	a2R10G10B10UintPack32 = 62,
	a2R10G10B10SintPack32 = 63,
	a2B10G10R10UnormPack32 = 64,
	a2B10G10R10SnormPack32 = 65,
	a2B10G10R10UscaledPack32 = 66,
	a2B10G10R10SscaledPack32 = 67,
	a2B10G10R10UintPack32 = 68,
	a2B10G10R10SintPack32 = 69,
	r16Unorm = 70,
	r16Snorm = 71,
	r16Uscaled = 72,
	r16Sscaled = 73,
	r16Uint = 74,
	r16Sint = 75,
	r16Sfloat = 76,
	r16G16Unorm = 77,
	r16G16Snorm = 78,
	r16G16Uscaled = 79,
	r16G16Sscaled = 80,
	r16G16Uint = 81,
	r16G16Sint = 82,
	r16G16Sfloat = 83,
	r16G16B16Unorm = 84,
	r16G16B16Snorm = 85,
	r16G16B16Uscaled = 86,
	r16G16B16Sscaled = 87,
	r16G16B16Uint = 88,
	r16G16B16Sint = 89,
	r16G16B16Sfloat = 90,
	r16G16B16A16Unorm = 91,
	r16G16B16A16Snorm = 92,
	r16G16B16A16Uscaled = 93,
	r16G16B16A16Sscaled = 94,
	r16G16B16A16Uint = 95,
	r16G16B16A16Sint = 96,
	r16G16B16A16Sfloat = 97,
	r32Uint = 98,
	r32Sint = 99,
	r32Sfloat = 100,
	r32G32Uint = 101,
	r32G32Sint = 102,
	r32G32Sfloat = 103,
	r32G32B32Uint = 104,
	r32G32B32Sint = 105,
	r32G32B32Sfloat = 106,
	r32G32B32A32Uint = 107,
	r32G32B32A32Sint = 108,
	r32G32B32A32Sfloat = 109,
	r64Uint = 110,
	r64Sint = 111,
	r64Sfloat = 112,
	r64G64Uint = 113,
	r64G64Sint = 114,
	r64G64Sfloat = 115,
	r64G64B64Uint = 116,
	r64G64B64Sint = 117,
	r64G64B64Sfloat = 118,
	r64G64B64A64Uint = 119,
	r64G64B64A64Sint = 120,
	r64G64B64A64Sfloat = 121,
	b10G11R11UfloatPack32 = 122,
	e5B9G9R9UfloatPack32 = 123,
	d16Unorm = 124,
	x8D24UnormPack32 = 125,
	d32Sfloat = 126,
	s8Uint = 127,
	d16UnormS8Uint = 128,
	d24UnormS8Uint = 129,
	d32SfloatS8Uint = 130,
	bc1RgbUnormBlock = 131,
	bc1RgbSrgbBlock = 132,
	bc1RgbaUnormBlock = 133,
	bc1RgbaSrgbBlock = 134,
	bc2UnormBlock = 135,
	bc2SrgbBlock = 136,
	bc3UnormBlock = 137,
	bc3SrgbBlock = 138,
	bc4UnormBlock = 139,
	bc4SnormBlock = 140,
	bc5UnormBlock = 141,
	bc5SnormBlock = 142,
	bc6HUfloatBlock = 143,
	bc6HSfloatBlock = 144,
	bc7UnormBlock = 145,
	bc7SrgbBlock = 146,
	etc2R8G8B8UnormBlock = 147,
	etc2R8G8B8SrgbBlock = 148,
	etc2R8G8B8A1UnormBlock = 149,
	etc2R8G8B8A1SrgbBlock = 150,
	etc2R8G8B8A8UnormBlock = 151,
	etc2R8G8B8A8SrgbBlock = 152,
	eacR11UnormBlock = 153,
	eacR11SnormBlock = 154,
	eacR11G11UnormBlock = 155,
	eacR11G11SnormBlock = 156,
	astc4X4UnormBlock = 157,
	astc4X4SrgbBlock = 158,
	astc5X4UnormBlock = 159,
	astc5X4SrgbBlock = 160,
	astc5X5UnormBlock = 161,
	astc5X5SrgbBlock = 162,
	astc6X5UnormBlock = 163,
	astc6X5SrgbBlock = 164,
	astc6X6UnormBlock = 165,
	astc6X6SrgbBlock = 166,
	astc8X5UnormBlock = 167,
	astc8X5SrgbBlock = 168,
	astc8X6UnormBlock = 169,
	astc8X6SrgbBlock = 170,
	astc8X8UnormBlock = 171,
	astc8X8SrgbBlock = 172,
	astc10X5UnormBlock = 173,
	astc10X5SrgbBlock = 174,
	astc10X6UnormBlock = 175,
	astc10X6SrgbBlock = 176,
	astc10X8UnormBlock = 177,
	astc10X8SrgbBlock = 178,
	astc10X10UnormBlock = 179,
	astc10X10SrgbBlock = 180,
	astc12X10UnormBlock = 181,
	astc12X10SrgbBlock = 182,
	astc12X12UnormBlock = 183,
	astc12X12SrgbBlock = 184,
};

enum class FormatFeatureFlagBits : std::int32_t
{
	sampledImage = (1 << 0),
	storageImage = (1 << 1),
	storageImageAtomic = (1 << 2),
	uniformTexelBuffer = (1 << 3),
	storageTexelBuffer = (1 << 4),
	storageTexelBufferAtomic = (1 << 5),
	vertexBuffer = (1 << 6),
	colorAttachment = (1 << 7),
	colorAttachmentBlend = (1 << 8),
	depthStencilAttachment = (1 << 9),
	blitSrc = (1 << 10),
	blitDst = (1 << 11),
	sampledImageFilterLinear = (1 << 12),
	sampledImageFilterCubicBitIMG = 1000015000,
};

enum class ImageType : std::int32_t
{
	e1D = 0,
	e2D = 1,
	e3D = 2,
};

enum class ImageTiling : std::int32_t
{
	optimal = 0,
	linear = 1,
};

enum class ImageUsageFlagBits : std::int32_t
{
	transferSrc = (1 << 0),
	transferDst = (1 << 1),
	sampled = (1 << 2),
	storage = (1 << 3),
	colorAttachment = (1 << 4),
	depthStencilAttachment = (1 << 5),
	transientAttachment = (1 << 6),
	inputAttachment = (1 << 7),
};

enum class ImageCreateFlagBits : std::int32_t
{
	sparseBinding = (1 << 0),
	sparseResidency = (1 << 1),
	sparseAliased = (1 << 2),
	mutableFormat = (1 << 3),
	cubeCompatible = (1 << 4),
};

enum class SampleCountFlagBits : std::int32_t
{
	e1 = (1 << 0),
	e2 = (1 << 1),
	e4 = (1 << 2),
	e8 = (1 << 3),
	e16 = (1 << 4),
	e32 = (1 << 5),
	e64 = (1 << 6),
};

enum class PhysicalDeviceType : std::int32_t
{
	other = 0,
	integratedGpu = 1,
	discreteGpu = 2,
	virtualGpu = 3,
	cpu = 4,
};

enum class QueueFlagBits : std::int32_t
{
	graphics = (1 << 0),
	compute = (1 << 1),
	transfer = (1 << 2),
	sparseBinding = (1 << 3),
};

enum class MemoryPropertyFlagBits : std::int32_t
{
	deviceLocal = (1 << 0),
	hostVisible = (1 << 1),
	hostCoherent = (1 << 2),
	hostCached = (1 << 3),
	lazilyAllocated = (1 << 4),
};

enum class MemoryHeapFlagBits : std::int32_t
{
	deviceLocal = (1 << 0),
};

enum class PipelineStageFlagBits : std::int32_t
{
	topOfPipe = (1 << 0),
	drawIndirect = (1 << 1),
	vertexInput = (1 << 2),
	vertexShader = (1 << 3),
	tessellationControlShader = (1 << 4),
	tessellationEvaluationShader = (1 << 5),
	geometryShader = (1 << 6),
	fragmentShader = (1 << 7),
	earlyFragmentTests = (1 << 8),
	lateFragmentTests = (1 << 9),
	colorAttachmentOutput = (1 << 10),
	computeShader = (1 << 11),
	transfer = (1 << 12),
	bottomOfPipe = (1 << 13),
	host = (1 << 14),
	allGraphics = (1 << 15),
	allCommands = (1 << 16),
};

enum class ImageAspectFlagBits : std::int32_t
{
	color = (1 << 0),
	depth = (1 << 1),
	stencil = (1 << 2),
	metadata = (1 << 3),
};

enum class SparseImageFormatFlagBits : std::int32_t
{
	singleMiptail = (1 << 0),
	alignedMipSize = (1 << 1),
	nonstandardBlockSize = (1 << 2),
};

enum class SparseMemoryBindFlagBits : std::int32_t
{
	metadata = (1 << 0),
};

enum class FenceCreateFlagBits : std::int32_t
{
	signaled = (1 << 0),
};

enum class QueryType : std::int32_t
{
	occlusion = 0,
	pipelineStatistics = 1,
	timestamp = 2,
};

enum class QueryPipelineStatisticFlagBits : std::int32_t
{
	inputAssemblyVertices = (1 << 0),
	inputAssemblyPrimitives = (1 << 1),
	vertexShaderInvocations = (1 << 2),
	geometryShaderInvocations = (1 << 3),
	geometryShaderPrimitives = (1 << 4),
	clippingInvocations = (1 << 5),
	clippingPrimitives = (1 << 6),
	fragmentShaderInvocations = (1 << 7),
	tessellationControlShaderPatches = (1 << 8),
	tessellationEvaluationShaderInvocations = (1 << 9),
	computeShaderInvocations = (1 << 10),
};

enum class QueryResultFlagBits : std::int32_t
{
	e64 = (1 << 0),
	wait = (1 << 1),
	withAvailability = (1 << 2),
	partial = (1 << 3),
};

enum class BufferCreateFlagBits : std::int32_t
{
	sparseBinding = (1 << 0),
	sparseResidency = (1 << 1),
	sparseAliased = (1 << 2),
};

enum class BufferUsageFlagBits : std::int32_t
{
	transferSrc = (1 << 0),
	transferDst = (1 << 1),
	uniformTexelBuffer = (1 << 2),
	storageTexelBuffer = (1 << 3),
	uniformBuffer = (1 << 4),
	storageBuffer = (1 << 5),
	indexBuffer = (1 << 6),
	vertexBuffer = (1 << 7),
	indirectBuffer = (1 << 8),
};

enum class SharingMode : std::int32_t
{
	exclusive = 0,
	concurrent = 1,
};

enum class ImageLayout : std::int32_t
{
	undefined = 0,
	general = 1,
	colorAttachmentOptimal = 2,
	depthStencilAttachmentOptimal = 3,
	depthStencilReadOnlyOptimal = 4,
	shaderReadOnlyOptimal = 5,
	transferSrcOptimal = 6,
	transferDstOptimal = 7,
	preinitialized = 8,
	presentSrcKHR = 1000001002,
};

enum class ImageViewType : std::int32_t
{
	e1D = 0,
	e2D = 1,
	e3D = 2,
	cube = 3,
	e1DArray = 4,
	e2DArray = 5,
	cubeArray = 6,
};

enum class ComponentSwizzle : std::int32_t
{
	identity = 0,
	zero = 1,
	one = 2,
	r = 3,
	g = 4,
	b = 5,
	a = 6,
};

enum class PipelineCreateFlagBits : std::int32_t
{
	disableOptimization = (1 << 0),
	allowDerivatives = (1 << 1),
	derivative = (1 << 2),
};

enum class ShaderStageFlagBits : std::int32_t
{
	vertex = (1 << 0),
	tessellationControl = (1 << 1),
	tessellationEvaluation = (1 << 2),
	geometry = (1 << 3),
	fragment = (1 << 4),
	compute = (1 << 5),
	allGraphics = 31,
	all = 2147483647,
};

enum class VertexInputRate : std::int32_t
{
	vertex = 0,
	instance = 1,
};

enum class PrimitiveTopology : std::int32_t
{
	pointList = 0,
	lineList = 1,
	lineStrip = 2,
	triangleList = 3,
	triangleStrip = 4,
	triangleFan = 5,
	lineListWithAdjacency = 6,
	lineStripWithAdjacency = 7,
	triangleListWithAdjacency = 8,
	triangleStripWithAdjacency = 9,
	patchList = 10,
};

enum class PolygonMode : std::int32_t
{
	fill = 0,
	line = 1,
	point = 2,
};

enum class CullModeFlagBits : std::int32_t
{
	none = 0,
	front = (1 << 0),
	back = (1 << 1),
	frontAndBack = 3,
};

enum class FrontFace : std::int32_t
{
	counterClockwise = 0,
	clockwise = 1,
};

enum class CompareOp : std::int32_t
{
	never = 0,
	less = 1,
	equal = 2,
	lessOrEqual = 3,
	greater = 4,
	notEqual = 5,
	greaterOrEqual = 6,
	always = 7,
};

enum class StencilOp : std::int32_t
{
	keep = 0,
	zero = 1,
	replace = 2,
	incrementAndClamp = 3,
	decrementAndClamp = 4,
	invert = 5,
	incrementAndWrap = 6,
	decrementAndWrap = 7,
};

enum class LogicOp : std::int32_t
{
	clear = 0,
	Eand = 1,
	andReverse = 2,
	copy = 3,
	andInverted = 4,
	noOp = 5,
	Exor = 6,
	Eor = 7,
	nor = 8,
	equivalent = 9,
	invert = 10,
	orReverse = 11,
	copyInverted = 12,
	orInverted = 13,
	nand = 14,
	set = 15,
};

enum class BlendFactor : std::int32_t
{
	zero = 0,
	one = 1,
	srcColor = 2,
	oneMinusSrcColor = 3,
	dstColor = 4,
	oneMinusDstColor = 5,
	srcAlpha = 6,
	oneMinusSrcAlpha = 7,
	dstAlpha = 8,
	oneMinusDstAlpha = 9,
	constantColor = 10,
	oneMinusConstantColor = 11,
	constantAlpha = 12,
	oneMinusConstantAlpha = 13,
	srcAlphaSaturate = 14,
	src1Color = 15,
	oneMinusSrc1Color = 16,
	src1Alpha = 17,
	oneMinusSrc1Alpha = 18,
};

enum class BlendOp : std::int32_t
{
	add = 0,
	subtract = 1,
	reverseSubtract = 2,
	min = 3,
	max = 4,
};

enum class ColorComponentFlagBits : std::int32_t
{
	r = (1 << 0),
	g = (1 << 1),
	b = (1 << 2),
	a = (1 << 3),
};

enum class DynamicState : std::int32_t
{
	viewport = 0,
	scissor = 1,
	lineWidth = 2,
	depthBias = 3,
	blendConstants = 4,
	depthBounds = 5,
	stencilCompareMask = 6,
	stencilWriteMask = 7,
	stencilReference = 8,
};

enum class Filter : std::int32_t
{
	nearest = 0,
	linear = 1,
	cubicIMG = 1000015000,
};

enum class SamplerMipmapMode : std::int32_t
{
	nearest = 0,
	linear = 1,
};

enum class SamplerAddressMode : std::int32_t
{
	repeat = 0,
	mirroredRepeat = 1,
	clampToEdge = 2,
	clampToBorder = 3,
	mirrorClampToEdge = 1000014000,
};

enum class BorderColor : std::int32_t
{
	floatTransparentBlack = 0,
	intTransparentBlack = 1,
	floatOpaqueBlack = 2,
	intOpaqueBlack = 3,
	floatOpaqueWhite = 4,
	intOpaqueWhite = 5,
};

enum class DescriptorType : std::int32_t
{
	sampler = 0,
	combinedImageSampler = 1,
	sampledImage = 2,
	storageImage = 3,
	uniformTexelBuffer = 4,
	storageTexelBuffer = 5,
	uniformBuffer = 6,
	storageBuffer = 7,
	uniformBufferDynamic = 8,
	storageBufferDynamic = 9,
	inputAttachment = 10,
};

enum class DescriptorPoolCreateFlagBits : std::int32_t
{
	freeDescriptorSet = (1 << 0),
};

enum class AttachmentDescriptionFlagBits : std::int32_t
{
	mayAlias = (1 << 0),
};

enum class AttachmentLoadOp : std::int32_t
{
	load = 0,
	clear = 1,
	dontCare = 2,
};

enum class AttachmentStoreOp : std::int32_t
{
	store = 0,
	dontCare = 1,
};

enum class PipelineBindPoint : std::int32_t
{
	graphics = 0,
	compute = 1,
};

enum class AccessFlagBits : std::int32_t
{
	indirectCommandRead = (1 << 0),
	indexRead = (1 << 1),
	vertexAttributeRead = (1 << 2),
	uniformRead = (1 << 3),
	inputAttachmentRead = (1 << 4),
	shaderRead = (1 << 5),
	shaderWrite = (1 << 6),
	colorAttachmentRead = (1 << 7),
	colorAttachmentWrite = (1 << 8),
	depthStencilAttachmentRead = (1 << 9),
	depthStencilAttachmentWrite = (1 << 10),
	transferRead = (1 << 11),
	transferWrite = (1 << 12),
	hostRead = (1 << 13),
	hostWrite = (1 << 14),
	memoryRead = (1 << 15),
	memoryWrite = (1 << 16),
};

enum class DependencyFlagBits : std::int32_t
{
	byRegion = (1 << 0),
};

enum class CommandPoolCreateFlagBits : std::int32_t
{
	transient = (1 << 0),
	resetCommandBuffer = (1 << 1),
};

enum class CommandPoolResetFlagBits : std::int32_t
{
	releaseResources = (1 << 0),
};

enum class CommandBufferLevel : std::int32_t
{
	primary = 0,
	secondary = 1,
};

enum class CommandBufferUsageFlagBits : std::int32_t
{
	oneTimeSubmit = (1 << 0),
	renderPassContinue = (1 << 1),
	simultaneousUse = (1 << 2),
};

enum class QueryControlFlagBits : std::int32_t
{
	precise = (1 << 0),
};

enum class CommandBufferResetFlagBits : std::int32_t
{
	releaseResources = (1 << 0),
};

enum class StencilFaceFlagBits : std::int32_t
{
	front = (1 << 0),
	back = (1 << 1),
	tAndBack = 3,
};

enum class IndexType : std::int32_t
{
	uint16 = 0,
	uint32 = 1,
};

enum class SubpassContents : std::int32_t
{
	Einline = 0,
	secondaryCommandBuffers = 1,
};


struct ApplicationInfo
{
	StructureType sType {StructureType::applicationInfo};
	const void* pNext {};
	const char* pApplicationName {};
	uint32_t applicationVersion {};
	const char* pEngineName {};
	uint32_t engineVersion {};
	uint32_t apiVersion {};

	operator const VkApplicationInfo&() const { return reinterpret_cast<const VkApplicationInfo&>(*this); }
};
struct InstanceCreateInfo
{
	StructureType sType {StructureType::instanceCreateInfo};
	const void* pNext {};
	InstanceCreateFlags flags {};
	const ApplicationInfo* pApplicationInfo {};
	uint32_t enabledLayerCount {};
	const char** ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char** ppEnabledExtensionNames {};

	operator const VkInstanceCreateInfo&() const { return reinterpret_cast<const VkInstanceCreateInfo&>(*this); }
};
struct AllocationCallbacks
{
	void* pUserData {};
	PFN_vkAllocationFunction pfnAllocation {};
	PFN_vkReallocationFunction pfnReallocation {};
	PFN_vkFreeFunction pfnFree {};
	PFN_vkInternalAllocationNotification pfnInternalAllocation {};
	PFN_vkInternalFreeNotification pfnInternalFree {};

	operator const VkAllocationCallbacks&() const { return reinterpret_cast<const VkAllocationCallbacks&>(*this); }
};
struct PhysicalDeviceFeatures
{
	Bool32 robustBufferAccess {};
	Bool32 fullDrawIndexUint32 {};
	Bool32 imageCubeArray {};
	Bool32 independentBlend {};
	Bool32 geometryShader {};
	Bool32 tessellationShader {};
	Bool32 sampleRateShading {};
	Bool32 dualSrcBlend {};
	Bool32 logicOp {};
	Bool32 multiDrawIndirect {};
	Bool32 drawIndirectFirstInstance {};
	Bool32 depthClamp {};
	Bool32 depthBiasClamp {};
	Bool32 fillModeNonSolid {};
	Bool32 depthBounds {};
	Bool32 wideLines {};
	Bool32 largePoints {};
	Bool32 alphaToOne {};
	Bool32 multiViewport {};
	Bool32 samplerAnisotropy {};
	Bool32 textureCompressionETC2 {};
	Bool32 textureCompressionASTC_LDR {};
	Bool32 textureCompressionBC {};
	Bool32 occlusionQueryPrecise {};
	Bool32 pipelineStatisticsQuery {};
	Bool32 vertexPipelineStoresAndAtomics {};
	Bool32 fragmentStoresAndAtomics {};
	Bool32 shaderTessellationAndGeometryPointSize {};
	Bool32 shaderImageGatherExtended {};
	Bool32 shaderStorageImageExtendedFormats {};
	Bool32 shaderStorageImageMultisample {};
	Bool32 shaderStorageImageReadWithoutFormat {};
	Bool32 shaderStorageImageWriteWithoutFormat {};
	Bool32 shaderUniformBufferArrayDynamicIndexing {};
	Bool32 shaderSampledImageArrayDynamicIndexing {};
	Bool32 shaderStorageBufferArrayDynamicIndexing {};
	Bool32 shaderStorageImageArrayDynamicIndexing {};
	Bool32 shaderClipDistance {};
	Bool32 shaderCullDistance {};
	Bool32 shaderFloat64 {};
	Bool32 shaderInt64 {};
	Bool32 shaderInt16 {};
	Bool32 shaderResourceResidency {};
	Bool32 shaderResourceMinLod {};
	Bool32 sparseBinding {};
	Bool32 sparseResidencyBuffer {};
	Bool32 sparseResidencyImage2D {};
	Bool32 sparseResidencyImage3D {};
	Bool32 sparseResidency2Samples {};
	Bool32 sparseResidency4Samples {};
	Bool32 sparseResidency8Samples {};
	Bool32 sparseResidency16Samples {};
	Bool32 sparseResidencyAliased {};
	Bool32 variableMultisampleRate {};
	Bool32 inheritedQueries {};

	operator const VkPhysicalDeviceFeatures&() const { return reinterpret_cast<const VkPhysicalDeviceFeatures&>(*this); }
};
struct FormatProperties
{
	FormatFeatureFlags linearTilingFeatures {};
	FormatFeatureFlags optimalTilingFeatures {};
	FormatFeatureFlags bufferFeatures {};

	operator const VkFormatProperties&() const { return reinterpret_cast<const VkFormatProperties&>(*this); }
};
struct Extent3D
{
	uint32_t width {};
	uint32_t height {};
	uint32_t depth {};

	operator const VkExtent3D&() const { return reinterpret_cast<const VkExtent3D&>(*this); }
};
struct ImageFormatProperties
{
	Extent3D maxExtent {};
	uint32_t maxMipLevels {};
	uint32_t maxArrayLayers {};
	SampleCountFlags sampleCounts {};
	DeviceSize maxResourceSize {};

	operator const VkImageFormatProperties&() const { return reinterpret_cast<const VkImageFormatProperties&>(*this); }
};
struct PhysicalDeviceLimits
{
	uint32_t maxImageDimension1D {};
	uint32_t maxImageDimension2D {};
	uint32_t maxImageDimension3D {};
	uint32_t maxImageDimensionCube {};
	uint32_t maxImageArrayLayers {};
	uint32_t maxTexelBufferElements {};
	uint32_t maxUniformBufferRange {};
	uint32_t maxStorageBufferRange {};
	uint32_t maxPushConstantsSize {};
	uint32_t maxMemoryAllocationCount {};
	uint32_t maxSamplerAllocationCount {};
	DeviceSize bufferImageGranularity {};
	DeviceSize sparseAddressSpaceSize {};
	uint32_t maxBoundDescriptorSets {};
	uint32_t maxPerStageDescriptorSamplers {};
	uint32_t maxPerStageDescriptorUniformBuffers {};
	uint32_t maxPerStageDescriptorStorageBuffers {};
	uint32_t maxPerStageDescriptorSampledImages {};
	uint32_t maxPerStageDescriptorStorageImages {};
	uint32_t maxPerStageDescriptorInputAttachments {};
	uint32_t maxPerStageResources {};
	uint32_t maxDescriptorSetSamplers {};
	uint32_t maxDescriptorSetUniformBuffers {};
	uint32_t maxDescriptorSetUniformBuffersDynamic {};
	uint32_t maxDescriptorSetStorageBuffers {};
	uint32_t maxDescriptorSetStorageBuffersDynamic {};
	uint32_t maxDescriptorSetSampledImages {};
	uint32_t maxDescriptorSetStorageImages {};
	uint32_t maxDescriptorSetInputAttachments {};
	uint32_t maxVertexInputAttributes {};
	uint32_t maxVertexInputBindings {};
	uint32_t maxVertexInputAttributeOffset {};
	uint32_t maxVertexInputBindingStride {};
	uint32_t maxVertexOutputComponents {};
	uint32_t maxTessellationGenerationLevel {};
	uint32_t maxTessellationPatchSize {};
	uint32_t maxTessellationControlPerVertexInputComponents {};
	uint32_t maxTessellationControlPerVertexOutputComponents {};
	uint32_t maxTessellationControlPerPatchOutputComponents {};
	uint32_t maxTessellationControlTotalOutputComponents {};
	uint32_t maxTessellationEvaluationInputComponents {};
	uint32_t maxTessellationEvaluationOutputComponents {};
	uint32_t maxGeometryShaderInvocations {};
	uint32_t maxGeometryInputComponents {};
	uint32_t maxGeometryOutputComponents {};
	uint32_t maxGeometryOutputVertices {};
	uint32_t maxGeometryTotalOutputComponents {};
	uint32_t maxFragmentInputComponents {};
	uint32_t maxFragmentOutputAttachments {};
	uint32_t maxFragmentDualSrcAttachments {};
	uint32_t maxFragmentCombinedOutputResources {};
	uint32_t maxComputeSharedMemorySize {};
	uint32_t maxComputeWorkGroupCount[3] {};
	uint32_t maxComputeWorkGroupInvocations {};
	uint32_t maxComputeWorkGroupSize[3] {};
	uint32_t subPixelPrecisionBits {};
	uint32_t subTexelPrecisionBits {};
	uint32_t mipmapPrecisionBits {};
	uint32_t maxDrawIndexedIndexValue {};
	uint32_t maxDrawIndirectCount {};
	float maxSamplerLodBias {};
	float maxSamplerAnisotropy {};
	uint32_t maxViewports {};
	uint32_t maxViewportDimensions[2] {};
	float viewportBoundsRange[2] {};
	uint32_t viewportSubPixelBits {};
	size_t minMemoryMapAlignment {};
	DeviceSize minTexelBufferOffsetAlignment {};
	DeviceSize minUniformBufferOffsetAlignment {};
	DeviceSize minStorageBufferOffsetAlignment {};
	int32_t minTexelOffset {};
	uint32_t maxTexelOffset {};
	int32_t minTexelGatherOffset {};
	uint32_t maxTexelGatherOffset {};
	float minInterpolationOffset {};
	float maxInterpolationOffset {};
	uint32_t subPixelInterpolationOffsetBits {};
	uint32_t maxFramebufferWidth {};
	uint32_t maxFramebufferHeight {};
	uint32_t maxFramebufferLayers {};
	SampleCountFlags framebufferColorSampleCounts {};
	SampleCountFlags framebufferDepthSampleCounts {};
	SampleCountFlags framebufferStencilSampleCounts {};
	SampleCountFlags framebufferNoAttachmentsSampleCounts {};
	uint32_t maxColorAttachments {};
	SampleCountFlags sampledImageColorSampleCounts {};
	SampleCountFlags sampledImageIntegerSampleCounts {};
	SampleCountFlags sampledImageDepthSampleCounts {};
	SampleCountFlags sampledImageStencilSampleCounts {};
	SampleCountFlags storageImageSampleCounts {};
	uint32_t maxSampleMaskWords {};
	Bool32 timestampComputeAndGraphics {};
	float timestampPeriod {};
	uint32_t maxClipDistances {};
	uint32_t maxCullDistances {};
	uint32_t maxCombinedClipAndCullDistances {};
	uint32_t discreteQueuePriorities {};
	float pointSizeRange[2] {};
	float lineWidthRange[2] {};
	float pointSizeGranularity {};
	float lineWidthGranularity {};
	Bool32 strictLines {};
	Bool32 standardSampleLocations {};
	DeviceSize optimalBufferCopyOffsetAlignment {};
	DeviceSize optimalBufferCopyRowPitchAlignment {};
	DeviceSize nonCoherentAtomSize {};

	operator const VkPhysicalDeviceLimits&() const { return reinterpret_cast<const VkPhysicalDeviceLimits&>(*this); }
};
struct PhysicalDeviceSparseProperties
{
	Bool32 residencyStandard2DBlockShape {};
	Bool32 residencyStandard2DMultisampleBlockShape {};
	Bool32 residencyStandard3DBlockShape {};
	Bool32 residencyAlignedMipSize {};
	Bool32 residencyNonResidentStrict {};

	operator const VkPhysicalDeviceSparseProperties&() const { return reinterpret_cast<const VkPhysicalDeviceSparseProperties&>(*this); }
};
struct PhysicalDeviceProperties
{
	uint32_t apiVersion {};
	uint32_t driverVersion {};
	uint32_t vendorID {};
	uint32_t deviceID {};
	PhysicalDeviceType deviceType {};
	char deviceName {};
	uint8_t pipelineCacheUUID {};
	PhysicalDeviceLimits limits {};
	PhysicalDeviceSparseProperties sparseProperties {};

	operator const VkPhysicalDeviceProperties&() const { return reinterpret_cast<const VkPhysicalDeviceProperties&>(*this); }
};
struct QueueFamilyProperties
{
	QueueFlags queueFlags {};
	uint32_t queueCount {};
	uint32_t timestampValidBits {};
	Extent3D minImageTransferGranularity {};

	operator const VkQueueFamilyProperties&() const { return reinterpret_cast<const VkQueueFamilyProperties&>(*this); }
};
struct MemoryType
{
	MemoryPropertyFlags propertyFlags {};
	uint32_t heapIndex {};

	operator const VkMemoryType&() const { return reinterpret_cast<const VkMemoryType&>(*this); }
};
struct MemoryHeap
{
	DeviceSize size {};
	MemoryHeapFlags flags {};

	operator const VkMemoryHeap&() const { return reinterpret_cast<const VkMemoryHeap&>(*this); }
};
struct PhysicalDeviceMemoryProperties
{
	uint32_t memoryTypeCount {};
	MemoryType memoryTypes {};
	uint32_t memoryHeapCount {};
	MemoryHeap memoryHeaps {};

	operator const VkPhysicalDeviceMemoryProperties&() const { return reinterpret_cast<const VkPhysicalDeviceMemoryProperties&>(*this); }
};
struct DeviceQueueCreateInfo
{
	StructureType sType {StructureType::deviceQueueCreateInfo};
	const void* pNext {};
	DeviceQueueCreateFlags flags {};
	uint32_t queueFamilyIndex {};
	uint32_t queueCount {};
	const float* pQueuePriorities {};

	operator const VkDeviceQueueCreateInfo&() const { return reinterpret_cast<const VkDeviceQueueCreateInfo&>(*this); }
};
struct DeviceCreateInfo
{
	StructureType sType {StructureType::deviceCreateInfo};
	const void* pNext {};
	DeviceCreateFlags flags {};
	uint32_t queueCreateInfoCount {};
	const DeviceQueueCreateInfo* pQueueCreateInfos {};
	uint32_t enabledLayerCount {};
	const char** ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char** ppEnabledExtensionNames {};
	const PhysicalDeviceFeatures* pEnabledFeatures {};

	operator const VkDeviceCreateInfo&() const { return reinterpret_cast<const VkDeviceCreateInfo&>(*this); }
};
struct ExtensionProperties
{
	char extensionName {};
	uint32_t specVersion {};

	operator const VkExtensionProperties&() const { return reinterpret_cast<const VkExtensionProperties&>(*this); }
};
struct LayerProperties
{
	char layerName {};
	uint32_t specVersion {};
	uint32_t implementationVersion {};
	char description {};

	operator const VkLayerProperties&() const { return reinterpret_cast<const VkLayerProperties&>(*this); }
};
struct SubmitInfo
{
	StructureType sType {StructureType::submitInfo};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	const PipelineStageFlags* pWaitDstStageMask {};
	uint32_t commandBufferCount {};
	const CommandBuffer* pCommandBuffers {};
	uint32_t signalSemaphoreCount {};
	const Semaphore* pSignalSemaphores {};

	operator const VkSubmitInfo&() const { return reinterpret_cast<const VkSubmitInfo&>(*this); }
};
struct MemoryAllocateInfo
{
	StructureType sType {StructureType::memoryAllocateInfo};
	const void* pNext {};
	DeviceSize allocationSize {};
	uint32_t memoryTypeIndex {};

	operator const VkMemoryAllocateInfo&() const { return reinterpret_cast<const VkMemoryAllocateInfo&>(*this); }
};
struct MappedMemoryRange
{
	StructureType sType {StructureType::mappedMemoryRange};
	const void* pNext {};
	DeviceMemory memory {};
	DeviceSize offset {};
	DeviceSize size {};

	operator const VkMappedMemoryRange&() const { return reinterpret_cast<const VkMappedMemoryRange&>(*this); }
};
struct MemoryRequirements
{
	DeviceSize size {};
	DeviceSize alignment {};
	uint32_t memoryTypeBits {};

	operator const VkMemoryRequirements&() const { return reinterpret_cast<const VkMemoryRequirements&>(*this); }
};
struct SparseImageFormatProperties
{
	ImageAspectFlags aspectMask {};
	Extent3D imageGranularity {};
	SparseImageFormatFlags flags {};

	operator const VkSparseImageFormatProperties&() const { return reinterpret_cast<const VkSparseImageFormatProperties&>(*this); }
};
struct SparseImageMemoryRequirements
{
	SparseImageFormatProperties formatProperties {};
	uint32_t imageMipTailFirstLod {};
	DeviceSize imageMipTailSize {};
	DeviceSize imageMipTailOffset {};
	DeviceSize imageMipTailStride {};

	operator const VkSparseImageMemoryRequirements&() const { return reinterpret_cast<const VkSparseImageMemoryRequirements&>(*this); }
};
struct SparseMemoryBind
{
	DeviceSize resourceOffset {};
	DeviceSize size {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};

	operator const VkSparseMemoryBind&() const { return reinterpret_cast<const VkSparseMemoryBind&>(*this); }
};
struct SparseBufferMemoryBindInfo
{
	Buffer buffer {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	operator const VkSparseBufferMemoryBindInfo&() const { return reinterpret_cast<const VkSparseBufferMemoryBindInfo&>(*this); }
};
struct SparseImageOpaqueMemoryBindInfo
{
	Image image {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};

	operator const VkSparseImageOpaqueMemoryBindInfo&() const { return reinterpret_cast<const VkSparseImageOpaqueMemoryBindInfo&>(*this); }
};
struct ImageSubresource
{
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t arrayLayer {};

	operator const VkImageSubresource&() const { return reinterpret_cast<const VkImageSubresource&>(*this); }
};
struct Offset3D
{
	int32_t x {};
	int32_t y {};
	int32_t z {};

	operator const VkOffset3D&() const { return reinterpret_cast<const VkOffset3D&>(*this); }
};
struct SparseImageMemoryBind
{
	ImageSubresource subresource {};
	Offset3D offset {};
	Extent3D extent {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};

	operator const VkSparseImageMemoryBind&() const { return reinterpret_cast<const VkSparseImageMemoryBind&>(*this); }
};
struct SparseImageMemoryBindInfo
{
	Image image {};
	uint32_t bindCount {};
	const SparseImageMemoryBind* pBinds {};

	operator const VkSparseImageMemoryBindInfo&() const { return reinterpret_cast<const VkSparseImageMemoryBindInfo&>(*this); }
};
struct BindSparseInfo
{
	StructureType sType {StructureType::bindSparseInfo};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	uint32_t bufferBindCount {};
	const SparseBufferMemoryBindInfo* pBufferBinds {};
	uint32_t imageOpaqueBindCount {};
	const SparseImageOpaqueMemoryBindInfo* pImageOpaqueBinds {};
	uint32_t imageBindCount {};
	const SparseImageMemoryBindInfo* pImageBinds {};
	uint32_t signalSemaphoreCount {};
	const Semaphore* pSignalSemaphores {};

	operator const VkBindSparseInfo&() const { return reinterpret_cast<const VkBindSparseInfo&>(*this); }
};
struct FenceCreateInfo
{
	StructureType sType {StructureType::fenceCreateInfo};
	const void* pNext {};
	FenceCreateFlags flags {};

	operator const VkFenceCreateInfo&() const { return reinterpret_cast<const VkFenceCreateInfo&>(*this); }
};
struct SemaphoreCreateInfo
{
	StructureType sType {StructureType::semaphoreCreateInfo};
	const void* pNext {};
	SemaphoreCreateFlags flags {};

	operator const VkSemaphoreCreateInfo&() const { return reinterpret_cast<const VkSemaphoreCreateInfo&>(*this); }
};
struct EventCreateInfo
{
	StructureType sType {StructureType::eventCreateInfo};
	const void* pNext {};
	EventCreateFlags flags {};

	operator const VkEventCreateInfo&() const { return reinterpret_cast<const VkEventCreateInfo&>(*this); }
};
struct QueryPoolCreateInfo
{
	StructureType sType {StructureType::queryPoolCreateInfo};
	const void* pNext {};
	QueryPoolCreateFlags flags {};
	QueryType queryType {};
	uint32_t queryCount {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	operator const VkQueryPoolCreateInfo&() const { return reinterpret_cast<const VkQueryPoolCreateInfo&>(*this); }
};
struct BufferCreateInfo
{
	StructureType sType {StructureType::bufferCreateInfo};
	const void* pNext {};
	BufferCreateFlags flags {};
	DeviceSize size {};
	BufferUsageFlags usage {};
	SharingMode sharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};

	operator const VkBufferCreateInfo&() const { return reinterpret_cast<const VkBufferCreateInfo&>(*this); }
};
struct BufferViewCreateInfo
{
	StructureType sType {StructureType::bufferViewCreateInfo};
	const void* pNext {};
	BufferViewCreateFlags flags {};
	Buffer buffer {};
	Format format {};
	DeviceSize offset {};
	DeviceSize range {};

	operator const VkBufferViewCreateInfo&() const { return reinterpret_cast<const VkBufferViewCreateInfo&>(*this); }
};
struct ImageCreateInfo
{
	StructureType sType {StructureType::imageCreateInfo};
	const void* pNext {};
	ImageCreateFlags flags {};
	ImageType imageType {};
	Format format {};
	Extent3D extent {};
	uint32_t mipLevels {};
	uint32_t arrayLayers {};
	SampleCountFlagBits samples {};
	ImageTiling tiling {};
	ImageUsageFlags usage {};
	SharingMode sharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};
	ImageLayout initialLayout {};

	operator const VkImageCreateInfo&() const { return reinterpret_cast<const VkImageCreateInfo&>(*this); }
};
struct SubresourceLayout
{
	DeviceSize offset {};
	DeviceSize size {};
	DeviceSize rowPitch {};
	DeviceSize arrayPitch {};
	DeviceSize depthPitch {};

	operator const VkSubresourceLayout&() const { return reinterpret_cast<const VkSubresourceLayout&>(*this); }
};
struct ComponentMapping
{
	ComponentSwizzle r {};
	ComponentSwizzle g {};
	ComponentSwizzle b {};
	ComponentSwizzle a {};

	operator const VkComponentMapping&() const { return reinterpret_cast<const VkComponentMapping&>(*this); }
};
struct ImageSubresourceRange
{
	ImageAspectFlags aspectMask {};
	uint32_t baseMipLevel {};
	uint32_t levelCount {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	operator const VkImageSubresourceRange&() const { return reinterpret_cast<const VkImageSubresourceRange&>(*this); }
};
struct ImageViewCreateInfo
{
	StructureType sType {StructureType::imageViewCreateInfo};
	const void* pNext {};
	ImageViewCreateFlags flags {};
	Image image {};
	ImageViewType viewType {};
	Format format {};
	ComponentMapping components {};
	ImageSubresourceRange subresourceRange {};

	operator const VkImageViewCreateInfo&() const { return reinterpret_cast<const VkImageViewCreateInfo&>(*this); }
};
struct ShaderModuleCreateInfo
{
	StructureType sType {StructureType::shaderModuleCreateInfo};
	const void* pNext {};
	ShaderModuleCreateFlags flags {};
	size_t codeSize {};
	const uint32_t* pCode {};

	operator const VkShaderModuleCreateInfo&() const { return reinterpret_cast<const VkShaderModuleCreateInfo&>(*this); }
};
struct PipelineCacheCreateInfo
{
	StructureType sType {StructureType::pipelineCacheCreateInfo};
	const void* pNext {};
	PipelineCacheCreateFlags flags {};
	size_t initialDataSize {};
	const void* pInitialData {};

	operator const VkPipelineCacheCreateInfo&() const { return reinterpret_cast<const VkPipelineCacheCreateInfo&>(*this); }
};
struct SpecializationMapEntry
{
	uint32_t constantID {};
	uint32_t offset {};
	size_t size {};

	operator const VkSpecializationMapEntry&() const { return reinterpret_cast<const VkSpecializationMapEntry&>(*this); }
};
struct SpecializationInfo
{
	uint32_t mapEntryCount {};
	const SpecializationMapEntry* pMapEntries {};
	size_t dataSize {};
	const void* pData {};

	operator const VkSpecializationInfo&() const { return reinterpret_cast<const VkSpecializationInfo&>(*this); }
};
struct PipelineShaderStageCreateInfo
{
	StructureType sType {StructureType::pipelineShaderStageCreateInfo};
	const void* pNext {};
	PipelineShaderStageCreateFlags flags {};
	ShaderStageFlagBits stage {};
	ShaderModule module {};
	const char* pName {};
	const SpecializationInfo* pSpecializationInfo {};

	operator const VkPipelineShaderStageCreateInfo&() const { return reinterpret_cast<const VkPipelineShaderStageCreateInfo&>(*this); }
};
struct VertexInputBindingDescription
{
	uint32_t binding {};
	uint32_t stride {};
	VertexInputRate inputRate {};

	operator const VkVertexInputBindingDescription&() const { return reinterpret_cast<const VkVertexInputBindingDescription&>(*this); }
};
struct VertexInputAttributeDescription
{
	uint32_t location {};
	uint32_t binding {};
	Format format {};
	uint32_t offset {};

	operator const VkVertexInputAttributeDescription&() const { return reinterpret_cast<const VkVertexInputAttributeDescription&>(*this); }
};
struct PipelineVertexInputStateCreateInfo
{
	StructureType sType {StructureType::pipelineVertexInputStateCreateInfo};
	const void* pNext {};
	PipelineVertexInputStateCreateFlags flags {};
	uint32_t vertexBindingDescriptionCount {};
	const VertexInputBindingDescription* pVertexBindingDescriptions {};
	uint32_t vertexAttributeDescriptionCount {};
	const VertexInputAttributeDescription* pVertexAttributeDescriptions {};

	operator const VkPipelineVertexInputStateCreateInfo&() const { return reinterpret_cast<const VkPipelineVertexInputStateCreateInfo&>(*this); }
};
struct PipelineInputAssemblyStateCreateInfo
{
	StructureType sType {StructureType::pipelineInputAssemblyStateCreateInfo};
	const void* pNext {};
	PipelineInputAssemblyStateCreateFlags flags {};
	PrimitiveTopology topology {};
	Bool32 primitiveRestartEnable {};

	operator const VkPipelineInputAssemblyStateCreateInfo&() const { return reinterpret_cast<const VkPipelineInputAssemblyStateCreateInfo&>(*this); }
};
struct PipelineTessellationStateCreateInfo
{
	StructureType sType {StructureType::pipelineTessellationStateCreateInfo};
	const void* pNext {};
	PipelineTessellationStateCreateFlags flags {};
	uint32_t patchControlPoints {};

	operator const VkPipelineTessellationStateCreateInfo&() const { return reinterpret_cast<const VkPipelineTessellationStateCreateInfo&>(*this); }
};
struct Viewport
{
	float x {};
	float y {};
	float width {};
	float height {};
	float minDepth {};
	float maxDepth {};

	operator const VkViewport&() const { return reinterpret_cast<const VkViewport&>(*this); }
};
struct Offset2D
{
	int32_t x {};
	int32_t y {};

	operator const VkOffset2D&() const { return reinterpret_cast<const VkOffset2D&>(*this); }
};
struct Extent2D
{
	uint32_t width {};
	uint32_t height {};

	operator const VkExtent2D&() const { return reinterpret_cast<const VkExtent2D&>(*this); }
};
struct Rect2D
{
	Offset2D offset {};
	Extent2D extent {};

	operator const VkRect2D&() const { return reinterpret_cast<const VkRect2D&>(*this); }
};
struct PipelineViewportStateCreateInfo
{
	StructureType sType {StructureType::pipelineViewportStateCreateInfo};
	const void* pNext {};
	PipelineViewportStateCreateFlags flags {};
	uint32_t viewportCount {};
	const Viewport* pViewports {};
	uint32_t scissorCount {};
	const Rect2D* pScissors {};

	operator const VkPipelineViewportStateCreateInfo&() const { return reinterpret_cast<const VkPipelineViewportStateCreateInfo&>(*this); }
};
struct PipelineRasterizationStateCreateInfo
{
	StructureType sType {StructureType::pipelineRasterizationStateCreateInfo};
	const void* pNext {};
	PipelineRasterizationStateCreateFlags flags {};
	Bool32 depthClampEnable {};
	Bool32 rasterizerDiscardEnable {};
	PolygonMode polygonMode {};
	CullModeFlags cullMode {};
	FrontFace frontFace {};
	Bool32 depthBiasEnable {};
	float depthBiasConstantFactor {};
	float depthBiasClamp {};
	float depthBiasSlopeFactor {};
	float lineWidth {};

	operator const VkPipelineRasterizationStateCreateInfo&() const { return reinterpret_cast<const VkPipelineRasterizationStateCreateInfo&>(*this); }
};
struct PipelineMultisampleStateCreateInfo
{
	StructureType sType {StructureType::pipelineMultisampleStateCreateInfo};
	const void* pNext {};
	PipelineMultisampleStateCreateFlags flags {};
	SampleCountFlagBits rasterizationSamples {};
	Bool32 sampleShadingEnable {};
	float minSampleShading {};
	const SampleMask* pSampleMask {};
	Bool32 alphaToCoverageEnable {};
	Bool32 alphaToOneEnable {};

	operator const VkPipelineMultisampleStateCreateInfo&() const { return reinterpret_cast<const VkPipelineMultisampleStateCreateInfo&>(*this); }
};
struct StencilOpState
{
	StencilOp failOp {};
	StencilOp passOp {};
	StencilOp depthFailOp {};
	CompareOp compareOp {};
	uint32_t compareMask {};
	uint32_t writeMask {};
	uint32_t reference {};

	operator const VkStencilOpState&() const { return reinterpret_cast<const VkStencilOpState&>(*this); }
};
struct PipelineDepthStencilStateCreateInfo
{
	StructureType sType {StructureType::pipelineDepthStencilStateCreateInfo};
	const void* pNext {};
	PipelineDepthStencilStateCreateFlags flags {};
	Bool32 depthTestEnable {};
	Bool32 depthWriteEnable {};
	CompareOp depthCompareOp {};
	Bool32 depthBoundsTestEnable {};
	Bool32 stencilTestEnable {};
	StencilOpState front {};
	StencilOpState back {};
	float minDepthBounds {};
	float maxDepthBounds {};

	operator const VkPipelineDepthStencilStateCreateInfo&() const { return reinterpret_cast<const VkPipelineDepthStencilStateCreateInfo&>(*this); }
};
struct PipelineColorBlendAttachmentState
{
	Bool32 blendEnable {};
	BlendFactor srcColorBlendFactor {};
	BlendFactor dstColorBlendFactor {};
	BlendOp colorBlendOp {};
	BlendFactor srcAlphaBlendFactor {};
	BlendFactor dstAlphaBlendFactor {};
	BlendOp alphaBlendOp {};
	ColorComponentFlags colorWriteMask {};

	operator const VkPipelineColorBlendAttachmentState&() const { return reinterpret_cast<const VkPipelineColorBlendAttachmentState&>(*this); }
};
struct PipelineColorBlendStateCreateInfo
{
	StructureType sType {StructureType::pipelineColorBlendStateCreateInfo};
	const void* pNext {};
	PipelineColorBlendStateCreateFlags flags {};
	Bool32 logicOpEnable {};
	LogicOp logicOp {};
	uint32_t attachmentCount {};
	const PipelineColorBlendAttachmentState* pAttachments {};
	float blendConstants[4] {};

	operator const VkPipelineColorBlendStateCreateInfo&() const { return reinterpret_cast<const VkPipelineColorBlendStateCreateInfo&>(*this); }
};
struct PipelineDynamicStateCreateInfo
{
	StructureType sType {StructureType::pipelineDynamicStateCreateInfo};
	const void* pNext {};
	PipelineDynamicStateCreateFlags flags {};
	uint32_t dynamicStateCount {};
	const DynamicState* pDynamicStates {};

	operator const VkPipelineDynamicStateCreateInfo&() const { return reinterpret_cast<const VkPipelineDynamicStateCreateInfo&>(*this); }
};
struct GraphicsPipelineCreateInfo
{
	StructureType sType {StructureType::graphicsPipelineCreateInfo};
	const void* pNext {};
	PipelineCreateFlags flags {};
	uint32_t stageCount {};
	const PipelineShaderStageCreateInfo* pStages {};
	const PipelineVertexInputStateCreateInfo* pVertexInputState {};
	const PipelineInputAssemblyStateCreateInfo* pInputAssemblyState {};
	const PipelineTessellationStateCreateInfo* pTessellationState {};
	const PipelineViewportStateCreateInfo* pViewportState {};
	const PipelineRasterizationStateCreateInfo* pRasterizationState {};
	const PipelineMultisampleStateCreateInfo* pMultisampleState {};
	const PipelineDepthStencilStateCreateInfo* pDepthStencilState {};
	const PipelineColorBlendStateCreateInfo* pColorBlendState {};
	const PipelineDynamicStateCreateInfo* pDynamicState {};
	PipelineLayout layout {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Pipeline basePipelineHandle {};
	int32_t basePipelineIndex {};

	operator const VkGraphicsPipelineCreateInfo&() const { return reinterpret_cast<const VkGraphicsPipelineCreateInfo&>(*this); }
};
struct ComputePipelineCreateInfo
{
	StructureType sType {StructureType::computePipelineCreateInfo};
	const void* pNext {};
	PipelineCreateFlags flags {};
	PipelineShaderStageCreateInfo stage {};
	PipelineLayout layout {};
	Pipeline basePipelineHandle {};
	int32_t basePipelineIndex {};

	operator const VkComputePipelineCreateInfo&() const { return reinterpret_cast<const VkComputePipelineCreateInfo&>(*this); }
};
struct PushConstantRange
{
	ShaderStageFlags stageFlags {};
	uint32_t offset {};
	uint32_t size {};

	operator const VkPushConstantRange&() const { return reinterpret_cast<const VkPushConstantRange&>(*this); }
};
struct PipelineLayoutCreateInfo
{
	StructureType sType {StructureType::pipelineLayoutCreateInfo};
	const void* pNext {};
	PipelineLayoutCreateFlags flags {};
	uint32_t setLayoutCount {};
	const DescriptorSetLayout* pSetLayouts {};
	uint32_t pushConstantRangeCount {};
	const PushConstantRange* pPushConstantRanges {};

	operator const VkPipelineLayoutCreateInfo&() const { return reinterpret_cast<const VkPipelineLayoutCreateInfo&>(*this); }
};
struct SamplerCreateInfo
{
	StructureType sType {StructureType::samplerCreateInfo};
	const void* pNext {};
	SamplerCreateFlags flags {};
	Filter magFilter {};
	Filter minFilter {};
	SamplerMipmapMode mipmapMode {};
	SamplerAddressMode addressModeU {};
	SamplerAddressMode addressModeV {};
	SamplerAddressMode addressModeW {};
	float mipLodBias {};
	Bool32 anisotropyEnable {};
	float maxAnisotropy {};
	Bool32 compareEnable {};
	CompareOp compareOp {};
	float minLod {};
	float maxLod {};
	BorderColor borderColor {};
	Bool32 unnormalizedCoordinates {};

	operator const VkSamplerCreateInfo&() const { return reinterpret_cast<const VkSamplerCreateInfo&>(*this); }
};
struct DescriptorSetLayoutBinding
{
	uint32_t binding {};
	DescriptorType descriptorType {};
	uint32_t descriptorCount {};
	ShaderStageFlags stageFlags {};
	const Sampler* pImmutableSamplers {};

	operator const VkDescriptorSetLayoutBinding&() const { return reinterpret_cast<const VkDescriptorSetLayoutBinding&>(*this); }
};
struct DescriptorSetLayoutCreateInfo
{
	StructureType sType {StructureType::descriptorSetLayoutCreateInfo};
	const void* pNext {};
	DescriptorSetLayoutCreateFlags flags {};
	uint32_t bindingCount {};
	const DescriptorSetLayoutBinding* pBindings {};

	operator const VkDescriptorSetLayoutCreateInfo&() const { return reinterpret_cast<const VkDescriptorSetLayoutCreateInfo&>(*this); }
};
struct DescriptorPoolSize
{
	DescriptorType type {};
	uint32_t descriptorCount {};

	operator const VkDescriptorPoolSize&() const { return reinterpret_cast<const VkDescriptorPoolSize&>(*this); }
};
struct DescriptorPoolCreateInfo
{
	StructureType sType {StructureType::descriptorPoolCreateInfo};
	const void* pNext {};
	DescriptorPoolCreateFlags flags {};
	uint32_t maxSets {};
	uint32_t poolSizeCount {};
	const DescriptorPoolSize* pPoolSizes {};

	operator const VkDescriptorPoolCreateInfo&() const { return reinterpret_cast<const VkDescriptorPoolCreateInfo&>(*this); }
};
struct DescriptorSetAllocateInfo
{
	StructureType sType {StructureType::descriptorSetAllocateInfo};
	const void* pNext {};
	DescriptorPool descriptorPool {};
	uint32_t descriptorSetCount {};
	const DescriptorSetLayout* pSetLayouts {};

	operator const VkDescriptorSetAllocateInfo&() const { return reinterpret_cast<const VkDescriptorSetAllocateInfo&>(*this); }
};
struct DescriptorImageInfo
{
	Sampler sampler {};
	ImageView imageView {};
	ImageLayout imageLayout {};

	operator const VkDescriptorImageInfo&() const { return reinterpret_cast<const VkDescriptorImageInfo&>(*this); }
};
struct DescriptorBufferInfo
{
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize range {};

	operator const VkDescriptorBufferInfo&() const { return reinterpret_cast<const VkDescriptorBufferInfo&>(*this); }
};
struct WriteDescriptorSet
{
	StructureType sType {StructureType::writeDescriptorSet};
	const void* pNext {};
	DescriptorSet dstSet {};
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};
	DescriptorType descriptorType {};
	const DescriptorImageInfo* pImageInfo {};
	const DescriptorBufferInfo* pBufferInfo {};
	const BufferView* pTexelBufferView {};

	operator const VkWriteDescriptorSet&() const { return reinterpret_cast<const VkWriteDescriptorSet&>(*this); }
};
struct CopyDescriptorSet
{
	StructureType sType {StructureType::copyDescriptorSet};
	const void* pNext {};
	DescriptorSet srcSet {};
	uint32_t srcBinding {};
	uint32_t srcArrayElement {};
	DescriptorSet dstSet {};
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};

	operator const VkCopyDescriptorSet&() const { return reinterpret_cast<const VkCopyDescriptorSet&>(*this); }
};
struct FramebufferCreateInfo
{
	StructureType sType {StructureType::framebufferCreateInfo};
	const void* pNext {};
	FramebufferCreateFlags flags {};
	RenderPass renderPass {};
	uint32_t attachmentCount {};
	const ImageView* pAttachments {};
	uint32_t width {};
	uint32_t height {};
	uint32_t layers {};

	operator const VkFramebufferCreateInfo&() const { return reinterpret_cast<const VkFramebufferCreateInfo&>(*this); }
};
struct AttachmentDescription
{
	AttachmentDescriptionFlags flags {};
	Format format {};
	SampleCountFlagBits samples {};
	AttachmentLoadOp loadOp {};
	AttachmentStoreOp storeOp {};
	AttachmentLoadOp stencilLoadOp {};
	AttachmentStoreOp stencilStoreOp {};
	ImageLayout initialLayout {};
	ImageLayout finalLayout {};

	operator const VkAttachmentDescription&() const { return reinterpret_cast<const VkAttachmentDescription&>(*this); }
};
struct AttachmentReference
{
	uint32_t attachment {};
	ImageLayout layout {};

	operator const VkAttachmentReference&() const { return reinterpret_cast<const VkAttachmentReference&>(*this); }
};
struct SubpassDescription
{
	SubpassDescriptionFlags flags {};
	PipelineBindPoint pipelineBindPoint {};
	uint32_t inputAttachmentCount {};
	const AttachmentReference* pInputAttachments {};
	uint32_t colorAttachmentCount {};
	const AttachmentReference* pColorAttachments {};
	const AttachmentReference* pResolveAttachments {};
	const AttachmentReference* pDepthStencilAttachment {};
	uint32_t preserveAttachmentCount {};
	const uint32_t* pPreserveAttachments {};

	operator const VkSubpassDescription&() const { return reinterpret_cast<const VkSubpassDescription&>(*this); }
};
struct SubpassDependency
{
	uint32_t srcSubpass {};
	uint32_t dstSubpass {};
	PipelineStageFlags srcStageMask {};
	PipelineStageFlags dstStageMask {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	DependencyFlags dependencyFlags {};

	operator const VkSubpassDependency&() const { return reinterpret_cast<const VkSubpassDependency&>(*this); }
};
struct RenderPassCreateInfo
{
	StructureType sType {StructureType::renderPassCreateInfo};
	const void* pNext {};
	RenderPassCreateFlags flags {};
	uint32_t attachmentCount {};
	const AttachmentDescription* pAttachments {};
	uint32_t subpassCount {};
	const SubpassDescription* pSubpasses {};
	uint32_t dependencyCount {};
	const SubpassDependency* pDependencies {};

	operator const VkRenderPassCreateInfo&() const { return reinterpret_cast<const VkRenderPassCreateInfo&>(*this); }
};
struct CommandPoolCreateInfo
{
	StructureType sType {StructureType::commandPoolCreateInfo};
	const void* pNext {};
	CommandPoolCreateFlags flags {};
	uint32_t queueFamilyIndex {};

	operator const VkCommandPoolCreateInfo&() const { return reinterpret_cast<const VkCommandPoolCreateInfo&>(*this); }
};
struct CommandBufferAllocateInfo
{
	StructureType sType {StructureType::commandBufferAllocateInfo};
	const void* pNext {};
	CommandPool commandPool {};
	CommandBufferLevel level {};
	uint32_t commandBufferCount {};

	operator const VkCommandBufferAllocateInfo&() const { return reinterpret_cast<const VkCommandBufferAllocateInfo&>(*this); }
};
struct CommandBufferInheritanceInfo
{
	StructureType sType {StructureType::commandBufferInheritanceInfo};
	const void* pNext {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Framebuffer framebuffer {};
	Bool32 occlusionQueryEnable {};
	QueryControlFlags queryFlags {};
	QueryPipelineStatisticFlags pipelineStatistics {};

	operator const VkCommandBufferInheritanceInfo&() const { return reinterpret_cast<const VkCommandBufferInheritanceInfo&>(*this); }
};
struct CommandBufferBeginInfo
{
	StructureType sType {StructureType::commandBufferBeginInfo};
	const void* pNext {};
	CommandBufferUsageFlags flags {};
	const CommandBufferInheritanceInfo* pInheritanceInfo {};

	operator const VkCommandBufferBeginInfo&() const { return reinterpret_cast<const VkCommandBufferBeginInfo&>(*this); }
};
struct BufferCopy
{
	DeviceSize srcOffset {};
	DeviceSize dstOffset {};
	DeviceSize size {};

	operator const VkBufferCopy&() const { return reinterpret_cast<const VkBufferCopy&>(*this); }
};
struct ImageSubresourceLayers
{
	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	operator const VkImageSubresourceLayers&() const { return reinterpret_cast<const VkImageSubresourceLayers&>(*this); }
};
struct ImageCopy
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	operator const VkImageCopy&() const { return reinterpret_cast<const VkImageCopy&>(*this); }
};
struct ImageBlit
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffsets[2] {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffsets[2] {};

	operator const VkImageBlit&() const { return reinterpret_cast<const VkImageBlit&>(*this); }
};
struct BufferImageCopy
{
	DeviceSize bufferOffset {};
	uint32_t bufferRowLength {};
	uint32_t bufferImageHeight {};
	ImageSubresourceLayers imageSubresource {};
	Offset3D imageOffset {};
	Extent3D imageExtent {};

	operator const VkBufferImageCopy&() const { return reinterpret_cast<const VkBufferImageCopy&>(*this); }
};
union ClearColorValue
{
	float float32[4] {};
	int32_t int32[4];
	uint32_t uint32[4];

	operator const VkClearColorValue&() const { return reinterpret_cast<const VkClearColorValue&>(*this); }
};
struct ClearDepthStencilValue
{
	float depth {};
	uint32_t stencil {};

	operator const VkClearDepthStencilValue&() const { return reinterpret_cast<const VkClearDepthStencilValue&>(*this); }
};
union ClearValue
{
	ClearColorValue color {};
	ClearDepthStencilValue depthStencil;

	operator const VkClearValue&() const { return reinterpret_cast<const VkClearValue&>(*this); }
};
struct ClearAttachment
{
	ImageAspectFlags aspectMask {};
	uint32_t colorAttachment {};
	ClearValue clearValue {};

	operator const VkClearAttachment&() const { return reinterpret_cast<const VkClearAttachment&>(*this); }
};
struct ClearRect
{
	Rect2D rect {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};

	operator const VkClearRect&() const { return reinterpret_cast<const VkClearRect&>(*this); }
};
struct ImageResolve
{
	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};

	operator const VkImageResolve&() const { return reinterpret_cast<const VkImageResolve&>(*this); }
};
struct MemoryBarrier
{
	StructureType sType {StructureType::memoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};

	operator const VkMemoryBarrier&() const { return reinterpret_cast<const VkMemoryBarrier&>(*this); }
};
struct BufferMemoryBarrier
{
	StructureType sType {StructureType::bufferMemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	uint32_t srcQueueFamilyIndex {};
	uint32_t dstQueueFamilyIndex {};
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize size {};

	operator const VkBufferMemoryBarrier&() const { return reinterpret_cast<const VkBufferMemoryBarrier&>(*this); }
};
struct ImageMemoryBarrier
{
	StructureType sType {StructureType::imageMemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	ImageLayout oldLayout {};
	ImageLayout newLayout {};
	uint32_t srcQueueFamilyIndex {};
	uint32_t dstQueueFamilyIndex {};
	Image image {};
	ImageSubresourceRange subresourceRange {};

	operator const VkImageMemoryBarrier&() const { return reinterpret_cast<const VkImageMemoryBarrier&>(*this); }
};
struct RenderPassBeginInfo
{
	StructureType sType {StructureType::renderPassBeginInfo};
	const void* pNext {};
	RenderPass renderPass {};
	Framebuffer framebuffer {};
	Rect2D renderArea {};
	uint32_t clearValueCount {};
	const ClearValue* pClearValues {};

	operator const VkRenderPassBeginInfo&() const { return reinterpret_cast<const VkRenderPassBeginInfo&>(*this); }
};
struct DispatchIndirectCommand
{
	uint32_t x {};
	uint32_t y {};
	uint32_t z {};

	operator const VkDispatchIndirectCommand&() const { return reinterpret_cast<const VkDispatchIndirectCommand&>(*this); }
};
struct DrawIndexedIndirectCommand
{
	uint32_t indexCount {};
	uint32_t instanceCount {};
	uint32_t firstIndex {};
	int32_t vertexOffset {};
	uint32_t firstInstance {};

	operator const VkDrawIndexedIndirectCommand&() const { return reinterpret_cast<const VkDrawIndexedIndirectCommand&>(*this); }
};
struct DrawIndirectCommand
{
	uint32_t vertexCount {};
	uint32_t instanceCount {};
	uint32_t firstVertex {};
	uint32_t firstInstance {};

	operator const VkDrawIndirectCommand&() const { return reinterpret_cast<const VkDrawIndirectCommand&>(*this); }
};

Result createInstance(const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance)
{
	return static_cast<Result>(vkCreateInstance(reinterpret_cast<const VkInstanceCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkInstance*>(pInstance)));
}
void destroyInstance(Instance instance, const AllocationCallbacks* pAllocator)
{
	vkDestroyInstance(instance, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result enumeratePhysicalDevices(Instance instance, uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices)
{
	return static_cast<Result>(vkEnumeratePhysicalDevices(instance, reinterpret_cast<uint32_t*>(pPhysicalDeviceCount), reinterpret_cast<VkPhysicalDevice*>(pPhysicalDevices)));
}
void getPhysicalDeviceFeatures(PhysicalDevice physicalDevice, PhysicalDeviceFeatures* pFeatures)
{
	vkGetPhysicalDeviceFeatures(physicalDevice, reinterpret_cast<VkPhysicalDeviceFeatures*>(pFeatures));
}
void getPhysicalDeviceFormatProperties(PhysicalDevice physicalDevice, Format format, FormatProperties* pFormatProperties)
{
	vkGetPhysicalDeviceFormatProperties(physicalDevice, static_cast<VkFormat>(format), reinterpret_cast<VkFormatProperties*>(pFormatProperties));
}
Result getPhysicalDeviceImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties)
{
	return static_cast<Result>(vkGetPhysicalDeviceImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkImageTiling>(tiling), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageCreateFlags>(flags), reinterpret_cast<VkImageFormatProperties*>(pImageFormatProperties)));
}
void getPhysicalDeviceProperties(PhysicalDevice physicalDevice, PhysicalDeviceProperties* pProperties)
{
	vkGetPhysicalDeviceProperties(physicalDevice, reinterpret_cast<VkPhysicalDeviceProperties*>(pProperties));
}
void getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties)
{
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, reinterpret_cast<uint32_t*>(pQueueFamilyPropertyCount), reinterpret_cast<VkQueueFamilyProperties*>(pQueueFamilyProperties));
}
void getPhysicalDeviceMemoryProperties(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties* pMemoryProperties)
{
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, reinterpret_cast<VkPhysicalDeviceMemoryProperties*>(pMemoryProperties));
}
PFN_vkVoidFunction getInstanceProcAddr(Instance instance, const char* pName)
{
	return static_cast<PFN_vkVoidFunction>(vkGetInstanceProcAddr(instance, reinterpret_cast<const char*>(pName)));
}
PFN_vkVoidFunction getDeviceProcAddr(Device device, const char* pName)
{
	return static_cast<PFN_vkVoidFunction>(vkGetDeviceProcAddr(device, reinterpret_cast<const char*>(pName)));
}
Result createDevice(PhysicalDevice physicalDevice, const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice)
{
	return static_cast<Result>(vkCreateDevice(physicalDevice, reinterpret_cast<const VkDeviceCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDevice*>(pDevice)));
}
void destroyDevice(Device device, const AllocationCallbacks* pAllocator)
{
	vkDestroyDevice(device, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result enumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties)
{
	return static_cast<Result>(vkEnumerateInstanceExtensionProperties(reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties)));
}
Result enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties)
{
	return static_cast<Result>(vkEnumerateDeviceExtensionProperties(physicalDevice, reinterpret_cast<const char*>(pLayerName), reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkExtensionProperties*>(pProperties)));
}
Result enumerateInstanceLayerProperties(uint32_t* pPropertyCount, LayerProperties* pProperties)
{
	return static_cast<Result>(vkEnumerateInstanceLayerProperties(reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties)));
}
Result enumerateDeviceLayerProperties(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, LayerProperties* pProperties)
{
	return static_cast<Result>(vkEnumerateDeviceLayerProperties(physicalDevice, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkLayerProperties*>(pProperties)));
}
void getDeviceQueue(Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue)
{
	vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, reinterpret_cast<VkQueue*>(pQueue));
}
Result queueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence)
{
	return static_cast<Result>(vkQueueSubmit(queue, submitCount, reinterpret_cast<const VkSubmitInfo*>(pSubmits), fence));
}
Result queueWaitIdle(Queue queue)
{
	return static_cast<Result>(vkQueueWaitIdle(queue));
}
Result deviceWaitIdle(Device device)
{
	return static_cast<Result>(vkDeviceWaitIdle(device));
}
Result allocateMemory(Device device, const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory)
{
	return static_cast<Result>(vkAllocateMemory(device, reinterpret_cast<const VkMemoryAllocateInfo*>(pAllocateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDeviceMemory*>(pMemory)));
}
void freeMemory(Device device, DeviceMemory memory, const AllocationCallbacks* pAllocator)
{
	vkFreeMemory(device, memory, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result mapMemory(Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData)
{
	return static_cast<Result>(vkMapMemory(device, memory, offset, size, static_cast<VkMemoryMapFlags>(flags), reinterpret_cast<void**>(ppData)));
}
void unmapMemory(Device device, DeviceMemory memory)
{
	vkUnmapMemory(device, memory);
}
Result flushMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges)
{
	return static_cast<Result>(vkFlushMappedMemoryRanges(device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges)));
}
Result invalidateMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges)
{
	return static_cast<Result>(vkInvalidateMappedMemoryRanges(device, memoryRangeCount, reinterpret_cast<const VkMappedMemoryRange*>(pMemoryRanges)));
}
void getDeviceMemoryCommitment(Device device, DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes)
{
	vkGetDeviceMemoryCommitment(device, memory, reinterpret_cast<VkDeviceSize*>(pCommittedMemoryInBytes));
}
Result bindBufferMemory(Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset)
{
	return static_cast<Result>(vkBindBufferMemory(device, buffer, memory, memoryOffset));
}
Result bindImageMemory(Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset)
{
	return static_cast<Result>(vkBindImageMemory(device, image, memory, memoryOffset));
}
void getBufferMemoryRequirements(Device device, Buffer buffer, MemoryRequirements* pMemoryRequirements)
{
	vkGetBufferMemoryRequirements(device, buffer, reinterpret_cast<VkMemoryRequirements*>(pMemoryRequirements));
}
void getImageMemoryRequirements(Device device, Image image, MemoryRequirements* pMemoryRequirements)
{
	vkGetImageMemoryRequirements(device, image, reinterpret_cast<VkMemoryRequirements*>(pMemoryRequirements));
}
void getImageSparseMemoryRequirements(Device device, Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements)
{
	vkGetImageSparseMemoryRequirements(device, image, reinterpret_cast<uint32_t*>(pSparseMemoryRequirementCount), reinterpret_cast<VkSparseImageMemoryRequirements*>(pSparseMemoryRequirements));
}
void getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties)
{
	vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, static_cast<VkFormat>(format), static_cast<VkImageType>(type), static_cast<VkSampleCountFlagBits>(samples), static_cast<VkImageUsageFlags>(usage), static_cast<VkImageTiling>(tiling), reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkSparseImageFormatProperties*>(pProperties));
}
Result queueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence)
{
	return static_cast<Result>(vkQueueBindSparse(queue, bindInfoCount, reinterpret_cast<const VkBindSparseInfo*>(pBindInfo), fence));
}
Result createFence(Device device, const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence)
{
	return static_cast<Result>(vkCreateFence(device, reinterpret_cast<const VkFenceCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFence*>(pFence)));
}
void destroyFence(Device device, Fence fence, const AllocationCallbacks* pAllocator)
{
	vkDestroyFence(device, fence, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result resetFences(Device device, uint32_t fenceCount, const Fence* pFences)
{
	return static_cast<Result>(vkResetFences(device, fenceCount, reinterpret_cast<const VkFence*>(pFences)));
}
Result getFenceStatus(Device device, Fence fence)
{
	return static_cast<Result>(vkGetFenceStatus(device, fence));
}
Result waitForFences(Device device, uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout)
{
	return static_cast<Result>(vkWaitForFences(device, fenceCount, reinterpret_cast<const VkFence*>(pFences), waitAll, timeout));
}
Result createSemaphore(Device device, const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore)
{
	return static_cast<Result>(vkCreateSemaphore(device, reinterpret_cast<const VkSemaphoreCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSemaphore*>(pSemaphore)));
}
void destroySemaphore(Device device, Semaphore semaphore, const AllocationCallbacks* pAllocator)
{
	vkDestroySemaphore(device, semaphore, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createEvent(Device device, const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent)
{
	return static_cast<Result>(vkCreateEvent(device, reinterpret_cast<const VkEventCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkEvent*>(pEvent)));
}
void destroyEvent(Device device, Event event, const AllocationCallbacks* pAllocator)
{
	vkDestroyEvent(device, event, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getEventStatus(Device device, Event event)
{
	return static_cast<Result>(vkGetEventStatus(device, event));
}
Result setEvent(Device device, Event event)
{
	return static_cast<Result>(vkSetEvent(device, event));
}
Result resetEvent(Device device, Event event)
{
	return static_cast<Result>(vkResetEvent(device, event));
}
Result createQueryPool(Device device, const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool)
{
	return static_cast<Result>(vkCreateQueryPool(device, reinterpret_cast<const VkQueryPoolCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkQueryPool*>(pQueryPool)));
}
void destroyQueryPool(Device device, QueryPool queryPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyQueryPool(device, queryPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags)
{
	return static_cast<Result>(vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, reinterpret_cast<void*>(pData), stride, static_cast<VkQueryResultFlags>(flags)));
}
Result createBuffer(Device device, const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer)
{
	return static_cast<Result>(vkCreateBuffer(device, reinterpret_cast<const VkBufferCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBuffer*>(pBuffer)));
}
void destroyBuffer(Device device, Buffer buffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyBuffer(device, buffer, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createBufferView(Device device, const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView)
{
	return static_cast<Result>(vkCreateBufferView(device, reinterpret_cast<const VkBufferViewCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkBufferView*>(pView)));
}
void destroyBufferView(Device device, BufferView bufferView, const AllocationCallbacks* pAllocator)
{
	vkDestroyBufferView(device, bufferView, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createImage(Device device, const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage)
{
	return static_cast<Result>(vkCreateImage(device, reinterpret_cast<const VkImageCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImage*>(pImage)));
}
void destroyImage(Device device, Image image, const AllocationCallbacks* pAllocator)
{
	vkDestroyImage(device, image, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
void getImageSubresourceLayout(Device device, Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout)
{
	vkGetImageSubresourceLayout(device, image, reinterpret_cast<const VkImageSubresource*>(pSubresource), reinterpret_cast<VkSubresourceLayout*>(pLayout));
}
Result createImageView(Device device, const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView)
{
	return static_cast<Result>(vkCreateImageView(device, reinterpret_cast<const VkImageViewCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkImageView*>(pView)));
}
void destroyImageView(Device device, ImageView imageView, const AllocationCallbacks* pAllocator)
{
	vkDestroyImageView(device, imageView, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createShaderModule(Device device, const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule)
{
	return static_cast<Result>(vkCreateShaderModule(device, reinterpret_cast<const VkShaderModuleCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkShaderModule*>(pShaderModule)));
}
void destroyShaderModule(Device device, ShaderModule shaderModule, const AllocationCallbacks* pAllocator)
{
	vkDestroyShaderModule(device, shaderModule, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createPipelineCache(Device device, const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache)
{
	return static_cast<Result>(vkCreatePipelineCache(device, reinterpret_cast<const VkPipelineCacheCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineCache*>(pPipelineCache)));
}
void destroyPipelineCache(Device device, PipelineCache pipelineCache, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineCache(device, pipelineCache, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getPipelineCacheData(Device device, PipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
	return static_cast<Result>(vkGetPipelineCacheData(device, pipelineCache, reinterpret_cast<size_t*>(pDataSize), reinterpret_cast<void*>(pData)));
}
Result mergePipelineCaches(Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches)
{
	return static_cast<Result>(vkMergePipelineCaches(device, dstCache, srcCacheCount, reinterpret_cast<const VkPipelineCache*>(pSrcCaches)));
}
Result createGraphicsPipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines)
{
	return static_cast<Result>(vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, reinterpret_cast<const VkGraphicsPipelineCreateInfo*>(pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(pPipelines)));
}
Result createComputePipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines)
{
	return static_cast<Result>(vkCreateComputePipelines(device, pipelineCache, createInfoCount, reinterpret_cast<const VkComputePipelineCreateInfo*>(pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipeline*>(pPipelines)));
}
void destroyPipeline(Device device, Pipeline pipeline, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipeline(device, pipeline, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createPipelineLayout(Device device, const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout)
{
	return static_cast<Result>(vkCreatePipelineLayout(device, reinterpret_cast<const VkPipelineLayoutCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkPipelineLayout*>(pPipelineLayout)));
}
void destroyPipelineLayout(Device device, PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineLayout(device, pipelineLayout, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createSampler(Device device, const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler)
{
	return static_cast<Result>(vkCreateSampler(device, reinterpret_cast<const VkSamplerCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSampler*>(pSampler)));
}
void destroySampler(Device device, Sampler sampler, const AllocationCallbacks* pAllocator)
{
	vkDestroySampler(device, sampler, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createDescriptorSetLayout(Device device, const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout)
{
	return static_cast<Result>(vkCreateDescriptorSetLayout(device, reinterpret_cast<const VkDescriptorSetLayoutCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorSetLayout*>(pSetLayout)));
}
void destroyDescriptorSetLayout(Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createDescriptorPool(Device device, const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool)
{
	return static_cast<Result>(vkCreateDescriptorPool(device, reinterpret_cast<const VkDescriptorPoolCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDescriptorPool*>(pDescriptorPool)));
}
void destroyDescriptorPool(Device device, DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorPool(device, descriptorPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result resetDescriptorPool(Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags)
{
	return static_cast<Result>(vkResetDescriptorPool(device, descriptorPool, static_cast<VkDescriptorPoolResetFlags>(flags)));
}
Result allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets)
{
	return static_cast<Result>(vkAllocateDescriptorSets(device, reinterpret_cast<const VkDescriptorSetAllocateInfo*>(pAllocateInfo), reinterpret_cast<VkDescriptorSet*>(pDescriptorSets)));
}
Result freeDescriptorSets(Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets)
{
	return static_cast<Result>(vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets)));
}
void updateDescriptorSets(Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies)
{
	vkUpdateDescriptorSets(device, descriptorWriteCount, reinterpret_cast<const VkWriteDescriptorSet*>(pDescriptorWrites), descriptorCopyCount, reinterpret_cast<const VkCopyDescriptorSet*>(pDescriptorCopies));
}
Result createFramebuffer(Device device, const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer)
{
	return static_cast<Result>(vkCreateFramebuffer(device, reinterpret_cast<const VkFramebufferCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkFramebuffer*>(pFramebuffer)));
}
void destroyFramebuffer(Device device, Framebuffer framebuffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyFramebuffer(device, framebuffer, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result createRenderPass(Device device, const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass)
{
	return static_cast<Result>(vkCreateRenderPass(device, reinterpret_cast<const VkRenderPassCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkRenderPass*>(pRenderPass)));
}
void destroyRenderPass(Device device, RenderPass renderPass, const AllocationCallbacks* pAllocator)
{
	vkDestroyRenderPass(device, renderPass, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
void getRenderAreaGranularity(Device device, RenderPass renderPass, Extent2D* pGranularity)
{
	vkGetRenderAreaGranularity(device, renderPass, reinterpret_cast<VkExtent2D*>(pGranularity));
}
Result createCommandPool(Device device, const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool)
{
	return static_cast<Result>(vkCreateCommandPool(device, reinterpret_cast<const VkCommandPoolCreateInfo*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkCommandPool*>(pCommandPool)));
}
void destroyCommandPool(Device device, CommandPool commandPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyCommandPool(device, commandPool, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result resetCommandPool(Device device, CommandPool commandPool, CommandPoolResetFlags flags)
{
	return static_cast<Result>(vkResetCommandPool(device, commandPool, static_cast<VkCommandPoolResetFlags>(flags)));
}
Result allocateCommandBuffers(Device device, const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers)
{
	return static_cast<Result>(vkAllocateCommandBuffers(device, reinterpret_cast<const VkCommandBufferAllocateInfo*>(pAllocateInfo), reinterpret_cast<VkCommandBuffer*>(pCommandBuffers)));
}
void freeCommandBuffers(Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers)
{
	vkFreeCommandBuffers(device, commandPool, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers));
}
Result beginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo* pBeginInfo)
{
	return static_cast<Result>(vkBeginCommandBuffer(commandBuffer, reinterpret_cast<const VkCommandBufferBeginInfo*>(pBeginInfo)));
}
Result endCommandBuffer(CommandBuffer commandBuffer)
{
	return static_cast<Result>(vkEndCommandBuffer(commandBuffer));
}
Result resetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags)
{
	return static_cast<Result>(vkResetCommandBuffer(commandBuffer, static_cast<VkCommandBufferResetFlags>(flags)));
}
void cmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline)
{
	vkCmdBindPipeline(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), pipeline);
}
void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, reinterpret_cast<const VkViewport*>(pViewports));
}
void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors)
{
	vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, reinterpret_cast<const VkRect2D*>(pScissors));
}
void cmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth)
{
	vkCmdSetLineWidth(commandBuffer, lineWidth);
}
void cmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
	vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
void cmdSetBlendConstants(CommandBuffer commandBuffer, const float blendConstants[4])
{
	vkCmdSetBlendConstants(commandBuffer, blendConstants);
}
void cmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
	vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
void cmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask)
{
	vkCmdSetStencilCompareMask(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), compareMask);
}
void cmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask)
{
	vkCmdSetStencilWriteMask(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), writeMask);
}
void cmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference)
{
	vkCmdSetStencilReference(commandBuffer, static_cast<VkStencilFaceFlags>(faceMask), reference);
}
void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, static_cast<VkPipelineBindPoint>(pipelineBindPoint), layout, firstSet, descriptorSetCount, reinterpret_cast<const VkDescriptorSet*>(pDescriptorSets), dynamicOffsetCount, reinterpret_cast<const uint32_t*>(pDynamicOffsets));
}
void cmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer, offset, static_cast<VkIndexType>(indexType));
}
void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets)
{
	vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, reinterpret_cast<const VkBuffer*>(pBuffers), reinterpret_cast<const VkDeviceSize*>(pOffsets));
}
void cmdDraw(CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
	vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}
void cmdDrawIndexed(CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
{
	vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}
void cmdDrawIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
void cmdDrawIndexedIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride)
{
	vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}
void cmdDispatch(CommandBuffer commandBuffer, uint32_t x, uint32_t y, uint32_t z)
{
	vkCmdDispatch(commandBuffer, x, y, z);
}
void cmdDispatchIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset)
{
	vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}
void cmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy* pRegions)
{
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, reinterpret_cast<const VkBufferCopy*>(pRegions));
}
void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions)
{
	vkCmdCopyImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageCopy*>(pRegions));
}
void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter)
{
	vkCmdBlitImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageBlit*>(pRegions), static_cast<VkFilter>(filter));
}
void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkBufferImageCopy*>(pRegions));
}
void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions)
{
	vkCmdCopyImageToBuffer(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstBuffer, regionCount, reinterpret_cast<const VkBufferImageCopy*>(pRegions));
}
void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t* pData)
{
	vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, reinterpret_cast<const uint32_t*>(pData));
}
void cmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data)
{
	vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges)
{
	vkCmdClearColorImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearColorValue*>(pColor), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(pRanges));
}
void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges)
{
	vkCmdClearDepthStencilImage(commandBuffer, image, static_cast<VkImageLayout>(imageLayout), reinterpret_cast<const VkClearDepthStencilValue*>(pDepthStencil), rangeCount, reinterpret_cast<const VkImageSubresourceRange*>(pRanges));
}
void cmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects)
{
	vkCmdClearAttachments(commandBuffer, attachmentCount, reinterpret_cast<const VkClearAttachment*>(pAttachments), rectCount, reinterpret_cast<const VkClearRect*>(pRects));
}
void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions)
{
	vkCmdResolveImage(commandBuffer, srcImage, static_cast<VkImageLayout>(srcImageLayout), dstImage, static_cast<VkImageLayout>(dstImageLayout), regionCount, reinterpret_cast<const VkImageResolve*>(pRegions));
}
void cmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdSetEvent(commandBuffer, event, static_cast<VkPipelineStageFlags>(stageMask));
}
void cmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdResetEvent(commandBuffer, event, static_cast<VkPipelineStageFlags>(stageMask));
}
void cmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers)
{
	vkCmdWaitEvents(commandBuffer, eventCount, reinterpret_cast<const VkEvent*>(pEvents), static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers));
}
void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers)
{
	vkCmdPipelineBarrier(commandBuffer, static_cast<VkPipelineStageFlags>(srcStageMask), static_cast<VkPipelineStageFlags>(dstStageMask), static_cast<VkDependencyFlags>(dependencyFlags), memoryBarrierCount, reinterpret_cast<const VkMemoryBarrier*>(pMemoryBarriers), bufferMemoryBarrierCount, reinterpret_cast<const VkBufferMemoryBarrier*>(pBufferMemoryBarriers), imageMemoryBarrierCount, reinterpret_cast<const VkImageMemoryBarrier*>(pImageMemoryBarriers));
}
void cmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags)
{
	vkCmdBeginQuery(commandBuffer, queryPool, query, static_cast<VkQueryControlFlags>(flags));
}
void cmdEndQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query)
{
	vkCmdEndQuery(commandBuffer, queryPool, query);
}
void cmdResetQueryPool(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
	vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}
void cmdWriteTimestamp(CommandBuffer commandBuffer, PipelineStageFlagBits pipelineStage, QueryPool queryPool, uint32_t query)
{
	vkCmdWriteTimestamp(commandBuffer, static_cast<VkPipelineStageFlagBits>(pipelineStage), queryPool, query);
}
void cmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags)
{
	vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, static_cast<VkQueryResultFlags>(flags));
}
void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
	vkCmdPushConstants(commandBuffer, layout, static_cast<VkShaderStageFlags>(stageFlags), offset, size, reinterpret_cast<const void*>(pValues));
}
void cmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents)
{
	vkCmdBeginRenderPass(commandBuffer, reinterpret_cast<const VkRenderPassBeginInfo*>(pRenderPassBegin), static_cast<VkSubpassContents>(contents));
}
void cmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents)
{
	vkCmdNextSubpass(commandBuffer, static_cast<VkSubpassContents>(contents));
}
void cmdEndRenderPass(CommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}
void cmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers)
{
	vkCmdExecuteCommands(commandBuffer, commandBufferCount, reinterpret_cast<const VkCommandBuffer*>(pCommandBuffers));
}

enum class SurfaceTransformFlagBitsKHR : std::int32_t
{
	identityBit = 0,
	rotate90Bit = 1,
	rotate180Bit = 2,
	rotate270Bit = 3,
	horizontalMirrorBit = 4,
	horizontalMirrorRotate90Bit = 5,
	horizontalMirrorRotate180Bit = 6,
	horizontalMirrorRotate270Bit = 7,
	inheritBit = 8,
};

enum class CompositeAlphaFlagBitsKHR : std::int32_t
{
	opaqueBit = 0,
	preMultipliedBit = 1,
	postMultipliedBit = 2,
	inheritBit = 3,
};

enum class ColorSpaceKHR : std::int32_t
{
	srgbNonlinear = 0,
};

enum class PresentModeKHR : std::int32_t
{
	immediate = 0,
	mailbox = 1,
	fifo = 2,
	fifoRelaxed = 3,
};


struct SurfaceCapabilitiesKHR
{
	uint32_t minImageCount {};
	uint32_t maxImageCount {};
	Extent2D currentExtent {};
	Extent2D minImageExtent {};
	Extent2D maxImageExtent {};
	uint32_t maxImageArrayLayers {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	SurfaceTransformFlagBitsKHR currentTransform {};
	CompositeAlphaFlagsKHR supportedCompositeAlpha {};
	ImageUsageFlags supportedUsageFlags {};

	operator const VkSurfaceCapabilitiesKHR&() const { return reinterpret_cast<const VkSurfaceCapabilitiesKHR&>(*this); }
};
struct SurfaceFormatKHR
{
	Format format {};
	ColorSpaceKHR colorSpace {};

	operator const VkSurfaceFormatKHR&() const { return reinterpret_cast<const VkSurfaceFormatKHR&>(*this); }
};

void destroySurfaceKHR(Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator)
{
	vkDestroySurfaceKHR(instance, surface, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getPhysicalDeviceSurfaceSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported)
{
	return static_cast<Result>(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, reinterpret_cast<VkBool32*>(pSupported)));
}
Result getPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
	return static_cast<Result>(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, reinterpret_cast<VkSurfaceCapabilitiesKHR*>(pSurfaceCapabilities)));
}
Result getPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats)
{
	return static_cast<Result>(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, reinterpret_cast<uint32_t*>(pSurfaceFormatCount), reinterpret_cast<VkSurfaceFormatKHR*>(pSurfaceFormats)));
}
Result getPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes)
{
	return static_cast<Result>(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, reinterpret_cast<uint32_t*>(pPresentModeCount), reinterpret_cast<VkPresentModeKHR*>(pPresentModes)));
}

struct SwapchainCreateInfoKHR
{
	StructureType sType {StructureType::swapchainCreateInfoKHR};
	const void* pNext {};
	SwapchainCreateFlagsKHR flags {};
	SurfaceKHR surface {};
	uint32_t minImageCount {};
	Format imageFormat {};
	ColorSpaceKHR imageColorSpace {};
	Extent2D imageExtent {};
	uint32_t imageArrayLayers {};
	ImageUsageFlags imageUsage {};
	SharingMode imageSharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};
	SurfaceTransformFlagBitsKHR preTransform {};
	CompositeAlphaFlagBitsKHR compositeAlpha {};
	PresentModeKHR presentMode {};
	Bool32 clipped {};
	SwapchainKHR oldSwapchain {};

	operator const VkSwapchainCreateInfoKHR&() const { return reinterpret_cast<const VkSwapchainCreateInfoKHR&>(*this); }
};
struct PresentInfoKHR
{
	StructureType sType {StructureType::presentInfoKHR};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	uint32_t swapchainCount {};
	const SwapchainKHR* pSwapchains {};
	const uint32_t* pImageIndices {};
	Result* pResults {};

	operator const VkPresentInfoKHR&() const { return reinterpret_cast<const VkPresentInfoKHR&>(*this); }
};

Result createSwapchainKHR(Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain)
{
	return static_cast<Result>(vkCreateSwapchainKHR(device, reinterpret_cast<const VkSwapchainCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(pSwapchain)));
}
void destroySwapchainKHR(Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator)
{
	vkDestroySwapchainKHR(device, swapchain, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
Result getSwapchainImagesKHR(Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages)
{
	return static_cast<Result>(vkGetSwapchainImagesKHR(device, swapchain, reinterpret_cast<uint32_t*>(pSwapchainImageCount), reinterpret_cast<VkImage*>(pSwapchainImages)));
}
Result acquireNextImageKHR(Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex)
{
	return static_cast<Result>(vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, reinterpret_cast<uint32_t*>(pImageIndex)));
}
Result queuePresentKHR(Queue queue, const PresentInfoKHR* pPresentInfo)
{
	return static_cast<Result>(vkQueuePresentKHR(queue, reinterpret_cast<const VkPresentInfoKHR*>(pPresentInfo)));
}

enum class DisplayPlaneAlphaFlagBitsKHR : std::int32_t
{
	opaqueBit = 0,
	globalBit = 1,
	perPixelBit = 2,
	perPixelPremultipliedBit = 3,
};


struct DisplayPropertiesKHR
{
	DisplayKHR display {};
	const char* displayName {};
	Extent2D physicalDimensions {};
	Extent2D physicalResolution {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	Bool32 planeReorderPossible {};
	Bool32 persistentContent {};

	operator const VkDisplayPropertiesKHR&() const { return reinterpret_cast<const VkDisplayPropertiesKHR&>(*this); }
};
struct DisplayModeParametersKHR
{
	Extent2D visibleRegion {};
	uint32_t refreshRate {};

	operator const VkDisplayModeParametersKHR&() const { return reinterpret_cast<const VkDisplayModeParametersKHR&>(*this); }
};
struct DisplayModePropertiesKHR
{
	DisplayModeKHR displayMode {};
	DisplayModeParametersKHR parameters {};

	operator const VkDisplayModePropertiesKHR&() const { return reinterpret_cast<const VkDisplayModePropertiesKHR&>(*this); }
};
struct DisplayModeCreateInfoKHR
{
	StructureType sType {StructureType::displayModeCreateInfoKHR};
	const void* pNext {};
	DisplayModeCreateFlagsKHR flags {};
	DisplayModeParametersKHR parameters {};

	operator const VkDisplayModeCreateInfoKHR&() const { return reinterpret_cast<const VkDisplayModeCreateInfoKHR&>(*this); }
};
struct DisplayPlaneCapabilitiesKHR
{
	DisplayPlaneAlphaFlagsKHR supportedAlpha {};
	Offset2D minSrcPosition {};
	Offset2D maxSrcPosition {};
	Extent2D minSrcExtent {};
	Extent2D maxSrcExtent {};
	Offset2D minDstPosition {};
	Offset2D maxDstPosition {};
	Extent2D minDstExtent {};
	Extent2D maxDstExtent {};

	operator const VkDisplayPlaneCapabilitiesKHR&() const { return reinterpret_cast<const VkDisplayPlaneCapabilitiesKHR&>(*this); }
};
struct DisplayPlanePropertiesKHR
{
	DisplayKHR currentDisplay {};
	uint32_t currentStackIndex {};

	operator const VkDisplayPlanePropertiesKHR&() const { return reinterpret_cast<const VkDisplayPlanePropertiesKHR&>(*this); }
};
struct DisplaySurfaceCreateInfoKHR
{
	StructureType sType {StructureType::displaySurfaceCreateInfoKHR};
	const void* pNext {};
	DisplaySurfaceCreateFlagsKHR flags {};
	DisplayModeKHR displayMode {};
	uint32_t planeIndex {};
	uint32_t planeStackIndex {};
	SurfaceTransformFlagBitsKHR transform {};
	float globalAlpha {};
	DisplayPlaneAlphaFlagBitsKHR alphaMode {};
	Extent2D imageExtent {};

	operator const VkDisplaySurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR&>(*this); }
};

Result getPhysicalDeviceDisplayPropertiesKHR(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties)
{
	return static_cast<Result>(vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkDisplayPropertiesKHR*>(pProperties)));
}
Result getPhysicalDeviceDisplayPlanePropertiesKHR(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties)
{
	return static_cast<Result>(vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkDisplayPlanePropertiesKHR*>(pProperties)));
}
Result getDisplayPlaneSupportedDisplaysKHR(PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays)
{
	return static_cast<Result>(vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, reinterpret_cast<uint32_t*>(pDisplayCount), reinterpret_cast<VkDisplayKHR*>(pDisplays)));
}
Result getDisplayModePropertiesKHR(PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties)
{
	return static_cast<Result>(vkGetDisplayModePropertiesKHR(physicalDevice, display, reinterpret_cast<uint32_t*>(pPropertyCount), reinterpret_cast<VkDisplayModePropertiesKHR*>(pProperties)));
}
Result createDisplayModeKHR(PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode)
{
	return static_cast<Result>(vkCreateDisplayModeKHR(physicalDevice, display, reinterpret_cast<const VkDisplayModeCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDisplayModeKHR*>(pMode)));
}
Result getDisplayPlaneCapabilitiesKHR(PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities)
{
	return static_cast<Result>(vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, reinterpret_cast<VkDisplayPlaneCapabilitiesKHR*>(pCapabilities)));
}
Result createDisplayPlaneSurfaceKHR(Instance instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateDisplayPlaneSurfaceKHR(instance, reinterpret_cast<const VkDisplaySurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}

struct DisplayPresentInfoKHR
{
	StructureType sType {StructureType::displayPresentInfoKHR};
	const void* pNext {};
	Rect2D srcRect {};
	Rect2D dstRect {};
	Bool32 persistent {};

	operator const VkDisplayPresentInfoKHR&() const { return reinterpret_cast<const VkDisplayPresentInfoKHR&>(*this); }
};

Result createSharedSwapchainsKHR(Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains)
{
	return static_cast<Result>(vkCreateSharedSwapchainsKHR(device, swapchainCount, reinterpret_cast<const VkSwapchainCreateInfoKHR*>(pCreateInfos), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSwapchainKHR*>(pSwapchains)));
}

#ifdef VK_USE_PLATFORM_XLIB_KHR

struct XlibSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::xlibSurfaceCreateInfoKHR};
	const void* pNext {};
	XlibSurfaceCreateFlagsKHR flags {};
	Display* dpy {};
	Window window {};

	operator const VkXlibSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkXlibSurfaceCreateInfoKHR&>(*this); }
};

Result createXlibSurfaceKHR(Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateXlibSurfaceKHR(instance, reinterpret_cast<const VkXlibSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}
Bool32 getPhysicalDeviceXlibPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<Display*>(dpy), visualID));
}

#endif //VK_USE_PLATFORM_XLIB_KHR

#ifdef VK_USE_PLATFORM_XCB_KHR

struct XcbSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::xcbSurfaceCreateInfoKHR};
	const void* pNext {};
	XcbSurfaceCreateFlagsKHR flags {};
	xcb_connection_t* connection {};
	xcb_window_t window {};

	operator const VkXcbSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkXcbSurfaceCreateInfoKHR&>(*this); }
};

Result createXcbSurfaceKHR(Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateXcbSurfaceKHR(instance, reinterpret_cast<const VkXcbSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}
Bool32 getPhysicalDeviceXcbPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<xcb_connection_t*>(connection), visual_id));
}

#endif //VK_USE_PLATFORM_XCB_KHR

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

struct WaylandSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::waylandSurfaceCreateInfoKHR};
	const void* pNext {};
	WaylandSurfaceCreateFlagsKHR flags {};
	wl_display* display {};
	wl_surface* surface {};

	operator const VkWaylandSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR&>(*this); }
};

Result createWaylandSurfaceKHR(Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateWaylandSurfaceKHR(instance, reinterpret_cast<const VkWaylandSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}
Bool32 getPhysicalDeviceWaylandPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<wl_display*>(display)));
}

#endif //VK_USE_PLATFORM_WAYLAND_KHR

#ifdef VK_USE_PLATFORM_MIR_KHR

struct MirSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::mirSurfaceCreateInfoKHR};
	const void* pNext {};
	MirSurfaceCreateFlagsKHR flags {};
	MirConnection* connection {};
	MirSurface* mirSurface {};

	operator const VkMirSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkMirSurfaceCreateInfoKHR&>(*this); }
};

Result createMirSurfaceKHR(Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateMirSurfaceKHR(instance, reinterpret_cast<const VkMirSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}
Bool32 getPhysicalDeviceMirPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceMirPresentationSupportKHR(physicalDevice, queueFamilyIndex, reinterpret_cast<MirConnection*>(connection)));
}

#endif //VK_USE_PLATFORM_MIR_KHR

#ifdef VK_USE_PLATFORM_ANDROID_KHR

struct AndroidSurfaceCreateInfoKHR
{
	StructureType sType {StructureType::androidSurfaceCreateInfoKHR};
	const void* pNext {};
	AndroidSurfaceCreateFlagsKHR flags {};
	ANativeWindow* window {};

	operator const VkAndroidSurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR&>(*this); }
};

Result createAndroidSurfaceKHR(Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateAndroidSurfaceKHR(instance, reinterpret_cast<const VkAndroidSurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}

#endif //VK_USE_PLATFORM_ANDROID_KHR

#ifdef VK_USE_PLATFORM_WIN32_KHR

struct Win32SurfaceCreateInfoKHR
{
	StructureType sType {StructureType::win32SurfaceCreateInfoKHR};
	const void* pNext {};
	Win32SurfaceCreateFlagsKHR flags {};
	HINSTANCE hinstance {};
	HWND hwnd {};

	operator const VkWin32SurfaceCreateInfoKHR&() const { return reinterpret_cast<const VkWin32SurfaceCreateInfoKHR&>(*this); }
};

Result createWin32SurfaceKHR(Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	return static_cast<Result>(vkCreateWin32SurfaceKHR(instance, reinterpret_cast<const VkWin32SurfaceCreateInfoKHR*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkSurfaceKHR*>(pSurface)));
}
Bool32 getPhysicalDeviceWin32PresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
	return static_cast<Bool32>(vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex));
}

#endif //VK_USE_PLATFORM_WIN32_KHR

enum class DebugReportObjectTypeEXT : std::int32_t
{
	unknown = 0,
	instance = 1,
	physicalDevice = 2,
	device = 3,
	queue = 4,
	semaphore = 5,
	commandBuffer = 6,
	fence = 7,
	deviceMemory = 8,
	buffer = 9,
	image = 10,
	event = 11,
	queryPool = 12,
	bufferView = 13,
	imageView = 14,
	shaderModule = 15,
	pipelineCache = 16,
	pipelineLayout = 17,
	renderPass = 18,
	pipeline = 19,
	descriptorSetLayout = 20,
	sampler = 21,
	descriptorPool = 22,
	descriptorSet = 23,
	framebuffer = 24,
	commandPool = 25,
	surfaceKHR = 26,
	swapchainKHR = 27,
	debugReport = 28,
};

enum class DebugReportErrorEXT : std::int32_t
{
	none = 0,
	callbackRef = 1,
};

enum class DebugReportFlagBitsEXT : std::int32_t
{
	informationBit = 0,
	warningBit = 1,
	performanceWarningBit = 2,
	errorBit = 3,
	debugBit = 4,
};


struct DebugReportCallbackCreateInfoEXT
{
	StructureType sType {StructureType::debugReportCallbackCreateInfoEXT};
	const void* pNext {};
	DebugReportFlagsEXT flags {};
	PFN_vkDebugReportCallbackEXT pfnCallback {};
	void* pUserData {};

	operator const VkDebugReportCallbackCreateInfoEXT&() const { return reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT&>(*this); }
};

Result createDebugReportCallbackEXT(Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback)
{
	return static_cast<Result>(vkCreateDebugReportCallbackEXT(instance, reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugReportCallbackEXT*>(pCallback)));
}
void destroyDebugReportCallbackEXT(Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator)
{
	vkDestroyDebugReportCallbackEXT(instance, callback, reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
}
void debugReportMessageEXT(Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
	vkDebugReportMessageEXT(instance, static_cast<VkDebugReportFlagsEXT>(flags), static_cast<VkDebugReportObjectTypeEXT>(objectType), object, location, messageCode, reinterpret_cast<const char*>(pLayerPrefix), reinterpret_cast<const char*>(pMessage));
}

enum class RasterizationOrderAMD : std::int32_t
{
	strict = 0,
	relaxed = 1,
};


struct PipelineRasterizationStateRasterizationOrderAMD
{
	StructureType sType {StructureType::pipelineRasterizationStateRasterizationOrderAMD};
	const void* pNext {};
	RasterizationOrderAMD rasterizationOrder {};

	operator const VkPipelineRasterizationStateRasterizationOrderAMD&() const { return reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD&>(*this); }
};

struct DebugMarkerObjectNameInfoEXT
{
	StructureType sType {StructureType::debugMarkerObjectNameInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	const char* pObjectName {};

	operator const VkDebugMarkerObjectNameInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerObjectNameInfoEXT&>(*this); }
};
struct DebugMarkerObjectTagInfoEXT
{
	StructureType sType {StructureType::debugMarkerObjectTagInfoEXT};
	const void* pNext {};
	DebugReportObjectTypeEXT objectType {};
	uint64_t object {};
	uint64_t tagName {};
	size_t tagSize {};
	const void* pTag {};

	operator const VkDebugMarkerObjectTagInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerObjectTagInfoEXT&>(*this); }
};
struct DebugMarkerMarkerInfoEXT
{
	StructureType sType {StructureType::debugMarkerMarkerInfoEXT};
	const void* pNext {};
	const char* pMarkerName {};
	float color[4] {};

	operator const VkDebugMarkerMarkerInfoEXT&() const { return reinterpret_cast<const VkDebugMarkerMarkerInfoEXT&>(*this); }
};

Result debugMarkerSetObjectTagEXT(Device device, DebugMarkerObjectTagInfoEXT* pTagInfo)
{
	return static_cast<Result>(vkDebugMarkerSetObjectTagEXT(device, reinterpret_cast<VkDebugMarkerObjectTagInfoEXT*>(pTagInfo)));
}
Result debugMarkerSetObjectNameEXT(Device device, DebugMarkerObjectNameInfoEXT* pNameInfo)
{
	return static_cast<Result>(vkDebugMarkerSetObjectNameEXT(device, reinterpret_cast<VkDebugMarkerObjectNameInfoEXT*>(pNameInfo)));
}
void cmdDebugMarkerBeginEXT(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo)
{
	vkCmdDebugMarkerBeginEXT(commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>(pMarkerInfo));
}
void cmdDebugMarkerEndEXT(CommandBuffer commandBuffer)
{
	vkCmdDebugMarkerEndEXT(commandBuffer);
}
void cmdDebugMarkerInsertEXT(CommandBuffer commandBuffer, DebugMarkerMarkerInfoEXT* pMarkerInfo)
{
	vkCmdDebugMarkerInsertEXT(commandBuffer, reinterpret_cast<VkDebugMarkerMarkerInfoEXT*>(pMarkerInfo));
}


}