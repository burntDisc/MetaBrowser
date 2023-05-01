#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Drawable.h"
#include "Shader.h"
#include "Model.h"
#include "SolidObject.h"

class GameObject :
	public Drawable,
	public SolidObject
{
public:
	GameObject(
		Shader shader,
		const char* modelFile = nullptr,
		glm::vec3 initTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 initScale = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat initRotation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 modelOffset = glm::vec3(0.0f, 0.0f, 0.0f));

	void Draw();
	void Delete();
	std::vector<glm::vec3> GetVertexWorldNormals();
	std::vector<glm::vec3> GetVertexWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
	std::vector<glm::vec3> GetTriangleWorldPositions();

	Model model;
protected:
	void UpdateWorldVertices();
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	std::vector<glm::vec3> vertexWorldNormals;
	std::vector<glm::vec3> vertexWorldPositions;
	std::vector<glm::vec3> triangleWorldNormals;
	std::vector<glm::vec3> triangleWorldPositions;
	bool validWorldPositions;
};