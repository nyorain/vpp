#codegen executable
add_executable(vkGenerator
	output.cpp
	registry.cpp
	main.cpp
	pugixml/pugixml.cpp)

# old vulkan version
target_compile_definitions(vkGenerator PRIVATE VPP_OLD_LENGTH_PARAM=1)

#target_include_directories(vkGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set(output-dir "${CMAKE_SOURCE_DIR}/include/vpp/vulkan")

set(outputs
	"${output-dir}/vulkan.hpp"
	"${output-dir}/enums.hpp"
	"${output-dir}/fwd.hpp"
	"${output-dir}/functions.hpp"
	"${output-dir}/structs.hpp")

set(pres
	"${output-dir}/flags.hpp"
	"${output-dir}/span.hpp"
	"${output-dir}/handle.hpp"
	"${output-dir}/error.hpp")

#command to copy the pre files to the vulkan codegen dir
add_custom_command(
	OUTPUT ${pres}
	COMMAND
		${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/pre"
		"${output-dir}"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/flags.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/error.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/handle.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/pre/span.hpp"
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	COMMENT "Copying vulkan headers dependency files")

# check that the xml spec exists
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vk-${ApiVersion}.xml")
	message(SEND_ERROR "Invalid ApiVersion given. Cannot find spec for ${ApiVersion}")
endif()

add_custom_command(
	OUTPUT ${outputs}
	COMMAND vkGenerator "${CMAKE_CURRENT_SOURCE_DIR}/vk-${ApiVersion}.xml"
	DEPENDS
		vkGenerator
		"${CMAKE_CURRENT_SOURCE_DIR}/vk-${ApiVersion}.xml"
	WORKING_DIRECTORY "${output-dir}"
	COMMENT "Generating vulkan hpp headers")

add_custom_target(vkHeaders ALL
	COMMENT "Generating vulkan headers"
	DEPENDS ${outputs} ${pres})
