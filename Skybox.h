#pragma once

#include <string>

#include "Drawable.h"
#include "Shader.h"

class Skybox :
	public Drawable
{
public:
	Skybox(Shader shader, std::string facesDirectory);
	void Draw();
private:
	float skyboxVertices[24];
	unsigned int skyboxIndices[36];
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO, cubemapTexture;
};

