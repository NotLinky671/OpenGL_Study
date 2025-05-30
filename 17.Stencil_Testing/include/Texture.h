#pragma once

#include <string>

class Texture {
private:
    unsigned int m_ID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width;
    int m_Height;
    int m_BPP;
public:
    Texture(std::string path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};