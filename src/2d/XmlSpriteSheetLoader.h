#pragma once
#include "2d/SpriteSheetLoader.h"

namespace ocf {

class XmlSpriteSheetLoader : public ISpriteSheetLoader {
public:
    uint32_t getFormat() override { return SpriteSheetFormat::Xml; }
    void load(std::string_view filePath, SpriteFrameManager& manager) override;
};

} // namespace ocf