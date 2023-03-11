find_library(lunatic_core "../lunatic_engine/core")
find_library(LIB_COMPONENT "../lunatic_engine/function")
include_directories("../lunatic_engine/function")

add_executable(EntityManagerTestT EntityManagerTest.cpp)
target_link_libraries(EntityManagerTestT PRIVATE lunatic_core)
target_link_libraries(EntityManagerTestT PRIVATE LIB_COMPONENT)

add_executable(RenderingSystemTestT RenderingSystemTest.cpp)
target_link_libraries(RenderingSystemTestT PRIVATE lunatic_core)
target_link_libraries(RenderingSystemTestT PRIVATE LIB_COMPONENT)
