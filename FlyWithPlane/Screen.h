#pragma once

#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
static const unsigned int SCR_WIDTH = 1600;
static const unsigned int SCR_HEIGHT = 900;

class Screen
{
public:
	Screen();
	~Screen();
	void run();

private:
	GLFWwindow* window;
	void render();
};