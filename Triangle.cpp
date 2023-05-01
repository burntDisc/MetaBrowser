#include "Triangle.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Triangle::Triangle(Shader shader) : Drawable(shader)
{
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);

	vertices.push_back({
		glm::vec3(-1500.0f, -500.0f, -1500.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
		});
	vertices.push_back({
		glm::vec3(-1500.0f, -500.0f, 1500.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
		});
	vertices.push_back({
		glm::vec3(2000.0f, -500.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		});

	VAO.Bind();
	VertexBufferObject VBO;
	VBO.Update(vertices);
	// Generates Element Buffer Object and links it to indices
	ElementBufferObject EBO;
	EBO.Update(indices);
	// Links VertexBufferObject attributes such as coordinates and colors to VertexArrayObject
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(VertexRaw), (void*)0);
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(VertexRaw), (void*)(6 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Triangle::Draw()
{
	// Bind shader to be set uniforms
	shader.Activate();
	VAO.Bind();

	// Draw mesh
	glDepthFunc(GL_LESS);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}

std::vector<glm::vec3> Triangle::GetTriangleWorldNormals()
{
	std::vector<glm::vec3> normals;

	for (int i = 0; i < vertices.size(); ++i)
		normals.push_back(vertices[i].normal);

	return normals;
}

std::vector<glm::vec3> Triangle::GetTriangleWorldPositions()
{
	std::vector<glm::vec3> positions;

	for (int i = 0; i < vertices.size(); ++i)
		positions.push_back(vertices[i].position);

	return positions;
}
