#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const void * data, GLsizeiptr count, GLenum usage) : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID), __FILE__, __LINE__);
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID), __FILE__, __LINE__);
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLsizeiptr), data, usage), __FILE__, __LINE__);
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID), __FILE__, __LINE__);
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID), __FILE__, __LINE__);
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0), __FILE__, __LINE__);
}