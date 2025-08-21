#pragma once
#include <unordered_map>
#include <map>
#include <memory>
#include "2d/SpriteFrame.h"
#include "2d/SpriteSheetLoader.h"

namespace ocf {

class SpriteFrameManager {
public:
    static SpriteFrameManager* getInstance();

    static void destroyInstance();

    virtual ~SpriteFrameManager();

    bool init();

    void addSpriteFramesWithFile(std::string_view spriteSheetFilename, uint32_t spriteSheetFormat = SpriteSheetFormat::Xml);

    SpriteFrame* findFrame(std::string_view frame);

    SpriteFrame* getSpriteFrameByName(std::string_view name);

    void insertFrame(const std::shared_ptr<SpriteSheet>& spriteSheet, std::string_view frameName, SpriteFrame* spriteFrame);

    void clear();

    void registerSpriteSheetLoader(std::shared_ptr<ISpriteSheetLoader> loader);

    ISpriteSheetLoader* getSpriteSheetLoader(uint32_t spriteSheetFormat);

protected:
    SpriteFrameManager();

private:
    std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> m_spriteFrames;
    std::unordered_map<std::string, std::shared_ptr<SpriteSheet>> m_spriteSheets;
    std::map<uint32_t, std::shared_ptr<ISpriteSheetLoader>> m_spriteSheetLoader;
};

} // namespace ocf