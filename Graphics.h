#pragma once

#pragma comment(lib,"lib/freeglut.lib")
#pragma comment(lib,"lib/glew32.lib")
#pragma comment(lib,"lib/glfw3.lib")

#include "Singleton.cpp"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glfw3.h"
#include<iostream>


class Graphics : public Singleton<Graphics>
{
public:
    int glInit(int argc, char *argv[]);
    void run();
	
private:
    GLFWwindow* window;
    GLuint vertexbuffer;
};

