project('vpp', ['c', 'cpp'],
	license: 'BSL',
		version: '0.3.0',
		meson_version: '>=0.46',
	default_options: [
		'cpp_std=c++17',
		'warning_level=3',
  ])

# options
example_glfw = get_option('example_glfw')
example_ny = get_option('example_ny')
tests = get_option('tests')
one_device = get_option('one_device')

shared = (get_option('default_library') == 'shared')

cc = meson.get_compiler('cpp')

# default arrguments
# warnings and stuff
arguments = [
	'-Wno-unused-parameter',
	'-Wno-missing-braces',
	'-fvisibility=hidden'
]

add_project_arguments(
	cc.get_supported_arguments(arguments),
	language: 'cpp')

# dependencies
dep_threads = dependency('threads')
dep_dlg = dependency('dlg', fallback: ['dlg', 'dlg_dep'])
dep_vkpp = dependency('vkpp', fallback: ['vkpp', 'vkpp_dep'])
dep_vulkan = dependency('vulkan')

deps = [
	dep_threads,
	dep_dlg,
	dep_vkpp,
	dep_vulkan]

vpp_inc = include_directories('include')
vpp_inc_private = include_directories('src')

subdir('include/vpp')
subdir('src/vpp')

# declare dependency
vpp_dep = declare_dependency(
	include_directories: vpp_inc,
	dependencies: deps,
	link_with: vpp_lib)

# examples, test
# must come after dependency
subdir('docs/examples')

if tests
	subdir('docs/tests')
endif

# pkgconfig
pkg = import('pkgconfig')
pkg_dirs = ['.']
pkg.generate(
	vpp_lib,
	name: 'vpp',
	requires: ['vulkan'],
	filebase: 'vpp',
	subdirs: pkg_dirs,
	version: meson.project_version(),
	description: 'Vulkan utility library')
