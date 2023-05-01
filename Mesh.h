#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include "Shader.h"

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// Store VertexArrayObject in public so it can be used in the Draw function
	VertexArrayObject VAO;

	// Initializes the mesh
	Mesh
	(
		std::vector <Vertex>& vertices,
		std::vector <GLuint>& indices,
		std::vector <Texture>& textures
	);

	// Draws the mesh
	void Draw
	(
		Shader& shader,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::mat4 object = glm::mat4(1.0f)
	);
};