#[[
add_executable(vpptest main.cpp mesh.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(ASSIMP REQUIRED)

target_include_directories(vpptest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(vpptest PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(vpptest ${ASSIMP_LIBRARIES})
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

add_custom_target(shaders ALL
	DEPENDS "${CMAKE_BINARY_DIR}/vert.spv" "${CMAKE_BINARY_DIR}/frag.spv")

add_dependencies(vpptest shaders)
]]



add_executable(vpptest main.cpp)

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

add_custom_target(shaders ALL
	DEPENDS "${CMAKE_BINARY_DIR}/vert.spv" "${CMAKE_BINARY_DIR}/frag.spv")

add_dependencies(vpptest shaders)
