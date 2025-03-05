#include "VertexBufferLayout.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

#include <iostream>

float cube_vertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

float plane_vertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
// -------------------------------------------------------------------------------------------------
    // 初始化GLFW
    glfwInit();
    // 配置GLFW，设置OpenGL的版本为3.3，设置使用OpenGL核心模式
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// -------------------------------------------------------------------------------------------------
    // 创建Window(窗口)
    GLFWwindow* window = glfwCreateWindow(1000, 720, "OpenGL_Study_Depth_Testing", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
// -------------------------------------------------------------------------------------------------
    // 设置上下文
    glfwMakeContextCurrent(window);
// -------------------------------------------------------------------------------------------------
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
// -------------------------------------------------------------------------------------------------
    // 告诉OpenGL需要渲染多大的画面
    glViewport(0, 0, 1000, 720);
    // 线框模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // 默认模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // 开启深度缓冲
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
// -------------------------------------------------------------------------------------------------
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
// -------------------------------------------------------------------------------------------------
    // Shader
    Shader* cube_shader = new Shader("shaders/Depth_Testing.vert", "shaders/Depth_Testing.frag");
    Shader* plane_shader = new Shader("shaders/Depth_Testing_Plane.vert", "shaders/Depth_Testing_Plane.frag");
// -------------------------------------------------------------------------------------------------
    // cube_VBO
    VertexBuffer cube_VBO(cube_vertices, sizeof(cube_vertices));
    // cube_VAO
    VertexArray cube_VAO;
    VertexBufferLayout cube_Layout;
    cube_Layout.Push<float>(3);
    cube_Layout.Push<float>(2);
    cube_VAO.AddBuffer(cube_VBO, cube_Layout);
// -------------------------------------------------------------------------------------------------
    // light_VBO
    VertexBuffer plane_VBO(plane_vertices, sizeof(plane_vertices));
    // light_VAO
    VertexArray plane_VAO;
    VertexBufferLayout plane_Layout;
    plane_Layout.Push<float>(3);
    plane_Layout.Push<float>(2);
    plane_VAO.AddBuffer(plane_VBO, plane_Layout);
// -------------------------------------------------------------------------------------------------
    // Texture
    Texture cube_texture("Texture/marble.jpg");
    cube_texture.Bind();
    Texture plane_texture("Texture/metal.png");
    plane_texture.Bind();
// -------------------------------------------------------------------------------------------------
    // Camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 0.0f, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
// -------------------------------------------------------------------------------------------------
    // Cube_Model
    glm::mat4 model;
    glm::vec3 cube_position(0.0f, 0.0f, 0.0f);
    glm::vec3 cube_rotation(0.0f, 0.0f, 0.0f);
    glm::vec3 cube_scale(1.0f, 1.0f, 1.0);
    // View
    glm::mat4 view;
    // Projection
    glm::mat4 projection;
// -------------------------------------------------------------------------------------------------
    // Light_Model
    glm::mat4 light_model;
    glm::vec3 light_position(-3.0f, 0.0f, 0.0f);
// -------------------------------------------------------------------------------------------------
    // Light
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
// -------------------------------------------------------------------------------------------------
    // 循环
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
// -------------------------------------------------------------------------------------------------
        // Cube Transform
        model = glm::translate(glm::mat4(1.0f), cube_position);
        model = glm::rotate(model, glm::radians(cube_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, cube_scale); 
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), 1000.0f / 720.0f, 0.1f, 100.0f);       
        cube_shader->use();
        cube_shader->setMat4("model", model);
        cube_shader->setMat4("view", view);
        cube_shader->setMat4("projection", projection);
        cube_VAO.Bind();
        cube_texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
// -------------------------------------------------------------------------------------------------
        // Light Transform
        light_model = glm::translate(glm::mat4(1.0f), light_position);
        plane_shader->use();
        plane_shader->setMat4("model", light_model);
        plane_shader->setMat4("view", view);
        plane_shader->setMat4("projection", projection);
        plane_VAO.Bind();
        plane_texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
// -------------------------------------------------------------------------------------------------
        // Camera Transform
        camera.UpdateCameraVectors();
// -------------------------------------------------------------------------------------------------
        // ImGui::ShowDemoWindow();

        ImGui::Begin("Test");

        ImGui::SeparatorText("Camera");
        ImGui::DragFloat3("Camera_Position", &camera.Position.x, 0.01f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat("Pitch", &camera.Pitch, 0.1f, -89.0f, 89.0f);
        ImGui::DragFloat("Yaw", &camera.Yaw, 0.1f, -FLT_MAX, FLT_MAX);

        ImGui::SeparatorText("Cube");
        ImGui::DragFloat3("Cube_Position", &cube_position.x, 0.01f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Cube_Rotation", &cube_rotation.x, 0.1f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Cube_Scale", &cube_scale.x, 0.1f, -FLT_MAX, FLT_MAX);

        ImGui::SeparatorText("Light");
        ImGui::DragFloat3("Light_Position", &light_position.x, 0.01f, -FLT_MAX, FLT_MAX);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
// -------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
// -------------------------------------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}