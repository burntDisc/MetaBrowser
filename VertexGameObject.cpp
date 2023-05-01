#include "Triangle.h"

Triangle::Triangle(Shader shader, glm::vec3 initTranslation, glm::vec3 initScale, glm::quat initRotation, glm::vec3 modelOffset) :
	Drawable(shader),
	validWorldPositions(false),
	translation(initTranslation),
	rotation(initRotation),
	scale(initScale)
{

	VAO.Bind();
	VertexBufferObject VBO;
	VBO.Update(vertices);
	// Generates Element Buffer Object and links it to indices
	ElementBufferObject EBO;
	EBO.Update(indices);
	// Links VertexBufferObject attributes such as coordinates and colors to VertexArrayObject
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}



void Mesh::Draw
(
	Shader& shader,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale,
	glm::mat4 object
)
{
	// Bind shader to be set uniforms
	shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].TexUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// set position to glm matrices
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);

	translationMatrix = glm::translate(translationMatrix, translation);
	rotationMatrix = glm::mat4_cast(rotation);
	scaleMatrix = glm::scale(scaleMatrix, scale);

	// set position uniforms in shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "object"), 1, GL_FALSE, glm::value_ptr(object));

	// Draw mesh
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}

#include "Overlay.h"
#include <glm/gtc/matrix_transform.hpp>

Overlay::Overlay(Shader shader) : Drawable(shader)
{

}

Overlay::Overlay(Shader shader, std::vector<Vertex2D> vertices, std::vector<GLuint> indices) :
	Drawable(shader),
	vertices(vertices),
	indices(indices)
{
	Refresh();
}

void Overlay::Refresh()
{
	VAO.Bind();
	VBO.Update(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO.Update(indices);

	// Links VertexBufferObject attributes such as coordinates and colors to VertexArrayObject

	VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, sizeof(Vertex2D), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex2D), (void*)(2 * sizeof(float)));

	VBO_ID = VBO.ID;

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Overlay::Draw()
{
	// Bind shader to be set uniforms
	shader.Activate();
	VAO.Bind();

	// Draw mesh
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}