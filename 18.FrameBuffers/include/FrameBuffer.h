//
// Created by not on 2025/5/31.
//

#ifndef OPENGL_STUDY_FRAMEBUFFER_H
#define OPENGL_STUDY_FRAMEBUFFER_H

class FrameBuffer
{
public:
    FrameBuffer(unsigned int width, unsigned int hright);
    ~FrameBuffer();
    void Init();
    void Resize(unsigned int width, unsigned int height);
    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }
    inline unsigned int GetTextureID() const { return m_TextureColorID;};

    void Bind();
    void UnBind();

private:
    unsigned int m_FrameBufferID;
    unsigned int m_Width;
    unsigned int m_Height;
    unsigned int m_TextureColorID;
    unsigned int m_RenderBufferID;
};

#endif //OPENGL_STUDY_FRAMEBUFFER_H
