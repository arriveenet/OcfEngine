#include "UICheckBox.h"
#include "2d/DrawShape.h"

static constexpr float CHECKBOX_DEFAULT_SIZE = 14.0f;

NS_OCF_BEGIN

namespace ui {

CheckBox *CheckBox::create(bool value)
{
    CheckBox* checkBox = new CheckBox();
    if (checkBox->init()) {
        checkBox->setSelected(value);
        return checkBox;
    }
    OCF_SAFE_DELETE(checkBox);
    return nullptr;
}

CheckBox::CheckBox()
    : m_isSelected(false)
    , m_pCheckMark(nullptr)
{}

CheckBox::~CheckBox() {}

void CheckBox::setSize(const glm::vec2& size)
{
    Widget::setSize(size);

    resizeContent(size.x, size.y);
}

void CheckBox::setSize(float width, float height)
{
    Widget::setSize(width, height);

    resizeContent(width, height);
}

bool CheckBox::init()
{
    if (Widget::init()) {
        return true;
    }
    return false;
}

void CheckBox::initRenderer()
{
    m_pButtonBackground = DrawShape::create();
    m_pCheckMark = DrawShape::create();

    m_pButtonBackground->setLineWidth(2.0f);
    m_pCheckMark->setLineWidth(1.5f);

    addChild(m_pButtonBackground);
    addChild(m_pCheckMark);

    setSize(CHECKBOX_DEFAULT_SIZE, CHECKBOX_DEFAULT_SIZE);
}

void CheckBox::onMouseClicked()
{
    m_isSelected = !m_isSelected;

    if (m_onAction) {
        m_onAction();
    }

    m_pCheckMark->setVisible(m_isSelected);
}

void CheckBox::resizeContent(float width, float height)
{
    constexpr glm::vec4 backgroundColor(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 1.0f);
    constexpr glm::vec4 outlineColor(204.0f / 255.0f, 204.0f / 255.0f, 204.0f / 255.0f, 1.0f);
    constexpr glm::vec4 checkedBackgoundColor(0.0f, 95.0f / 255.0f, 184.0f / 255.0f, 1.0f);
    const glm::vec2 contentSize(width, height);

    glm::vec2 p1{}, p2{}, p3{};
    p1.x = (3.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.x;
    p1.y = (7.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.y;
    p2.x = (6.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.x;
    p2.y = (4.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.y;
    p3.x = (11.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.x;
    p3.y= (10.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.y;

    m_pButtonBackground->clear();
    m_pCheckMark->clear();

    m_pButtonBackground->drawFillRect(glm::vec2(0, 0), contentSize, backgroundColor);
    m_pButtonBackground->drawRect(glm::vec2(0, 0), contentSize, outlineColor);

    m_pCheckMark->drawLine(p1, p2, Color4f::WHITE);
    m_pCheckMark->drawLine(p2, p3, Color4f::WHITE);
}

} // namespace ui

NS_OCF_END