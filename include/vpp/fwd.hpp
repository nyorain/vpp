// Copyright (c) 2016-2018 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <vkpp/fwd.hpp>
#include <vpp/config.hpp>

#include <cstdint> // std::uint8_t
#include <cstddef> // std::size_t

namespace vpp {

// treat them as built-in
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;

using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;

using std::size_t;

/// Tag that initializes resources in a deferred way.
/// Usually better for performance but you have to call an additional
/// function when you use them.
struct DeferTag {};
constexpr DeferTag defer {};

class Resource;
class WorkBase;

class Buffer;
class Image;
class ImageView;
class Surface;
class Swapchain;
class ShaderProgram;
class ShaderModule;
class DeviceMemory;
class Pipeline;
class DebugCallback;
class DescriptorSet;
class DescriptorPool;
class DescriptorSetLayout;
class Framebuffer;
class RenderPass;
class CommandPool;
class CommandBuffer;
class PipelineCache;
class PipelineLayout;
class Fence;
class Semaphore;
class Event;

class Instance;
class Device;
class Queue;
class Renderer;
class DefaultRenderer;
class DeviceMemoryAllocator;
class MemoryEntry;
class ViewableImage;

class WorkManager;
class SharedBuffer;
class SubBuffer;
class CommandAllocator;
class BufferAllocator;
class QueueSubmitter;
class BufferSpan;

class TrDsLayout;
class TrDsPool;
class TrDs;
class DescriptorAllocator;

struct ViewableImageCreateInfo;

template<typename T> class CommandWork;
template<typename T> class TransferWork;
class DownloadWork;
class UploadWork;

template<typename T> class ThreadStorage;
struct DynamicStorageBase;

} // namespace vpp
