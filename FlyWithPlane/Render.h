#pragma once
#include "Shader.h"

class Render
{
public:
	Render() = default;
	~Render() = default;

	void RenderVertices(const float* vertices, unsigned int& VAO, unsigned int& VBO);
	void RenderFloor();
	void RenderScene(const Shader& shader);
};

