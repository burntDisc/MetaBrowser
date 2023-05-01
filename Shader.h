#pragma once

#include<glad/glad.h>

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructors to build the Shader Program from shader files
	Shader(const char* vertexFile, const char* geometryFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();

	// Deletes the Shader Program
	void Delete();
private:
	// Checks if the different Shaders have compiled properly
	void CompileErrors(unsigned int shader, const char* type);
};