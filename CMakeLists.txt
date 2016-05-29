CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(vpp)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32 OR MSYS OR MINGW)
	set(VPP_WINDOWS 1)
else()
	set(VPP_WINDOWS 0)
endif()

if(${VPP_WINDOWS})
	include_directories("$ENV{VK_SDK_PATH}/Include")
	set(vulkan-lib "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")
else()
	include_directories("/usr/include/vulkan")
	set(vulkan-lib "vulkan")
endif()

add_subdirectory(codegen)
add_subdirectory(include/vpp)
add_subdirectory(src/vpp)
add_subdirectory(tests)
