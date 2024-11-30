cmake_minimum_required(VERSION 3.10)

project(Uniform)

set(SRC
    ${CMAKE_SOURCE_DIR}/3rdParty/glad/src/glad.c
    src/main.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/3rdParty/glfw/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdParty/glfw/include
    ${CMAKE_SOURCE_DIR}/3rdParty/glad/include
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/3rdParty/glfw/lib/glfw3dll.lib)
target_link_libraries(${PROJECT_NAME} opengl32)