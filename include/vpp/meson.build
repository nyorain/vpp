vpp_headers = [
	'allocator.hpp',
	'buffer.hpp',
	'bufferOps.hpp',
	'commandBuffer.hpp',
	'debug.hpp',
	'descriptor.hpp',
	'device.hpp',
	'framebuffer.hpp',
	'fwd.hpp',
	'image.hpp',
	'init.hpp',
	'instance.hpp',
	'memory.hpp',
	'memoryMap.hpp',
	'memoryResource.hpp',
	'physicalDevice.hpp',
	'pipeline.hpp',
	'procAddr.hpp',
	'queue.hpp',
	'renderer.hpp',
	'renderPass.hpp',
	'resource.hpp',
	'shader.hpp',
	'sharedBuffer.hpp',
	'submit.hpp',
	'surface.hpp',
	'swapchain.hpp',
	'sync.hpp',
	'vk.hpp',
	'vpp.hpp',
	'work.hpp']

vpp_headers_bits = [
	'bits/bufferOps.inl',
	'bits/vulkanTypes.inl',
	'bits/work.inl']

vpp_headers_util = [
	'util/allocation.hpp',
	'util/file.hpp',
	'util/nonCopyable.hpp',
	'util/sharedLock.hpp',
	'util/span.hpp',
	'util/threadStorage.hpp',
	'util/tmp.hpp']

vpp_headers_vulkan = [
	'vulkan/enums.hpp',
	'vulkan/error.hpp',
	'vulkan/flags.hpp',
	'vulkan/functions.hpp',
	'vulkan/fwd.hpp',
	'vulkan/handle.hpp',
	'vulkan/span.hpp',
	'vulkan/structs.hpp',
	'vulkan/vulkan.hpp']

install_headers(vpp_headers, subdir: 'vpp')
install_headers(vpp_headers_bits, subdir: 'vpp/bits')
install_headers(vpp_headers_util, subdir: 'vpp/util')

# config file
version = meson.project_version().split('.')

conf_data = configuration_data()
conf_data.set('vmajor', version[0])
conf_data.set('vminor', version[1])
conf_data.set('vpatch', version[2])
conf_data.set('VPP_ONE_DEVICE_OPTIMIZATION', one_device)

configure_file(input: 'config.hpp.in',
               output: 'config.hpp',
							 install_dir: 'include/vpp',
               configuration: conf_data)
