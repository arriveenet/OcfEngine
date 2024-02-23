#include "Label.h"

#include <glad/glad.h>
#include "Game.h"

OCF_BEGIN

Label* Label::create(const std::string& text)
{
	Label* label = new Label();

	Game::getInstance()->getRenderer()->addLabel(label);

	label->setString(text);

	return label;
}

Label::Label()
	: m_isUpdate(true)
{
	Font* pFont = Game::getInstance()->getFont();
	m_texture = pFont->getTexture();
}

Label::~Label()
{
}

void Label::setString(const std::string& text)
{
	if (m_text != text) {
		m_text = text;
		m_isUpdate = true;
	}
}

void Label::update(float deltaTime)
{
	if (m_isUpdate) {
		updateQuads();

		m_isUpdate = false;
	}

}

void Label::draw()
{
#if 0
	Font* pFont = Game::getInstance()->getFont();
	pFont->setText(m_position.x, m_position.y, m_text.c_str());
	pFont->draw();

#else
	if (m_quads.empty())
		return;

	glPushMatrix();

	glTranslatef(m_position.x, m_position.y, 0.0f);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3fT2f), &m_quads[0].topLeft.position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3fT2f), &m_quads[0].topLeft.texCoord);

	m_texture->setActive();

	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_SHORT, m_indices.data());

	glPopMatrix();
#endif
}

void Label::updateQuads()
{
	Font* pFont = Game::getInstance()->getFont();
	FntCommon common = pFont->getFntCommon();

	m_quads.clear();
	m_indices.clear();

	int x = 0, y = 0;

	for (int i = 0; i < m_text.size(); i++) {

		const char p = m_text.at(i);

		if (p == '\n') {
			x = 0;
			y += common.lineHeight;
			continue;
		}

		const FntChars pChar = pFont->getChar(p);

		float tx0 = static_cast<float>(pChar.x) / common.scaleW;
		float ty0 = static_cast<float>(pChar.y) / common.scaleH;
		float tx1 = static_cast<float>((pChar.x) + pChar.width) / common.scaleW;
		float ty1 = static_cast<float>((pChar.y) + pChar.height) / common.scaleH;

		QuadV3fT2f quad = { };
		quad.topLeft.position = { x + pChar.xoffset, y + pChar.yoffset, 0.0f };
		quad.topLeft.texCoord = { tx0, ty0 };

		quad.bottomLeft.position = { x + pChar.xoffset, y + pChar.yoffset + pChar.height, 0.0f };
		quad.bottomLeft.texCoord = { tx0, ty1 };

		quad.topRight.position = { x + pChar.xoffset + pChar.width, y + pChar.yoffset, 0.0f };
		quad.topRight.texCoord = { tx1, ty0 };

		quad.bottomRight.position = { x + pChar.xoffset + pChar.width, y + pChar.yoffset + pChar.height, 0.0f };
		quad.bottomRight.texCoord = { tx1, ty1 };

		m_quads.emplace_back(quad);

		x += pChar.xadvance;
	}

	m_indices.resize(m_text.size() * 6);
	for (int i = 0; i < m_text.size(); i++) {
		m_indices[(size_t)i * 6 + 0] = (unsigned short)i * 4 + 0;
		m_indices[(size_t)i * 6 + 1] = (unsigned short)i * 4 + 1;
		m_indices[(size_t)i * 6 + 2] = (unsigned short)i * 4 + 2;
		m_indices[(size_t)i * 6 + 3] = (unsigned short)i * 4 + 3;
		m_indices[(size_t)i * 6 + 4] = (unsigned short)i * 4 + 2;
		m_indices[(size_t)i * 6 + 5] = (unsigned short)i * 4 + 1;
	}
}

OCF_END
