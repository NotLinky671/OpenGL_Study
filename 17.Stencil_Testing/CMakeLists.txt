cmake_minimum_required(VERSION 3.10)

project(Stencil_Testing)

set(SRC
    ${CMAKE_SOURCE_DIR}/3rdParty/glad/src/glad.c
    src/main.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/VertexBufferLayout.cpp
    src/Texture.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/src/imgui_widgets.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Texture DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/3rdParty/glfw/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdParty/glfw/include
    ${CMAKE_SOURCE_DIR}/3rdParty/glad/include
    ${CMAKE_SOURCE_DIR}/3rdParty
    ${CMAKE_SOURCE_DIR}/3rdParty/glm
    ${CMAKE_SOURCE_DIR}/3rdParty/imgui/include
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/3rdParty/glfw/lib/glfw3dll.lib)
target_link_libraries(${PROJECT_NAME} opengl32)