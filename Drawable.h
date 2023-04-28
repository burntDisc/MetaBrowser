#pragma once
#include "Shader.h"

class Drawable
{
public:
	Drawable(Shader shader) : shader(shader) {};
	virtual void Draw() = 0;

protected:
	Shader shader;
};

