cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
option(PRODUCTION_BUILD "Make this a Production Build" OFF)

project(Platformer)

# Enable Link Time Optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Paths
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw")

# Executables
file(GLOB_RECURSE engine_sources "${CMAKE_CURRENT_SOURCE_DIR}/engine/src/*.cpp")
file(GLOB_RECURSE util_sources "${CMAKE_CURRENT_SOURCE_DIR}/engine/include/tenshiUtil/*.cpp")

file(GLOB_RECURSE game_sources "${CMAKE_CURRENT_SOURCE_DIR}/game/src/*.cpp")
file(GLOB_RECURSE engine_includes "${CMAKE_CURRENT_SOURCE_DIR}/engine/include/*.h")
file(GLOB_RECURSE game_includes "${CMAKE_CURRENT_SOURCE_DIR}/game/include/*.h")

add_executable(${CMAKE_PROJECT_NAME} ${engine_sources} ${game_sources} ${util_sources})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include Paths
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/engine/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/game/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/json/single_include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include")

if(MSVC) # If using the VS compiler...

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	#add this line if you want to remove the console!
	#set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
	
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

endif()

# Linking
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glfw GLEW::glew OpenGL::GL)

message("Using Compiler: " ${CMAKE_CXX_COMPILER_ID})
