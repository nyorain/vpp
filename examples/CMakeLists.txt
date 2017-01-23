# function to easily bake shaders using glslandValidator
function(add_shader name path project)
	add_custom_command(
		OUTPUT "${CMAKE_BINARY_DIR}/${name}.spv"
		COMMAND glslangValidator
					-V "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
					-o "${name}.spv"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${path}"
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	COMMENT "Building spirv shader - ${name}")

	add_custom_target(shader_${name} ALL DEPENDS "${CMAKE_BINARY_DIR}/${name}.spv")
	add_dependencies(${project} shader_${name})
endfunction(add_shader)

# intro
add_executable(vpp-intro intro.cpp)
target_link_libraries(vpp-intro vpp ${Vulkan_LIBRARY})

add_shader("intro.frag" "data/intro.frag" vpp-intro)
add_shader("intro.vert" "data/intro.vert" vpp-intro)
