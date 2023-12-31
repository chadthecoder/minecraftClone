#pragma once

//#include "Renderer.hpp"
#include <GL/glew.h>

#include "Errors.hpp"

#include "stb/stb_image.h"

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    bool firstLoad;
public:
    Texture(const std::string& path, std::string dimension);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};