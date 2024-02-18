#pragma once
#include <glm/glm.hpp>

enum class PixelFormat {
	GRAY,
	RGB,
	RGBA,
	NONE
};


struct Vertex2f {
	glm::vec2 position;
	glm::vec2 texCoord;
};
