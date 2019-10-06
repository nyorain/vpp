if example_ny
	ny_dep = dependency('ny', fallback: ['ny', 'ny_dep'])
	executable('intro_ny',
		sources: ['intro_ny.cpp', 'render.cpp'],
		dependencies: [ny_dep, vpp_dep])
endif

if example_glfw
	glfw_dep = dependency('glfw3')
	executable('intro_glfw',
		sources: ['intro_glfw.cpp', 'render.cpp'],
		dependencies: [glfw_dep, vpp_dep])
endif
