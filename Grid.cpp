#include "Grid.h"
#include "SpatialIndexer.h"

Grid::Grid(Shader& rawShader, Shader& textShader, Camera& camera, float baseLength, int iterations, Player& player) :
	Drawable(rawShader, camera)
{
	float height = -500.0f;
	int baseNum = pow(2, iterations);
	float colorMax = pow(4, iterations);
	float triangleSideLength = baseLength / baseNum;
	float triangleHeight = triangleSideLength * sqrt(3) / 2.0f;


	glm::vec3 vert0Init(0.0f, height, 0.0f);
	glm::vec3 vert1Init(triangleSideLength / 2, height, triangleHeight);

	glm::vec3 levelAdjustment(triangleSideLength / 2.0f, 0.0f, triangleHeight);

	for (int level = 0; level < baseNum; ++level)
	{
		glm::vec3 vert0 = vert0Init + levelAdjustment * (float)level;
		glm::vec3 vert1 = vert1Init + levelAdjustment * (float)level;

		for (int i = 0; i < baseNum - level; ++i)
		{
			glm::vec3 vert3 = vert0 + glm::vec3(triangleSideLength, 0.0f, 0.0f);

			int colorVal0 = SpatialIndexer::IndexFromPosition(level, i * 2, baseNum, iterations);
			float colorFac0 = (float)colorVal0 / colorMax;
			triangles.push_back(new Triangle(rawShader, textShader, camera, vert0, vert1, vert3, colorFac0, player, std::to_string(colorVal0)));

			if (i + 1 < baseNum - level)
			{
				vert0 = vert3;
				vert3 = vert1 + glm::vec3(triangleSideLength, 0.0f, 0.0f);
				int colorVal1 = SpatialIndexer::IndexFromPosition(level, i * 2 + 1, baseNum, iterations);
				float colorFac1 = (float)colorVal1 / colorMax;
				triangles.push_back(new Triangle(rawShader, textShader, camera, vert0, vert1, vert3, colorFac1, player, std::to_string(colorVal1)));

				vert1 = vert3;
			}
		}

	}
}

Grid::~Grid()
{
	for (int i = 0; i < triangles.size(); ++i)
	{
		delete triangles[i];
	}
}

std::vector<glm::vec3> Grid::GetTriangleWorldPositions()
{
	std::vector<glm::vec3> positions;
	for (int i = 0; i < triangles.size(); ++i)
	{
		std::vector trianglePositions = triangles[i]->GetTriangleWorldPositions();
		positions.insert(positions.end(), trianglePositions.begin(), trianglePositions.end());
	}
	return positions;
}

std::vector<glm::vec3> Grid::GetTriangleWorldNormals()
{
	std::vector<glm::vec3> normals;
	for (int i = 0; i < triangles.size(); ++i)
	{
		std::vector triangleNormals = triangles[i]->GetTriangleWorldNormals();
		normals.insert(normals.end(), triangleNormals.begin(), triangleNormals.end());
	}
	return normals;
}
void Grid::Draw()
{
	for (int i = 0; i < triangles.size(); ++i)
	{
		triangles[i]->Draw();
	}
}
