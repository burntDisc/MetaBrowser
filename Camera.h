#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(int windowWidth, int windowHeight, glm::vec3& translation, glm::vec3& orientation, float& FOV);
	// Exports the POV related unifroms
	void SetCameraUniforms(Shader& shader);
	void SetSkyboxUniforms(Shader& skyboxShader);
private:;
	   const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	   int windowWidth, windowHeight;
	   glm::vec3& translation;
	   glm::vec3& orientation;
	   float& feildOfView;
};

