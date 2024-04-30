cmake_minimum_required(VERSION 3.0)
project(SpaceInvaders)

# Find and include ImGui
set(IMGUI_DIR "dependencies/imgui")
file(GLOB IMGUI_SOURCES
	${IMGUI_DIR}/*.cpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
file(GLOB IMGUI_HEADERS
	${IMGUI_DIR}/*.h
	${IMGUI_DIR}/backends/imgui_impl_glfw.h
)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# # Find stb_image header file
# find_path(STB_IMAGE_INCLUDE_DIR stb_image.h)
# if(STB_IMAGE_INCLUDE_DIR)
#     message(STATUS "Found stb_image.h: ${STB_IMAGE_INCLUDE_DIR}")
# else()
#     message(FATAL_ERROR "stb_image.h not found")
# endif()
# # Include the directory containing the stb_image header file
# include_directories(${STB_IMAGE_INCLUDE_DIR})

# Include directories for SpaceInvaders headers
include_directories(include)

# # Set the path to the directory containing Eigen headers
# set(EIGEN_INCLUDE_DIR "/usr/include/eigen3/")

# Find Eigen include directory
find_path(EIGEN_INCLUDE_DIR NAMES Eigen/Core PATHS /usr/include /usr/local/include /opt/local/include)
if(EIGEN_INCLUDE_DIR)
    message(STATUS "Found Eigen: ${EIGEN_INCLUDE_DIR}")
    include_directories(${EIGEN_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Eigen not found")
endif()
include_directories(${EIGEN_INCLUDE_DIR})

# Find and include GLFW3 library using pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Find OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Find all source files in the src directory
file(GLOB_RECURSE SRC_FILES src/*.cpp)
# set(SRC_FILES src/main.cpp src/scene.cpp src/common.cpp)

# Add the executable
add_executable(space_invaders ${SRC_FILES})

# Link GLFW library
target_link_libraries(space_invaders ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

# Link ImGui library
target_link_libraries(space_invaders imgui)
