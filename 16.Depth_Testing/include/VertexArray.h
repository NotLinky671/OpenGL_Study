#pragma once

#include "glad/glad.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
    unsigned int m_ID;
public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
    void Bind();
    void UnBind();
};