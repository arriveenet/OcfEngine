#include "SpriteFrame.h"
#include "base/Game.h"
#include "base/FileUtils.h"
#include "renderer/Texture2D.h"
#include "renderer/TextureManager.h"

namespace ocf {

SpriteFrame* SpriteFrame::createWithTexture(Texture2D* texture,
                                            const Rect& rect,
                                            bool rotated,
                                            const glm::vec2& offset, 
                                            const glm::vec2& originalSize)
{
    SpriteFrame* spriteFrame = new SpriteFrame();
    if (spriteFrame->initWithTexture(texture, rect, rotated, offset, originalSize)) {
        spriteFrame->autorelease();
        return spriteFrame;
    }

    delete spriteFrame;
    return nullptr;
}

SpriteFrame::SpriteFrame()
    : m_pTexture(nullptr)
    , m_offset(0.0f, 0.0f)
    , m_originalSize(0.0f, 0.0f)
    , m_rotated(false)
{
}

SpriteFrame::~SpriteFrame()
{
    OCF_SAFE_RELEASE(m_pTexture);
}

bool SpriteFrame::initWithTexture(Texture2D* texture,
                                  const Rect& rect,
                                  bool rotated,
                                  const glm::vec2& offset,
                                  const glm::vec2& originalSize)
{
    m_pTexture = texture;
    if (texture) {
        texture->retain();
    }

    m_rect = rect;
    m_offset = offset;
    m_originalSize = originalSize;
    m_rotated = rotated;

    return true;
}

bool SpriteFrame::initWithTextureFilename(std::string_view filename, const Rect& rect, bool rotated, const glm::vec2& offset, const glm::vec2& originalSize)
{
    if (FileUtils::getInstance()->isFileExist(filename.data())) {
        m_pTexture = nullptr;
        m_textureFilename = filename;
        m_rect = rect;
        m_rotated = rotated;
        m_offset = offset;
        m_originalSize = originalSize;
        return true;
    }
    return false;
}

Texture2D* SpriteFrame::getTexture() const
{
    if (m_pTexture != nullptr) {
        return m_pTexture;
    }

    if (!m_textureFilename.empty()) {
        return Game::getInstance()->getTextureManager()->addImage(m_textureFilename);
    }

    return nullptr;
}

} // namespace ocf
