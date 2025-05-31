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
#include "FrameBuffer.h"

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

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
};

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int WindowWidth = 1000;
int WindowHeight = 720;

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
    GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "OpenGL_Study_FrameBuffers", nullptr, nullptr);
    if(window == nullptr) {
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
    // 开启模板测试
    glEnable(GL_STENCIL_TEST);
// -------------------------------------------------------------------------------------------------
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
// -------------------------------------------------------------------------------------------------
    // Shader
    auto* cube_shader = new Shader("shaders/Stencil_Testing.vert", "shaders/Shader_SingleColor.frag");
    auto* plane_shader = new Shader("shaders/Stencil_Testing.vert", "shaders/Stencil_Testing_Plane.frag");
    auto* cube_texture_shader = new Shader("shaders/Stencil_Testing.vert", "shaders/Stencil_Testing.frag");
    auto* scene_shader = new Shader("shaders/Scene.vert", "shaders/Scene.frag");
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
    // quadVBO
    VertexBuffer quad_VBO(quadVertices, sizeof(quadVertices));
    // light_VAO
    VertexArray quad_VAO;
    VertexBufferLayout quad_Layout;
    plane_Layout.Push<float>(2);
    plane_Layout.Push<float>(2);
    plane_VAO.AddBuffer(quad_VBO, quad_Layout);
// -------------------------------------------------------------------------------------------------

    // FBO
    FrameBuffer FBO(1000, 720);
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
    // Single Color Width
    float scale = 1.1f;
    // Scene Size
    ImVec2 SceneSize((float)WindowWidth, (float)WindowHeight);
    // Shader
    scene_shader->use();
    scene_shader->setInt("screenTexture", 0);
// -------------------------------------------------------------------------------------------------
    // 循环
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        FBO.Bind();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
// -------------------------------------------------------------------------------------------------
        light_model = glm::translate(glm::mat4(1.0f), light_position);
        plane_shader->use();
        plane_shader->setMat4("model", light_model);
        plane_shader->setMat4("view", view);
        plane_shader->setMat4("projection", projection);
        plane_VAO.Bind();
        plane_texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);

        model = glm::translate(glm::mat4(1.0f), cube_position);
        model = glm::rotate(model, glm::radians(cube_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, cube_scale);
        view = camera.GetViewMatrix();
        cube_texture_shader->use();
        cube_texture_shader->setMat4("model", model);
        cube_texture_shader->setMat4("view", view);
        cube_texture_shader->setMat4("projection", projection);
        cube_VAO.Bind();
        cube_texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        model = glm::translate(glm::mat4(1.0f), cube_position);
        model = glm::rotate(model, glm::radians(cube_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(cube_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        cube_shader->use();
        cube_shader->setMat4("model", model);
        cube_shader->setMat4("view", view);
        cube_shader->setMat4("projection", projection);
        cube_VAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0xFF);

        FBO.UnBind();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        scene_shader->use();
        quad_VAO.Bind();
        glBindTexture(GL_TEXTURE_2D, FBO.GetTextureID());
        glDrawArrays(GL_TRIANGLES, 0, 6);
// -------------------------------------------------------------------------------------------------
        // Camera Transform
        camera.UpdateCameraVectors();
// -------------------------------------------------------------------------------------------------
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
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

        ImGui::SeparatorText("Single Color");
        ImGui::DragFloat("Single Color Width", &scale, 0.01f, -FLT_MAX, FLT_MAX);

        ImGui::End();

        ImGui::Begin("Scene");
        SceneSize = ImGui::GetContentRegionAvail();
        if (SceneSize.x * SceneSize.y > 0 && (SceneSize.x != FBO.GetWidth() || SceneSize.y != FBO.GetHeight())) {
            FBO.Resize((unsigned int)SceneSize.x, (unsigned int)SceneSize.y);
            glViewport(0, 0, (int)SceneSize.x, (int)SceneSize.y);
            projection = glm::perspective(glm::radians(45.0f), SceneSize.x / SceneSize.y, 0.1f, 100.0f);
        }

        //获取颜色纹理的ID
        unsigned int textureID = FBO.GetTextureID();
        ImGui::Image((ImTextureID)(intptr_t)textureID, SceneSize, ImVec2(0.0, 1.0), ImVec2(1.0, 0.0));
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
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