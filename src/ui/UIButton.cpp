#include "UIButton.h"
#include "2d/Sprite.h"

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
	//printf("mousePos: %f, %f\n", mousePos.x, mousePos.y);
	if (hitTest(mousePos)) {
		if (inputState.mouse.getButtonState(Mouse::Left) == ButtonState::Pressed) {
			if (!m_onClick) {
				m_onClick;
			}
		}
	}
}

void Button::setOnClickCallback(std::function<void()> onClick)
{
	m_onClick = onClick;
}

void Button::initRenderer()
{
	m_pButtonNormalRenderer = Sprite::create("ButtonBlue.png");
	addChild(m_pButtonNormalRenderer);

	m_size = m_pButtonNormalRenderer->getSize();
}

}

OCF_END
