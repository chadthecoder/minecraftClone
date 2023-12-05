#pragma once

//glew and glfw includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size, GLenum usage);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    //for some reason gives error if this function isnt const
    unsigned int GetRendererID() const;
};
