#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(int windowWidth, int windowHeight);
	void Bind(glm::vec3* newTranslation, glm::vec3* newOrientation, float* newFeildOfView);
	// Exports the POV related unifroms
	void SetCameraUniforms(Shader& shader);
	void SetSkyboxUniforms(Shader& skyboxShader);
private:;
	   const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	   int windowWidth, windowHeight;
	   glm::vec3* translation = nullptr;
	   glm::vec3* orientation = nullptr;
	   float* feildOfView = nullptr;
};

