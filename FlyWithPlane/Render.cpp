#include "Render.h"

void Render::RenderVertices(const float* vertices, unsigned int& VAO, unsigned int& VBO)
{
	if (VAO == 0) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}
}

void Render::RenderFloor(float* vertices, unsigned int& planeVAO, unsigned int& planeVBO)
{
	if (planeVAO == 0)
	{
		RenderVertices(vertices, planeVAO, planeVBO);
	}
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//void Render::RenderScene(const Shader& shader)
//{
//	glm::vec3 cubePositions[] = {
//	glm::vec3(3.0f,  2.0f,   0.0f),
//	glm::vec3(0.0f,  3.0f,  -5.0f),
//	glm::vec3(-3.0f, 2.0f,  -5.0f),
//	};
//
//	// floor
//	glm::mat4 model;
//	shader.SetMat4("model", model);
//	renderFloor();
//
//	// cube
//	model = glm::mat4();
//	model = glm::translate(model, glm::vec3(0.0f, 1.75f, 0.0));
//	model = glm::scale(model, glm::vec3(0.75f));
//	shader.SetMat4("model", model);
//}