#include "UIRadioButton.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"

static constexpr float RADIO_BUTTON_DEFAULT_SIZE = 14.0f;

NS_OCF_BEGIN

namespace ui {

RadioButton* RadioButton::create(std::string_view text)
{
    RadioButton* radioButton = new RadioButton();
    if (radioButton->init()) {
        if (!text.empty())
            radioButton->setText(text);
        return radioButton;
    }
    OCF_SAFE_DELETE(radioButton);
    return nullptr;
}

RadioButton::RadioButton()
{
}

RadioButton::~RadioButton()
{
}

bool RadioButton::init()
{
    if (Widget::init()) {
        return true;
    }
    return false;
}
void RadioButton::initRenderer()
{
    m_pButtonBackground = DrawShape::create();
    m_pCheckMark = DrawShape::create();

    addChild(m_pButtonBackground);
    addChild(m_pCheckMark);

    m_pButtonBackground->drawFillCircle(glm::vec2(), 7.0f, Color4f::GRAY);
    m_pButtonBackground->drawFillCircle(glm::vec2(), 6.0f, Color4f::WHITE);
    m_pCheckMark->drawFillCircle(glm::vec2(), 3.0f, Color4f::BLACK);

    setSize(RADIO_BUTTON_DEFAULT_SIZE, RADIO_BUTTON_DEFAULT_SIZE);
}

void RadioButton::updateTextLocation()
{
    if (m_pTextRenderer) {
        auto size = m_pTextRenderer->getSize();
        m_pTextRenderer->setPosition(
            glm::vec2(size.x + 5.0f, size.y / 2.0f));
    }
}

void RadioButton::onMouseClicked()
{
    m_isSelected = !m_isSelected;

    if (m_onAction) {
        m_onAction();
    }

    m_pCheckMark->setVisible(m_isSelected);
}

}; // namespace ui

NS_OCF_END
