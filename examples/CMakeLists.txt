include_directories("${CMAKE_CURRENT_SOURCE_DIR}") #mainly for nytl, example.hpp

#function to easily bake shaders using glslandValidator
function(add_shader name project)
	add_custom_command(
		OUTPUT "${CMAKE_BINARY_DIR}/${name}.spv"
		COMMAND glslangValidator
					-V "${CMAKE_CURRENT_SOURCE_DIR}/${name}"
					-o "${name}.spv"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${name}"
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	COMMENT "Building spirv shader - ${name}")

	add_custom_target(shader_${name} ALL DEPENDS "${CMAKE_BINARY_DIR}/${name}.spv")
	add_dependencies(${project} shader_${name})
endfunction(add_shader)

#add included tests
add_subdirectory(particles)
add_subdirectory(fragment)
add_subdirectory(texture)
add_subdirectory(font)
