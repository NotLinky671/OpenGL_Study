#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

float vertices[] = {
      0.5f,   0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  // 右上角
      0.5f,  -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 左下角
    -0.5f,  0.5f, 0.0f, 0.4f, 0.2f, 0.0f // 左上角
};

unsigned int indices[] = {
    0, 1, 2, // 第一个三角形
    2, 3, 0  // 第二个三角形
};

const char* VertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "ourColor = aColor;\n"
    "}\0";

const char* FragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(ourColor, 1.0);\n"
    "}\0";

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
    GLFWwindow* window = glfwCreateWindow(1000, 720, "OpenGL_Study_Uniform", NULL, NULL);
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
                    6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                    6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
// -------------------------------------------------------------------------------------------------
    // EBO
    unsigned int EBO;
    // 生成一个ID
    glGenBuffers(1, &EBO);
    // 将创建的缓冲绑定到GL_ELEMENT_ARRAY_BUFFER目标上
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// -------------------------------------------------------------------------------------------------
    // VertexShader
    unsigned int VertexShader;
    // 创建VertexShader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 将VertexShaderSource附加到VertexShader上
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);
    // 检验
    int  success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
// -------------------------------------------------------------------------------------------------
    // FragmentSHader
    unsigned int FragmentShader;
    // 创建FragmentShader
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 将FragmentShaderSource附加到FragmentShader上
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    // 检验
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMNET::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
// -------------------------------------------------------------------------------------------------
    // ShaderPragram
    unsigned int ShaderPragram;
    // 创建ShaderPragram
    ShaderPragram = glCreateProgram();
    // 引用VertexShader
    glAttachShader(ShaderPragram, VertexShader);
    // 引用FragmentShader
    glAttachShader(ShaderPragram, FragmentShader);
    // 链接
    glLinkProgram(ShaderPragram);
    // 检验
    glGetShaderiv(ShaderPragram, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(ShaderPragram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::SHADERPRAGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 激活ShaderPragram
    glUseProgram(ShaderPragram);
// -------------------------------------------------------------------------------------------------
    // 循环
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ShaderPragram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
// -------------------------------------------------------------------------------------------------
    glfwTerminate();
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    return 0;
}