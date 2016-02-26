add_executable(VkCppGenerator
	VkCppGenerator.cpp
	tinyxml2/tinyxml2.cpp
)

add_custom_command(
	OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vulkan.hpp"
  	COMMAND VkCppGenerator
    DEPENDS VkCppGenerator
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating vkcpp headers in ${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(
	OUTPUT "${CMAKE_SOURCE_DIR}/include/vpp/vulkan/vulkan.hpp"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/vulkan" "${CMAKE_SOURCE_DIR}/include/vpp/vulkan"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vulkan.hpp"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "copying vkcpp headers"
	SOURCES VkCppGenerator.cpp)

add_custom_target(VkHeader DEPENDS "${CMAKE_SOURCE_DIR}/include/vpp/vulkan/vulkan.hpp")
