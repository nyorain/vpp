project('vpp', ['c', 'cpp'],
  license: 'BSL',
	version: '0.3.0',
	meson_version: '>=0.42',
  default_options: ['cpp_std=c++1z'])

# options
examples = get_option('examples')
tests = get_option('tests')
one_device = get_option('one_device')

# default arrguments
# warnings and stuff
add_project_arguments(
	'-Wall',
	'-Wextra',
	'-Wno-unused-parameter',
	'-Wpedantic',
	'-Wno-missing-braces',
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
if examples
	subdir('docs/examples')
endif

if tests
	subdir('docs/tests')
endif

# pkgconfig
pkg = import('pkgconfig')
pkg_dirs = ['.']
pkg.generate(
	name: 'vpp',
	requires: ['vulkan'],
	libraries: vpp_lib,
	filebase: 'vpp',
	subdirs: pkg_dirs,
	version: meson.project_version(),
	description: 'Vulkan utility library')
