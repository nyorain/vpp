CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)
PROJECT(vpp)

set(VMajor 0)
set(VMinor 1)
set(VPatch 0)

include(CMakeDependentOption)

option(BuildExamples "Build Examples" on)
option(Debug "Compile in debug mode" on)
option(OneDevice "Enable the one device optimization. Not recommended" off)

#DebugThrow option can only be set if debug is enabled
cmake_dependent_option(DebugThrow
	"Throw debug exceptions instead of printing warnings" on
	"Debug" off)

if(Debug)
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)

#enable warnings
#for msvc we first check if some level is set by default and replace it
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
	  list(APPEND CMAKE_CXX_FLAGS "/W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	list(APPEND CMAKE_CXX_FLAGS "-Wall -Wno-long-long")
endif()

#to disable ninja byproduct warning
cmake_policy(SET CMP0058 NEW)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include) #e.g. config file

install(DIRECTORY external/boost DESTINATION include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/cmake")
find_package(Vulkan REQUIRED)

add_subdirectory(codegen)
add_subdirectory(include/vpp)
add_subdirectory(src/vpp)

#config file
set(VPP_DEBUG ${Debug})
set(VPP_DEBUG_THROW ${DebugThrow})
set(VPP_ONE_DEVICE_OPTIMIZATION ${OneDevice})

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/src/vpp/config.hpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/vpp/config.hpp")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/vpp/config.hpp" DESTINATION include/vpp)

if(BuildExamples)
	if(Win32 OR MSYS OR MINGW)
		add_subdirectory(examples)
	else()
		message("The examples are windows-only at the moment. Pull requests appreciated.")
	endif()
endif()
