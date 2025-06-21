#include "UIRadioButton.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"
#include "UICheckBox.h"

static constexpr float RADIO_BUTTON_DEFAULT_SIZE = 14.0f;

NS_OCF_BEGIN

namespace ui {

void ToggleGroup::addToggle(Toggle* toggle)
{
    m_toggles.emplace_back(toggle);
}

void ToggleGroup::selectedToggle(Toggle* value)
{
    for (const auto toggle : m_toggles) {
        toggle->setSelected(toggle == value);
    }
}

Toggle* ToggleGroup::getSelectedToggle() const
{
    for (const auto toggle : m_toggles ) {
        if (toggle->isSelected())
            return toggle;
    }
    return nullptr;
}

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
    , m_isSelected(false)
    , m_radioSize(RADIO_BUTTON_DEFAULT_SIZE, RADIO_BUTTON_DEFAULT_SIZE)
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

bool RadioButton::isSelected() const
{
     return m_isSelected; 
}

void RadioButton::setSelected(bool selected)
{
    m_isSelected = selected;
    m_pCheckMark->setVisible(m_isSelected);
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
    m_pCheckMark->setVisible(m_isSelected);

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
        constexpr float textOffset = 5.0f;
        const glm::vec2 textSize = m_pTextRenderer->getSize();
        m_pTextRenderer->setPosition(m_radioSize.x + textOffset, 0.0f);

        const float width = m_radioSize.x + textOffset + textSize.x;
        const float height = m_radioSize.y;
        setSize(width, height);
    }
}

void RadioButton::onMouseClicked()
{
    if (m_toggleGroup) {
        m_toggleGroup->selectedToggle(this);
    }

    if (m_onAction) {
        m_onAction();
    }
}

}; // namespace ui

NS_OCF_END
