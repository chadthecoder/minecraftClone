#include "Vertex.hpp"

namespace openglStuff
{
    VertexValue Vertex::setPosition(GLfloat x, GLfloat y, GLfloat z)
    {
        this->Position[0] = x;
        this->Position[1] = y;
        this->Position[2] = z;
        //myArray[0] = x;
        //myArray[1] = y;
        //myArray[2] = z;
        VertexValue return1;
        return return1;
    }

    void Vertex::setColor(GLfloat r, GLfloat g, GLfloat b, int (&myArray)[3])
    {
        this->Color[0] = r;
        this->Color[1] = g;
        this->Color[2] = b;
    }

    void Vertex::setPosition(GLfloat s, GLfloat t, int (&myArray)[2])
    {
        this->Texture[0] = s;
        this->Texture[1] = t;
    }

    /* void Vertex::getPosition()
    {
        this->Position[0];
        this->Position[1];
        this->Position[2];
    }

    void Vertex::getColor()
    {
        this->Color[0];
        this->Color[1];
        this->Color[2];
    }

    void Vertex::getPosition()
    {
        this->Texture[0];
        this->Texture[1];
    } */
}