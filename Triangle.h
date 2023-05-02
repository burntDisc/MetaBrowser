#pragma once
#include "Drawable.h"
#include "SolidObject.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

class Triangle :
	public Drawable,
	public SolidObject
{
public:
	Triangle(Shader shader, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float colorFac, glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f));
	void Draw();
	std::vector<glm::vec3> GetTriangleWorldPositions();
	std::vector<glm::vec3> GetTriangleWorldNormals();
protected:
	GLuint VBO_ID = 0;
	std::vector<VertexRaw> vertices;
	std::vector<GLuint> indices;
	VertexArrayObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;
};

