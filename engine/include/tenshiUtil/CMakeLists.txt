file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}/engine/include/tenshiUtil/*.cpp")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/engine/include/tenshiUtil")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${src})
