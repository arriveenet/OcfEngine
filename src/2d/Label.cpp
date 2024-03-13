#include "Label.h"
#include "base/Game.h"
#include "renderer/OpenGLInclude.h"

OCF_BEGIN

Label* Label::create(const std::string& text)
{
	Label* label = new Label();

	Game::getInstance()->getRenderer()->addLabel(label);

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
}

Label::~Label()
{
}

bool Label::init()
{
	m_vertexArray.bind();

	m_vertexArray.setStride(sizeof(Vertex3fC3fT2f));

	m_vertexArray.updateVertexData(m_quads.data(), sizeof(Vertex3fC3fT2f) * m_quads.size());
	m_vertexArray.updateIndexData(m_indices.data(), sizeof(unsigned short) * m_indices.size());

	m_vertexArray.setAttribute("inPosition", 0, 3, false, 0);
	m_vertexArray.setAttribute("inColor", 1, 3, false, sizeof(float) * 3);
	m_vertexArray.setAttribute("inTexCoord", 2, 2, false, sizeof(float) * 6);

	m_vertexArray.bindVertexBuffer();

	m_vertexArray.unbind();

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

void Label::update(float deltaTime)
{
	if (m_isDirty) {
		updateQuads();
		updateVertex();

		m_isDirty = false;
	}

}

void Label::draw()
{
	if (m_quads.empty())
		return;

	// シェーダを設定
	Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Label);
	glUseProgram(pProgram->getProgram());

	// プロジェクション行列、モデルビュー行列を設定
	Scene* scene = Game::getInstance()->getCurrentScene();
	glm::mat4 projection = scene->getDefaultCamera()->getProjectionMatrix();
	glm::mat4 view = scene->getDefaultCamera()->getViewMatrix();

	updateTransform();

	glm::mat4 modelView = view * m_transform;

	// 行列をシェーダに設定
	pProgram->setUniform("uViewProj", projection);
	pProgram->setUniform("uWorldTransform", modelView);

	// テクスチャを設定
	if (m_texture)
		m_texture->setActive();

	// VAOを設定
	m_vertexArray.bind();

	// 描画
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_SHORT, nullptr);
}

void Label::updateQuads()
{
	Font* pFont = Game::getInstance()->getFont();
	FntCommon common = pFont->getFntCommon();

	m_quads.clear();
	m_indices.clear();

	m_indices.resize(m_text.size() * 6);

	int x = 0, y = 0;

	for (int i = 0; i < m_text.size(); i++) {

		const char p = m_text.at(i);

		if (p == '\n') {
			x = 0;
			y -= common.lineHeight;
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

		m_indices[(size_t)i * 6 + 0] = (unsigned short)i * 4 + 0;
		m_indices[(size_t)i * 6 + 1] = (unsigned short)i * 4 + 1;
		m_indices[(size_t)i * 6 + 2] = (unsigned short)i * 4 + 2;
		m_indices[(size_t)i * 6 + 3] = (unsigned short)i * 4 + 3;
		m_indices[(size_t)i * 6 + 4] = (unsigned short)i * 4 + 2;
		m_indices[(size_t)i * 6 + 5] = (unsigned short)i * 4 + 1;
	}
}

void Label::updateVertex()
{
	m_vertexArray.bind();
	m_vertexArray.updateVertexData(m_quads.data(), sizeof(Vertex3fC3fT2f) * m_quads.size() * 4);
	m_vertexArray.updateIndexData(m_indices.data(), sizeof(unsigned short) * m_indices.size());
	m_vertexArray.unbind();
}

OCF_END
