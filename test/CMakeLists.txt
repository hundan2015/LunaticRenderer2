find_library(lunatic_core "../lunatic_engine/core")
find_library(LIB_COMPONENT "../lunatic_engine/function")
include_directories("../lunatic_engine/function")

add_executable(EntityManagerTestT EntityManagerTest.cpp JsonDemo.cpp)
target_link_libraries(EntityManagerTestT PRIVATE lunatic_core)
target_link_libraries(EntityManagerTestT PRIVATE LIB_COMPONENT)

add_executable(RenderingSystemTestT RenderingSystemTest.cpp)
#target_link_libraries(RenderingSystemTestT PUBLIC lunatic_core)
target_link_libraries(RenderingSystemTestT PUBLIC LIB_COMPONENT)

add_executable(GetWorldTransformTest GetWorldTransformTest.cpp)
target_link_libraries(GetWorldTransformTest PRIVATE lunatic_core)
target_link_libraries(GetWorldTransformTest PRIVATE LIB_COMPONENT)

add_executable(JsonDemo JsonDemo.cpp)
target_link_libraries(JsonDemo PRIVATE lunatic_core)
target_link_libraries(JsonDemo PRIVATE LIB_COMPONENT)

add_executable(MemoryDemo MemoryDemo.cpp)
target_link_libraries(MemoryDemo PRIVATE lunatic_core)
target_link_libraries(MemoryDemo PRIVATE LIB_COMPONENT)

# Copy method.
add_custom_command(TARGET RenderingSystemTestT
        POST_BUILD
        COMMAND echo "Move ${CMAKE_SOURCE_DIR} to ${CMAKE_CURRENT_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/lunatic_engine/assets/ $<TARGET_FILE_DIR:RenderingSystemTestT>/assets
        )