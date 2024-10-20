#include "Texture.h"
#include "glad/glad.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(std::string path)
    : m_ID(0), m_FilePath(path), m_LocalBuffer(nullptr),
      m_Width(0), m_Height(0), m_BPP(0) {
        stbi_set_flip_vertically_on_load(true);
        m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 0);

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (m_LocalBuffer) {
            GLenum format;
            if (m_BPP == 1)
                format = GL_RED;
            else if (m_BPP == 3)
                format = GL_RGB;
            else if (m_BPP == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Failed to load texture: " << m_FilePath << std::endl;
        }

        stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_ID);
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::UnBind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
}