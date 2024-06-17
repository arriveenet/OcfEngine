#version 460

uniform mat4 uProjection;
uniform mat4 uModelView;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

out vec4 fragColor;

void main()
{
	vec4 pos = vec4(inPosition, 0.0, 1.0);

	gl_Position = uProjection * uModelView * pos;

	fragColor = inColor;
}