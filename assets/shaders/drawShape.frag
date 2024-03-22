#version 460

// Tex coord input from vertex shader
in vec4 fragColor;

// This corresponds to the output color to the color buffer
out vec4 outColor;

void main()
{
	outColor = vec4(1.0, 1.0, 1.0, 1.0);
}