#pragma once
#include <glm/glm.hpp>

enum class PixelFormat {
	GRAY,
	RGB,
	RGBA,
	NONE
};

enum class ButtonState {
	None,
	Pressed,
	Released,
	Held
};


struct Vertex2fT2f {
	glm::vec2 position;
	glm::vec2 texCoord;
};

struct Vertex3fT2f {
	glm::vec3 position;
	glm::vec2 texCoord;
};

struct QuadV3fT2f {
	Vertex3fT2f topLeft;
	Vertex3fT2f bottomLeft;
	Vertex3fT2f topRight;
	Vertex3fT2f bottomRight;
};
