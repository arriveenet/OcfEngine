#pragma once
#include <glm/glm.hpp>
#include "base/GameObject.h"
#include "base/Macros.h"

NS_OCF_BEGIN

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
    Hold
};

enum class MatrixStack {
    Projection,
    ModelView
};

struct Color3b {
    static const glm::u8vec3 BLACK;
    static const glm::u8vec3 WHITE;
    static const glm::u8vec3 RED;
    static const glm::u8vec3 GREEN;
    static const glm::u8vec3 BLUE;
};

struct Color4f {
    static const glm::vec4 BLACK;
    static const glm::vec4 WHITE;
    static const glm::vec4 RED;
    static const glm::vec4 GREEN;
    static const glm::vec4 BLUE;
};


struct Vertex2fT2f {
    glm::vec2 position;
    glm::vec2 texCoord;
};

struct Vertex2fC4 {
    glm::vec2 position;
    glm::vec4 color;
};

struct Vertex3fT2f {
    glm::vec3 position;
    glm::vec2 texCoord;
};

struct Vertex3fC3fT2f {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};

struct QuadV3fT2f {
    Vertex3fT2f topLeft;
    Vertex3fT2f bottomLeft;
    Vertex3fT2f topRight;
    Vertex3fT2f bottomRight;
};

struct QuadV3fC3fT2f {
    Vertex3fC3fT2f topLeft;
    Vertex3fC3fT2f bottomLeft;
    Vertex3fC3fT2f topRight;
    Vertex3fC3fT2f bottomRight;
};

NS_OCF_END
