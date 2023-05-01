#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 color;

out vec3 VertColor;

// Imports the camera matrix
uniform mat4 view;
uniform mat4 projection;


void main()
{
	VertColor = color;
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * vec4(aPos, 1.0);
}