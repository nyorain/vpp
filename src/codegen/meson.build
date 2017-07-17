codegen_src = [
	'main.cpp',
	'output.cpp',
	'registry.cpp',
	'pugixml/pugixml.cpp']

codegen_inc = include_directories('.')

executable('vk-codegen',
	codegen_src,
	cpp_args: '-DVPP_OLD_LENGTH_PARAM=1',
	include_directories: codegen_inc)

# TODO: custom command and stuff
