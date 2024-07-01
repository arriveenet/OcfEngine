#include "Label.h"
#include "2d/DrawShape.h"
#include "base/Game.h"
#include "renderer/OpenGLInclude.h"

NS_OCF_BEGIN

Label* Label::create(const std::string& text)
{
	Label* label = new Label();

	label->setString(text);

	return label;
}

Label::Label()
	: m_isDirty(true)
	, m_textColor(1.0f, 1.0f, 1.0f)
{
	init();

	Font* pFont = Game::getInstance()->getFont();
	m_texture = pFont->getTexture();
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
	Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Label);
	m_quadCommand.getProgramState().setProgram(pProgram);

	return true;
}

void Label::setString(const std::string& text)
{
	if (m_text != text) {
		m_text = text;
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
	programState.setUniform("uMVPMatrix", projection);

	m_quadCommand.init(m_globalZOrder, m_texture, m_quads.data(), m_indices.data(), m_quads.size(), transform);
	renderer->addCommand(&m_quadCommand);
}

void Label::updateQuads()
{
	Font* pFont = Game::getInstance()->getFont();
	FntCommon common = pFont->getFntCommon();

	m_quads.clear();
	m_indices.clear();

	m_indices.resize(m_text.size() * 6);

	int x = 0, y = 0;
	int lineWidth = 0;
	int numberOfLines = 1;

	for (int i = 0; i < m_text.size(); i++) {

		const char p = m_text.at(i);

		if (p == '\n') {
			x = 0;
			y -= common.lineHeight;
			numberOfLines++;
			continue;
		}

		const FntChars pChar = pFont->getChar(p);

		float tx0 = static_cast<float>(pChar.x) / common.scaleW;
		float ty0 = static_cast<float>(pChar.y) / common.scaleH;
		float tx1 = static_cast<float>((pChar.x) + pChar.width) / common.scaleW;
		float ty1 = static_cast<float>((pChar.y) + pChar.height) / common.scaleH;

		const float offsetY = static_cast<float>(common.lineHeight - pChar.yoffset - pChar.height);

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

	const float sizeWidth = static_cast<float>(lineWidth) + 2.0f;
	const float sizeHeight = static_cast<float>(common.lineHeight) * numberOfLines;
	setSize(sizeWidth, sizeHeight);

#if OCF_LABEL_DEBUG_DRAW
	m_pDebugDrawShape->clear();
	m_pDebugDrawShape->drawRect(glm::vec2(0.0f, 0.0f), glm::vec2(m_size), Color4f::WHITE);
#endif
}

NS_OCF_END
