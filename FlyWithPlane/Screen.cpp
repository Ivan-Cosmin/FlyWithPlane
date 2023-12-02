#include "Screen.h"

Screen::Screen()
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FlyWithPlane", NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

Screen::~Screen()
{
	glfwTerminate();
}

void Screen::run()
{
	while (!glfwWindowShouldClose(window)) {
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Screen::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
