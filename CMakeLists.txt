cmake_minimum_required(VERSION 3.13)
project(gltut)
set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(gltut
        src/main.cpp
     #   src/ObjectManager.cpp
)

find_package(OpenGL REQUIRED)

target_include_directories(gltut PRIVATE
        ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/include
        ${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include
        ${CMAKE_SOURCE_DIR}/Dependencies/spdlog
        ${CMAKE_SOURCE_DIR}/Dependencies/glm
        ${CMAKE_SOURCE_DIR}/Dependencies/stb
        ${CMAKE_SOURCE_DIR}/src
        ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries(gltut PRIVATE
        ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/glfw3.lib
        ${CMAKE_SOURCE_DIR}/Dependencies/GLEW/glew32s.lib
        OpenGL::GL
)

target_compile_definitions(gltut PRIVATE GLEW_STATIC)





