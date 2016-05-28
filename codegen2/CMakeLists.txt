add_executable(vkGenerator
	output.cpp
	registry.cpp
	main.cpp
	pugixml/pugixml.cpp
)

target_include_directories(vkGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(
	OUTPUT
		"${CMAKE_CURRENT_SOURCE_DIR}/output/flags.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/output/call.hpp"
	COMMAND
		${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/pre"
		"${CMAKE_CURRENT_SOURCE_DIR}/output"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/flags.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/call.hpp"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "Copying vulkan headers dependency files")

add_custom_command(
	OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/output/vulkan.hpp"
  	COMMAND vkGenerator
    DEPENDS vkGenerator
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "Generating vulkan hpp headers in ${CMAKE_CURRENT_SOURCE_DIR}")

set(outputs
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/vulkan.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/call.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/enums.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/functions.hpp"
	"${CMAKE_SOURCE_DIR}/include/vpp/vulkan/structs.hpp")

add_custom_command(
	OUTPUT ${outputs}
	COMMAND
		${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/output"
		"${CMAKE_SOURCE_DIR}/include/vpp/vulkan"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/output/vulkan.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/output/call.hpp"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "copying vkcpp headers")

add_custom_target(VkHeader ALL
	COMMENT "Generating vulkan headers"
	DEPENDS "${CMAKE_SOURCE_DIR}/include/vpp/vulkan/vulkan.hpp")
