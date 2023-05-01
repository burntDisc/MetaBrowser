#include"ElementBufferObject.h"

// Constructor that generates a Elements Buffer Object and links it to indices
ElementBufferObject::ElementBufferObject()
{
	glGenBuffers(1, &ID);
}

void ElementBufferObject::Update(std::vector<GLuint>& indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Binds the ElementBufferObject
void ElementBufferObject::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the ElementBufferObject
void ElementBufferObject::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the ElementBufferObject
void ElementBufferObject::Delete()
{
	glDeleteBuffers(1, &ID);
}