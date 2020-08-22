vpp_sources = [
	'buffer.cpp',
	'bufferOps.cpp',
	'commandAllocator.cpp',
	'debug.cpp',
	'debugReport.cpp',
	'devMemAllocator.cpp',
	'descriptor.cpp',
	'device.cpp',
	'formats.cpp',
	'handles.cpp',
	'image.cpp',
	'imageOps.cpp',
	'memory.cpp',
	'memoryMap.cpp',
	'memoryResource.cpp',
	'physicalDevice.cpp',
	'pipeline.cpp',
	'queue.cpp',
	'renderer.cpp',
	'resource.cpp',
	'shader.cpp',
	'sharedBuffer.cpp',
	'submit.cpp',
	'swapchain.cpp',
	'trackedDescriptor.cpp',
	'util/file.cpp']

vpp_args = ['-DDLG_DEFAULT_TAGS="vpp"']
if host_machine.system() == 'windows' or host_machine.system().contains('mingw')
	if shared
		vpp_args += '-DVPP_API=__declspec(dllexport)'
	endif
endif

vpp_lib = library('vpp',
	sources: vpp_sources,
	dependencies: deps,
	cpp_args: vpp_args,
	include_directories: [vpp_inc, vpp_inc_private],
	install: true)
