#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <sstream>
#include <GL/glew.h>
#include <iostream>
#include <fstream>

class Shader
{
public:
	Shader() = default;
	Shader(const char* vertexPath, const char* fragmentPath);

	~Shader();

	// activate the shader
	void Use() const;

	unsigned int GetID() const; 

	unsigned int loc_model_matrix;
	unsigned int loc_view_matrix;
	unsigned int loc_projection_matrix;

	// utility uniform functions
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetFloat(const std::string& name, const float& value) const;
	void SetInt(const std::string& name, int value) const;
	
private:
	void Init(const char* vertexPath, const char* fragmentPath);

	// utility function for checking shader compilation/linking errors.
	void CheckCompileErrors(unsigned int shader, std::string type);
private:
	unsigned int ID;
};



