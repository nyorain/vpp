add_executable(vpptest main.cpp particles.cpp)

target_include_directories(vpptest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vpptest
    vpp
    "C:/VulkanSDK/1.0.3.1/Source/lib/vulkan-1.dll"
    "gdi32"
    "kernel32")

add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/frag.spv"
	COMMAND glslangValidator -V "${CMAKE_CURRENT_SOURCE_DIR}/shader.frag"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shader.frag"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	COMMENT "Building spirv shaders - fragment")

add_custom_command(
	OUTPUT  "${CMAKE_BINARY_DIR}/vert.spv"
	COMMAND glslangValidator -V "${CMAKE_CURRENT_SOURCE_DIR}/shader.vert"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shader.vert"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	COMMENT "Building spirv shaders - vertex")

add_custom_command(
	OUTPUT  "${CMAKE_BINARY_DIR}/comp.spv"
	COMMAND glslangValidator -V "${CMAKE_CURRENT_SOURCE_DIR}/shader.comp"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/shader.comp"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	COMMENT "Building spirv shaders - compute")

add_custom_target(shaders ALL
	DEPENDS
		"${CMAKE_BINARY_DIR}/vert.spv"
		"${CMAKE_BINARY_DIR}/frag.spv"
		"${CMAKE_BINARY_DIR}/comp.spv")

add_dependencies(vpptest shaders)
