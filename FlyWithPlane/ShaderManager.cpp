#include "ShaderManager.h"

ShaderManager::ShaderManager(const char* vertexShaderSource, const char* fragmentShaderSource) {
	// Creează programul OpenGL
	programID = glCreateProgram();

	// Creează shader-ul de vârf și leagă-l la program
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShaderID, vertexShaderSource);
	glAttachShader(programID, vertexShaderID);

	// Creează shader-ul de fragment și leagă-l la program
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShaderID, fragmentShaderSource);
	glAttachShader(programID, fragmentShaderID);

	// Link programul OpenGL
	glLinkProgram(programID);

	// Verifică starea de linkare a programului
	GLint success = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLchar errorLog[1024] = { 0 };
		glGetProgramInfoLog(programID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	// Validează programul OpenGL
	glValidateProgram(programID);
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &success);
	if (success == GL_FALSE) {
		GLchar errorLog[1024] = { 0 };
		glGetProgramInfoLog(programID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	// Utilizează programul OpenGL
	glUseProgram(programID);

	// Eliberează resursele shaderelor, deoarece acestea sunt deja legate la program
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

ShaderManager::~ShaderManager() {
	// Distrugă programul OpenGL
	glUseProgram(0);

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(fragmentShaderID);
	glDeleteShader(vertexShaderID);

	glDeleteProgram(programID);
}

GLuint ShaderManager::getProgramID() const {
	return programID;
}

GLuint ShaderManager::getVertexShaderID() const
{
	return vertexShaderID;
}

GLuint ShaderManager::getFragmentShaderID() const
{
	return fragmentShaderID;
}

void ShaderManager::compileShader(GLuint shaderID, const char* shaderSource) {
	// Compilează shader-ul
	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);

	// Verifică starea de compilare a shader-ului
	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLchar errorLog[1024] = { 0 };
		glGetShaderInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error compiling shader: '%s'\n", errorLog);
		exit(1);
	}
}

