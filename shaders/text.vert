#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;


// Imports the camera matrix
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
	vec3 vertexWorldPosition = vec3(translation * rotation  * scale * vec4(vertex.xy, 0.0, 1.0));
	gl_Position = projection * view * vec4(vertexWorldPosition, 1.0);
    TexCoords = vertex.zw;
}