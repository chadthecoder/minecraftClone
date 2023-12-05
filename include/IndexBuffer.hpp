#pragma once

#include "Errors.hpp"

class IndexBuffer
{
private:
    GLuint m_RendererID;
    GLsizeiptr m_Count;
public:
    IndexBuffer(const void * data, GLsizeiptr count, GLenum usage);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline void PrintGetCount() const {  std::cout << m_Count << std::endl; }
    inline GLsizeiptr GetCount() const { return m_Count; }
};
