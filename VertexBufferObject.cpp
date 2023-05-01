#include"VertexBufferObject.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &ID);
}

void VertexBufferObject::Update(std::vector<VertexRaw>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexRaw), vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::Update(std::vector<Vertex>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::Update(std::vector<Vertex2D>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::Update(std::vector<glm::mat4>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::mat4), vertices.data(), GL_STATIC_DRAW);
}
// Binds the VertexBufferObject
void VertexBufferObject::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VertexBufferObject
void VertexBufferObject::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VertexBufferObject
void VertexBufferObject::Delete()
{
	glDeleteBuffers(1, &ID);
}