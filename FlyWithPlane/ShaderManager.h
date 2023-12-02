#pragma once
#include "Screen.h"

class ShaderManager {
public:
	ShaderManager(const char* vertexShaderSource, const char* fragmentShaderSource);
	~ShaderManager();

	GLuint getProgramID() const;

private:
	GLuint programID;

	// Metode pentru încărcarea, compilarea și linkarea shaderelor
	void compileShader(GLuint shaderID, const char* shaderSource);
};