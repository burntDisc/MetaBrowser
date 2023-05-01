#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 3) in vec2 aTex;


// Outputs the current position for the Fragment Shader
out vec3 vertexWorldPosition;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;



// Imports the camera matrix
uniform mat4 view;
uniform mat4 projection;
// Imports the transformation matrices
uniform mat4 object;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{
	// calculates current position
	vertexWorldPosition = vec3(model * translation * rotation * object * scale * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * vec4(vertexWorldPosition, 1.0);
}