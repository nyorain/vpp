CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(vpp)

option(build-tests "Build Tests" OFF)
option(debug "Compile in debug mode" ON)
option(debug-throw "Throw debug exceptions instead of printing warnings" ON)

if(debug)
	set(CMAKE_BUILD_TYPE Debug)
	add_definitions(-DVPP_DEBUG)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

if(debug-throw)
	add_definitions(-DVPP_DEBUG_THROW)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

#to disable ninja byproduct warning
cmake_policy(SET CMP0058 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)

install(DIRECTORY external/boost DESTINATION include)

if(WIN32 OR MSYS OR MINGW)
	set(VPP_WINDOWS 1)
else()
	set(VPP_WINDOWS 0)
endif()

if(${VPP_WINDOWS})
	include_directories("$ENV{VK_SDK_PATH}/Include")
	set(vulkan-lib "$ENV{VK_SDK_PATH}/Source/lib/vulkan-1.dll")
else()
	include_directories("/usr/include/vulkan") #needed?
	set(vulkan-lib "vulkan")
endif()

add_subdirectory(codegen)
add_subdirectory(include/vpp)
add_subdirectory(src/vpp)

if(build-tests)
	add_subdirectory(tests)
endif()
