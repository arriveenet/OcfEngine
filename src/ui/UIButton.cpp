#include "UIButton.h"
#include "2d/Sprite.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"

NS_OCF_BEGIN

namespace ui {

Button* Button::create()
{
	Button* button = new Button();
	if (button->init()) {
		return button;
	}
	OCF_SAFE_DELETE(button);
	return nullptr;
}

Button::Button()
	: m_pButtonNormalRenderer(nullptr)
	, m_pTextRenderer(nullptr)
	, m_onClick(nullptr)
{
}

Button::~Button()
{
}

bool Button::init()
{
	if (Widget::init()) {
		return true;
	}
	return false;
}

void Button::processInput(const InputState& inputState)
{
	m_pLineRenderer->clear();

	glm::vec2 mousePos = inputState.mouse.getPosition();
	if (this->hitTest(mousePos)) {
		if (inputState.mouse.getButtonState(Mouse::Left) == ButtonState::Pressed) {
			m_onClick();
		}

		m_pLineRenderer->drawRect(glm::vec2(0.0f, 0.0f), m_size, Color4f::WHITE);
	}
}

void Button::setOnClickCallback(std::function<void()> onClick)
{
	m_onClick = onClick;
}

void Button::setText(const std::string& text)
{
	if (text.compare(getText()) == 0) {
		return;
	}

	createTextRendererIfNull();

	m_pTextRenderer->setString(text);
	m_pTextRenderer->update(0.0f);

	updateTextLocation();
}

std::string Button::getText() const
{
	if (m_pTextRenderer == nullptr) {
		return "";
	}

	return m_pTextRenderer->getString();
}

void Button::initRenderer()
{
	m_pButtonNormalRenderer = Sprite::create("ButtonBlue.png");
	m_pButtonNormalRenderer->setAnchorPoint(glm::vec2(0.0f, 0.0f));
	addChild(m_pButtonNormalRenderer);

	m_pLineRenderer = DrawShape::create();
	addChild(m_pLineRenderer);

	setSize(m_pButtonNormalRenderer->getSize());
}

bool Button::createTextRendererIfNull()
{
	if (m_pTextRenderer == nullptr) {
		m_pTextRenderer = Label::create("");
		addChild(m_pTextRenderer);

		return true;
	}

	return false;
}

void Button::updateTextLocation()
{
	float x = (m_size.x - m_pTextRenderer->getSize().x) / 2.0f;
	glm::vec2 pos = (m_size - m_pTextRenderer->getSize()) / 2.0f;
	m_pTextRenderer->setPosition(pos);
}

}

NS_OCF_END
