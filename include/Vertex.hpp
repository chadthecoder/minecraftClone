#pragma once
//only header file for now

//glew and glfw includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

struct VertexValue {
    GLfloat pos[3];
    GLfloat col[3];
    GLfloat tex[2];
};

namespace openglStuff {

    /* struct vec2
    {
        float x, y;
    };

    struct vec3
    {
        float x, y, z;
    }; */

    class Vertex
    {
    public:
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec2 Texture;

        VertexValue setPosition(GLfloat x, GLfloat y, GLfloat z);
        void setColor(GLfloat r, GLfloat g, GLfloat b, int (&myArray)[3]);
        void setPosition(GLfloat s, GLfloat t, int (&myArray)[2]);

        //int TexIndex;

        Vertex(/* args */) {}
        ~Vertex() {}

    private:
        /* GLfloat Position[3];
        GLfloat Color[3];
        GLfloat Texture[2]; */
    };

}