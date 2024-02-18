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


struct Vertex2f {
	glm::vec2 position;
	glm::vec2 texCoord;
};
