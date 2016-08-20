add_executable(modelViewer modelViewer.cpp)
target_link_libraries(modelViewer vpp ${ASSIMP_LIBRARIES})
target_include_directories(modelViewer PUBLIC ${ASSIMP_INCLUDE_DIRS})

add_shader("model.frag" modelViewer)
add_shader("model.vert" modelViewer)
