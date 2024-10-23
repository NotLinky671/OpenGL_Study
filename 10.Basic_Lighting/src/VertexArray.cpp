#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) {
    Bind();
    vbo.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, 
                    layout.GetStride(), (const void*)(uintptr_t)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() {
    glBindVertexArray(m_ID);
}

void VertexArray::UnBind() {
    glBindVertexArray(0);
}