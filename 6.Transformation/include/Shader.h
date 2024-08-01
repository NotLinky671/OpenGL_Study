#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
    // 程序ID
    unsigned int ShaderPragram;

    Shader(const char* VertexShaderPath, const char* FragmentShaderPath);

    // 使用激活程序
    void use();

    // Uniform工具函数
    void setBool1i(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ShaderPragram, name.c_str()), (int)value); 
    }
    void setInt1i(const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(ShaderPragram, name.c_str()), value); 
    }
    void setFloat1f(const std::string &name, float value) const { 
        glUniform1f(glGetUniformLocation(ShaderPragram, name.c_str()), value); 
    }
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};