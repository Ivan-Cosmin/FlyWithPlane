#include <GL/glew.h>
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

enum EMovementType {
	ENone,
	EHorizontal,
	EVertical,
	ECircularClockWise,
	ECircularCounterClockWise,
	ERotation,
	EScale
};

class ScreenApp {
public:
	ScreenApp() {
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

		g_eMovementType = EMovementType::ENone;
	}

	void run() {
		while (!glfwWindowShouldClose(window)) {
			render();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void CreateVBO()
	{
		float vertices[] = 
		{
			// Front face
			-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Vertex 0
			 0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Vertex 1
			 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Vertex 2
			-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Vertex 3

			// Back face
			-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Vertex 4
			 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Vertex 5
			 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Vertex 6
			-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Vertex 7
		};

		unsigned int indices[] = 
		{
			// Front face
			0, 1, 2,
			2, 3, 0,

			// Right face
			1, 5, 6,
			6, 2, 1,

			// Back face
			4, 5, 6,
			6, 7, 4,

			// Left face
			0, 4, 7,
			7, 3, 0,

			// Top face
			3, 2, 6,
			6, 7, 3,

			// Bottom face
			0, 1, 5,
			5, 4, 0
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void DestroyVBO()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void CreateShaders()
	{
		VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderID, 1, &VertexShader, NULL);
		glCompileShader(VertexShaderID);

		FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShaderID, 1, &FragmentShader, NULL);
		glCompileShader(FragmentShaderID);

		GLint sucess = 0;
		GLchar ErrorLog[1024] = { 0 };

		glGetProgramiv(ProgramID, GL_LINK_STATUS, &sucess);
		if (sucess == 0)
		{
			glGetProgramInfoLog(ProgramID, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
			exit(1);
		}

		glValidateProgram(ProgramID);
		glGetProgramiv(ProgramID, GL_VALIDATE_STATUS, &sucess);
		if (sucess == 0)
		{
			glGetProgramInfoLog(ProgramID, sizeof(ErrorLog), NULL, ErrorLog);
			fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
			exit(1);
		}

		glUseProgram(ProgramID);

		// to be continued

	}

	~ScreenApp() {
		glfwTerminate();
	}

private:
	GLFWwindow* window;
	EMovementType g_eMovementType;
	GLuint VAO, VBO, EBO, ColorBufferID, VertexShaderID, FragmentShaderID, ProgramID;
	GLuint WorldMatrixLocation;

	const GLchar* VertexShader =
	{
		"#version 400\n"\
		"layout(location=0) in vec4 in_Position;\n"\
		"layout(location=1) in vec4 in_Color;\n"\
		"out vec4 ex_Color;\n"\
		"uniform mat4 WorldMatrix;\n"\
		"void main()\n"\
		"{\n"\
		"  gl_Position = WorldMatrix * in_Position;\n"\
		"  ex_Color = in_Color;\n"\
		"}\n"
	};
	// Shader-ul de fragment / Fragment shader (este privit ca un sir de caractere)
	const GLchar* FragmentShader =
	{
		"#version 400\n"\
		"in vec4 ex_Color;\n"\
		"out vec4 out_Color;\n"\
		"void main()\n"\
		"{\n"\
		"  out_Color = ex_Color;\n"\
		"}\n"
	};

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