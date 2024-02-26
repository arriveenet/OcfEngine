#version 460

in vec2 fragTexCoord;

out vec4 outColor;


uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
    outColor = texture(uTexture, fragTexCoord);
}