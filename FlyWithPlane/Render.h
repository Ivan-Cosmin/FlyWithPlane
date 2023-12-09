#pragma once
#include <GL/glew.h>

class Render
{	
	Render() = default;
	~Render() = default;

	void RenderVertices(const float* vertices, unsigned int& VAO, unsigned int& VBO);
	void RenderFloor(float* vertices, unsigned int& planeVAO, unsigned int& planeVBO);
	//void RenderScene(const Shader& shader);
};

