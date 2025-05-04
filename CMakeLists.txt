cmake_minimum_required(VERSION 3.10)
project(WaterShader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GLAD
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

include_directories(${CMAKE_SOURCE_DIR}/glfw/include)
link_directories(${CMAKE_SOURCE_DIR}/glfw/bin)

# Executable
add_executable(WaterShader main.cpp)

target_link_libraries(WaterShader
    PRIVATE
    glad
    glfw3
)

target_include_directories(WaterShader
    PRIVATE
    glad/include
    ${CMAKE_SOURCE_DIR}/glfw/include
)

add_custom_command(TARGET WaterShader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:WaterShader>/shaders
    COMMENT "Copying shaders to output directory"
)

file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*.glsl")

# Add to project for visibility in Solution Explorer
source_group("Shaders" FILES ${SHADER_FILES})
target_sources(WaterShader PRIVATE ${SHADER_FILES})