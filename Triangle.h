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
	Triangle(Shader shader);
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

