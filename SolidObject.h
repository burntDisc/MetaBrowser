#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

class SolidObject
{
public:
	virtual std::vector<glm::vec3> GetTriangleWorldPositions() = 0;
	virtual std::vector<glm::vec3> GetTriangleWorldNormals() = 0;
};
