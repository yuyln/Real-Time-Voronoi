cmake_minimum_required(VERSION 3.20.0)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

project(voronoi)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.c)

target_include_directories(${PROJECT_NAME} PUBLIC ./libs/OpenCL/include)
include_directories( ${OPENGL_INCLUDE_DIRS} )
add_subdirectory(./libs/glfw)

target_link_libraries(${PROJECT_NAME} glfw OpenCL)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

target_compile_options(${PROJECT_NAME} PUBLIC -ggdb -Ofast -Wall -Wextra -Werror -pedantic -fopenmp)
target_link_directories(${PROJECT_NAME} PUBLIC ./libs/OpenCL/lib)
