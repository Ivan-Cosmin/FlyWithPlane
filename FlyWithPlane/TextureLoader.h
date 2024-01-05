#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <stb_image.h>

unsigned int CreateTexture(const std::string& strTexturePath);
unsigned int LoadSkybox(std::vector<std::string> faces);