#pragma once

#include<glad/glad.h>
#include"VertexBufferObject.h"

class VertexArrayObject
{
public:
	GLuint ID;
	// Constructor that generates a VertexArrayObject ID
	VertexArrayObject();
	// Links a VBO Attribute such as a position or color to the VertexArrayObject
	void LinkAttrib(VertexBufferObject& VertexBufferObject, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VertexArrayObject
	void Bind();
	// Unbinds the VertexArrayObject
	void Unbind();
	// Deletes the VertexArrayObject
	void Delete();
};