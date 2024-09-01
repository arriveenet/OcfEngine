#include "Label.h"
#include "2d/FontManager.h"
#include "2d/DrawShape.h"
#include "base/Game.h"
#include "renderer/OpenGLInclude.h"

NS_OCF_BEGIN

static std::u32string convertUtf8ToUtf32(const std::string& utf8String)
{
    std::u32string result;

    for (int i = 0; i < utf8String.size(); i++) {
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

    m_texture = m_font->getTexture();

    Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Label);
    m_quadCommand.getProgramState().setProgram(pProgram);

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

void Label::update(float deltaTime)
{
    if (m_isDirty) {
        updateQuads();

        m_isDirty = false;
    }

}

void Label::draw(Renderer* renderer, const glm::mat4& transform)
{
    if (m_quads.empty())
        return;

    glm::mat4 projection = m_pGame->getMatrix(MatrixStack::Projection);
    auto& programState = m_quadCommand.getProgramState();
    programState.setUniform("uMVPMatrix", &projection, sizeof(projection));

    m_quadCommand.init(m_globalZOrder, m_texture, m_quads.data(), m_indices.data(), m_quads.size(), transform);
    renderer->addCommand(&m_quadCommand);
}

void Label::updateQuads()
{
    m_quads.clear();
    m_indices.clear();

    m_indices.resize(m_text.size() * 6);

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

        float tx0 = static_cast<float>(pChar.x) / m_texture->getSize().x;
        float ty0 = static_cast<float>(pChar.y) / m_texture->getSize().y;
        float tx1 = static_cast<float>((pChar.x) + pChar.width) / m_texture->getSize().x;
        float ty1 = static_cast<float>((pChar.y) + pChar.height) / m_texture->getSize().y;

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

        m_quads.emplace_back(quad);

        x += pChar.xadvance;

        lineWidth = (std::max)(x, lineWidth);

        m_indices[(size_t)i * 6 + 0] = (unsigned short)i * 4 + 0;
        m_indices[(size_t)i * 6 + 1] = (unsigned short)i * 4 + 1;
        m_indices[(size_t)i * 6 + 2] = (unsigned short)i * 4 + 2;
        m_indices[(size_t)i * 6 + 3] = (unsigned short)i * 4 + 3;
        m_indices[(size_t)i * 6 + 4] = (unsigned short)i * 4 + 2;
        m_indices[(size_t)i * 6 + 5] = (unsigned short)i * 4 + 1;
    }

    const float sizeWidth = lineWidth + 2.0f;
    const float sizeHeight = lineHeight * numberOfLines;
    setSize(sizeWidth, sizeHeight);

#if OCF_LABEL_DEBUG_DRAW
    m_pDebugDrawShape->clear();
    m_pDebugDrawShape->drawRect(glm::vec2(0.0f, 0.0f), glm::vec2(m_size), Color4f::WHITE);
#endif
}

NS_OCF_END
