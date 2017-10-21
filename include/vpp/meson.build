install_subdir('.', 
	install_dir: 'include/vpp',
	exclude_files: ['meson.build', 'config.hpp.in'])

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
