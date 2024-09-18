cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
option(PRODUCTION_BUILD "Make this a Production Build" OFF)

project(Platformer)

# Enable Link Time Optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Executables
add_executable(${CMAKE_PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/engine/src/Main.cpp")

# Paths
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/game/src")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine/include/tenshiUtil")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw")

# Include Paths
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/single_include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include")

# Linking
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glfw GLEW::glew OpenGL::GL)

message("Using Compiler: " ${CMAKE_CXX_COMPILER_ID})
