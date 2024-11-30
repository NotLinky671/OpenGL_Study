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
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float light_vertices[] = {
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
    GLFWwindow* window = glfwCreateWindow(1000, 720, "OpenGL_Study_Multiple_Lights", NULL, NULL);
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
    Shader* LightShader = new Shader("shaders/V_Light.vert", "shaders/F_Light.frag");
// -------------------------------------------------------------------------------------------------
    // cube_VBO
    VertexBuffer cube_VBO(cube_vertices, sizeof(cube_vertices));
    // cube_VAO
    VertexArray cube_VAO;
    VertexBufferLayout cube_Layout;
    cube_Layout.Push<float>(3);
    cube_Layout.Push<float>(3);
    cube_Layout.Push<float>(2);
    cube_VAO.AddBuffer(cube_VBO, cube_Layout);
// -------------------------------------------------------------------------------------------------
    // PointLight_VBO
    VertexBuffer pointlight_VBO(light_vertices, sizeof(light_vertices));
    // PointLight_VAO
    VertexArray pointlight_VAO;
    VertexBufferLayout pointlight_Layout;
    pointlight_Layout.Push<float>(3);
    pointlight_VAO.AddBuffer(pointlight_VBO, pointlight_Layout);
// -------------------------------------------------------------------------------------------------
    // Texture
    Texture diffuseMap("Texture/container2.png");
    Texture specularMap("Texture/container2_specular.png");
    MyShader->use();
    MyShader->setInt("material.ambient", 0);
    MyShader->setInt("material.specular", 1);
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
// -------------------------------------------------------------------------------------------------
    // DirectionLight
    struct DirLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    DirLight dirlight {
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.2f, 0.2f, 0.2f),          
        glm::vec3(0.5f, 0.5f, 0.5f),          
        glm::vec3(1.0f, 1.0f, 1.0f)      
    };
    // PointLight
    struct PointLight {
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float constant;
        float linear;
        float quadratic;
    };
    PointLight pointlight {
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),          
        glm::vec3(0.5f, 0.5f, 0.5f),          
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f   
    };

    // SpotLight
    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;

        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
    SpotLight spotlight {
        glm::vec3(0.0f, 3.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        12.5f,
        17.5f,
        1.0f,
        0.09f,
        0.032f,
        glm::vec3(0.2f, 0.2f, 0.2f),          
        glm::vec3(0.5f, 0.5f, 0.5f),          
        glm::vec3(1.0f, 1.0f, 1.0f)      
    };
// -------------------------------------------------------------------------------------------------
    // Material
    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
    Material material {
        glm::vec3(1.0f, 0.5f, 0.31f),          
        glm::vec3(1.0f, 0.5f, 0.31f),          
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f          
    };
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
        MyShader->setVec3("light.position", pointlight.position);
        MyShader->setVec3("viewPos", camera.Position);
        MyShader->setMat4("model", model);
        MyShader->setMat4("view", view);
        MyShader->setMat4("projection", projection);
        MyShader->setVec3("material.ambient", material.ambient);
        MyShader->setVec3("material.diffuse", material.diffuse);
        MyShader->setVec3("material.specular", material.specular);
        MyShader->setFloat("material.shininess", material.shininess);

        MyShader->setVec3("dirLight.direction", dirlight.direction);
        MyShader->setVec3("dirLight.ambient", dirlight.ambient);
        MyShader->setVec3("dirLight.diffuse", dirlight.diffuse);
        MyShader->setVec3("dirLight.specular", dirlight.specular);

        MyShader->setVec3("pointLight.position", pointlight.position);
        MyShader->setVec3("pointLight.ambient", pointlight.ambient);
        MyShader->setVec3("pointLight.diffuse", pointlight.diffuse);
        MyShader->setVec3("pointLight.specular", pointlight.specular);
        MyShader->setFloat("pointLight.constant", pointlight.constant);
        MyShader->setFloat("pointLight.linear", pointlight.linear);
        MyShader->setFloat("pointLight.quadratic", pointlight.quadratic);

        MyShader->setVec3("spotLight.position", spotlight.position);
        MyShader->setVec3("spotLight.direction", spotlight.direction);
        MyShader->setVec3("spotLight.ambient", spotlight.ambient);
        MyShader->setVec3("spotLight.diffuse", spotlight.diffuse);
        MyShader->setVec3("spotLight.specular", spotlight.specular);
        MyShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(spotlight.cutOff)));
        MyShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotlight.outerCutOff)));
        MyShader->setFloat("spotLight.constant", spotlight.constant);
        MyShader->setFloat("spotLight.linear", spotlight.linear);
        MyShader->setFloat("spotLight.quadratic", spotlight.quadratic);
        
        diffuseMap.Bind(0);
        specularMap.Bind(1);
        cube_VAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
// -------------------------------------------------------------------------------------------------
        // Light Transform
        light_model = glm::translate(glm::mat4(1.0f), pointlight.position);
        LightShader->use();
        LightShader->setMat4("model", light_model);
        LightShader->setMat4("view", view);
        LightShader->setMat4("projection", projection);
        pointlight_VAO.Bind();
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
        ImGui::DragFloat3("Light_Position", &pointlight.position.x, 0.01f, -FLT_MAX, FLT_MAX);

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