cmake_minimum_required(VERSION 3.17)

project(gzz C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


add_subdirectory(third_party)

add_compile_options(-Wall -pedantic)

add_subdirectory(src)

find_package(OpenGL REQUIRED)

target_link_libraries(gzz stb_image cglm glad glfw OpenGL::GL)
