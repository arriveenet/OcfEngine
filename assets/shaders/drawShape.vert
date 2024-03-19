#version 460

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

// Any vertex outputs (other than position)
out vec4 fragColor;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 0.0, 1.0);
	// Transform to position world space, then clip space
	gl_Position = pos * uWorldTransform * uViewProj;

	fragColor = inColor;
}