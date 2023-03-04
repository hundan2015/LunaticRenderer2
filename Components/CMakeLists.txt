aux_source_directory(. DIR_COPONENT)
aux_source_directory(.hpp DIR_COPONENT_HPP)
add_library(LIB_COMPONENT STATIC ${DIR_COPONENT} ${DIR_COPONENT_HPP})
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(LIB_COMPONENT PRIVATE glad::glad)
target_link_libraries(LIB_COMPONENT PRIVATE glfw)

add_executable(EntityManagerTest test/EntityManagerTest.cpp)
target_link_libraries(EntityManagerTest PUBLIC LIB_COMPONENT)

add_executable(RenderingSystemTest test/RenderingSystemTest.cpp)
target_link_libraries(RenderingSystemTest PUBLIC LIB_COMPONENT)
