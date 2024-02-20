#include "Label.h"
#include "Game.h"

Label* Label::create(const std::string& text)
{
	Label* result = new Label();

	Game::getInstance()->getRenderer()->addLabel(result);

	result->setString(text);

	return result;
}

Label::Label()
{
}

Label::~Label()
{
}

void Label::setString(const std::string& text)
{
	if (m_text != text) {
		m_text = text;
	}
}

void Label::update(float deltaTime)
{
	updateQuads();
}

void Label::draw()
{
	glPushMatrix();

	glTranslatef(m_position.x, m_position.y, 0.0f);

	Font* pFont = Game::getInstance()->getFont();
	pFont->getTexture()->setActive();

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3fT2f), &m_quads[0].topLeft.position);
	glTexCoordPointer(2, GL_FLAT, sizeof(Vertex3fT2f), &m_quads[0].topLeft.texCoord);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());

	glPopMatrix();
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
		float tx1 =(static_cast<float>(pChar.x) + pChar.width) / common.scaleW;
		float ty1 = (static_cast<float>(pChar.y) + pChar.height) / common.scaleH;

		QuadV3fT2f quad = { };
		quad.topLeft.position = { x + pChar.xoffset, y + pChar.yoffset, 0.0f };
		quad.topLeft.texCoord = { tx0, ty0 };

		quad.bottomLeft.position = { x + pChar.xoffset + pChar.width, y + pChar.yoffset, 0.0f };
		quad.bottomLeft.texCoord = {tx1, ty0};

		quad.topRight.position = { x + pChar.xoffset, y + pChar.yoffset + pChar.height, 0.0f };
		quad.topRight.texCoord = {tx0, ty1};

		quad.bottomRight.position = { x + pChar.xoffset + pChar.width, y + pChar.yoffset + pChar.height, 0.0f };
		quad.bottomRight.texCoord = {tx1, ty1};

		static unsigned int index[] = { 0,1,3,0,3,2 };
		int start = i * 6;
		for (int j = 0; j < 6; j++) {
			m_indices.push_back(start + index[j]);
		}

		m_quads.emplace_back(quad);

		x += pChar.xadvance;
	}
}
