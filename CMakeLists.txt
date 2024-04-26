cmake_minimum_required(VERSION 3.0)
project(SpaceInvaders)

# Find and include ImGui
file(GLOB IMGUI_SOURCES imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS imgui/*.h imgui/backends/imgui_impl_glfw.h)
include_directories(imgui imgui/backends)
add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# Include directories for SpaceInvaders headers
include_directories(include)

# Find and include GLFW3 library using pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Find OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

# Find all source files in the src directory
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add the executable
add_executable(space_invaders ${SRC_FILES})

# Link GLFW library
target_link_libraries(space_invaders ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

# Link ImGui library
target_link_libraries(space_invaders imgui)
