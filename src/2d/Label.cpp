#include "Label.h"
#include "2d/FontManager.h"
#include "2d/FontAtlas.h"
#include "2d/FontFreeType.h"
#include "2d/DrawShape.h"
#include "base/Game.h"
#include "base/StringUtils.h"
#include "renderer/ShaderManager.h"
#include "renderer/Renderer.h"

NS_OCF_BEGIN

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
    , m_labelType(LabelType::BMFONT)
    , m_textColor(1.0f, 1.0f, 1.0f)
    , m_isDirty(true)
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
    m_font = FontManager::getFontFNT("NatoSansJP.fnt");
    if (m_font == nullptr) {
        return false;
    }

    m_fontAtlas = m_font->getFontAtlas();

    updateBatchCommands();

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
    m_labelType = LabelType::BMFONT;

    updateBatchCommands();

    ShaderManager* shaderManager = ShaderManager::getInstance();
    setProgram(shaderManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

bool Label::initWithTTF(std::string_view ttfPath, int fontSize)
{
    FontFreeTypeConfig config;
    config.fontPath = ttfPath;
    config.fontSize = fontSize;
    config.glyphs = GlyphCollection::Dynamic;

    m_font = FontManager::getFontTTF(config);
    if (m_font == nullptr) {
        return false;
    }

    m_fontAtlas = m_font->getFontAtlas();
    m_labelType = LabelType::TTF;
    updateBatchCommands();

    ShaderManager* shaderManager = ShaderManager::getInstance();
    setProgram(shaderManager->getBuiltinProgram(ProgramType::Label));

    return true;
}

void Label::setString(std::string_view text)
{
    if (m_text != text) {
        m_text = text;
        m_utf32Text = StringUtils::convertUtf8ToUtf32(text);
        m_isDirty = true;
    }
}

void Label::setTextColor(const glm::vec3& textColor)
{
    m_textColor = textColor;
}

void Label::setTextColor(unsigned char r, unsigned char g, unsigned char b)
{
    m_textColor.r = r / 255.0f;
    m_textColor.g = g / 255.0f;
    m_textColor.b = b / 255.0f;
}

void Label::update(float /* deltaTime */)
{
    if (m_isDirty) {
        updateContent();
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

void Label::updateContent()
{
    if (m_fontAtlas != nullptr) {
        FontFreeType* freeType = dynamic_cast<FontFreeType*>(m_font);
        if (freeType != nullptr) {
            freeType->prepareLetterDefinitions(m_utf32Text);
        }
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

    for (size_t i = 0; i < m_utf32Text.size(); i++) {

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

        const float offsetY = pChar.yoffset;

        QuadV3fC3fT2f quad = { };
        quad.topLeft.position = { x + pChar.xoffset, y + offsetY + pChar.height, 0.0f };
        quad.topLeft.texCoord = { tx0, ty1 };
        quad.topLeft.color = m_textColor;

        quad.bottomLeft.position = { x + pChar.xoffset, y + offsetY, 0.0f };
        quad.bottomLeft.texCoord = { tx0, ty0 };
        quad.bottomLeft.color = m_textColor;

        quad.topRight.position = { x + pChar.xoffset + pChar.width, y + offsetY + pChar.height, 0.0f };
        quad.topRight.texCoord = { tx1, ty1 };
        quad.topRight.color = m_textColor;

        quad.bottomRight.position = { x + pChar.xoffset + pChar.width, y + offsetY, 0.0f };
        quad.bottomRight.texCoord = { tx1, ty0 };
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

void Label::updateBatchCommands()
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
