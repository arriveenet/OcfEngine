#include "UIButton.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

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
	, m_pTextLabel(nullptr)
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
	glm::vec2 mousePos = inputState.mouse.getPosition();
	if (this->hitTest(mousePos)) {
		if (inputState.mouse.getButtonState(Mouse::Left) == ButtonState::Pressed) {
			m_onClick();
		}
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

	m_pTextLabel->setString(text);
}

std::string Button::getText() const
{
	if (m_pTextLabel == nullptr) {
		return "";
	}

	return m_pTextLabel->getString();
}

void Button::initRenderer()
{
	m_pButtonNormalRenderer = Sprite::create("ButtonBlue.png");
	m_pButtonNormalRenderer->setAnchorPoint(glm::vec2(0.0f, 0.0f));
	addChild(m_pButtonNormalRenderer);

	setSize(m_pButtonNormalRenderer->getSize());
}

bool Button::createTextRendererIfNull()
{
	if (m_pTextLabel == nullptr) {
		m_pTextLabel = Label::create("");
		addChild(m_pTextLabel);

		return true;
	}

	return false;
}

}

OCF_END
