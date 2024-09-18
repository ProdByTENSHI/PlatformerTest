file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}/engine/src/*.cpp")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/engine/include")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${src})
