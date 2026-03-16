#pragma once
#include <string>
#include <set>
#include <stdint.h>
#include "base/GameObject.h"

namespace ocf {

class SpriteFrameManager;

struct SpriteSheetFormat {
    enum : uint32_t {
        Xml = 1,
        Custom
    };
};

class SpriteSheet {
public:
    std::string path;
    uint32_t format;
    std::set<std::string> frames;
};

class ISpriteSheetLoader {
public:
    ISpriteSheetLoader();
    virtual ~ISpriteSheetLoader() = default;
    virtual uint32_t getFormat()                                              = 0;
    virtual void load(std::string_view filePath, SpriteFrameManager& manager) = 0;
};

} // namespace ocf
