#include "Triangle.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm> 

Triangle::Triangle(Shader& rawShader, Shader& textShader, Camera& camera, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float colorFac, Player& player, std::string label, glm::vec3 normal) :
	textShader(textShader),
	Drawable(rawShader, camera)
{

	glm::vec3 textOrigin = (v0 + v1 + v2) / 3.0f + glm::vec3(0.0f, 10.0f, 0.0f);
	//create Text
	glm::quat textRotation = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 textScale(1.0f, 1.0f, 1.0f);
	text = new Text(textShader, label, "arial.ttf", textOrigin, textRotation, textScale, player);


	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);


	glm::vec3 color;
	color.x = colorFac;
	color.y = colorFac;
	color.z = colorFac;;

	vertices.push_back({
		v0,
		normal,
		color
		});
	vertices.push_back({
		v1,
		normal,
		color
		});
	vertices.push_back({
		v2,
		normal,
		color
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

Triangle::~Triangle()
{
	delete text;
}

void Triangle::Draw()
{
	Drawable::Draw();
	text->RenderText(textShader, 0.0f, 0.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
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
