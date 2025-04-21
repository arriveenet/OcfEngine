#include "SpritePalette.h"

NS_OCF_BEGIN

SpritePalette* SpritePalette::create()
{
    return new SpritePalette();
}

SpritePalette::SpritePalette()
    : m_texture(nullptr)
    , m_palette{ glm::uvec4(0, 0, 0, 0) }
{
}


SpritePalette::~SpritePalette()
{
}

NS_OCF_END