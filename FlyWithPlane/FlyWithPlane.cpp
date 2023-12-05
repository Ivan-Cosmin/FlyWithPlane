#include "ShaderManager.h"
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// settings
static const unsigned int SCR_WIDTH = 1600;
static const unsigned int SCR_HEIGHT = 900;


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

GLuint VAO, VBO, EBO, ColorBufferID, VertexShaderID, FragmentShaderID, ProgramID;
GLuint WorldMatrixLocation;
// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

float skylight = 1.0f;
float clearR = 0.309f;
float clearG = 0.268f;
float clearB = 0.552f;

enum EMovementType {
	ENone,
	EHorizontal,
	EVertical,
	ECircularClockWise,
	ECircularCounterClockWise,
	ERotation,
	EScale
};

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

void darkenBackgroundColor(GLFWwindow* window)
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

		glfwSwapBuffers(window);
		glfwPollEvents();

		// Sleep for the specified update time
		std::this_thread::sleep_for(updateTime);

		skylight -= decreaseRate;
	}
}


int main() {
	
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FlyWithPlane", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	CreateVBO();
	ShaderManager shader(VertexShader, FragmentShader);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	while (!glfwWindowShouldClose(window)) 
	{
		//darkenBackgroundColor(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
		glfwTerminate();


	return 0;
}