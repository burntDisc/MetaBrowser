#pragma once
#include "Drawable.h"
#include "Triangle.h"
#include "Player.h"
#include <vector>
#include <memory>

class Grid
	: public Drawable, public SolidObject
{
public:
	Grid(Shader& rawShader, Shader& textShader, Camera& camera, float unitLength, int iterations, Player& player);
	void TestTriangles();
	void FillTriangles();
	std::vector<glm::vec3> GetTriangleWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
	void Draw();
	void Update();
	bool ValidateCoord(int level, int index);
private:
	std::string ToIP(int input);
	const float height = 500.0f;
	Shader& textShader;
	int sideUnits, iterations;
	float unitLength, unitWidth;
	Player& player;
	std::vector<std::unique_ptr<Triangle>> triangles;
};

