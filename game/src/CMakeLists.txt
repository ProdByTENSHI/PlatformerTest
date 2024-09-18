file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}/game/src/*.cpp")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/game/include")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${src})
