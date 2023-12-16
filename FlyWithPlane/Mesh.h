#pragma once
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

#include <string>
#include <vector>
using namespace std;
class Mesh
{
public:
	// mesh Data
	unsigned int numVertices;
	std::shared_ptr <Vertex> vertices;

	unsigned int numIndexes;
	std::shared_ptr <unsigned int> indices;
	std::vector<Texture>textures;
	unsigned int VAO;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	Mesh(unsigned int numVertices, std::shared_ptr <Vertex> vertices, unsigned int numIndexes, std::shared_ptr <unsigned int> indices, const std::vector<Texture>& textures);
	void Draw(Shader& shader);
private:
	// render data 
	unsigned int VBO, EBO;
	void setupMesh();
};
