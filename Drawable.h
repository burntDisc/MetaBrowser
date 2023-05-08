#pragma once
#include "Shader.h"
#include "Camera.h"

class Drawable
{
public:
	Drawable(Shader& shader, Camera& camera) : shader(shader), camera(camera){};
	void Draw();

protected:
	Shader& shader;
	Camera& camera;
};

