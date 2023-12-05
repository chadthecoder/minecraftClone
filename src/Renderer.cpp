#include "Renderer.hpp"

Renderer::Renderer(std::string dimension, unsigned int maxVerticesSize, const void *indices,
     GLsizeiptr sizeIndex, const std::string& shaderPath, const std::string& texturePath)
    : vb(nullptr, maxVerticesSize, GL_DYNAMIC_DRAW), shader(shaderPath),
        texture(texturePath, dimension), ib(indices, sizeIndex, GL_DYNAMIC_DRAW)
    //: vb(vertices, sizeVertex, GL_DYNAMIC_DRAW), ib(indices, sizeIndex, GL_DYNAMIC_DRAW)
{
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), __FILE__, __LINE__);
    GLCall(glEnable(GL_BLEND), __FILE__, __LINE__);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); 

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);  


    Bind();

    //square
    if(dimension == "2D")
    {

       /*  layout.Push(GL_FLOAT, 2);
        layout.Push(GL_FLOAT, 2); */
        
        layout.Push(GL_FLOAT, 3);
        layout.Push(GL_FLOAT, 3);
        layout.Push(GL_FLOAT, 2);

        va.AddBuffer(vb, layout);
    }
    //pyramid, should work for others in same format
    else if(dimension == "3D")
    {
        layout.Push(GL_FLOAT, 3);
        layout.Push(GL_FLOAT, 3);
        layout.Push(GL_FLOAT, 2);
    
        va.AddBuffer(vb, layout);
    }
}

Renderer::~Renderer()
{
    //need to take care of unbinding later, where?
    Unbind();
}


//gives pyramid rendered in positive directions from given point
std::array<openglStuff::Vertex, 5> Renderer::Pyramid(float x, float y, float z)
{

    float size = 1.0f;

/* 0.0f, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	1.0f, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	1.0f, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	0.5f, 1.3f,  0.5f,     0.92f, 0.86f, 0.76f,	    2.5f, 5.0f, */

    openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    v0.Position = glm::vec3(x+0.0f, y+0.5f,  z+1.0f);
    v0.Color = glm::vec3(0.83f, 0.70f, 0.44f);
    v0.Texture = glm::vec2(0.0f, 0.0f);

    openglStuff::Vertex v1;
    v1.Position = glm::vec3(x+0.0f, y+0.5f, z+0.0f);
    v1.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v1.Texture = glm::vec2(5.0f, 0.0f);

    openglStuff::Vertex v2;
    v2.Position = glm::vec3(x+1.0f, y+0.5f, z+0.0f);
    v2.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v2.Texture = glm::vec2(0.0f, 0.0f);

    openglStuff::Vertex v3;
    v3.Position = glm::vec3(x+1.0f, y+0.5f,  z+1.0f);
    v3.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v3.Texture = glm::vec2(5.0f, 0.0f);

    openglStuff::Vertex v4;
    v4.Position = glm::vec3(x+0.5f, y+1.3f,  z+0.5f);
    v4.Color = glm::vec3(0.92f, 0.86f, 0.76f);
    v4.Texture = glm::vec2(2.5f, 5.0f);

    return { v0, v1, v2, v3, v4 };
}

//gives cube rendered in positive directions from given point
/* std::array<openglStuff::Vertex, 8> Renderer::Cube(float x, float y, float z)
{

    //float size = 1.0f;



    openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    v0.Position = glm::vec3(x+0.0f, y+0.0f,  z+1.0f);
    v0.Color = glm::vec3(0.83f, 0.70f, 0.44f);
    v0.Texture = glm::vec2(0.0f, 0.0f);

    openglStuff::Vertex v1;
    v1.Position = glm::vec3(x+0.0f, y+0.0f, z+0.0f);
    v1.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v1.Texture = glm::vec2(5.0f, 0.0f);

    openglStuff::Vertex v2;
    v2.Position = glm::vec3(x+1.0f, y+0.0f, z+0.0f);
    v2.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v2.Texture = glm::vec2(0.0f, 0.0f);

    openglStuff::Vertex v3;
    v3.Position = glm::vec3(x+1.0f, y+0.0f,  z+1.0f);
    v3.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v3.Texture = glm::vec2(5.0f, 0.0f);

    openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    v4.Position = glm::vec3(x+0.0f, y+1.0f,  z+1.0f);
    v4.Color = glm::vec3(0.83f, 0.70f, 0.44f);
    v4.Texture = glm::vec2(0.0f, 0.0f);

    openglStuff::Vertex v5;
    v5.Position = glm::vec3(x+0.0f, y+1.0f, z+0.0f);
    v5.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v5.Texture = glm::vec2(5.0f, 0.0f);

    openglStuff::Vertex v6;
    v6.Position = glm::vec3(x+1.0f, y+1.0f, z+0.0f);
    v6.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v6.Texture = glm::vec2(0.0f, 0.0f);

    openglStuff::Vertex v7;
    v7.Position = glm::vec3(x+1.0f, y+1.0f,  z+1.0f);
    v7.Color = glm::vec3(0.83f, 0.70f, 0.44);
    v7.Texture = glm::vec2(5.0f, 0.0f);

    return { v0, v1, v2, v3, v4, v5, v6, v7 };
} */

openglStuff::Vertex* Renderer::Cube(openglStuff::Vertex* target, float x, float y, float z)
{

    float size = 1.0f;

/* 0.0f, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	1.0f, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	1.0f, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	0.5f, 1.3f,  0.5f,     0.92f, 0.86f, 0.76f,	    2.5f, 5.0f, */

    //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+0.0f, y+0.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(5.0f, 0.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+1.0f, y+0.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+1.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(5.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+1.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+0.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(5.0f, 0.0f);
    target++;

    //openglStuff::Vertex v6;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v7;
    target->Position = glm::vec3(x+1.0f, y+1.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(5.0f, 0.0f);
    target++;

    return target;
}

openglStuff::Vertex* Renderer::Cube2(openglStuff::Vertex* target, float x, float y, float z)
{

    float size = 1.0f;

    /* -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f */

    //std::cout << "cube in: " << target << "\n";

    //left
    //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+0.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 1.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+0.0f, y+1.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+1.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+0.0f, y+0.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 0.0f);
    target++;

    //front
   //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+0.0f, y+1.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 1.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+1.0f, y+1.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+1.0f, y+0.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 0.0f);
    target++;

    //back
    //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+1.0f, y+0.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 1.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+0.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+1.0f, y+0.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+1.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+0.0f, y+0.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 0.0f);
    target++;

    //bottom
    //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+0.0f, y+0.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 1.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+1.0f, y+0.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+0.0f, y+0.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+1.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+1.0f, y+0.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 0.0f);
    target++;

    //top
    //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+0.0f, y+1.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+0.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 1.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+0.0f, y+1.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+1.0f, y+1.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 0.0f);
    target++;

    //right
    //openglStuff::Vertex v0;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+1.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v1;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 1.0f);
    target++;

    //openglStuff::Vertex v2;
    target->Position = glm::vec3(x+1.0f, y+1.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v3;
    target->Position = glm::vec3(x+1.0f, y+0.0f,  z+1.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(0.0f, 0.0f);
    target++;

    //openglStuff::Vertex v4;
    //float* data = glm::value_ptr(vec);
    target->Position = glm::vec3(x+1.0f, y+1.0f,  z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44f);
    target->Texture = glm::vec2(1.0f, 1.0f);
    target++;

    //openglStuff::Vertex v5;
    target->Position = glm::vec3(x+1.0f, y+0.0f, z+0.0f);
    target->Color = glm::vec3(0.83f, 0.70f, 0.44);
    target->Texture = glm::vec2(1.0f, 0.0f);
    target++;

    //std::cout << "cube out: " << target << "\n";

    return target;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), __FILE__, __LINE__);
}

void Renderer::Draw(const void* data, unsigned int size) const //Draw(float* vertices, unsigned int* indexArray, const Shader& shader) const
{
    Bind();
    //std::cout << "subdata: " << size << " : " << data << "\n";
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data), __FILE__, __LINE__);

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr), __FILE__, __LINE__);
    //ib.GetCountPrint();
    //GLCall(glDrawArrays(GL_TRIANGLES, 0, (36*20*20)), __FILE__, __LINE__);
}

void Renderer::SetUniform1i(const std::string& name, int i0)
{
    shader.SetUniform1i(name, i0);
}

void Renderer::SetUniform1f(const std::string& name, float f0)
{
    shader.SetUniform1f(name, f0);
}

void Renderer::SetUniform2f(const std::string& name, float f0, float f1)
{
    shader.SetUniform2f(name, f0, f1);
}

void Renderer::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    shader.SetUniform4f(name, f0, f1, f2, f3);
}

void Renderer::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    shader.SetUniformMat4f(name, matrix);
}

Shader& Renderer::GetShader()
{
    return shader;
}

void Renderer::Bind() const
{
    shader.Bind();
    va.Bind();
    vb.Bind();
    ib.Bind();
    texture.Bind();
}

void Renderer::Unbind()
{
    shader.Unbind();
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    texture.Unbind();
}