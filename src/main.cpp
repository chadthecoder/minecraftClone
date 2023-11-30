//glew and glfw includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//C and C++ std lib includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <cmath>

//glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//imgui includes
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
//#include <imgui/backends/imgui_impl_opengl3_loader.h>
#include <imgui/backends/imgui_impl_opengl3.h>

//perlin noise
#include "PerlinNoise/PerlinNoise.hpp"

#include "Errors.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Sound.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "RenderAPI.hpp"
#include "Vertex.hpp"

bool cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}

static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
{
    // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
    //ImGuiIO& io = ImGui::GetIO();

    // (2) ONLY forward mouse data to your underlying app/game.
    //if (!io.WantCaptureMouse) std::cout << xPos << " : " << yPos << "\n";
    //my_game->HandleMouseData(...);
}

static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    
    
    // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
    /* ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action); */

    // (2) ONLY forward mouse data to your underlying app/game.
    //if (!io.WantCaptureMouse) std::cout << button << " : " << action << " : " << mods << "\n";
    //my_game->HandleMouseData(...);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

struct counts {
  size_t vertexCount;
  uint32_t indexCount;
  openglStuff::Vertex* bufferC;
} ;

counts renderBlocksUnder(Renderer& rend, openglStuff::Vertex* bufferC, float x, float y, float z, uint32_t indexCount, size_t vertexCount)
{
    y--;
    //size_t test [2]; //0-indexCount, 1-vertexCount
    while(y>-1)
    {
        //std::cout << "in loooooooooooopy: " << y << "\n";
        bufferC = rend.Cube2(bufferC, x, (float)y, z);
        indexCount += 36;
        vertexCount += 36 * 8;

        y--;
    }


    counts counts2;

    counts2.indexCount = indexCount;
    counts2.vertexCount = vertexCount;
    counts2.bufferC = bufferC;
    return counts2;
}

int main(void)
{
    //glfw window and cursor stuff

    GLFWwindow *window;
    // unsigned int gScaleLocation;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glsl_version currently only used for imgui code
    const char* glsl_version = "#version 330"; //if 3.3, then 330 and same for newer versions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //make so window is not resizable
    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    GLFWmonitor* monitor = glfwGetPrimaryMonitor(); 
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //window = glfwCreateWindow(mode->width, mode->height, "Hello World", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(10);

    // Init glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error\n";
    }

    //get opengl version
    //std::cout << glGetString(GL_VERSION) << "\n";

    //temp scope to prevent errors
    {

    //get shader from text

    //Shader shader("res/shaders/Basic.shader");
    //shader.Bind();

    const size_t vertPerPoint = 8;
    const size_t vertPerCube = vertPerPoint * 36; //vertPerPoint * 8;
    const size_t indexPerCube = 36;
    //const size_t strideC = indexPerCube;
    //const size_t numCube = 166; //memory leak causes less to be able to be drawn w/o creating a segfault?
    const size_t maxCubeCount = 500; //166;
    //const size_t maxQuadCount = maxCubeCount * 6;
    const size_t maxVertexCount = maxCubeCount * vertPerCube;
    const size_t maxIndexCount = maxCubeCount * indexPerCube;

    //positions and indices

    std::vector<float> vertices {
        -0.5f, -0.5f, 0.0f,    0.83f, 0.70f, 0.44f,     0.0f, 0.0f, // 0
        0.5f, -0.5f, 0.0f,     0.83f, 0.70f, 0.44f,     1.0f, 0.0f, // 1
        0.5f, 0.5f, 0.0f,      0.83f, 0.70f, 0.44f,     1.0f, 1.0f, // 2
        -0.5f, 0.5f, 0.0f,     0.92f, 0.86f, 0.76f,     0.0f, 1.0f // 3
    };

    float incBy = 1.0f;

/*     std::vector<float> verticesPyramid {

        0.0f, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	    0.0f, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	    1.0f, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	    1.0f, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	    0.5f, 1.0f,  0.5f,     0.92f, 0.86f, 0.76f,	    2.5f, 5.0f,

        0.0f+incBy, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	    0.0f+incBy, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	    1.0f+incBy, 0.5f, 0.0f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
	    1.0f+incBy, 0.5f,  1.0f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
	    0.5f+incBy, 1.0f,  0.5f,     0.92f, 0.86f, 0.76f,	    2.5f, 5.0f
    }; */


    std::vector<unsigned int> indices {
        0, 1, 2,
        2, 3, 0 };

    std::vector<unsigned int> indicesPyramid {        
        0, 1, 2,
	    0, 2, 3,
	    0, 1, 4,
	    1, 2, 4,
	    2, 3, 4,
	    3, 0, 4,

        0, 1, 2,
	    0, 2, 3,
	    0, 1, 4,
	    1, 2, 4,
	    2, 3, 4,
	    3, 0, 4

        /* 5, 6, 7,
	    5, 7, 8,
	    5, 6, 9,
	    6, 7, 9,
	    7, 8, 9,
	    8, 5, 9 */
        };

        /* std::vector<unsigned int> indicesCube {        
         4, 5, 6,
        6, 5, 7,
        0, 4, 5,
        4, 5, 1,
        0, 1, 2,
        1, 2, 3,
        7, 6, 2,
        6, 2, 3,
        4, 7, 3,
        7, 3, 0,
        5, 6, 2,
        6, 2, 1

        4, 5, 6,
        4, 6, 7,
        1, 5, 4,
        1, 4, 0,
        1, 0, 3,
        1, 3, 2,
        3, 7, 6,
        3, 6, 2,
        0, 4, 7,
        0, 7, 3,
        2, 6, 5,
        2, 5, 1

        4, 5, 6,
        4, 6, 7,
        1, 5, 4,
        1, 4, 0,
        1, 0, 3,
        1, 3, 2,
        3, 7, 6,
        3, 6, 2,
        0, 4, 7,
        0, 7, 3,
        2, 6, 5,
        2, 5, 1 
        }; */

        //add cubeIndices
        uint32_t indicesCube[maxIndexCount];
        uint32_t offset = 0;
        uint32_t indexCount = 0;
        /* for(size_t i = 0; i < maxIndexCount; i += indexPerCube)
        {
            indicesCube[i+0] = 4 + offset;
            indicesCube[i+1] = 5 + offset;
            indicesCube[i+2] = 6 + offset;
            indicesCube[i+3] = 4 + offset;
            indicesCube[i+4] = 6 + offset;
            indicesCube[i+5] = 7 + offset;

            indicesCube[i+6] = 1 + offset;
            indicesCube[i+7] = 5 + offset;
            indicesCube[i+8] = 4 + offset;
            indicesCube[i+9] = 1 + offset;
            indicesCube[i+10] = 4 + offset;
            indicesCube[i+11] = 0 + offset;

            indicesCube[i+12] = 1 + offset;
            indicesCube[i+13] = 0 + offset;
            indicesCube[i+14] = 3 + offset;
            indicesCube[i+15] = 1 + offset;
            indicesCube[i+16] = 3 + offset;
            indicesCube[i+17] = 2 + offset;

            indicesCube[i+18] = 3 + offset;
            indicesCube[i+19] = 7 + offset;
            indicesCube[i+20] = 6 + offset;
            indicesCube[i+21] = 3 + offset;
            indicesCube[i+22] = 6 + offset;
            indicesCube[i+23] = 2 + offset;

            indicesCube[i+24] = 0 + offset;
            indicesCube[i+25] = 4 + offset;
            indicesCube[i+26] = 7 + offset;
            indicesCube[i+27] = 0 + offset;
            indicesCube[i+28] = 7 + offset;
            indicesCube[i+29] = 3 + offset;

            indicesCube[i+30] = 2 + offset;
            indicesCube[i+31] = 6 + offset;
            indicesCube[i+32] = 5 + offset;
            indicesCube[i+33] = 2 + offset;
            indicesCube[i+34] = 5 + offset;
            indicesCube[i+35] = 1 + offset;

            //std::cout << "stuff: " << maxIndexCount << " : " << i << " : " << offset << "\n";

            offset += vertPerPoint;
        } */
        for(size_t i = 0; i < maxIndexCount; i += indexPerCube)
        {
            indicesCube[i+0] = 0 + offset;
            indicesCube[i+1] = 1 + offset;
            indicesCube[i+2] = 2 + offset;
            indicesCube[i+3] = 3 + offset;
            indicesCube[i+4] = 4 + offset;
            indicesCube[i+5] = 5 + offset;

            indicesCube[i+6] = 6 + offset;
            indicesCube[i+7] = 7 + offset;
            indicesCube[i+8] = 8 + offset;
            indicesCube[i+9] = 9 + offset;
            indicesCube[i+10] = 10 + offset;
            indicesCube[i+11] = 11 + offset;

            indicesCube[i+12] = 12 + offset;
            indicesCube[i+13] = 13 + offset;
            indicesCube[i+14] = 14 + offset;
            indicesCube[i+15] = 15 + offset;
            indicesCube[i+16] = 16 + offset;
            indicesCube[i+17] = 17 + offset;

            indicesCube[i+18] = 18 + offset;
            indicesCube[i+19] = 19 + offset;
            indicesCube[i+20] = 20 + offset;
            indicesCube[i+21] = 21 + offset;
            indicesCube[i+22] = 22 + offset;
            indicesCube[i+23] = 23 + offset;

            indicesCube[i+24] = 24 + offset;
            indicesCube[i+25] = 25 + offset;
            indicesCube[i+26] = 26 + offset;
            indicesCube[i+27] = 27 + offset;
            indicesCube[i+28] = 28 + offset;
            indicesCube[i+29] = 29 + offset;

            indicesCube[i+30] = 30 + offset;
            indicesCube[i+31] = 31 + offset;
            indicesCube[i+32] = 32 + offset;
            indicesCube[i+33] = 33 + offset;
            indicesCube[i+34] = 34 + offset;
            indicesCube[i+35] = 35 + offset;

            //std::cout << "stuff: " << maxIndexCount << " : " << i << " : " << offset << "\n";

            offset += 6; //vertPerPoint;
        }

        //changes the second half of the indices to be the same pattern but for the new vertices
        int stride = indicesPyramid.size()/2;
        //std::cout << "Stride Pyramid: " << stride << "\n";
        for(int i=stride; i<(stride+stride); i++)
        {
            //std::cout << "Index " << i << " is " << indicesPyramid[i] << "\n";
            indicesPyramid[i] = (indicesPyramid[i]+5);
            //std::cout << "Index " << i << " is now " << indicesPyramid[i] << "\n";
        }

        //changes the second half of the indices to be the same pattern but for the new vertices
        
        //std::cout << "Stride Cube: " << strideC << "\n";

        //add indices for cubes
        /* for(unsigned int i=strideC; i<(strideC * numCube); i++)
        {
            //std::cout << "Index " << i << " is " << indicesCube[i] << "\n";
            indicesCube.push_back(indicesCube[i-indexPerCube]+vertPerCube);
            //std::cout << "Index " << i << " is now " << indicesCube[i] << "\n";
        } */

    //time stuff
    float rotation = 0.0f;
	double prevTime = glfwGetTime();

    //texture stuff, cpp logo
    //Texture texture("res/img/brick.png", "3D");
    //texture.Bind();

    Renderer renderer("3D", sizeof(openglStuff::Vertex) * maxVertexCount, indicesCube,
        maxIndexCount, "res/shaders/Basic.shader", "res/img/brick.png");

    //renderer.SetUniform1i("u_Texture", 0);

    //create pyramids
    auto q0 = renderer.Pyramid(0.0f, 0.0f, 0.0f);
    auto q1 = renderer.Pyramid(1.0f, 0.0f, 0.0f);
    openglStuff::Vertex verticesPyramid[10];
    memcpy(verticesPyramid, q0.data(), q0.size()*sizeof(openglStuff::Vertex));
    memcpy(verticesPyramid+q0.size(), q1.data(), q1.size()*sizeof(openglStuff::Vertex));


    size_t vertexCount = 0;
    std::array<openglStuff::Vertex, maxVertexCount> verticesCube;
    openglStuff::Vertex* bufferC = verticesCube.data();

    //bufferC = renderer.Cube2(bufferC, 0.0f, 0.0f, 0.0f);

    /* for(int y = 0; y < 5; y++)
    {
        for(int x = 0; x < 5; x++)
        {
            bufferC = renderer.Cube2(bufferC, (float)x, (float)y, -(float)y);
            indexCount += 36;
            vertexCount += vertPerCube;
        }
    } */

    const siv::PerlinNoise::seed_type seed = 123456u;

	const siv::PerlinNoise perlin{ seed };

	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			const double noise = perlin.octave2D_01((x * 1), (y * 1), 4);

            const float noise1 = sin(x/10.0f);
            const double noise2 = cos(x);

            //std::cout << (float)(int)(noise1*10) << "\n";
            //std::cout << (float)(int)(noise*10) << "\n";

            float x1 = (float)x;
            float y1 = (float)(int)(noise1*10);
            float z1 = -(float)y;

            float x2 = (float)x;
            float y2 = (float)(int)(noise1*10);
            float z2 = -(float)y;

            bufferC = renderer.Cube2(bufferC, x2, y2, z2);
            counts testy;
            //std::cout << "rend 1: " << x2 << " : " << y2 << " : " << z2 << "\n";
            //testy = renderBlocksUnder(renderer, bufferC, x2, y2, z2, indexCount, vertexCount);
            //std::cout << "rend 2: " << indexCount << " : " << vertexCount << "\n";
            //indexCount += testy.indexCount;
            //vertexCount += testy.vertexCount;
            //bufferC = testy.bufferC;
            bufferC = renderer.Cube2(bufferC, x2, y2, z2);
            indexCount += 36;
            vertexCount += vertPerCube;

			//std::cout << noise << '\t';
		}

        

		//std::cout << '\n';
	}

    //create vertex buffer for numCubes, can change Cube function to change color and texture of cubes
    /* openglStuff::Vertex verticesCube[vertPerCube*numCube];
    for(int i = 0; i < numCube; i++)
    {
        auto q = renderer.Cube(0.0f, (float)i, -(float)i);
        memcpy(verticesCube+(q.size()*i), q.data(), q.size()*sizeof(openglStuff::Vertex));
         std::cout << "sizey: " << verticesCube+(q.size()*i) << " : " << i << " : "
            << q.size() << " : " << verticesCube << " : " << verticesCube+0x8 << "\n"; 
    } */

    /* auto q2 = renderer.Cube(0.0f, 0.0f, 0.0f);
    auto q3 = renderer.Cube(1.0f, 0.0f, 0.0f);
    auto q4 = renderer.Cube(2.0f, 0.0f, 0.0f);
    auto q5 = renderer.Cube(3.0f, 0.0f, 0.0f);
    auto q6 = renderer.Cube(4.0f, 0.0f, 0.0f);
    
    //openglStuff::Vertex verticesCube[16];
    memcpy(verticesCube, q2.data(), q2.size()*sizeof(openglStuff::Vertex));
    memcpy(verticesCube+q2.size(), q3.data(), q3.size()*sizeof(openglStuff::Vertex));
    memcpy(verticesCube+q2.size()+q3.size(), q4.data(), q4.size()*sizeof(openglStuff::Vertex));
    memcpy(verticesCube+q2.size()+q3.size()+q4.size(), q5.data(), q5.size()*sizeof(openglStuff::Vertex));
    memcpy(verticesCube+q2.size()+q3.size()+q4.size()+q5.size(), q6.data(), q6.size()*sizeof(openglStuff::Vertex)); */

    //std::cout << q0.size() << "\n";

    //imgui stuff
    // Setup Dear ImGui context
    /* IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version); */

    //change font to google font
    //io.Fonts->AddFontFromFileTTF("res/fonts/kellyFont/KellySlab-Regular.ttf", 16.0f);

    //imgui variables
    static float rotationAngle1 = 0.0f;
    static float rotationAngle2 = 45.0f;
    static int counter = 0;
    bool timed = true;
    //end imgui stuff

    //timed rotation stuff
    double crntTime = glfwGetTime();

    //3d victor gordon matrices
	// Initializes matrices so they are not the null matrix
	glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 mvp;

    // Assigns different transformations to each matrix
	view = glm::translate(view, glm::vec3(0.0f, -0.5f, -3.0f));
	proj = glm::perspective(glm::radians(45.0f), ((float)mode->width/(float)mode->height), 0.1f, 100.0f);
    glm::vec3 modelTranslation1(-0.5f, 0.0f, 0.0f);
    glm::vec3 modelTranslation2(0.5f, 0.0f, 0.0f);

    //setup engine for playing sound
    Sound sndEngine;

    //setup camera
    Camera camera(mode->width, mode->height, glm::vec3(0.8f, 0.8f, 3.0f));
    
    //std::cout << "hi2\n";

    //renderer.Bind();

    clock_t current_ticks, delta_ticks;
clock_t fps = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //std::cout << "hi1\n";

        current_ticks = clock();

       

        

        /* Render here */
        renderer.Clear();
        //shader.Bind();

        //get inputs and set camera shader
        //if (!io.WantCaptureMouse) camera.MnKInputs(window);
        camera.MnKInputs(window);
        camera.Matrix(45.0f, 0.1f, 100.f, renderer.GetShader(), "u_camMatrix");
        
        /* ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); */

        //draw
        //std::cout << "size: " << sizeof(verticesCube)/sizeof(openglStuff::Vertex) << "\n";
        //std::cout << "size: " << verticesCube << "\n";
        //std::cout << "size: " << sizeof(float) << "\n";

        //std::cout << "vertex count: " << vertexCount << "\n";
        //std::cout << "hi2\n";
        renderer.Draw(verticesCube.data(), maxVertexCount * sizeof(openglStuff::Vertex));//25 * vertPerCube * sizeof(openglStuff::Vertex)); // vertexCount * sizeof(float));
        //std::cout << "size: " << sizeof(float) << "\n";
        //std::cout << "hi3\n";

        //imgui options
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        /* {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Translate and rotate two identical pyramids at will!");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Timed rotation or nah?", &timed);
            if(!timed)
            {
                ImGui::SliderFloat("Model 1 Rotation Angle", &rotationAngle1, 0.0f, 360.0f);
                ImGui::SliderFloat("Model 2 Rotation Angle", &rotationAngle2, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            }
            ImGui::SliderFloat3("Translate Model 1", &modelTranslation1.x, -1.0f, 1.0f);
            ImGui::SliderFloat3("Translate Model 2", &modelTranslation2.x, -1.0f, 1.0f);
            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        

        //imgui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
    if(delta_ticks > 0)
        fps = CLOCKS_PER_SEC / delta_ticks;
    //std::cout << "fps: " << fps << std::endl;
    }

    //victor gordon youtube, uncomment next line when done with other vertex buffer info
    //glDeleteTextures(1, &texture);
    //shader.Unbind();
    //end of temp scope to prevent errors
    }

    //imgui Cleanup
    /* ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); */

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
