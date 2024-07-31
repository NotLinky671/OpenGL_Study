#include "Shader.h"

Shader::Shader(const char* VertexShaderPath, const char* FragmentShaderPath) {
    // 从文件路径中获取顶点/片段着色器
    std::string VertexCode;
    std::string FragmentCode;
    std::ifstream VertexShaderFile;
    std::ifstream FragmentShaderFile;
    // 保证ifstream对象可以抛出异常
    VertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    FragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try{
        // 打开文件
        VertexShaderFile.open(VertexShaderPath);
        FragmentShaderFile.open(FragmentShaderPath);
        std::stringstream VertexShaderStream;
        std::stringstream FragmentShaderStream;
        // 读取文件的缓冲内容到数据流中
        VertexShaderStream << VertexShaderFile.rdbuf();
        FragmentShaderStream << FragmentShaderFile.rdbuf();
        // 关闭文件处理器
        VertexShaderFile.close();
        FragmentShaderFile.close();
        // 转换数据流到string
        VertexCode = VertexShaderStream.str();
        FragmentCode = FragmentShaderStream.str();
    }
    catch(std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* VertexShaderCode = VertexCode.c_str();
    const char* FragmentShaderCode = FragmentCode.c_str();

    if(VertexShaderFile.is_open()) {
            std::cout << "Open" << std::endl;
        }

    // 编译着色器
    unsigned int VertexShader;
    unsigned int FragmentShader;
    int success;
    char infoLog[512];

    // 顶点着色器
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 将VertexShaderCode附加到VertexShader上
    glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
    glCompileShader(VertexShader);
    // 打印错误
    checkCompileErrors(VertexShader, "VERTEX");

    // 片段着色器
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 将FragmentShaderCode附加到FragmentShader上
    glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
    glCompileShader(FragmentShader);
    // 打印错误
    checkCompileErrors(FragmentShader, "FRAGMENT");

    // 着色器程序
    ShaderPragram = glCreateProgram();
    // 引用VertexShader
    glAttachShader(ShaderPragram, VertexShader);
    // 引用FragmentShader
    glAttachShader(ShaderPragram, FragmentShader);
    // 链接
    glLinkProgram(ShaderPragram);
    // 打印错误
    checkCompileErrors(ShaderPragram, "PROGRAM");

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void Shader::use(){
    glUseProgram(ShaderPragram);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}