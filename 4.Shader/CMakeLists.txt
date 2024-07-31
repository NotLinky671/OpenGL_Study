cmake_minimum_required(VERSION 3.10)

project(Shader)

link_directories(${CMAKE_SOURCE_DIR}/3rdParty/glfw/lib)

set(SRC
    ${CMAKE_SOURCE_DIR}/3rdParty/glad/src/glad.c
    src/main.cpp
    src/Shader.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdParty/glfw/include
    ${CMAKE_SOURCE_DIR}/3rdParty/glad/include
)

target_link_libraries(${PROJECT_NAME} glfw3.lib opengl32.lib)