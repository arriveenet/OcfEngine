#pragma once
#include "2d/Sprite.h"
#include <vector>

NS_OCF_BEGIN

typedef std::vector<Color3b> ColorPalette;

class IndexedColorSprite : public Sprite {
public:
    IndexedColorSprite();
    virtual ~IndexedColorSprite();

protected:


};

NS_OCF_END