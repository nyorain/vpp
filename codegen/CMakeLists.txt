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

set(outputs
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/vulkan.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/call.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/enums.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/functions.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/structs.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/strings.hpp")

add_custom_command(
	OUTPUT ${outputs}
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/vulkan" "${CMAKE_SOURCE_DIR}/include/vpp/vulkan"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vulkan.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/vulkan/call.hpp"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "copying vkcpp headers")

add_custom_target(VkHeader ALL
	#COMMAND  ${CMAKE_COMMAND} -E echo yoyoyo
	DEPENDS "${CMAKE_SOURCE_DIR}/include/vpp/vulkan/vulkan.hpp")
