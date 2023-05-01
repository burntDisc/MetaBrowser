#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;
in vec3 VertColor;


void main()
{
	// outputs final color
	FragColor[0] = VertColor[0];
	FragColor[1] = VertColor[1];
	FragColor[2] = VertColor[2];
	FragColor[3] = 0.5;
}