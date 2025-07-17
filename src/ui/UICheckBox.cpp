#include "UICheckBox.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"

static constexpr float CHECKBOX_DEFAULT_SIZE = 14.0f;

NS_OCF_BEGIN

namespace ui {

CheckBox* CheckBox::create()
{
    CheckBox* checkBox = new CheckBox();
    if (checkBox->init()) {
        return checkBox;
    }
    OCF_SAFE_DELETE(checkBox);
    return nullptr;
}

CheckBox* CheckBox::create(std::string_view text)
{
    CheckBox* checkBox = new CheckBox();
    if (checkBox->init()) {
        checkBox->setText(text);
        return checkBox;
    }
    OCF_SAFE_DELETE(checkBox);
    return nullptr;
}

CheckBox::CheckBox()
    : m_isSelected(false)
    , m_isIndeterminate(false)
    , m_checkBoxSize(CHECKBOX_DEFAULT_SIZE, CHECKBOX_DEFAULT_SIZE)
{}

CheckBox::~CheckBox() {}

void CheckBox::initRenderer()
{
    m_pButtonBackground = DrawShape::create();
    m_pCheckMark = DrawShape::create();

    m_pButtonBackground->setLineWidth(2.0f);
    m_pCheckMark->setLineWidth(1.5f);
    m_pCheckMark->setVisible(m_isSelected);

    addChild(m_pButtonBackground);
    addChild(m_pCheckMark);

    setCheckBoxSize(CHECKBOX_DEFAULT_SIZE, CHECKBOX_DEFAULT_SIZE);
    setSize(glm::vec2(CHECKBOX_DEFAULT_SIZE, CHECKBOX_DEFAULT_SIZE));
}

void CheckBox::onMouseClicked()
{
    setSelected(!m_isSelected);

    if (m_onAction) {
        m_onAction();
    }
}

void CheckBox::setCheckBoxSize(float width, float height)
{
    constexpr glm::vec4 backgroundColor(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 1.0f);
    constexpr glm::vec4 outlineColor(204.0f / 255.0f, 204.0f / 255.0f, 204.0f / 255.0f, 1.0f);
    constexpr glm::vec4 checkedBackgoundColor(0.0f, 95.0f / 255.0f, 184.0f / 255.0f, 1.0f);
    const glm::vec2 checkBoxSize(width, height);

    glm::vec2 p1{}, p2{}, p3{};
    p1.x = (3.0f / CHECKBOX_DEFAULT_SIZE) * checkBoxSize.x;
    p1.y = (7.0f / CHECKBOX_DEFAULT_SIZE) * checkBoxSize.y;
    p2.x = (6.0f / CHECKBOX_DEFAULT_SIZE) * checkBoxSize.x;
    p2.y = (10.0f / CHECKBOX_DEFAULT_SIZE) * checkBoxSize.y;
    p3.x = (11.0f / CHECKBOX_DEFAULT_SIZE) * checkBoxSize.x;
    p3.y = (4.0f / CHECKBOX_DEFAULT_SIZE) * checkBoxSize.y;

    m_pButtonBackground->clear();
    m_pCheckMark->clear();

    m_pButtonBackground->drawFillRect(glm::vec2(0, 0), checkBoxSize, backgroundColor);
    m_pButtonBackground->drawRect(glm::vec2(0, 0), checkBoxSize, outlineColor);

    m_pCheckMark->drawLine(p1, p2, Color4f::WHITE);
    m_pCheckMark->drawLine(p2, p3, Color4f::WHITE);
}

void CheckBox::setSelected(bool selected)
{
    m_isSelected = selected;
    m_pCheckMark->setVisible(m_isSelected);
}

void CheckBox::setIndeterminate(bool indeterminate)
{
    m_isIndeterminate = indeterminate;
}

void CheckBox::updateTextLocation()
{
    if (m_pTextRenderer) {
        constexpr float textOffset = 5.0f;
        const glm::vec2 textSize = m_pTextRenderer->getSize();
        m_pTextRenderer->setPosition(glm::vec2(m_checkBoxSize.x + textOffset, 0.0f));
       
        const float width = m_checkBoxSize.x + textOffset + textSize.x;
        const float height = m_checkBoxSize.y;
        setSize(glm::vec2(width, height));
    }
}

} // namespace ui

NS_OCF_END