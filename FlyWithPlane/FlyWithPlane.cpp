#include <GL/glew.h>
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glfw3.h>
#include <iostream>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

class ScreenApp {
public:
	ScreenApp() {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW");
		}

		window = glfwCreateWindow(800, 600, "FlyWithPlane", NULL, NULL);
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

	void run() {
		while (!glfwWindowShouldClose(window)) {
			render();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	~ScreenApp() {
		glfwTerminate();
	}

private:
	GLFWwindow* window;

	void render() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

int main() {
	try {
		ScreenApp app;
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}