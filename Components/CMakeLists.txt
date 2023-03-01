aux_source_directory(. DIR_COPONENT)
add_library(LIB_COMPONENT STATIC ${DIR_COPONENT})
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(LIB_COMPONENT PRIVATE glad::glad)
target_link_libraries(LIB_COMPONENT PRIVATE glfw)

