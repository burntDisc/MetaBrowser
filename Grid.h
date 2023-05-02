#pragma once
#include "Drawable.h"
#include "Triangle.h"
#include <vector>

class Grid
	: public Drawable, public SolidObject
{
public:
	Grid(Shader shader, float baseLength, int baseNumber);
	~Grid();
	std::vector<glm::vec3> GetTriangleWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
	void Draw();
private:
	std::vector<Triangle*> triangles;
};

