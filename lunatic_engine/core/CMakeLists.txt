set(CMAKE_CXX_STANDARD 20)
add_definitions(-DEBUG_BARRIER=1)
aux_source_directory(. DIR_SYSTEM)

add_library(lunatic_core STATIC ${DIR_SYSTEM})
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(lunatic_core PRIVATE assimp::assimp)
target_link_libraries(lunatic_core PRIVATE glad::glad)
target_link_libraries(lunatic_core PRIVATE glfw)
