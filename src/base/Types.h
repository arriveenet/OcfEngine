#pragma once
#include <glm/glm.hpp>
#include "base/GameObject.h"
#include "base/Macros.h"

namespace ocf {

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

enum class GlyphCollection {
    Dynamic,
    Ascii,
    Custom,
};

enum class CameraFlag : uint16_t {
    Default = 1,
    User1 = 1 << 1,
    User2 = 1 << 2,
    User3 = 1 << 3,
    User4 = 1 << 4,
};

struct Color3b {
    static const glm::u8vec3 BLACK;
    static const glm::u8vec3 WHITE;
    static const glm::u8vec3 GRAY;
    static const glm::u8vec3 RED;
    static const glm::u8vec3 GREEN;
    static const glm::u8vec3 BLUE;
    static const glm::u8vec3 YELLOW;
    static const glm::u8vec3 ORANGE;
    static const glm::u8vec3 MAGENTA;
};

struct Color4f {
    static const glm::vec4 BLACK;
    static const glm::vec4 WHITE;
    static const glm::vec4 GRAY;
    static const glm::vec4 RED;
    static const glm::vec4 GREEN;
    static const glm::vec4 BLUE;
    static const glm::vec4 YELLOW;
    static const glm::vec4 ORANGE;
    static const glm::vec4 MAGENTA;
};


struct Vertex2fT2f {
    glm::vec2 position;
    glm::vec2 texCoord;
};

struct Vertex2fC4f {
    glm::vec2 position;
    glm::vec4 color;
};

struct Vertex3fT2f {
    glm::vec3 position;
    glm::vec2 texCoord;
};

struct Vertex3fC4f {
    glm::vec3 position;
    glm::vec4 color;
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

} // namespace ocf
