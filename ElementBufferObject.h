#pragma once

#include <glad/glad.h>
#include <vector>

class ElementBufferObject
{
public:
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	ElementBufferObject();

	void Update(std::vector<GLuint>& indices);

	// Binds the ElementBufferObject
	void Bind();
	// Unbinds the ElementBufferObject
	void Unbind();
	// Deletes the ElementBufferObject
	void Delete();
};