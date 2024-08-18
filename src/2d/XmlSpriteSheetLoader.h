#pragma once
#include "2d/SpriteSheetLoader.h"

NS_OCF_BEGIN

class XmlSpriteSheetLoader : public ISpriteSheetLoader {
public:
    uint32_t getFormat() override { return SpriteSheetFormat::Xml; }
    void load(std::string_view filePath, SpriteFrameManager& manager) override;
};

NS_OCF_END