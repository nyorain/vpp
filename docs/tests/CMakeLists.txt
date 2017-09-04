# meta-target for all tests
add_custom_target(tests)

# function to create test
function(create_test name)
	add_executable(${name} ${name}.cpp)
	target_link_libraries(${name} vpp ${Vulkan_LIBRARY})
	add_dependencies(tests ${name})
	add_test(NAME ${name}
		COMMAND "${name}"
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endfunction()

create_test(memory)
create_test(copy)
create_test(allocator)
create_test(bufferOps)
