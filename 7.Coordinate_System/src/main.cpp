#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <iostream>

float vertices[] = {
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

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};

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
    GLFWwindow* window = glfwCreateWindow(1000, 720, "OpenGL_Study_Coordinate_System", NULL, NULL);
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
    // Shader
    Shader* MyShader = new Shader("shaders/VertexShader.vs", "shaders/FragmentShader.fs");
// -------------------------------------------------------------------------------------------------
    // VAO
    unsigned int VAO;
    // 生成一个ID
    glGenVertexArrays(1, &VAO);
    // 绑定VAO
    glBindVertexArray(VAO);
// -------------------------------------------------------------------------------------------------
    // VBO
    unsigned int VBO;
    // 生成一个ID
    glGenBuffers(1, &VBO);
    // 将创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将顶点数据复制到缓存的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// -------------------------------------------------------------------------------------------------
    // 设置顶点属性指针
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                    5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
                    5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
// -------------------------------------------------------------------------------------------------
    // Texture
    // 加载木箱
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int nrChannels;
    unsigned char* BoxData = stbi_load("Texture/container.jpg", &width, &height, &nrChannels, 0);
    // 生成纹理
    unsigned int BoxTexture;
    glGenTextures(1, &BoxTexture);
    glBindTexture(GL_TEXTURE_2D, BoxTexture);
    // 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 生成
    if(BoxData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, BoxData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // 释放
    stbi_image_free(BoxData);

    // 加载笑脸
    unsigned char* FaceData = stbi_load("Texture/awesomeface.png", &width, &height, &nrChannels, 0);
    // 生成纹理
    unsigned int FaceTexture;
    glGenTextures(1, &FaceTexture);
    glBindTexture(GL_TEXTURE_2D, FaceTexture);
    // 设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 生成
    if(FaceData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, FaceData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // 释放
    stbi_image_free(FaceData);
    // 设置uniform
    MyShader->use();
    MyShader->setInt("u_BoxTexture", 0);
    MyShader->setInt("u_FaceTexture", 1);
// -------------------------------------------------------------------------------------------------
    
// -------------------------------------------------------------------------------------------------
    // 循环
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        MyShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BoxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FaceTexture);

        glBindVertexArray(VAO);

        // Transformation
        // Model
        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            MyShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // View
        glm::mat4 view;
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // Projection
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 1000.0f / 720.0f, 0.1f, 100.0f);
        // 设置Uniform
        MyShader->setMat4("view", view);
        MyShader->setMat4("projection", projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
// -------------------------------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}