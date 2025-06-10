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
    : m_toggleGroup(nullptr)
{
}

RadioButton::~RadioButton()
{
}

void RadioButton::setToggleGroup(std::shared_ptr<ToggleGroup> group)
{
    m_toggleGroup = group;
    if (m_toggleGroup) {
        m_toggleGroup->addToggle(this);
    }
}

std::shared_ptr<ToggleGroup> RadioButton::getToggleGroup() const
{
    return m_toggleGroup;
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

    m_pButtonBackground->drawFillCircle(glm::vec2(7.0f, 7.0f), 7.0f, Color4f::GRAY);
    m_pButtonBackground->drawFillCircle(glm::vec2(7.0f, 7.0f), 6.0f, Color4f::WHITE);
    m_pCheckMark->drawFillCircle(glm::vec2(7.0f, 7.0f), 3.0f, Color4f::BLACK);

    setSize(RADIO_BUTTON_DEFAULT_SIZE, RADIO_BUTTON_DEFAULT_SIZE);
}

void RadioButton::updateTextLocation()
{
    if (m_pTextRenderer) {
        auto size = m_pTextRenderer->getSize();
        m_pTextRenderer->setPosition(glm::vec2(m_size.x + 5.0f, 0.0f));
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
