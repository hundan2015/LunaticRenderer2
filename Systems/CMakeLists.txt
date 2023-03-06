set(CMAKE_CXX_STANDARD 20)
add_definitions(-DEBUG_BARRIER=1)
aux_source_directory(. DIR_SYSTEM)


add_library(systems STATIC ${DIR_SYSTEM} ImguiManager.h)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(systems PRIVATE glad::glad)
target_link_libraries(systems PRIVATE glfw)
