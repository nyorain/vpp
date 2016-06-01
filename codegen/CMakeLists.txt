#codegen executable
add_executable(vkGenerator
	output.cpp
	registry.cpp
	main.cpp
	pugixml/pugixml.cpp)

#target_include_directories(vkGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set(output-dir "${CMAKE_SOURCE_DIR}/include/vpp/vulkan")

#command to copy the pre files to the vulkan codegen dir
add_custom_command(
	OUTPUT
		"${output-dir}/flags.hpp"
		"${output-dir}/call.hpp"
		"${output-dir}/range.hpp"
	COMMAND
		${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/pre"
		"${output-dir}"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/flags.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/call.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/range.hpp"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "Copying vulkan headers dependency files")

#message(STATUS "${output-dir}/functions.hpp")

set(outputs
	"${output-dir}/vulkan.hpp"
	"${output-dir}/enums.hpp"
	"${output-dir}/fwd.hpp"
	"${output-dir}/functions.hpp"
	"${output-dir}/structs.hpp")

add_custom_command(
	OUTPUT ${outputs}
  	COMMAND vkGenerator "${CMAKE_CURRENT_SOURCE_DIR}/vk.xml"
    DEPENDS vkGenerator
	WORKING_DIRECTORY "${output-dir}"
	COMMENT "Generating vulkan hpp headers")

add_custom_target(vkHeaders ALL
	COMMENT "Generating vulkan headers"
	DEPENDS
		${outputs}
		"${output-dir}/flags.hpp"
		"${output-dir}/range.hpp"
		"${output-dir}/call.hpp")
