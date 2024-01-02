#pragma once
#include <vector>
#include <iostream>
#include "Texture.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const std::string path, bool gamma = false);
unsigned int LoadSkybox(std::vector<std::string> faces);
class Model
{
public:
	std::vector<Texture> textures_loaded;	std::vector<Mesh>    meshes;
	std::string directory;
	bool gammaCorrection;

	Model(std::string const& path, bool bSmoothNormals, bool gamma = false);
	void Draw(Shader& shader);

private:
	void loadModel(std::string const& path, bool bSmoothNormals);
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

