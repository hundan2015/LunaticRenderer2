set(CMAKE_CXX_STANDARD 20)
add_definitions(-DEBUG_BARRIER=1)
aux_source_directory(. DIR_SYSTEM)
add_library(systems STATIC ${DIR_SYSTEM})