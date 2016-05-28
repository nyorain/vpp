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

// The specification (vk.xml) itself is protected by the following license:

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
    
// Automaitcally generated vulkan header file for the nyorain/vpp library.
// Do not edit manually, rather edit the codegen files.

#pragma once

#include "fwd.hpp"

namespace vk
{

enum class PipelineCacheHeaderVersion : std::int32_t
{
	one = 1
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
	debugMarkerMarkerInfoEXT = 1000022002
};

enum class SystemAllocationScope : std::int32_t
{
	command = 0,
	object = 1,
	cache = 2,
	device = 3,
	instance = 4
};

enum class InternalAllocationType : std::int32_t
{
	executable = 0
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
	nvExtension1Error = -1000013000
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
	astc12X12SrgbBlock = 184
};

enum class FormatFeatureBits : std::int32_t
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
	sampledImageFilterCubicBitIMG = 1000015000
};
VPP_BITMASK_OPS(FormatFeatureBits)

enum class ImageType : std::int32_t
{
	e1D = 0,
	e2D = 1,
	e3D = 2
};

enum class ImageTiling : std::int32_t
{
	optimal = 0,
	linear = 1
};

enum class ImageUsageBits : std::int32_t
{
	transferSrc = (1 << 0),
	transferDst = (1 << 1),
	sampled = (1 << 2),
	storage = (1 << 3),
	colorAttachment = (1 << 4),
	depthStencilAttachment = (1 << 5),
	transientAttachment = (1 << 6),
	inputAttachment = (1 << 7)
};
VPP_BITMASK_OPS(ImageUsageBits)

enum class ImageCreateBits : std::int32_t
{
	sparseBinding = (1 << 0),
	sparseResidency = (1 << 1),
	sparseAliased = (1 << 2),
	mutableFormat = (1 << 3),
	cubeCompatible = (1 << 4)
};
VPP_BITMASK_OPS(ImageCreateBits)

enum class SampleCountBits : std::int32_t
{
	e1 = (1 << 0),
	e2 = (1 << 1),
	e4 = (1 << 2),
	e8 = (1 << 3),
	e16 = (1 << 4),
	e32 = (1 << 5),
	e64 = (1 << 6)
};
VPP_BITMASK_OPS(SampleCountBits)

enum class PhysicalDeviceType : std::int32_t
{
	other = 0,
	integratedGpu = 1,
	discreteGpu = 2,
	virtualGpu = 3,
	cpu = 4
};

enum class QueueBits : std::int32_t
{
	graphics = (1 << 0),
	compute = (1 << 1),
	transfer = (1 << 2),
	sparseBinding = (1 << 3)
};
VPP_BITMASK_OPS(QueueBits)

enum class MemoryPropertyBits : std::int32_t
{
	deviceLocal = (1 << 0),
	hostVisible = (1 << 1),
	hostCoherent = (1 << 2),
	hostCached = (1 << 3),
	lazilyAllocated = (1 << 4)
};
VPP_BITMASK_OPS(MemoryPropertyBits)

enum class MemoryHeapBits : std::int32_t
{
	deviceLocal = (1 << 0)
};
VPP_BITMASK_OPS(MemoryHeapBits)

enum class PipelineStageBits : std::int32_t
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
	allCommands = (1 << 16)
};
VPP_BITMASK_OPS(PipelineStageBits)

enum class ImageAspectBits : std::int32_t
{
	color = (1 << 0),
	depth = (1 << 1),
	stencil = (1 << 2),
	metadata = (1 << 3)
};
VPP_BITMASK_OPS(ImageAspectBits)

enum class SparseImageFormatBits : std::int32_t
{
	singleMiptail = (1 << 0),
	alignedMipSize = (1 << 1),
	nonstandardBlockSize = (1 << 2)
};
VPP_BITMASK_OPS(SparseImageFormatBits)

enum class SparseMemoryBindBits : std::int32_t
{
	metadata = (1 << 0)
};
VPP_BITMASK_OPS(SparseMemoryBindBits)

enum class FenceCreateBits : std::int32_t
{
	signaled = (1 << 0)
};
VPP_BITMASK_OPS(FenceCreateBits)

enum class QueryType : std::int32_t
{
	occlusion = 0,
	pipelineStatistics = 1,
	timestamp = 2
};

enum class QueryPipelineStatisticBits : std::int32_t
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
	computeShaderInvocations = (1 << 10)
};
VPP_BITMASK_OPS(QueryPipelineStatisticBits)

enum class QueryResultBits : std::int32_t
{
	e64 = (1 << 0),
	wait = (1 << 1),
	withAvailability = (1 << 2),
	partial = (1 << 3)
};
VPP_BITMASK_OPS(QueryResultBits)

enum class BufferCreateBits : std::int32_t
{
	sparseBinding = (1 << 0),
	sparseResidency = (1 << 1),
	sparseAliased = (1 << 2)
};
VPP_BITMASK_OPS(BufferCreateBits)

enum class BufferUsageBits : std::int32_t
{
	transferSrc = (1 << 0),
	transferDst = (1 << 1),
	uniformTexelBuffer = (1 << 2),
	storageTexelBuffer = (1 << 3),
	uniformBuffer = (1 << 4),
	storageBuffer = (1 << 5),
	indexBuffer = (1 << 6),
	vertexBuffer = (1 << 7),
	indirectBuffer = (1 << 8)
};
VPP_BITMASK_OPS(BufferUsageBits)

enum class SharingMode : std::int32_t
{
	exclusive = 0,
	concurrent = 1
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
	presentSrcKHR = 1000001002
};

enum class ImageViewType : std::int32_t
{
	e1D = 0,
	e2D = 1,
	e3D = 2,
	cube = 3,
	e1DArray = 4,
	e2DArray = 5,
	cubeArray = 6
};

enum class ComponentSwizzle : std::int32_t
{
	identity = 0,
	zero = 1,
	one = 2,
	r = 3,
	g = 4,
	b = 5,
	a = 6
};

enum class PipelineCreateBits : std::int32_t
{
	disableOptimization = (1 << 0),
	allowDerivatives = (1 << 1),
	derivative = (1 << 2)
};
VPP_BITMASK_OPS(PipelineCreateBits)

enum class ShaderStageBits : std::int32_t
{
	vertex = (1 << 0),
	tessellationControl = (1 << 1),
	tessellationEvaluation = (1 << 2),
	geometry = (1 << 3),
	fragment = (1 << 4),
	compute = (1 << 5),
	allGraphics = 31,
	all = 2147483647
};
VPP_BITMASK_OPS(ShaderStageBits)

enum class VertexInputRate : std::int32_t
{
	vertex = 0,
	instance = 1
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
	patchList = 10
};

enum class PolygonMode : std::int32_t
{
	fill = 0,
	line = 1,
	point = 2
};

enum class CullModeBits : std::int32_t
{
	none = 0,
	front = (1 << 0),
	back = (1 << 1),
	frontAndBack = 3
};
VPP_BITMASK_OPS(CullModeBits)

enum class FrontFace : std::int32_t
{
	counterClockwise = 0,
	clockwise = 1
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
	always = 7
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
	decrementAndWrap = 7
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
	set = 15
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
	oneMinusSrc1Alpha = 18
};

enum class BlendOp : std::int32_t
{
	add = 0,
	subtract = 1,
	reverseSubtract = 2,
	min = 3,
	max = 4
};

enum class ColorComponentBits : std::int32_t
{
	r = (1 << 0),
	g = (1 << 1),
	b = (1 << 2),
	a = (1 << 3)
};
VPP_BITMASK_OPS(ColorComponentBits)

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
	stencilReference = 8
};

enum class Filter : std::int32_t
{
	nearest = 0,
	linear = 1,
	cubicIMG = 1000015000
};

enum class SamplerMipmapMode : std::int32_t
{
	nearest = 0,
	linear = 1
};

enum class SamplerAddressMode : std::int32_t
{
	repeat = 0,
	mirroredRepeat = 1,
	clampToEdge = 2,
	clampToBorder = 3,
	mirrorClampToEdge = 1000014000
};

enum class BorderColor : std::int32_t
{
	floatTransparentBlack = 0,
	intTransparentBlack = 1,
	floatOpaqueBlack = 2,
	intOpaqueBlack = 3,
	floatOpaqueWhite = 4,
	intOpaqueWhite = 5
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
	inputAttachment = 10
};

enum class DescriptorPoolCreateBits : std::int32_t
{
	freeDescriptorSet = (1 << 0)
};
VPP_BITMASK_OPS(DescriptorPoolCreateBits)

enum class AttachmentDescriptionBits : std::int32_t
{
	mayAlias = (1 << 0)
};
VPP_BITMASK_OPS(AttachmentDescriptionBits)

enum class AttachmentLoadOp : std::int32_t
{
	load = 0,
	clear = 1,
	dontCare = 2
};

enum class AttachmentStoreOp : std::int32_t
{
	store = 0,
	dontCare = 1
};

enum class PipelineBindPoint : std::int32_t
{
	graphics = 0,
	compute = 1
};

enum class AccessBits : std::int32_t
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
	memoryWrite = (1 << 16)
};
VPP_BITMASK_OPS(AccessBits)

enum class DependencyBits : std::int32_t
{
	byRegion = (1 << 0)
};
VPP_BITMASK_OPS(DependencyBits)

enum class CommandPoolCreateBits : std::int32_t
{
	transient = (1 << 0),
	resetCommandBuffer = (1 << 1)
};
VPP_BITMASK_OPS(CommandPoolCreateBits)

enum class CommandPoolResetBits : std::int32_t
{
	releaseResources = (1 << 0)
};
VPP_BITMASK_OPS(CommandPoolResetBits)

enum class CommandBufferLevel : std::int32_t
{
	primary = 0,
	secondary = 1
};

enum class CommandBufferUsageBits : std::int32_t
{
	oneTimeSubmit = (1 << 0),
	renderPassContinue = (1 << 1),
	simultaneousUse = (1 << 2)
};
VPP_BITMASK_OPS(CommandBufferUsageBits)

enum class QueryControlBits : std::int32_t
{
	precise = (1 << 0)
};
VPP_BITMASK_OPS(QueryControlBits)

enum class CommandBufferResetBits : std::int32_t
{
	releaseResources = (1 << 0)
};
VPP_BITMASK_OPS(CommandBufferResetBits)

enum class StencilFaceBits : std::int32_t
{
	front = (1 << 0),
	back = (1 << 1),
	rontAndBack = 3
};
VPP_BITMASK_OPS(StencilFaceBits)

enum class IndexType : std::int32_t
{
	uint16 = 0,
	uint32 = 1
};

enum class SubpassContents : std::int32_t
{
	Einline = 0,
	secondaryCommandBuffers = 1
};


enum class SurfaceTransformBitsKHR : std::int32_t
{
	identity = (1 << 0),
	rotate90 = (1 << 1),
	rotate180 = (1 << 2),
	rotate270 = (1 << 3),
	horizontalMirror = (1 << 4),
	horizontalMirrorRotate90 = (1 << 5),
	horizontalMirrorRotate180 = (1 << 6),
	horizontalMirrorRotate270 = (1 << 7),
	inherit = (1 << 8)
};
VPP_BITMASK_OPS(SurfaceTransformBitsKHR)

enum class CompositeAlphaBitsKHR : std::int32_t
{
	opaque = (1 << 0),
	preMultiplied = (1 << 1),
	postMultiplied = (1 << 2),
	inherit = (1 << 3)
};
VPP_BITMASK_OPS(CompositeAlphaBitsKHR)

enum class ColorSpaceKHR : std::int32_t
{
	srgbNonlinear = 0
};

enum class PresentModeKHR : std::int32_t
{
	immediate = 0,
	mailbox = 1,
	fifo = 2,
	fifoRelaxed = 3
};


enum class DisplayPlaneAlphaBitsKHR : std::int32_t
{
	opaque = (1 << 0),
	global = (1 << 1),
	perPixel = (1 << 2),
	perPixelPremultiplied = (1 << 3)
};
VPP_BITMASK_OPS(DisplayPlaneAlphaBitsKHR)


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
	debugReport = 28
};

enum class DebugReportErrorEXT : std::int32_t
{
	none = 0,
	callbackRef = 1
};

enum class DebugReportBitsEXT : std::int32_t
{
	information = (1 << 0),
	warning = (1 << 1),
	performanceWarning = (1 << 2),
	error = (1 << 3),
	debug = (1 << 4)
};
VPP_BITMASK_OPS(DebugReportBitsEXT)


enum class RasterizationOrderAMD : std::int32_t
{
	strict = 0,
	relaxed = 1
};




} //namespace vk