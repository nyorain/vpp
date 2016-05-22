#pragma once

#include "fwd.hpp"

namespace vk
{





enum class AttachmentLoadOp : std::int32_t
{
	load = 0,
	clear = 1,
	EdontCare = 2,
};

enum class AttachmentStoreOp : std::int32_t
{
	store = 0,
	EdontCare = 1,
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
	EconstantColor = 10,
	oneMinusConstantColor = 11,
	EconstantAlpha = 12,
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

enum class BorderColor : std::int32_t
{
	EfloatTransparentBlack = 0,
	EintTransparentBlack = 1,
	EfloatOpaqueBlack = 2,
	EintOpaqueBlack = 3,
	EfloatOpaqueWhite = 4,
	EintOpaqueWhite = 5,
};

enum class FramebufferCreateFlagBits : std::int32_t
{
};

enum class QueryPoolCreateFlagBits : std::int32_t
{
};

enum class RenderPassCreateFlagBits : std::int32_t
{
};

enum class SamplerCreateFlagBits : std::int32_t
{
};

enum class PipelineCacheHeaderVersion : std::int32_t
{
	one = 1,
};

enum class PipelineLayoutCreateFlagBits : std::int32_t
{
};

enum class PipelineCacheCreateFlagBits : std::int32_t
{
};

enum class PipelineDepthStencilStateCreateFlagBits : std::int32_t
{
};

enum class PipelineDynamicStateCreateFlagBits : std::int32_t
{
};

enum class PipelineColorBlendStateCreateFlagBits : std::int32_t
{
};

enum class PipelineMultisampleStateCreateFlagBits : std::int32_t
{
};

enum class PipelineRasterizationStateCreateFlagBits : std::int32_t
{
};

enum class PipelineViewportStateCreateFlagBits : std::int32_t
{
};

enum class PipelineTessellationStateCreateFlagBits : std::int32_t
{
};

enum class PipelineInputAssemblyStateCreateFlagBits : std::int32_t
{
};

enum class PipelineVertexInputStateCreateFlagBits : std::int32_t
{
};

enum class PipelineShaderStageCreateFlagBits : std::int32_t
{
};

enum class DescriptorSetLayoutCreateFlagBits : std::int32_t
{
};

enum class BufferViewCreateFlagBits : std::int32_t
{
};

enum class InstanceCreateFlagBits : std::int32_t
{
};

enum class DeviceQueueCreateFlagBits : std::int32_t
{
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

enum class ColorComponentFlagBits : std::int32_t
{
	r = (1 << 0),
	g = (1 << 1),
	b = (1 << 2),
	a = (1 << 3),
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

enum class CommandPoolCreateFlagBits : std::int32_t
{
	transient = (1 << 0),
	resetCommandBuffer = (1 << 1),
};

enum class CommandPoolResetFlagBits : std::int32_t
{
	releaseResources = (1 << 0),
};

enum class CommandBufferResetFlagBits : std::int32_t
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

enum class CompareOp : std::int32_t
{
	never = 0,
	less = 1,
	equal = 2,
	lessOrEqual = 3,
	greater = 4,
	EnotEqual = 5,
	greaterOrEqual = 6,
	always = 7,
};

enum class CullModeFlagBits : std::int32_t
{
	none = 0,
	front = (1 << 0),
	back = (1 << 1),
	frontAndBack = 3,
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

enum class DeviceCreateFlagBits : std::int32_t
{
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

enum class FenceCreateFlagBits : std::int32_t
{
	signaled = (1 << 0),
};

enum class PolygonMode : std::int32_t
{
	fill = 0,
	line = 1,
	point = 2,
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
};

enum class FrontFace : std::int32_t
{
	counterClockwise = 0,
	clockwise = 1,
};

enum class ImageAspectFlagBits : std::int32_t
{
	color = (1 << 0),
	depth = (1 << 1),
	stencil = (1 << 2),
	metadata = (1 << 3),
};

enum class ImageCreateFlagBits : std::int32_t
{
	sparseBinding = (1 << 0),
	sparseResidency = (1 << 1),
	sparseAliased = (1 << 2),
	EmutableFormat = (1 << 3),
	cubeCompatible = (1 << 4),
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
};

enum class ImageTiling : std::int32_t
{
	optimal = 0,
	linear = 1,
};

enum class ImageType : std::int32_t
{
	e1D = 0,
	e2D = 1,
	e3D = 2,
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

enum class SharingMode : std::int32_t
{
	exclusive = 0,
	concurrent = 1,
};

enum class IndexType : std::int32_t
{
	uint16 = 0,
	uint32 = 1,
};

enum class LogicOp : std::int32_t
{
	clear = 0,
	Eand = 1,
	EandReverse = 2,
	copy = 3,
	EandInverted = 4,
	noOp = 5,
	Exor = 6,
	Eor = 7,
	nor = 8,
	equivalent = 9,
	invert = 10,
	EorReverse = 11,
	copyInverted = 12,
	EorInverted = 13,
	nand = 14,
	set = 15,
};

enum class MemoryHeapFlagBits : std::int32_t
{
	deviceLocal = (1 << 0),
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

enum class MemoryPropertyFlagBits : std::int32_t
{
	deviceLocal = (1 << 0),
	hostVisible = (1 << 1),
	hostCoherent = (1 << 2),
	hostCached = (1 << 3),
	lazilyAllocated = (1 << 4),
};

enum class PhysicalDeviceType : std::int32_t
{
	other = 0,
	EintegratedGpu = 1,
	discreteGpu = 2,
	EvirtualGpu = 3,
	cpu = 4,
};

enum class PipelineBindPoint : std::int32_t
{
	graphics = 0,
	compute = 1,
};

enum class PipelineCreateFlagBits : std::int32_t
{
	disableOptimization = (1 << 0),
	allowDerivatives = (1 << 1),
	derivative = (1 << 2),
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

enum class QueryControlFlagBits : std::int32_t
{
	precise = (1 << 0),
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

enum class QueryType : std::int32_t
{
	occlusion = 0,
	pipelineStatistics = 1,
	timestamp = 2,
};

enum class QueueFlagBits : std::int32_t
{
	graphics = (1 << 0),
	compute = (1 << 1),
	transfer = (1 << 2),
	sparseBinding = (1 << 3),
};

enum class SubpassContents : std::int32_t
{
	Einline = 0,
	secondaryCommandBuffers = 1,
};

enum class Result : std::int32_t
{
	success = 0,
	EnotReady = 1,
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

enum class SparseMemoryBindFlagBits : std::int32_t
{
	metadata = (1 << 0),
};

enum class StencilFaceFlagBits : std::int32_t
{
	front = (1 << 0),
	back = (1 << 1),
	tAndBack = 3,
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

enum class SamplerAddressMode : std::int32_t
{
	repeat = 0,
	mirroredRepeat = 1,
	clampToEdge = 2,
	clampToBorder = 3,
	mirrorClampToEdge = 4,
};

enum class Filter : std::int32_t
{
	nearest = 0,
	linear = 1,
};

enum class SamplerMipmapMode : std::int32_t
{
	nearest = 0,
	linear = 1,
};

enum class VertexInputRate : std::int32_t
{
	vertex = 0,
	instance = 1,
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

enum class SparseImageFormatFlagBits : std::int32_t
{
	singleMiptail = (1 << 0),
	alignedMipSize = (1 << 1),
	nonstandardBlockSize = (1 << 2),
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

enum class AttachmentDescriptionFlagBits : std::int32_t
{
	mayAlias = (1 << 0),
};

enum class DescriptorPoolCreateFlagBits : std::int32_t
{
	freeDescriptorSet = (1 << 0),
};

enum class DependencyFlagBits : std::int32_t
{
	byRegion = (1 << 0),
};

enum class ColorSpaceKHR : std::int32_t
{
	srgbNonlinear = 0,
};

enum class CompositeAlphaFlagBitsKHR : std::int32_t
{
	opaqueBit = 0,
	preMultipliedBit = 1,
	postMultipliedBit = 2,
	inheritBit = 3,
};

enum class DisplayPlaneAlphaFlagBitsKHR : std::int32_t
{
	opaqueBit = 0,
	globalBit = 1,
	perPixelBit = 2,
	perPixelPremultipliedBit = 3,
};

enum class PresentModeKHR : std::int32_t
{
	immediate = 0,
	mailbox = 1,
	fifo = 2,
	fifoRelaxed = 3,
};

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

enum class DebugReportFlagBitsEXT : std::int32_t
{
	informationBit = 0,
	warningBit = 1,
	performanceWarningBit = 2,
	errorBit = 3,
	debugBit = 4,
};

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



struct Offset2D
{	int32_t x {};
	int32_t y {};
};

struct Offset3D
{	int32_t x {};
	int32_t y {};
	int32_t z {};
};

struct Extent2D
{	uint32_t width {};
	uint32_t height {};
};

struct Extent3D
{	uint32_t width {};
	uint32_t height {};
	uint32_t depth {};
};

struct Viewport
{	float x {};
	float y {};
	float width {};
	float height {};
	float minDepth {};
	float maxDepth {};
};

struct Rect2D
{	Offset2D offset {};
	Extent2D extent {};
};

struct Rect3D
{	Offset3D offset {};
	Extent3D extent {};
};

struct ClearRect
{	Rect2D rect {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};
};

struct ComponentMapping
{	ComponentSwizzle r {};
	ComponentSwizzle g {};
	ComponentSwizzle b {};
	ComponentSwizzle a {};
};

struct PhysicalDeviceProperties
{	uint32_t apiVersion {};
	uint32_t driverVersion {};
	uint32_t vendorID {};
	uint32_t deviceID {};
	PhysicalDeviceType deviceType {};
	char deviceName {};
	uint8_t pipelineCacheUUID {};
	PhysicalDeviceLimits limits {};
	PhysicalDeviceSparseProperties sparseProperties {};
};

struct ExtensionProperties
{	char extensionName {};
	uint32_t specVersion {};
};

struct LayerProperties
{	char layerName {};
	uint32_t specVersion {};
	uint32_t implementationVersion {};
	char description {};
};

struct ApplicationInfo
{	StructureType sType {StructureType::ApplicationInfo};
	const void* pNext {};
	const char* pApplicationName {};
	uint32_t applicationVersion {};
	const char* pEngineName {};
	uint32_t engineVersion {};
	uint32_t apiVersion {};
};

struct AllocationCallbacks
{	void* pUserData {};
	PFN_vkAllocationFunction pfnAllocation {};
	PFN_vkReallocationFunction pfnReallocation {};
	PFN_vkFreeFunction pfnFree {};
	PFN_vkInternalAllocationNotification pfnInternalAllocation {};
	PFN_vkInternalFreeNotification pfnInternalFree {};
};

struct DeviceQueueCreateInfo
{	StructureType sType {StructureType::DeviceQueueCreateInfo};
	const void* pNext {};
	DeviceQueueCreateFlags flags {};
	uint32_t queueFamilyIndex {};
	uint32_t queueCount {};
	const float* pQueuePriorities {};
};

struct DeviceCreateInfo
{	StructureType sType {StructureType::DeviceCreateInfo};
	const void* pNext {};
	DeviceCreateFlags flags {};
	uint32_t queueCreateInfoCount {};
	const DeviceQueueCreateInfo* pQueueCreateInfos {};
	uint32_t enabledLayerCount {};
	const char** ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char** ppEnabledExtensionNames {};
	const PhysicalDeviceFeatures* pEnabledFeatures {};
};

struct InstanceCreateInfo
{	StructureType sType {StructureType::InstanceCreateInfo};
	const void* pNext {};
	InstanceCreateFlags flags {};
	const ApplicationInfo* pApplicationInfo {};
	uint32_t enabledLayerCount {};
	const char** ppEnabledLayerNames {};
	uint32_t enabledExtensionCount {};
	const char** ppEnabledExtensionNames {};
};

struct QueueFamilyProperties
{	QueueFlags queueFlags {};
	uint32_t queueCount {};
	uint32_t timestampValidBits {};
	Extent3D minImageTransferGranularity {};
};

struct PhysicalDeviceMemoryProperties
{	uint32_t memoryTypeCount {};
	MemoryType memoryTypes {};
	uint32_t memoryHeapCount {};
	MemoryHeap memoryHeaps {};
};

struct MemoryAllocateInfo
{	StructureType sType {StructureType::MemoryAllocateInfo};
	const void* pNext {};
	DeviceSize allocationSize {};
	uint32_t memoryTypeIndex {};
};

struct MemoryRequirements
{	DeviceSize size {};
	DeviceSize alignment {};
	uint32_t memoryTypeBits {};
};

struct SparseImageFormatProperties
{	ImageAspectFlags aspectMask {};
	Extent3D imageGranularity {};
	SparseImageFormatFlags flags {};
};

struct SparseImageMemoryRequirements
{	SparseImageFormatProperties formatProperties {};
	uint32_t imageMipTailFirstLod {};
	DeviceSize imageMipTailSize {};
	DeviceSize imageMipTailOffset {};
	DeviceSize imageMipTailStride {};
};

struct MemoryType
{	MemoryPropertyFlags propertyFlags {};
	uint32_t heapIndex {};
};

struct MemoryHeap
{	DeviceSize size {};
	MemoryHeapFlags flags {};
};

struct MappedMemoryRange
{	StructureType sType {StructureType::MappedMemoryRange};
	const void* pNext {};
	DeviceMemory memory {};
	DeviceSize offset {};
	DeviceSize size {};
};

struct FormatProperties
{	FormatFeatureFlags linearTilingFeatures {};
	FormatFeatureFlags optimalTilingFeatures {};
	FormatFeatureFlags bufferFeatures {};
};

struct ImageFormatProperties
{	Extent3D maxExtent {};
	uint32_t maxMipLevels {};
	uint32_t maxArrayLayers {};
	SampleCountFlags sampleCounts {};
	DeviceSize maxResourceSize {};
};

struct DescriptorBufferInfo
{	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize range {};
};

struct DescriptorImageInfo
{	Sampler sampler {};
	ImageView imageView {};
	ImageLayout imageLayout {};
};

struct WriteDescriptorSet
{	StructureType sType {StructureType::WriteDescriptorSet};
	const void* pNext {};
	DescriptorSet dstSet {};
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};
	DescriptorType descriptorType {};
	const DescriptorImageInfo* pImageInfo {};
	const DescriptorBufferInfo* pBufferInfo {};
	const BufferView* pTexelBufferView {};
};

struct CopyDescriptorSet
{	StructureType sType {StructureType::CopyDescriptorSet};
	const void* pNext {};
	DescriptorSet srcSet {};
	uint32_t srcBinding {};
	uint32_t srcArrayElement {};
	DescriptorSet dstSet {};
	uint32_t dstBinding {};
	uint32_t dstArrayElement {};
	uint32_t descriptorCount {};
};

struct BufferCreateInfo
{	StructureType sType {StructureType::BufferCreateInfo};
	const void* pNext {};
	BufferCreateFlags flags {};
	DeviceSize size {};
	BufferUsageFlags usage {};
	SharingMode sharingMode {};
	uint32_t queueFamilyIndexCount {};
	const uint32_t* pQueueFamilyIndices {};
};

struct BufferViewCreateInfo
{	StructureType sType {StructureType::BufferViewCreateInfo};
	const void* pNext {};
	BufferViewCreateFlags flags {};
	Buffer buffer {};
	Format format {};
	DeviceSize offset {};
	DeviceSize range {};
};

struct ImageSubresource
{	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t arrayLayer {};
};

struct ImageSubresourceLayers
{	ImageAspectFlags aspectMask {};
	uint32_t mipLevel {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};
};

struct ImageSubresourceRange
{	ImageAspectFlags aspectMask {};
	uint32_t baseMipLevel {};
	uint32_t levelCount {};
	uint32_t baseArrayLayer {};
	uint32_t layerCount {};
};

struct MemoryBarrier
{	StructureType sType {StructureType::MemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
};

struct BufferMemoryBarrier
{	StructureType sType {StructureType::BufferMemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	uint32_t srcQueueFamilyIndex {};
	uint32_t dstQueueFamilyIndex {};
	Buffer buffer {};
	DeviceSize offset {};
	DeviceSize size {};
};

struct ImageMemoryBarrier
{	StructureType sType {StructureType::ImageMemoryBarrier};
	const void* pNext {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	ImageLayout oldLayout {};
	ImageLayout newLayout {};
	uint32_t srcQueueFamilyIndex {};
	uint32_t dstQueueFamilyIndex {};
	Image image {};
	ImageSubresourceRange subresourceRange {};
};

struct ImageCreateInfo
{	StructureType sType {StructureType::ImageCreateInfo};
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
};

struct SubresourceLayout
{	DeviceSize offset {};
	DeviceSize size {};
	DeviceSize rowPitch {};
	DeviceSize arrayPitch {};
	DeviceSize depthPitch {};
};

struct ImageViewCreateInfo
{	StructureType sType {StructureType::ImageViewCreateInfo};
	const void* pNext {};
	ImageViewCreateFlags flags {};
	Image image {};
	ImageViewType viewType {};
	Format format {};
	ComponentMapping components {};
	ImageSubresourceRange subresourceRange {};
};

struct BufferCopy
{	DeviceSize srcOffset {};
	DeviceSize dstOffset {};
	DeviceSize size {};
};

struct SparseMemoryBind
{	DeviceSize resourceOffset {};
	DeviceSize size {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};
};

struct SparseImageMemoryBind
{	ImageSubresource subresource {};
	Offset3D offset {};
	Extent3D extent {};
	DeviceMemory memory {};
	DeviceSize memoryOffset {};
	SparseMemoryBindFlags flags {};
};

struct SparseBufferMemoryBindInfo
{	Buffer buffer {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};
};

struct SparseImageOpaqueMemoryBindInfo
{	Image image {};
	uint32_t bindCount {};
	const SparseMemoryBind* pBinds {};
};

struct SparseImageMemoryBindInfo
{	Image image {};
	uint32_t bindCount {};
	const SparseImageMemoryBind* pBinds {};
};

struct BindSparseInfo
{	StructureType sType {StructureType::BindSparseInfo};
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
};

struct ImageCopy
{	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};
};

struct ImageBlit
{	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffsets[2] {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffsets[2] {};
};

struct BufferImageCopy
{	DeviceSize bufferOffset {};
	uint32_t bufferRowLength {};
	uint32_t bufferImageHeight {};
	ImageSubresourceLayers imageSubresource {};
	Offset3D imageOffset {};
	Extent3D imageExtent {};
};

struct ImageResolve
{	ImageSubresourceLayers srcSubresource {};
	Offset3D srcOffset {};
	ImageSubresourceLayers dstSubresource {};
	Offset3D dstOffset {};
	Extent3D extent {};
};

struct ShaderModuleCreateInfo
{	StructureType sType {StructureType::ShaderModuleCreateInfo};
	const void* pNext {};
	ShaderModuleCreateFlags flags {};
	size_t codeSize {};
	const uint32_t* pCode {};
};

struct DescriptorSetLayoutBinding
{	uint32_t binding {};
	DescriptorType descriptorType {};
	uint32_t descriptorCount {};
	ShaderStageFlags stageFlags {};
	const Sampler* pImmutableSamplers {};
};

struct DescriptorSetLayoutCreateInfo
{	StructureType sType {StructureType::DescriptorSetLayoutCreateInfo};
	const void* pNext {};
	DescriptorSetLayoutCreateFlags flags {};
	uint32_t bindingCount {};
	const DescriptorSetLayoutBinding* pBindings {};
};

struct DescriptorPoolSize
{	DescriptorType type {};
	uint32_t descriptorCount {};
};

struct DescriptorPoolCreateInfo
{	StructureType sType {StructureType::DescriptorPoolCreateInfo};
	const void* pNext {};
	DescriptorPoolCreateFlags flags {};
	uint32_t maxSets {};
	uint32_t poolSizeCount {};
	const DescriptorPoolSize* pPoolSizes {};
};

struct DescriptorSetAllocateInfo
{	StructureType sType {StructureType::DescriptorSetAllocateInfo};
	const void* pNext {};
	DescriptorPool descriptorPool {};
	uint32_t descriptorSetCount {};
	const DescriptorSetLayout* pSetLayouts {};
};

struct SpecializationMapEntry
{	uint32_t constantID {};
	uint32_t offset {};
	size_t size {};
};

struct SpecializationInfo
{	uint32_t mapEntryCount {};
	const SpecializationMapEntry* pMapEntries {};
	size_t dataSize {};
	const void* pData {};
};

struct PipelineShaderStageCreateInfo
{	StructureType sType {StructureType::PipelineShaderStageCreateInfo};
	const void* pNext {};
	PipelineShaderStageCreateFlags flags {};
	ShaderStageFlagBits stage {};
	ShaderModule module {};
	const char* pName {};
	const SpecializationInfo* pSpecializationInfo {};
};

struct ComputePipelineCreateInfo
{	StructureType sType {StructureType::ComputePipelineCreateInfo};
	const void* pNext {};
	PipelineCreateFlags flags {};
	PipelineShaderStageCreateInfo stage {};
	PipelineLayout layout {};
	Pipeline basePipelineHandle {};
	int32_t basePipelineIndex {};
};

struct VertexInputBindingDescription
{	uint32_t binding {};
	uint32_t stride {};
	VertexInputRate inputRate {};
};

struct VertexInputAttributeDescription
{	uint32_t location {};
	uint32_t binding {};
	Format format {};
	uint32_t offset {};
};

struct PipelineVertexInputStateCreateInfo
{	StructureType sType {StructureType::PipelineVertexInputStateCreateInfo};
	const void* pNext {};
	PipelineVertexInputStateCreateFlags flags {};
	uint32_t vertexBindingDescriptionCount {};
	const VertexInputBindingDescription* pVertexBindingDescriptions {};
	uint32_t vertexAttributeDescriptionCount {};
	const VertexInputAttributeDescription* pVertexAttributeDescriptions {};
};

struct PipelineInputAssemblyStateCreateInfo
{	StructureType sType {StructureType::PipelineInputAssemblyStateCreateInfo};
	const void* pNext {};
	PipelineInputAssemblyStateCreateFlags flags {};
	PrimitiveTopology topology {};
	Bool32 primitiveRestartEnable {};
};

struct PipelineTessellationStateCreateInfo
{	StructureType sType {StructureType::PipelineTessellationStateCreateInfo};
	const void* pNext {};
	PipelineTessellationStateCreateFlags flags {};
	uint32_t patchControlPoints {};
};

struct PipelineViewportStateCreateInfo
{	StructureType sType {StructureType::PipelineViewportStateCreateInfo};
	const void* pNext {};
	PipelineViewportStateCreateFlags flags {};
	uint32_t viewportCount {};
	const Viewport* pViewports {};
	uint32_t scissorCount {};
	const Rect2D* pScissors {};
};

struct PipelineRasterizationStateCreateInfo
{	StructureType sType {StructureType::PipelineRasterizationStateCreateInfo};
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
};

struct PipelineMultisampleStateCreateInfo
{	StructureType sType {StructureType::PipelineMultisampleStateCreateInfo};
	const void* pNext {};
	PipelineMultisampleStateCreateFlags flags {};
	SampleCountFlagBits rasterizationSamples {};
	Bool32 sampleShadingEnable {};
	float minSampleShading {};
	const SampleMask* pSampleMask {};
	Bool32 alphaToCoverageEnable {};
	Bool32 alphaToOneEnable {};
};

struct PipelineColorBlendAttachmentState
{	Bool32 blendEnable {};
	BlendFactor srcColorBlendFactor {};
	BlendFactor dstColorBlendFactor {};
	BlendOp colorBlendOp {};
	BlendFactor srcAlphaBlendFactor {};
	BlendFactor dstAlphaBlendFactor {};
	BlendOp alphaBlendOp {};
	ColorComponentFlags colorWriteMask {};
};

struct PipelineColorBlendStateCreateInfo
{	StructureType sType {StructureType::PipelineColorBlendStateCreateInfo};
	const void* pNext {};
	PipelineColorBlendStateCreateFlags flags {};
	Bool32 logicOpEnable {};
	LogicOp logicOp {};
	uint32_t attachmentCount {};
	const PipelineColorBlendAttachmentState* pAttachments {};
	float blendConstants {};
};

struct PipelineDynamicStateCreateInfo
{	StructureType sType {StructureType::PipelineDynamicStateCreateInfo};
	const void* pNext {};
	PipelineDynamicStateCreateFlags flags {};
	uint32_t dynamicStateCount {};
	const DynamicState* pDynamicStates {};
};

struct StencilOpState
{	StencilOp failOp {};
	StencilOp passOp {};
	StencilOp depthFailOp {};
	CompareOp compareOp {};
	uint32_t compareMask {};
	uint32_t writeMask {};
	uint32_t reference {};
};

struct PipelineDepthStencilStateCreateInfo
{	StructureType sType {StructureType::PipelineDepthStencilStateCreateInfo};
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
};

struct GraphicsPipelineCreateInfo
{	StructureType sType {StructureType::GraphicsPipelineCreateInfo};
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
};

struct PipelineCacheCreateInfo
{	StructureType sType {StructureType::PipelineCacheCreateInfo};
	const void* pNext {};
	PipelineCacheCreateFlags flags {};
	size_t initialDataSize {};
	const void* pInitialData {};
};

struct PushConstantRange
{	ShaderStageFlags stageFlags {};
	uint32_t offset {};
	uint32_t size {};
};

struct PipelineLayoutCreateInfo
{	StructureType sType {StructureType::PipelineLayoutCreateInfo};
	const void* pNext {};
	PipelineLayoutCreateFlags flags {};
	uint32_t setLayoutCount {};
	const DescriptorSetLayout* pSetLayouts {};
	uint32_t pushConstantRangeCount {};
	const PushConstantRange* pPushConstantRanges {};
};

struct SamplerCreateInfo
{	StructureType sType {StructureType::SamplerCreateInfo};
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
};

struct CommandPoolCreateInfo
{	StructureType sType {StructureType::CommandPoolCreateInfo};
	const void* pNext {};
	CommandPoolCreateFlags flags {};
	uint32_t queueFamilyIndex {};
};

struct CommandBufferAllocateInfo
{	StructureType sType {StructureType::CommandBufferAllocateInfo};
	const void* pNext {};
	CommandPool commandPool {};
	CommandBufferLevel level {};
	uint32_t commandBufferCount {};
};

struct CommandBufferInheritanceInfo
{	StructureType sType {StructureType::CommandBufferInheritanceInfo};
	const void* pNext {};
	RenderPass renderPass {};
	uint32_t subpass {};
	Framebuffer framebuffer {};
	Bool32 occlusionQueryEnable {};
	QueryControlFlags queryFlags {};
	QueryPipelineStatisticFlags pipelineStatistics {};
};

struct CommandBufferBeginInfo
{	StructureType sType {StructureType::CommandBufferBeginInfo};
	const void* pNext {};
	CommandBufferUsageFlags flags {};
	const CommandBufferInheritanceInfo* pInheritanceInfo {};
};

struct RenderPassBeginInfo
{	StructureType sType {StructureType::RenderPassBeginInfo};
	const void* pNext {};
	RenderPass renderPass {};
	Framebuffer framebuffer {};
	Rect2D renderArea {};
	uint32_t clearValueCount {};
	const ClearValue* pClearValues {};
};

union ClearColorValue
{};

struct ClearDepthStencilValue
{	float depth {};
	uint32_t stencil {};
};

union ClearValue
{};

struct ClearAttachment
{	ImageAspectFlags aspectMask {};
	uint32_t colorAttachment {};
	ClearValue clearValue {};
};

struct AttachmentDescription
{	AttachmentDescriptionFlags flags {};
	Format format {};
	SampleCountFlagBits samples {};
	AttachmentLoadOp loadOp {};
	AttachmentStoreOp storeOp {};
	AttachmentLoadOp stencilLoadOp {};
	AttachmentStoreOp stencilStoreOp {};
	ImageLayout initialLayout {};
	ImageLayout finalLayout {};
};

struct AttachmentReference
{	uint32_t attachment {};
	ImageLayout layout {};
};

struct SubpassDescription
{	SubpassDescriptionFlags flags {};
	PipelineBindPoint pipelineBindPoint {};
	uint32_t inputAttachmentCount {};
	const AttachmentReference* pInputAttachments {};
	uint32_t colorAttachmentCount {};
	const AttachmentReference* pColorAttachments {};
	const AttachmentReference* pResolveAttachments {};
	const AttachmentReference* pDepthStencilAttachment {};
	uint32_t preserveAttachmentCount {};
	const uint32_t* pPreserveAttachments {};
};

struct SubpassDependency
{	uint32_t srcSubpass {};
	uint32_t dstSubpass {};
	PipelineStageFlags srcStageMask {};
	PipelineStageFlags dstStageMask {};
	AccessFlags srcAccessMask {};
	AccessFlags dstAccessMask {};
	DependencyFlags dependencyFlags {};
};

struct RenderPassCreateInfo
{	StructureType sType {StructureType::RenderPassCreateInfo};
	const void* pNext {};
	RenderPassCreateFlags flags {};
	uint32_t attachmentCount {};
	const AttachmentDescription* pAttachments {};
	uint32_t subpassCount {};
	const SubpassDescription* pSubpasses {};
	uint32_t dependencyCount {};
	const SubpassDependency* pDependencies {};
};

struct EventCreateInfo
{	StructureType sType {StructureType::EventCreateInfo};
	const void* pNext {};
	EventCreateFlags flags {};
};

struct FenceCreateInfo
{	StructureType sType {StructureType::FenceCreateInfo};
	const void* pNext {};
	FenceCreateFlags flags {};
};

struct PhysicalDeviceFeatures
{	Bool32 robustBufferAccess {};
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
};

struct PhysicalDeviceSparseProperties
{	Bool32 residencyStandard2DBlockShape {};
	Bool32 residencyStandard2DMultisampleBlockShape {};
	Bool32 residencyStandard3DBlockShape {};
	Bool32 residencyAlignedMipSize {};
	Bool32 residencyNonResidentStrict {};
};

struct PhysicalDeviceLimits
{	uint32_t maxImageDimension1D {};
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
	uint32_t maxComputeWorkGroupCount {};
	uint32_t maxComputeWorkGroupInvocations {};
	uint32_t maxComputeWorkGroupSize {};
	uint32_t subPixelPrecisionBits {};
	uint32_t subTexelPrecisionBits {};
	uint32_t mipmapPrecisionBits {};
	uint32_t maxDrawIndexedIndexValue {};
	uint32_t maxDrawIndirectCount {};
	float maxSamplerLodBias {};
	float maxSamplerAnisotropy {};
	uint32_t maxViewports {};
	uint32_t maxViewportDimensions {};
	float viewportBoundsRange {};
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
	float pointSizeRange {};
	float lineWidthRange {};
	float pointSizeGranularity {};
	float lineWidthGranularity {};
	Bool32 strictLines {};
	Bool32 standardSampleLocations {};
	DeviceSize optimalBufferCopyOffsetAlignment {};
	DeviceSize optimalBufferCopyRowPitchAlignment {};
	DeviceSize nonCoherentAtomSize {};
};

struct SemaphoreCreateInfo
{	StructureType sType {StructureType::SemaphoreCreateInfo};
	const void* pNext {};
	SemaphoreCreateFlags flags {};
};

struct QueryPoolCreateInfo
{	StructureType sType {StructureType::QueryPoolCreateInfo};
	const void* pNext {};
	QueryPoolCreateFlags flags {};
	QueryType queryType {};
	uint32_t queryCount {};
	QueryPipelineStatisticFlags pipelineStatistics {};
};

struct FramebufferCreateInfo
{	StructureType sType {StructureType::FramebufferCreateInfo};
	const void* pNext {};
	FramebufferCreateFlags flags {};
	RenderPass renderPass {};
	uint32_t attachmentCount {};
	const ImageView* pAttachments {};
	uint32_t width {};
	uint32_t height {};
	uint32_t layers {};
};

struct DrawIndirectCommand
{	uint32_t vertexCount {};
	uint32_t instanceCount {};
	uint32_t firstVertex {};
	uint32_t firstInstance {};
};

struct DrawIndexedIndirectCommand
{	uint32_t indexCount {};
	uint32_t instanceCount {};
	uint32_t firstIndex {};
	int32_t vertexOffset {};
	uint32_t firstInstance {};
};

struct DispatchIndirectCommand
{	uint32_t x {};
	uint32_t y {};
	uint32_t z {};
};

struct SubmitInfo
{	StructureType sType {StructureType::SubmitInfo};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	const PipelineStageFlags* pWaitDstStageMask {};
	uint32_t commandBufferCount {};
	const CommandBuffer* pCommandBuffers {};
	uint32_t signalSemaphoreCount {};
	const Semaphore* pSignalSemaphores {};
};

struct DisplayPropertiesKHR
{	DisplayKHR display {};
	const char* displayName {};
	Extent2D physicalDimensions {};
	Extent2D physicalResolution {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	Bool32 planeReorderPossible {};
	Bool32 persistentContent {};
};

struct DisplayPlanePropertiesKHR
{	DisplayKHR currentDisplay {};
	uint32_t currentStackIndex {};
};

struct DisplayModeParametersKHR
{	Extent2D visibleRegion {};
	uint32_t refreshRate {};
};

struct DisplayModePropertiesKHR
{	DisplayModeKHR displayMode {};
	DisplayModeParametersKHR parameters {};
};

struct DisplayModeCreateInfoKHR
{	StructureType sType {StructureType::DisplayModeCreateInfoKHR};
	const void* pNext {};
	DisplayModeCreateFlagsKHR flags {};
	DisplayModeParametersKHR parameters {};
};

struct DisplayPlaneCapabilitiesKHR
{	DisplayPlaneAlphaFlagsKHR supportedAlpha {};
	Offset2D minSrcPosition {};
	Offset2D maxSrcPosition {};
	Extent2D minSrcExtent {};
	Extent2D maxSrcExtent {};
	Offset2D minDstPosition {};
	Offset2D maxDstPosition {};
	Extent2D minDstExtent {};
	Extent2D maxDstExtent {};
};

struct DisplaySurfaceCreateInfoKHR
{	StructureType sType {StructureType::DisplaySurfaceCreateInfoKHR};
	const void* pNext {};
	DisplaySurfaceCreateFlagsKHR flags {};
	DisplayModeKHR displayMode {};
	uint32_t planeIndex {};
	uint32_t planeStackIndex {};
	SurfaceTransformFlagBitsKHR transform {};
	float globalAlpha {};
	DisplayPlaneAlphaFlagBitsKHR alphaMode {};
	Extent2D imageExtent {};
};

struct DisplayPresentInfoKHR
{	StructureType sType {StructureType::DisplayPresentInfoKHR};
	const void* pNext {};
	Rect2D srcRect {};
	Rect2D dstRect {};
	Bool32 persistent {};
};

struct SurfaceCapabilitiesKHR
{	uint32_t minImageCount {};
	uint32_t maxImageCount {};
	Extent2D currentExtent {};
	Extent2D minImageExtent {};
	Extent2D maxImageExtent {};
	uint32_t maxImageArrayLayers {};
	SurfaceTransformFlagsKHR supportedTransforms {};
	SurfaceTransformFlagBitsKHR currentTransform {};
	CompositeAlphaFlagsKHR supportedCompositeAlpha {};
	ImageUsageFlags supportedUsageFlags {};
};

struct AndroidSurfaceCreateInfoKHR
{	StructureType sType {StructureType::AndroidSurfaceCreateInfoKHR};
	const void* pNext {};
	AndroidSurfaceCreateFlagsKHR flags {};
	ANativeWindow* window {};
};

struct MirSurfaceCreateInfoKHR
{	StructureType sType {StructureType::MirSurfaceCreateInfoKHR};
	const void* pNext {};
	MirSurfaceCreateFlagsKHR flags {};
	MirConnection* connection {};
	MirSurface* mirSurface {};
};

struct WaylandSurfaceCreateInfoKHR
{	StructureType sType {StructureType::WaylandSurfaceCreateInfoKHR};
	const void* pNext {};
	WaylandSurfaceCreateFlagsKHR flags {};
	wl_display* display {};
	wl_surface* surface {};
};

struct Win32SurfaceCreateInfoKHR
{	StructureType sType {StructureType::Win32SurfaceCreateInfoKHR};
	const void* pNext {};
	Win32SurfaceCreateFlagsKHR flags {};
	HINSTANCE hinstance {};
	HWND hwnd {};
};

struct XlibSurfaceCreateInfoKHR
{	StructureType sType {StructureType::XlibSurfaceCreateInfoKHR};
	const void* pNext {};
	XlibSurfaceCreateFlagsKHR flags {};
	Display* dpy {};
	Window window {};
};

struct XcbSurfaceCreateInfoKHR
{	StructureType sType {StructureType::XcbSurfaceCreateInfoKHR};
	const void* pNext {};
	XcbSurfaceCreateFlagsKHR flags {};
	xcb_connection_t* connection {};
	xcb_window_t window {};
};

struct SurfaceFormatKHR
{	Format format {};
	ColorSpaceKHR colorSpace {};
};

struct SwapchainCreateInfoKHR
{	StructureType sType {StructureType::SwapchainCreateInfoKHR};
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
};

struct PresentInfoKHR
{	StructureType sType {StructureType::PresentInfoKHR};
	const void* pNext {};
	uint32_t waitSemaphoreCount {};
	const Semaphore* pWaitSemaphores {};
	uint32_t swapchainCount {};
	const SwapchainKHR* pSwapchains {};
	const uint32_t* pImageIndices {};
	Result* pResults {};
};

struct DebugReportCallbackCreateInfoEXT
{	StructureType sType {StructureType::DebugReportCallbackCreateInfoEXT};
	const void* pNext {};
	DebugReportFlagsEXT flags {};
	PFN_vkDebugReportCallbackEXT pfnCallback {};
	void* pUserData {};
};



VkResult createInstance(const InstanceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Instance* pInstance)
{
	vkCreateInstance(pCreateInfo, pAllocator, pInstance);
}
void destroyInstance(Instance instance, const AllocationCallbacks* pAllocator)
{
	vkDestroyInstance(instance, pAllocator);
}
VkResult enumeratePhysicalDevices(Instance instance, uint32_t* pPhysicalDeviceCount, PhysicalDevice* pPhysicalDevices)
{
	vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
}
PFN_vkVoidFunction getDeviceProcAddr(Device device, const char* pName)
{
	vkGetDeviceProcAddr(device, pName);
}
PFN_vkVoidFunction getInstanceProcAddr(Instance instance, const char* pName)
{
	vkGetInstanceProcAddr(instance, pName);
}
void getPhysicalDeviceProperties(PhysicalDevice physicalDevice, PhysicalDeviceProperties* pProperties)
{
	vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}
void getPhysicalDeviceQueueFamilyProperties(PhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, QueueFamilyProperties* pQueueFamilyProperties)
{
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}
void getPhysicalDeviceMemoryProperties(PhysicalDevice physicalDevice, PhysicalDeviceMemoryProperties* pMemoryProperties)
{
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}
void getPhysicalDeviceFeatures(PhysicalDevice physicalDevice, PhysicalDeviceFeatures* pFeatures)
{
	vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}
void getPhysicalDeviceFormatProperties(PhysicalDevice physicalDevice, Format format, FormatProperties* pFormatProperties)
{
	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}
VkResult getPhysicalDeviceImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, ImageTiling tiling, ImageUsageFlags usage, ImageCreateFlags flags, ImageFormatProperties* pImageFormatProperties)
{
	vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}
VkResult createDevice(PhysicalDevice physicalDevice, const DeviceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Device* pDevice)
{
	vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}
void destroyDevice(Device device, const AllocationCallbacks* pAllocator)
{
	vkDestroyDevice(device, pAllocator);
}
VkResult enumerateInstanceLayerProperties(uint32_t* pPropertyCount, LayerProperties* pProperties)
{
	vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}
VkResult enumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties)
{
	vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}
VkResult enumerateDeviceLayerProperties(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, LayerProperties* pProperties)
{
	vkEnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}
VkResult enumerateDeviceExtensionProperties(PhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, ExtensionProperties* pProperties)
{
	vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
}
void getDeviceQueue(Device device, uint32_t queueFamilyIndex, uint32_t queueIndex, Queue* pQueue)
{
	vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}
VkResult queueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo* pSubmits, Fence fence)
{
	vkQueueSubmit(queue, submitCount, pSubmits, fence);
}
VkResult queueWaitIdle(Queue queue)
{
	vkQueueWaitIdle(queue);
}
VkResult deviceWaitIdle(Device device)
{
	vkDeviceWaitIdle(device);
}
VkResult allocateMemory(Device device, const MemoryAllocateInfo* pAllocateInfo, const AllocationCallbacks* pAllocator, DeviceMemory* pMemory)
{
	vkAllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
}
void freeMemory(Device device, DeviceMemory memory, const AllocationCallbacks* pAllocator)
{
	vkFreeMemory(device, memory, pAllocator);
}
VkResult mapMemory(Device device, DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void** ppData)
{
	vkMapMemory(device, memory, offset, size, flags, ppData);
}
void unmapMemory(Device device, DeviceMemory memory)
{
	vkUnmapMemory(device, memory);
}
VkResult flushMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges)
{
	vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
VkResult invalidateMappedMemoryRanges(Device device, uint32_t memoryRangeCount, const MappedMemoryRange* pMemoryRanges)
{
	vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}
void getDeviceMemoryCommitment(Device device, DeviceMemory memory, DeviceSize* pCommittedMemoryInBytes)
{
	vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}
void getBufferMemoryRequirements(Device device, Buffer buffer, MemoryRequirements* pMemoryRequirements)
{
	vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}
VkResult bindBufferMemory(Device device, Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset)
{
	vkBindBufferMemory(device, buffer, memory, memoryOffset);
}
void getImageMemoryRequirements(Device device, Image image, MemoryRequirements* pMemoryRequirements)
{
	vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
}
VkResult bindImageMemory(Device device, Image image, DeviceMemory memory, DeviceSize memoryOffset)
{
	vkBindImageMemory(device, image, memory, memoryOffset);
}
void getImageSparseMemoryRequirements(Device device, Image image, uint32_t* pSparseMemoryRequirementCount, SparseImageMemoryRequirements* pSparseMemoryRequirements)
{
	vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}
void getPhysicalDeviceSparseImageFormatProperties(PhysicalDevice physicalDevice, Format format, ImageType type, SampleCountFlagBits samples, ImageUsageFlags usage, ImageTiling tiling, uint32_t* pPropertyCount, SparseImageFormatProperties* pProperties)
{
	vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}
VkResult queueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo* pBindInfo, Fence fence)
{
	vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}
VkResult createFence(Device device, const FenceCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Fence* pFence)
{
	vkCreateFence(device, pCreateInfo, pAllocator, pFence);
}
void destroyFence(Device device, Fence fence, const AllocationCallbacks* pAllocator)
{
	vkDestroyFence(device, fence, pAllocator);
}
VkResult resetFences(Device device, uint32_t fenceCount, const Fence* pFences)
{
	vkResetFences(device, fenceCount, pFences);
}
VkResult getFenceStatus(Device device, Fence fence)
{
	vkGetFenceStatus(device, fence);
}
VkResult waitForFences(Device device, uint32_t fenceCount, const Fence* pFences, Bool32 waitAll, uint64_t timeout)
{
	vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
}
VkResult createSemaphore(Device device, const SemaphoreCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Semaphore* pSemaphore)
{
	vkCreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
}
void destroySemaphore(Device device, Semaphore semaphore, const AllocationCallbacks* pAllocator)
{
	vkDestroySemaphore(device, semaphore, pAllocator);
}
VkResult createEvent(Device device, const EventCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Event* pEvent)
{
	vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
}
void destroyEvent(Device device, Event event, const AllocationCallbacks* pAllocator)
{
	vkDestroyEvent(device, event, pAllocator);
}
VkResult getEventStatus(Device device, Event event)
{
	vkGetEventStatus(device, event);
}
VkResult setEvent(Device device, Event event)
{
	vkSetEvent(device, event);
}
VkResult resetEvent(Device device, Event event)
{
	vkResetEvent(device, event);
}
VkResult createQueryPool(Device device, const QueryPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, QueryPool* pQueryPool)
{
	vkCreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
}
void destroyQueryPool(Device device, QueryPool queryPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyQueryPool(device, queryPool, pAllocator);
}
VkResult getQueryPoolResults(Device device, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, DeviceSize stride, QueryResultFlags flags)
{
	vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}
VkResult createBuffer(Device device, const BufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Buffer* pBuffer)
{
	vkCreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
}
void destroyBuffer(Device device, Buffer buffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyBuffer(device, buffer, pAllocator);
}
VkResult createBufferView(Device device, const BufferViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, BufferView* pView)
{
	vkCreateBufferView(device, pCreateInfo, pAllocator, pView);
}
void destroyBufferView(Device device, BufferView bufferView, const AllocationCallbacks* pAllocator)
{
	vkDestroyBufferView(device, bufferView, pAllocator);
}
VkResult createImage(Device device, const ImageCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Image* pImage)
{
	vkCreateImage(device, pCreateInfo, pAllocator, pImage);
}
void destroyImage(Device device, Image image, const AllocationCallbacks* pAllocator)
{
	vkDestroyImage(device, image, pAllocator);
}
void getImageSubresourceLayout(Device device, Image image, const ImageSubresource* pSubresource, SubresourceLayout* pLayout)
{
	vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
}
VkResult createImageView(Device device, const ImageViewCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ImageView* pView)
{
	vkCreateImageView(device, pCreateInfo, pAllocator, pView);
}
void destroyImageView(Device device, ImageView imageView, const AllocationCallbacks* pAllocator)
{
	vkDestroyImageView(device, imageView, pAllocator);
}
VkResult createShaderModule(Device device, const ShaderModuleCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, ShaderModule* pShaderModule)
{
	vkCreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
}
void destroyShaderModule(Device device, ShaderModule shaderModule, const AllocationCallbacks* pAllocator)
{
	vkDestroyShaderModule(device, shaderModule, pAllocator);
}
VkResult createPipelineCache(Device device, const PipelineCacheCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineCache* pPipelineCache)
{
	vkCreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
}
void destroyPipelineCache(Device device, PipelineCache pipelineCache, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineCache(device, pipelineCache, pAllocator);
}
VkResult getPipelineCacheData(Device device, PipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
	vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
}
VkResult mergePipelineCaches(Device device, PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache* pSrcCaches)
{
	vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
}
VkResult createGraphicsPipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines)
{
	vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
VkResult createComputePipelines(Device device, PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo* pCreateInfos, const AllocationCallbacks* pAllocator, Pipeline* pPipelines)
{
	vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
}
void destroyPipeline(Device device, Pipeline pipeline, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipeline(device, pipeline, pAllocator);
}
VkResult createPipelineLayout(Device device, const PipelineLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, PipelineLayout* pPipelineLayout)
{
	vkCreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
}
void destroyPipelineLayout(Device device, PipelineLayout pipelineLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyPipelineLayout(device, pipelineLayout, pAllocator);
}
VkResult createSampler(Device device, const SamplerCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Sampler* pSampler)
{
	vkCreateSampler(device, pCreateInfo, pAllocator, pSampler);
}
void destroySampler(Device device, Sampler sampler, const AllocationCallbacks* pAllocator)
{
	vkDestroySampler(device, sampler, pAllocator);
}
VkResult createDescriptorSetLayout(Device device, const DescriptorSetLayoutCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorSetLayout* pSetLayout)
{
	vkCreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
}
void destroyDescriptorSetLayout(Device device, DescriptorSetLayout descriptorSetLayout, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
}
VkResult createDescriptorPool(Device device, const DescriptorPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, DescriptorPool* pDescriptorPool)
{
	vkCreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
}
void destroyDescriptorPool(Device device, DescriptorPool descriptorPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyDescriptorPool(device, descriptorPool, pAllocator);
}
VkResult resetDescriptorPool(Device device, DescriptorPool descriptorPool, DescriptorPoolResetFlags flags)
{
	vkResetDescriptorPool(device, descriptorPool, flags);
}
VkResult allocateDescriptorSets(Device device, const DescriptorSetAllocateInfo* pAllocateInfo, DescriptorSet* pDescriptorSets)
{
	vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
}
VkResult freeDescriptorSets(Device device, DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets)
{
	vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}
void updateDescriptorSets(Device device, uint32_t descriptorWriteCount, const WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet* pDescriptorCopies)
{
	vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}
VkResult createFramebuffer(Device device, const FramebufferCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, Framebuffer* pFramebuffer)
{
	vkCreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
}
void destroyFramebuffer(Device device, Framebuffer framebuffer, const AllocationCallbacks* pAllocator)
{
	vkDestroyFramebuffer(device, framebuffer, pAllocator);
}
VkResult createRenderPass(Device device, const RenderPassCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, RenderPass* pRenderPass)
{
	vkCreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
}
void destroyRenderPass(Device device, RenderPass renderPass, const AllocationCallbacks* pAllocator)
{
	vkDestroyRenderPass(device, renderPass, pAllocator);
}
void getRenderAreaGranularity(Device device, RenderPass renderPass, Extent2D* pGranularity)
{
	vkGetRenderAreaGranularity(device, renderPass, pGranularity);
}
VkResult createCommandPool(Device device, const CommandPoolCreateInfo* pCreateInfo, const AllocationCallbacks* pAllocator, CommandPool* pCommandPool)
{
	vkCreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
}
void destroyCommandPool(Device device, CommandPool commandPool, const AllocationCallbacks* pAllocator)
{
	vkDestroyCommandPool(device, commandPool, pAllocator);
}
VkResult resetCommandPool(Device device, CommandPool commandPool, CommandPoolResetFlags flags)
{
	vkResetCommandPool(device, commandPool, flags);
}
VkResult allocateCommandBuffers(Device device, const CommandBufferAllocateInfo* pAllocateInfo, CommandBuffer* pCommandBuffers)
{
	vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
}
void freeCommandBuffers(Device device, CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers)
{
	vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}
VkResult beginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo* pBeginInfo)
{
	vkBeginCommandBuffer(commandBuffer, pBeginInfo);
}
VkResult endCommandBuffer(CommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);
}
VkResult resetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags)
{
	vkResetCommandBuffer(commandBuffer, flags);
}
void cmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline)
{
	vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}
void cmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport* pViewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}
void cmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D* pScissors)
{
	vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}
void cmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth)
{
	vkCmdSetLineWidth(commandBuffer, lineWidth);
}
void cmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
{
	vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}
void cmdSetBlendConstants(CommandBuffer commandBuffer, const float blendConstants)
{
	vkCmdSetBlendConstants(commandBuffer, blendConstants);
}
void cmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds)
{
	vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}
void cmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask)
{
	vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}
void cmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask)
{
	vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}
void cmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference)
{
	vkCmdSetStencilReference(commandBuffer, faceMask, reference);
}
void cmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
	vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}
void cmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType)
{
	vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}
void cmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers, const DeviceSize* pOffsets)
{
	vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
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
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}
void cmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy* pRegions)
{
	vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
void cmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit* pRegions, Filter filter)
{
	vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}
void cmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy* pRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}
void cmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy* pRegions)
{
	vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}
void cmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const uint32_t* pData)
{
	vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}
void cmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data)
{
	vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}
void cmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue* pColor, uint32_t rangeCount, const ImageSubresourceRange* pRanges)
{
	vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}
void cmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange* pRanges)
{
	vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}
void cmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment* pAttachments, uint32_t rectCount, const ClearRect* pRects)
{
	vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}
void cmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve* pRegions)
{
	vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}
void cmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdSetEvent(commandBuffer, event, stageMask);
}
void cmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStageFlags stageMask)
{
	vkCmdResetEvent(commandBuffer, event, stageMask);
}
void cmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event* pEvents, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers)
{
	vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
void cmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStageFlags srcStageMask, PipelineStageFlags dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier* pImageMemoryBarriers)
{
	vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}
void cmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags)
{
	vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
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
	vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}
void cmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags)
{
	vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}
void cmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
	vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}
void cmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo* pRenderPassBegin, SubpassContents contents)
{
	vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}
void cmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents)
{
	vkCmdNextSubpass(commandBuffer, contents);
}
void cmdEndRenderPass(CommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}
void cmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer* pCommandBuffers)
{
	vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}
VkResult createAndroidSurfaceKHR(Instance instance, const AndroidSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkResult getPhysicalDeviceDisplayPropertiesKHR(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPropertiesKHR* pProperties)
{
	vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
VkResult getPhysicalDeviceDisplayPlanePropertiesKHR(PhysicalDevice physicalDevice, uint32_t* pPropertyCount, DisplayPlanePropertiesKHR* pProperties)
{
	vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}
VkResult getDisplayPlaneSupportedDisplaysKHR(PhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, DisplayKHR* pDisplays)
{
	vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}
VkResult getDisplayModePropertiesKHR(PhysicalDevice physicalDevice, DisplayKHR display, uint32_t* pPropertyCount, DisplayModePropertiesKHR* pProperties)
{
	vkGetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
}
VkResult createDisplayModeKHR(PhysicalDevice physicalDevice, DisplayKHR display, const DisplayModeCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, DisplayModeKHR* pMode)
{
	vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
}
VkResult getDisplayPlaneCapabilitiesKHR(PhysicalDevice physicalDevice, DisplayModeKHR mode, uint32_t planeIndex, DisplayPlaneCapabilitiesKHR* pCapabilities)
{
	vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}
VkResult createDisplayPlaneSurfaceKHR(Instance instance, const DisplaySurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkResult createSharedSwapchainsKHR(Device device, uint32_t swapchainCount, const SwapchainCreateInfoKHR* pCreateInfos, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchains)
{
	vkCreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
}
VkResult createMirSurfaceKHR(Instance instance, const MirSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateMirSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkBool32 getPhysicalDeviceMirPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, MirConnection* connection)
{
	vkGetPhysicalDeviceMirPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection);
}
void destroySurfaceKHR(Instance instance, SurfaceKHR surface, const AllocationCallbacks* pAllocator)
{
	vkDestroySurfaceKHR(instance, surface, pAllocator);
}
VkResult getPhysicalDeviceSurfaceSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, SurfaceKHR surface, Bool32* pSupported)
{
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}
VkResult getPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, SurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}
VkResult getPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pSurfaceFormatCount, SurfaceFormatKHR* pSurfaceFormats)
{
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}
VkResult getPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice physicalDevice, SurfaceKHR surface, uint32_t* pPresentModeCount, PresentModeKHR* pPresentModes)
{
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
}
VkResult createSwapchainKHR(Device device, const SwapchainCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SwapchainKHR* pSwapchain)
{
	vkCreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
}
void destroySwapchainKHR(Device device, SwapchainKHR swapchain, const AllocationCallbacks* pAllocator)
{
	vkDestroySwapchainKHR(device, swapchain, pAllocator);
}
VkResult getSwapchainImagesKHR(Device device, SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, Image* pSwapchainImages)
{
	vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}
VkResult acquireNextImageKHR(Device device, SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t* pImageIndex)
{
	vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
}
VkResult queuePresentKHR(Queue queue, const PresentInfoKHR* pPresentInfo)
{
	vkQueuePresentKHR(queue, pPresentInfo);
}
VkResult createWaylandSurfaceKHR(Instance instance, const WaylandSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkBool32 getPhysicalDeviceWaylandPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, wl_display* display)
{
	vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
}
VkResult createWin32SurfaceKHR(Instance instance, const Win32SurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkBool32 getPhysicalDeviceWin32PresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
{
	vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
}
VkResult createXlibSurfaceKHR(Instance instance, const XlibSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkBool32 getPhysicalDeviceXlibPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID)
{
	vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
}
VkResult createXcbSurfaceKHR(Instance instance, const XcbSurfaceCreateInfoKHR* pCreateInfo, const AllocationCallbacks* pAllocator, SurfaceKHR* pSurface)
{
	vkCreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
}
VkBool32 getPhysicalDeviceXcbPresentationSupportKHR(PhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id)
{
	vkGetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection, visual_id);
}
VkResult createDebugReportCallbackEXT(Instance instance, const DebugReportCallbackCreateInfoEXT* pCreateInfo, const AllocationCallbacks* pAllocator, DebugReportCallbackEXT* pCallback)
{
	vkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}
void destroyDebugReportCallbackEXT(Instance instance, DebugReportCallbackEXT callback, const AllocationCallbacks* pAllocator)
{
	vkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}
void debugReportMessageEXT(Instance instance, DebugReportFlagsEXT flags, DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
{
	vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

}