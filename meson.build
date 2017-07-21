project('vpp', 'cpp',
  license: 'BSL',
	version: '0.1.0',
	meson_version: '>=0.38',
  default_options: ['cpp_std=c++1z'])

# options
examples = get_option('examples')
tests = get_option('tests')
codegen = get_option('codegen')
one_device = get_option('one_device')
vulkan_sdk = get_option('vulkan_sdk')

# default arrguments
# warnings and stuff
add_project_arguments(
	'-Wall',
	'-Wextra',
	'-Wno-unused-parameter',
	'-Wpedantic',
	'-Wno-missing-braces',
	language: 'cpp')

# vulkan lib name
if build_machine.system() == 'windows'
	vulkan_lib = 'vulkan-1'
else
	vulkan_lib = 'vulkan'

	# we only do this on linux since on windows the paths
	# uses backslashes which are not (or cannot be) handled by meson/ninja
	add_project_arguments(
		'-DDLG_BASE_PATH="@0@"'.format(meson.source_root()),
		language: 'cpp')
endif

# try to find vulkan library
vulkan_inc_arg = ''
if vulkan_sdk != 'none'
	if meson.get_compiler('cpp').sizeof('void*') == 8
		vulkan_lib_path = join_paths(vulkan_sdk, 'Lib')
	else
		vulkan_lib_path = join_paths(vulkan_sdk, 'Lib32')
	endif

	vulkan_inc_arg = '-I' + join_paths(vulkan_sdk, 'Include')
	add_project_arguments(vulkan_inc_arg, language: 'cpp')
	dep_vulkan = meson.get_compiler('cpp').find_library(vulkan_lib, dirs: vulkan_lib_path)
else
	dep_vulkan = meson.get_compiler('cpp').find_library(vulkan_lib)
endif

# dependencies
dep_threads = dependency('threads')
dep_dlg = dependency('dlg', fallback: ['dlg', 'dlg_dep'])

deps = [
	dep_threads,
	dep_dlg,
	dep_vulkan]

vpp_inc = include_directories('include')
vpp_inc_private = include_directories('src')

# subdirs
if codegen != 'none'
	subdir('src/codegen')
else
	vk_headers = []
endif

subdir('include/vpp')
subdir('src/vpp')

# declare dependency
vpp_dep = declare_dependency(
	include_directories: [vpp_inc],
	dependencies: deps,
	extra_cargs: vulkan_inc_arg,
	link_with: vpp_lib)

# examples, test
# must come after dependency
if examples
	subdir('doc/examples')
endif

if tests
	subdir('doc/tests')
endif

# pkgconfig
# TODO: make sure requires is correct
# test the packageconfig with an external project
pkg = import('pkgconfig')
pkg_dirs = ['vpp']
pkg.generate(
	name: 'vpp',
	requires: ['vulkan'],
	libraries: vpp_lib,
	filebase: 'vpp',
	subdirs: pkg_dirs,
	version: meson.project_version(),
	description: 'Vulkan utility library')
