cmake_minimum_required(VERSION 3.0.0)
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    set(VCPKG_ROOT "~/vcpkg")
    include_directories("${VCPKG_ROOT}/installed/x64-linux/include")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    set(VCPKG_ROOT "D:/dev/vcpkg")
    include_directories("${VCPKG_ROOT}/installed/x64-windows/include")
ENDIF()
set(VCPKG_ROOT "~/vcpkg")
#set(VCPKG_ROOT "D:/dev/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_GENERATOR "Ninja")
add_compile_options(-fno-elide-constructors)
set(CMAKE_CXX_STANDARD 20)
project(TestProject VERSION 0.1.0 LANGUAGES CXX)
#include_directories("${VCPKG_ROOT}/installed/x64-windows/include")
include(CTest)
enable_testing()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W3 /WX)
endif ()

add_executable(TestProject lunatic_engine/main.cpp)
find_package(glm CONFIG REQUIRED)

# find_package(glfw3 CONFIG REQUIRED)
# find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

# find_package(Boost COMPONENTS system REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(DCMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("lunatic_engine/function")
add_subdirectory("./lunatic_engine/core")
add_subdirectory("./test")

# add_subdirectory("./GTestTest")
target_link_libraries(TestProject PUBLIC lunatic_core)
target_link_libraries(TestProject PUBLIC LIB_COMPONENT)
target_link_libraries(TestProject PUBLIC fmt::fmt-header-only)

# target_link_libraries(TestProject PRIVATE glad::glad)
# target_link_libraries(TestProject PRIVATE glfw)
target_link_libraries(TestProject PRIVATE imgui::imgui)
target_link_libraries(TestProject PRIVATE nlohmann_json::nlohmann_json)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND echo "Move"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lunatic_engine/assets ${CMAKE_BUILD_RPATH}/assets
        )


# target_link_libraries(TestProject PRIVATE Boost)
