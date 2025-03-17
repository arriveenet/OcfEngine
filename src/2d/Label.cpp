#include "Label.h"
#include "2d/FontManager.h"
#include "2d/FontAtlas.h"
#include "2d/FontFreeType.h"
#include "2d/DrawShape.h"
#include "base/Game.h"
#include "renderer/ShaderManager.h"
#include "renderer/Renderer.h"

NS_OCF_BEGIN

static std::u32string convertUtf8ToUtf32(const std::string& utf8String)
{
    std::u32string result;
    result.reserve(utf8String.size());

    for (size_t i = 0, size = utf8String.size(); i < size; i++) {
        char p = utf8String.at(i);
        char32_t unicode = 0;

        int numBytes = 0;
        if ((p & 0x80) == 0x00) {
            numBytes = 1;
        }
        else if ((p & 0xE0) == 0xC0) {
            numBytes = 2;
        }
        else if ((p & 0xF0) == 0xE0) {
            numBytes = 3;
        }
        else if ((p & 0xF8) == 0xF0) {
            numBytes = 4;
        }

        switch (numBytes) {
        case 1:
            unicode = p;
            result.push_back(unicode);
            break;
        case 2:
            unicode = (p & 0x1F) << 6;
            unicode |= (utf8String[i + 1] & 0x3F);
            result.push_back(unicode);
            break;
        case 3:
            unicode = (p & 0x0F) << 12;
            unicode |= (utf8String[i + 1] & 0x3F) << 6;
            unicode |= (utf8String[i + 2] & 0x3F);
            result.push_back(unicode);
            break;
        case 4:
            unicode = (p & 0x07) << 18;
            unicode |= (utf8String[i + 1] & 0x3F) << 12;
            unicode |= (utf8String[i + 2] & 0x3F) << 6;
            unicode |= (utf8String[i + 3] & 0x3F) << 6;
            result.push_back(unicode);
            break;
        default:
            break;
        }
    }

    return result;
}

Label* Label::create(const std::string& text)
{
    Label* label = new Label();
    if (label->init()) {
        label->autorelease();
        label->setString(text);
        return label;
    }
    OCF_SAFE_DELETE(label);
    return nullptr;
}

Label* Label::createWithBMFont(std::string_view bmFontPath, std::string_view text)
{
    Label* label = new Label();
    if (label->initWithBMFont(bmFontPath)) {
        label->autorelease();
        label->setString(text.data());
        return label;
    }
    OCF_SAFE_DELETE(label);
    return nullptr;
}

Label* Label::createWithTTF(std::string_view ttfPath, std::string_view text, int fontSize)
{
    Label* label = new Label();
    if (label->initWithTTF(ttfPath, fontSize)) {
        label->autorelease();
        label->setString(text.data());
        return label;
    }
    OCF_SAFE_DELETE(label);
    return nullptr;
}

Label::Label()
    : m_font(nullptr)
    , m_isDirty(true)
    , m_textColor(1.0f, 1.0f, 1.0f)
{
#if OCF_LABEL_DEBUG_DRAW
    m_pDebugDrawShape = DrawShape::create();
    addChild(m_pDebugDrawShape);
#endif
}

Label::~Label()
{
}

bool Label::init()
{
    m_font = FontManager::getFontFNT("Consolas.fnt");
    if (m_font == nullptr) {
        return false;
    }

    m_fontAtlas = m_font->getFontAtlas();

    updateContent();

    ShaderManager* shaderManager = ShaderManager::getInstance();
    setProgram(shaderManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

bool Label::initWithBMFont(std::string_view bmFontPath)
{
    m_font = FontManager::getFontFNT(bmFontPath);
    if (m_font == nullptr) {
        return false;
    }

    m_fontAtlas = m_font->getFontAtlas();

    updateContent();

    ShaderManager* shaderManager = ShaderManager::getInstance();
    setProgram(shaderManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

bool Label::initWithTTF(std::string_view ttfPath, int fontSize)
{
    FontFreeTypeConfig config;
    config.fontPath = ttfPath;
    config.fontSize = fontSize;

    m_font = FontManager::getFontTTF(config);
    if (m_font == nullptr) {
        return false;
    }

    ShaderManager* shaderManager = ShaderManager::getInstance();
    setProgram(shaderManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

void Label::setString(const std::string& text)
{
    if (m_text != text) {
        m_text = text;
        m_utf32Text = convertUtf8ToUtf32(text);
        m_isDirty = true;
    }
}

void Label::setTextColor(const glm::vec3& textColor)
{
    m_textColor = textColor;
}

void Label::setTextColor(unsigned char r, unsigned char g, unsigned b)
{
    m_textColor.r = r / 255.0f;
    m_textColor.g = g / 255.0f;
    m_textColor.b = b / 255.0f;
}

void Label::update(float /* deltaTime */)
{
    if (m_isDirty) {
        updateQuads();

        m_isDirty = false;
    }

}

void Label::draw(Renderer* renderer, const glm::mat4& transform)
{
    if (m_batchCommands.empty())
        return;

    glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    for (auto& batchCommand : m_batchCommands) {
        if (batchCommand.quads.empty())
            continue;

        auto& programState = batchCommand.quadCommand.getProgramState();
        programState.setUniform("uMVPMatrix", &projection, sizeof(projection));
        batchCommand.quadCommand.init(m_globalZOrder,
                                      batchCommand.texture,
                                      batchCommand.quads.data(),
                                      batchCommand.indices.data(),
                                      batchCommand.quads.size(),
                                      transform);
        renderer->addCommand(&batchCommand.quadCommand);
    }
}

void Label::updateQuads()
{
    for (auto& batchCommand : m_batchCommands) {
        batchCommand.clear();
    }

    float x = 0.0f, y = 0.0f;
    float lineWidth = 0.0f;
    float lineHeight = m_font->getLineHeight();
    int numberOfLines = 1;

    for (int i = 0; i < m_utf32Text.size(); i++) {

        const char32_t p = m_utf32Text.at(i);

        if (p == '\n') {
            x = 0;
            y -= lineHeight;
            numberOfLines++;
            continue;
        }

        auto& pChar = m_font->m_characterDefinition[p];

        const float textureWidth = m_fontAtlas->getTexture(pChar.page)->getSize().x;
        const float textureHeight = m_fontAtlas->getTexture(pChar.page)->getSize().y;

        float tx0 = static_cast<float>(pChar.x) / textureWidth;
        float ty0 = static_cast<float>(pChar.y) / textureHeight;
        float tx1 = static_cast<float>((pChar.x) + pChar.width) / textureWidth;
        float ty1 = static_cast<float>((pChar.y) + pChar.height) / textureHeight;

        const float offsetY = static_cast<float>(lineHeight - pChar.yoffset - pChar.height);

        QuadV3fC3fT2f quad = { };
        quad.topLeft.position = { x + pChar.xoffset, y + offsetY + pChar.height, 0.0f };
        quad.topLeft.texCoord = { tx0, ty0 };
        quad.topLeft.color = m_textColor;

        quad.bottomLeft.position = { x + pChar.xoffset, y + offsetY, 0.0f };
        quad.bottomLeft.texCoord = { tx0, ty1 };
        quad.bottomLeft.color = m_textColor;

        quad.topRight.position = { x + pChar.xoffset + pChar.width, y + offsetY + pChar.height, 0.0f };
        quad.topRight.texCoord = { tx1, ty0 };
        quad.topRight.color = m_textColor;

        quad.bottomRight.position = { x + pChar.xoffset + pChar.width, y + offsetY, 0.0f };
        quad.bottomRight.texCoord = { tx1, ty1 };
        quad.bottomRight.color = m_textColor;

        x += pChar.xadvance;

        lineWidth = (std::max)(x, lineWidth);

        m_batchCommands.at(pChar.page).insertQuad(quad);
    }

    const float sizeWidth = lineWidth + 2.0f;
    const float sizeHeight = lineHeight * numberOfLines;
    setSize(sizeWidth, sizeHeight);

#if OCF_LABEL_DEBUG_DRAW
    m_pDebugDrawShape->clear();
    m_pDebugDrawShape->drawRect(glm::vec2(0.0f, 0.0f), glm::vec2(m_size), Color4f::WHITE);
#endif
}

void Label::updateContent()
{
    if (m_fontAtlas == nullptr) {
        return;
    }

    const auto pageCount = m_fontAtlas->getPageCount();
    m_batchCommands.resize(pageCount);

    for (size_t i = 0; i < pageCount; i++) {
        m_batchCommands[i].texture = m_fontAtlas->getTexture(static_cast<unsigned int>(i));
    }
}

void Label::setProgram(Program* program)
{
    for (auto& batchCommand : m_batchCommands) {
        batchCommand.quadCommand.getProgramState().setProgram(program);
    }
}

Label::BatchCommand::BatchCommand()
    : texture(nullptr)
{
}

void Label::BatchCommand::clear()
{
    quads.clear();
    indices.clear();
}

void Label::BatchCommand::insertQuad(const QuadV3fC3fT2f& quad)
{
    const auto index = quads.size();

    quads.emplace_back(quad);

    indices.reserve(indices.size() + 6);

    indices.emplace_back(static_cast<unsigned short>(index * 4 + 0));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 1));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 2));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 3));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 2));
    indices.emplace_back(static_cast<unsigned short>(index * 4 + 1));

}


NS_OCF_END
