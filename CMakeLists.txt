cmake_minimum_required(VERSION 3.0.0)
set(VCPKG_ROOT "D:/dev/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_GENERATOR "Ninja")

set(CMAKE_CXX_STANDARD 20)
project(TestProject VERSION 0.1.0 LANGUAGES CXX)
include_directories("${VCPKG_ROOT}/installed/x64-windows/include")
include(CTest)
enable_testing()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W3 /WX)
endif ()

add_executable(TestProject lunatic_engine/main.cpp lunatic_engine/editor/ImguiManager.h)
find_package(glm CONFIG REQUIRED)

# find_package(glfw3 CONFIG REQUIRED)
# find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

# find_package(Boost COMPONENTS system REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(DCMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("lunatic_engine/function/Components")
add_subdirectory("./lunatic_engine/core")

# add_subdirectory("./GTestTest")
target_link_libraries(TestProject PUBLIC lunatic_core)
target_link_libraries(TestProject PUBLIC LIB_COMPONENT)

# target_link_libraries(TestProject PRIVATE glad::glad)
# target_link_libraries(TestProject PRIVATE glfw)
target_link_libraries(TestProject PRIVATE imgui::imgui)
target_link_libraries(TestProject PRIVATE nlohmann_json::nlohmann_json)

# target_link_libraries(TestProject PRIVATE Boost)
