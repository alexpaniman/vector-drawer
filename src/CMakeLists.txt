add_executable(vector-drawer vector-drawer.cpp)

target_include_directories(vector-drawer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(vector-drawer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

target_link_libraries(vector-drawer gl)
