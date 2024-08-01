#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

#include <iostream>

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    0, 1, 2, // 第一个三角形
    2, 3, 0  // 第二个三角形
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
    GLFWwindow* window = glfwCreateWindow(1000, 720, "OpenGL_Study_Texture", NULL, NULL);
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
                    8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                    8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
                    8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
// -------------------------------------------------------------------------------------------------
    // EBO
    unsigned int EBO;
    // 生成一个ID
    glGenBuffers(1, &EBO);
    // 将创建的缓冲绑定到GL_ELEMENT_ARRAY_BUFFER目标上
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
    // 循环
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        MyShader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BoxTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FaceTexture);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
// -------------------------------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}