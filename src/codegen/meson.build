# codegen executable
codegen_src = [
	'main.cpp',
	'output.cpp',
	'registry.cpp',
	'pugixml/pugixml.cpp']

codegen_inc = include_directories('.')

defs = []
if codegen == '1.0.20'
	defs += '-DVPP_OLD_LENGTH_PARAM=1'
	defs += '-DVPP_NO_VALUES_STRUCTURE_TYPE=1'
	spec = 'vk-1-0-20.xml'
elif codegen == '1.0.40'
	defs += '-DVPP_NO_VALUES_STRUCTURE_TYPE=1'
	spec = 'vk-1-0-40.xml'
elif codegen == 'latest'
	spec = 'vk-1-0-latest.xml'
else
	error('Invalid/unsupported vulkan spec version')
endif

vk_codegen = executable('vk-codegen',
	codegen_src,
	cpp_args: defs,
	include_directories: codegen_inc)

output_dir = join_paths([meson.source_root(), 'include', 'vpp', 'vulkan'])
outputs = [
	join_paths([output_dir, 'enums.hpp']),
	join_paths([output_dir, 'flags.hpp']),
	join_paths([output_dir, 'functions.hpp']),
	join_paths([output_dir, 'fwd.hpp']),
	join_paths([output_dir, 'handle.hpp']),
	join_paths([output_dir, 'span.hpp']),
	join_paths([output_dir, 'structs.hpp']),
	join_paths([output_dir, 'vulkan.hpp'])]

# custom target
vk_headers = custom_target('vk-headers',
	input: spec,
	output: 'vk-headers',
	capture: true,
	command: [vk_codegen, '@INPUT@', output_dir])