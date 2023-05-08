#pragma once
#include "Drawable.h"
#include "Triangle.h"
#include "Player.h"
#include <vector>

class Grid
	: public Drawable, public SolidObject
{
public:
	Grid(Shader& rawShader, Shader& textShader, Camera& camera, float baseLength, int baseNumber, Player& player);
	~Grid();
	std::vector<glm::vec3> GetTriangleWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
	void Draw();
private:
	std::vector<Triangle*> triangles;
};

