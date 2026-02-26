cmake_minimum_required(VERSION 3.16)
project(planets)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# GLM
add_subdirectory(external/glm)

# ImGui
add_subdirectory(external/imgui)

# Collect sources
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(planets ${SOURCES})

target_include_directories(planets PRIVATE src/)

target_link_libraries(planets PRIVATE
    glad
    glfw
    glm
    imgui
    dl
    pthread
)

# Copy shaders to build directory
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:planets>/shaders
    COMMENT "Copying shaders to build directory"
)