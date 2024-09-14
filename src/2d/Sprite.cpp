#include "Sprite.h"

#include <glm/gtc/type_ptr.hpp>
#include "2d/SpriteFrameManager.h"
#include "base/Game.h"
#include "renderer/Texture2D.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/ShaderManager.h"

using namespace glm;

NS_OCF_BEGIN

static unsigned short indices[] = { 0, 1, 2, 3, 2, 1 };

Sprite* Sprite::create()
{
    Sprite* pSprite = new Sprite();
    if (pSprite->init()) {
        pSprite->autorelease();
        return pSprite;
    }
    OCF_SAFE_DELETE(pSprite);
    return nullptr;
}

Sprite* Sprite::create(const std::string& filenam)
{
    Sprite* pSprite = new Sprite();
    if (pSprite->initWithFile(filenam)) {
        pSprite->autorelease();
        return pSprite;
    }
    OCF_SAFE_DELETE(pSprite);
    return nullptr;
}

Sprite* Sprite::createWithSpriteFrame(SpriteFrame* spriteFrame)
{
    Sprite* pSprite = new Sprite();
    if (pSprite->initWithSpriteFrame(spriteFrame)) {
        pSprite->autorelease();
        return pSprite;
    }
    OCF_SAFE_DELETE(pSprite);
    return nullptr;
}


Sprite* Sprite::createWithSpriteFrameName(std::string_view spriteFrameName)
{
    SpriteFrame* frame = SpriteFrameManager::getInstance()->getSpriteFrameByName(spriteFrameName);

    return createWithSpriteFrame(frame);
}

Sprite::Sprite()
    : m_quad()
    , m_isDirty(true)
    , m_modelView(1.0f)
    , m_flippedX(false)
    , m_flippedY(false)
    , m_texture(nullptr)
    , m_spriteFrame(nullptr)
{
    Program* pProgram = ShaderManager::getInstance()->getBuiltinProgram(ProgramType::Basic);
    m_trianglesCommand.getProgramState().setProgram(pProgram);

#if OCF_SPRITE_DEBUG_DRAW
    m_pDebugDrawShape = DrawShape::create();
    addChild(m_pDebugDrawShape);
#endif
}

Sprite::~Sprite()
{
    OCF_SAFE_RELEASE(m_texture);
}

bool Sprite::init()
{
    initWithTexture(nullptr, Rect(0, 0, 0, 0));
    return true;
}

bool Sprite::initWithFile(const std::string& filename)
{
    Texture2D* texture = Game::getInstance()->getTextureManager()->addImage(filename);
    if (texture != nullptr) {
        Rect rect;
        rect.m_size = texture->getSize();
        return initWithTexture(texture, rect);
    }

    return true;
}

bool Sprite::initWithTexture(Texture2D* texture, const Rect& rect)
{
    bool result = false;
    if (Node::init()) {
        m_flippedX = m_flippedY = false;

        setAnchorPoint(glm::vec2(0.5f, 0.5f));

        // F‚ðÝ’è
        m_quad.topLeft.color     = { 1.0f, 1.0f, 1.0f };
        m_quad.bottomLeft.color  = { 1.0f, 1.0f, 1.0f };
        m_quad.topRight.color    = { 1.0f, 1.0f, 1.0f };
        m_quad.bottomRight.color = { 1.0f, 1.0f, 1.0f };

        m_rect = rect;

        setTexture(texture);
        setTextureRect(rect, rect.m_size);

        result = true;
    }

    return result;
}

bool Sprite::initWithSpriteFrame(SpriteFrame* spriteFrame)
{
    if (spriteFrame == nullptr) {
        return false;
    }

    bool result = initWithTexture(spriteFrame->getTexture(), spriteFrame->getRect());
    setSpriteFrame(spriteFrame);

    return result;
}

void Sprite::setTexture(Texture2D* texture)
{
    if (texture == nullptr) {
        texture = m_pGame->getTextureManager()->getWhiteTexture();
    }

    if (m_texture != texture) {
        OCF_SAFE_RETAIN(texture);
        OCF_SAFE_RELEASE(m_texture);
        m_texture = texture;
    }
}

void Sprite::setSpriteFrame(SpriteFrame* spriteFrame)
{
    if (m_spriteFrame != spriteFrame) {
        OCF_SAFE_RELEASE(m_spriteFrame);
        m_spriteFrame = spriteFrame;
        spriteFrame->retain();
    }

    Texture2D* texture = spriteFrame->getTexture();
    if (m_texture != texture) {
        setTexture(texture);
    }

    setTextureRect(spriteFrame->getRect(), spriteFrame->getRect().m_size);
}

SpriteFrame* Sprite::getSpriteFrame() const
{
    return m_spriteFrame;
}

void Sprite::setPosition(const glm::vec2& position)
{
    Node::setPosition(position);
}

void Sprite::setPosition(float x, float y)
{
    Node::setPosition(x, y);
}

void Sprite::setSize(float width, float height)
{
    Node::setSize(width, height);
    updatePolygon();

    m_isDirty = true;
}

Rect Sprite::getRect() const
{
    return m_rect;
}

void Sprite::draw(Renderer* renderer, const glm::mat4& transform)
{
    setMVPMarixUniform();

    m_trianglesCommand.init(m_globalZOrder, m_texture, m_triangles, transform);

    renderer->addCommand(&m_trianglesCommand);

#if OCF_SPRITE_DEBUG_DRAW
    m_pDebugDrawShape->clear();

    int count = m_triangles.indexCount / 3;
    auto vertices = m_triangles.vertices;
    auto indices = m_triangles.indices;
    for (int i = 0; i < count; i++) {
        glm::vec3 from = vertices[indices[i * 3]].position;
        glm::vec3 to   = vertices[indices[i * 3 + 1]].position;
        m_pDebugDrawShape->drawLine(glm::vec2(from.x, from.y), glm::vec2(to.x, to.y), Color4f::WHITE);

        from = vertices[indices[i * 3 + 1]].position;
        to   = vertices[indices[i * 3 + 2]].position;
        m_pDebugDrawShape->drawLine(glm::vec2(from.x, from.y), glm::vec2(to.x, to.y), Color4f::WHITE);

        from = vertices[indices[i * 3 + 2]].position;
        to   = vertices[indices[i * 3]].position;
        m_pDebugDrawShape->drawLine(glm::vec2(from.x, from.y), glm::vec2(to.x, to.y), Color4f::WHITE);
    }
#endif
}

void Sprite::setFlippedX(bool flippedX)
{
    if (m_flippedX != flippedX) {
        m_flippedX = flippedX;
        flipX();
    }
}

void Sprite::setFlippedY(bool flippedY)
{
    if (m_flippedY != flippedY) {
        m_flippedY = flippedY;
        flipY();
    }
}

bool Sprite::isFlippedX() const
{
    return m_flippedX;
}

bool Sprite::isFlippedY() const
{
    return m_flippedY;
}

void Sprite::updatePolygon()
{
    Rect copyRect;
    copyRect.m_position = glm::vec2(0.0f, 0.0f);
    copyRect.m_size = m_size;

    setTextureCoords(m_rect, &m_quad);
    setVertexCoords(copyRect, &m_quad);

    m_triangles.vertices = &m_quad.topLeft;
    m_triangles.vertexCount = 4;
    m_triangles.indices = indices;
    m_triangles.indexCount = 6;
}

void Sprite::setTextureRect(const Rect& rect, const glm::vec2& size)
{
    Node::setSize(size);

    setVertexRect(rect);
    updatePolygon();
}

void Sprite::setVertexRect(const Rect& rect)
{
    m_rect = rect;
}

void Sprite::setTextureCoords(const Rect& rectInPoints, QuadV3fC3fT2f* outQuad)
{
    if (m_texture == nullptr) {
        return;
    }

    const float atlasWidth = static_cast<float>(m_texture->getWidth());
    const float atlasHeight = static_cast<float>(m_texture->getHeight());

    float left   = rectInPoints.m_position.x / atlasWidth;
    float right  = (rectInPoints.m_position.x + rectInPoints.m_size.x) / atlasWidth;
    float top    = rectInPoints.m_position.y / atlasHeight;
    float bottom = (rectInPoints.m_position.y + rectInPoints.m_size.y) / atlasHeight;

    outQuad->bottomLeft.texCoord  = { left, bottom };
    outQuad->bottomRight.texCoord = { right, bottom };
    outQuad->topLeft.texCoord     = { left, top };
    outQuad->topRight.texCoord    = { right, top };
}

void Sprite::setVertexCoords(const Rect& rect, QuadV3fC3fT2f* outQuad)
{
    const float x1 = 0.0f;
    const float y1 = 0.0f;
    const float x2 = x1 + m_size.x;
    const float y2 = y1 + m_size.y;

    outQuad->bottomLeft.position  = { x1, y1, 0.0f };
    outQuad->bottomRight.position = { x2, y1, 0.0f };
    outQuad->topLeft.position     = { x1, y2, 0.0f };
    outQuad->topRight.position    = { x2, y2, 0.0f };
}

void Sprite::flipX()
{
    std::swap(m_quad.topLeft.texCoord, m_quad.topRight.texCoord);
    std::swap(m_quad.bottomLeft.texCoord, m_quad.bottomRight.texCoord);
}

void Sprite::flipY()
{
    std::swap(m_quad.topLeft.texCoord, m_quad.bottomLeft.texCoord);
    std::swap(m_quad.topRight.texCoord, m_quad.bottomRight.texCoord);
}

void Sprite::setMVPMarixUniform()
{
    glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    auto& programState = m_trianglesCommand.getProgramState();
    programState.setUniform("uMVPMatrix", &projection, sizeof(projection));
}

NS_OCF_END
