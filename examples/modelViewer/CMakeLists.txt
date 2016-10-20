add_executable(modelViewer modelViewer.cpp)
target_include_directories(modelViewer PUBLIC ${Vulkan_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(modelViewer vpp ${Vulkan_LIBRARY} ${ASSIMP_LIBRARIES})

add_shader("model.frag" modelViewer)
add_shader("model.vert" modelViewer)

add_shader("modelOffscreen.frag" modelViewer)
add_shader("modelOffscreen.vert" modelViewer)

add_shader("modelQuad.frag" modelViewer)
add_shader("modelQuad.vert" modelViewer)
