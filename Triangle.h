#pragma once
#include "Drawable.h"
#include "SolidObject.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Player.h"
#include "Text.h"
#include "ElementBufferObject.h"
#include <memory>

class Triangle :
	public Drawable,
	public SolidObject
{
public:
	Triangle(Shader& rawShader, Shader& textShader, Camera& camera, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float colorFac, Player& player, std::string label, glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f));
	~Triangle();
	void Draw();
	std::vector<glm::vec3> GetTriangleWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
private:
	Shader& textShader;
	std::unique_ptr<Text> text;
	glm::vec3 textOrigin = glm::vec3(0.1f);
	GLuint VBO_ID = 0;
	std::vector<VertexRaw> vertices;
	std::vector<GLuint> indices;
	VertexArrayObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;
};

