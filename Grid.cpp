#include "Grid.h"
#include "SpatialIndexer.h"
#include <memory>

Grid::Grid(Shader& rawShader, Shader& textShader, Camera& camera, float unitWidth, int iterations, Player& player) :
	Drawable(rawShader, camera),
	textShader(textShader),
	player(player),
	unitWidth(unitWidth),
	iterations(iterations)
{
	sideUnits = pow(2, iterations);
	unitLength = unitWidth * sqrt(3) / 2.0f;
	
	FillTriangles();
}

void Grid::TestTriangles()
{
	triangles.clear();
	glm::vec3 v0 = glm::vec3(-500.0f, -height, -500.0f) + player.translation;
	glm::vec3 v1 = glm::vec3(500.0f, -height, -500.0f) + player.translation;
	glm::vec3 v2 = glm::vec3(0.0f, -height, 200.0f) + player.translation;
	triangles.push_back(
		std::make_unique<Triangle>(shader, textShader, camera, v0, v1, v2, 0.1f, player, "0"));
}

void Grid::FillTriangles()
{
	triangles.clear();

	//Box center coord
	//
	//		      .___.  high,high
    //           /	 /
	//	low,low .___.
	//
	//
	//

	int lowRoundedLevel = (int) (player.translation.z / unitLength);
	float lowZ = lowRoundedLevel * unitLength;
	float highZ = (lowRoundedLevel + 1) * unitLength;

	
	int lowRoundedIndex = (int)((player.translation.x - lowRoundedLevel * (unitWidth / 2.0f)) / unitWidth);
	float lowX = lowRoundedIndex * unitWidth + lowRoundedLevel * (unitWidth / 2.0f);
	float highX = lowX + unitWidth;

	//find closests vertex
	float centerVertX, centerVertZ;
	float highHighSqrDist = pow((player.translation.x - highX), 2) * pow((player.translation.z - highZ), 2);
	float highLowSqrDist = pow((player.translation.x - highX), 2) * pow((player.translation.z - lowZ), 2);
	float lowHighSqrDist = pow((player.translation.x - lowX), 2) * pow((player.translation.z - highZ), 2);
	float lowLowSqrDist = pow((player.translation.x - lowX), 2) * pow((player.translation.z - lowZ), 2);

	float lowestSqrDist = highHighSqrDist;
	centerVertX = highX;
	centerVertZ = highZ;

	if (highLowSqrDist < lowestSqrDist)
	{
		lowestSqrDist = highLowSqrDist;
		centerVertX = highX;
		centerVertZ = lowZ;
	}
	if (lowHighSqrDist < lowestSqrDist)
	{
		lowestSqrDist = lowHighSqrDist;
		centerVertX = lowX;
		centerVertZ = highZ;
	}
	if (lowLowSqrDist < lowestSqrDist)
	{
		lowestSqrDist = lowLowSqrDist;
		centerVertX = lowX;
		centerVertZ = lowZ;
	}

	glm::vec3 centerVertex(centerVertX, height, centerVertZ);

	int lowerLevel = (int)(centerVertZ / unitLength) - 1;
	int upperLevel = (int)(centerVertZ / unitLength);
	int lowerBaseindex = (int)(centerVertX / unitWidth) * 2 - 1;
	int upperBaseindex = lowerBaseindex - 1;
	int totalTriangles = pow(4, iterations);
	glm::vec3 v1, v2;
	int labelNum, index;

	//top left
	v1 = centerVertex + glm::vec3(-unitWidth, 0.0f, 0.0f);
	v2 = centerVertex + glm::vec3(-unitWidth / 2.0f, 0.0f, unitLength);
	index = upperBaseindex;
	if (ValidateCoord(upperLevel, index))
	{
		labelNum = SpatialIndexer::IndexFromPosition(upperLevel, index, sideUnits, iterations);
		triangles.push_back(
			std::make_unique<Triangle>(shader, textShader, camera, centerVertex, v1, v2, labelNum / 16.0f, player, "top left"));
	}

	//top mid
	v1 = centerVertex + glm::vec3(-unitWidth / 2.0f, 0.0f, unitLength);
	v2 = centerVertex + glm::vec3(unitWidth / 2.0f, 0.0f, unitLength);
	index = upperBaseindex + 1;
	if (ValidateCoord(upperLevel, index))
	{
		labelNum = SpatialIndexer::IndexFromPosition(upperLevel, index, sideUnits, iterations);
		triangles.push_back(
			std::make_unique<Triangle>(shader, textShader, camera, centerVertex, v1, v2, labelNum / 16.0f, player, "top mid"));
	}

	//top right
	v1 = centerVertex + glm::vec3(unitWidth, 0.0f, 0.0f);
	v2 = centerVertex + glm::vec3(unitWidth / 2.0f, 0.0f, unitLength);
	index = upperBaseindex + 2;
	if (ValidateCoord(upperLevel, index))
	{
		labelNum = SpatialIndexer::IndexFromPosition(upperLevel, index, sideUnits, iterations);
		triangles.push_back(
			std::make_unique<Triangle>(shader, textShader, camera, centerVertex, v1, v2, labelNum / 16.0f, player, "top right"));
	}

	//bottom left
	v1 = centerVertex + glm::vec3(-unitWidth, 0.0f, 0.0f);
	v2 = centerVertex + glm::vec3(-unitWidth / 2.0f, 0.0f, -unitLength);
	index = lowerBaseindex;
	if (ValidateCoord(lowerLevel, index))
	{
		labelNum = SpatialIndexer::IndexFromPosition(lowerLevel, index, sideUnits, iterations);
		triangles.push_back(
			std::make_unique<Triangle>(shader, textShader, camera, centerVertex, v1, v2, labelNum / 16.0f, player, "bottom left"));
	}

	//bottom middle
	v1 = centerVertex + glm::vec3(-unitWidth / 2.0f, 0.0f, -unitLength);
	v2 = centerVertex + glm::vec3(unitWidth / 2.0f, 0.0f, -unitLength);
	index = lowerBaseindex + 1;
	if (ValidateCoord(lowerLevel, index))
	{
		labelNum = SpatialIndexer::IndexFromPosition(lowerLevel, index, sideUnits, iterations);
		triangles.push_back(
			std::make_unique<Triangle>(shader, textShader, camera, centerVertex, v1, v2, labelNum / 16.0f, player, "bottom mid"));
	}

	//bottom right
	v1 = centerVertex + glm::vec3(unitWidth, 0.0f, 0.0f);
	v2 = centerVertex + glm::vec3(unitWidth / 2.0f, 0.0f, -unitLength);
	index = lowerBaseindex + 2;
	if (ValidateCoord(lowerLevel, index))
	{
		labelNum = SpatialIndexer::IndexFromPosition(lowerLevel, index, sideUnits, iterations);
		triangles.push_back(
			std::make_unique<Triangle>(shader, textShader, camera, centerVertex, v1, v2, labelNum / 16.0f, player, "bottom right"));
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

bool Grid::ValidateCoord(int level, int index)
{
	if (level < 0 || index < 0)
		return false;

	if (level > sideUnits)
		return false;

	if (index > sideUnits * 2 - 4 * level)
		return false;
	return true;
}

void Grid::Update()
{
	FillTriangles();
}
