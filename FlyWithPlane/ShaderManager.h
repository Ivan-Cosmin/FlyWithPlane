#pragma once
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>

class ShaderManager {
public:
	ShaderManager(const char* vertexShaderSource, const char* fragmentShaderSource);
	~ShaderManager();

	GLuint getProgramID() const;

	GLuint getVertexShaderID() const;
	GLuint getFragmentShaderID() const;

private:
	GLuint programID;

	GLuint vertexShaderID, fragmentShaderID;

	// Metode pentru încărcarea, compilarea și linkarea shaderelor
	void compileShader(GLuint shaderID, const char* shaderSource);
};