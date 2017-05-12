function(create_test name)
	add_executable(${name} ${name}.cpp)
	target_link_libraries(${name} vpp ${Vulkan_LIBRARY})
	add_test(${name} "bin/${name}")
endfunction()

create_test(memory)
create_test(copy)
