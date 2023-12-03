#include "Screen.h"
#include <chrono>
#include <thread>

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
	
	glClearColor(clearR, clearG, clearB, 1.0f);
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
void Screen::darkenBackgroundColor()
{
	const float decreaseRate = 0.01f;
	const std::chrono::milliseconds updateTime(300);
	
	while (skylight > 0.0f) {
		
		clearR -= decreaseRate;
		clearG -= decreaseRate;
		clearB -= decreaseRate;

		// Ensure values are within valid range
		clearR = std::max(0.021f, clearR);
		clearG = std::max(0.0f, clearG);
		clearB = std::max(0.06f, clearB);


		// Update clear color
		glClearColor(clearR, clearG, clearB, 1.0f);

		// Render and swap buffers
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Sleep for the specified update time
		std::this_thread::sleep_for(updateTime);

		skylight -= decreaseRate;
	}
}

void Screen::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
