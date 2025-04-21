#pragma once
#include "base/GameObject.h"
#include <glm/glm.hpp>
#include <array>

NS_OCF_BEGIN

class Texture2D;

class SpritePalette : public GameObject {
public:
    static SpritePalette* create();

    SpritePalette();
    ~SpritePalette();

private:
    Texture2D* m_texture;
    std::array<glm::uvec4, 256> m_palette;
};

NS_OCF_END