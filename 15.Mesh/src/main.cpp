#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>
#include <filesystem>

float plane_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
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
    GLFWwindow* window = glfwCreateWindow(1000, 720, "OpenGL_Study_Mesh", NULL, NULL);
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
    stbi_set_flip_vertically_on_load(true);
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
    Shader* MyShader = new Shader("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
// -------------------------------------------------------------------------------------------------
    // LoadModel
    Model MyModel("Model/nanosuit/nanosuit.obj");
// -------------------------------------------------------------------------------------------------
    // Camera
    Camera camera(glm::vec3(0.0f, 10.0f, 18.0f), 0.0f, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
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
        MyShader->use();
        MyShader->setVec3("viewPos", camera.Position);
        MyShader->setMat4("model", model);
        MyShader->setMat4("view", view);
        MyShader->setMat4("projection", projection);
        MyModel.Draw(*MyShader);
// -------------------------------------------------------------------------------------------------
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

        ImGui::SeparatorText("Model");
        ImGui::DragFloat3("Model_Position", &cube_position.x, 0.01f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Model_Rotation", &cube_rotation.x, 0.1f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Model_Scale", &cube_scale.x, 0.1f, -FLT_MAX, FLT_MAX);

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