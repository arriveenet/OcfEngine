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
{ Widget::setSize(size); }

void CheckBox::setSize(float width, float height)
{ Widget::setSize(width, height); }

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

    m_pCheckMark->setLineWidth(1.5f);

    addChild(m_pButtonBackground);
    addChild(m_pCheckMark);

    setSize(CHECKBOX_DEFAULT_SIZE, CHECKBOX_DEFAULT_SIZE);
}

void CheckBox::updateNode(float deltaTime)
{
    ButtonBase::updateNode(deltaTime);

    constexpr glm::vec4 backgoundColor(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 1.0f);
    constexpr glm::vec4 outlineColor(204.0f / 255.0f, 204.0f / 255.0f, 204.0f / 255.0f, 1.0f);
    constexpr glm::vec4 checkedBackgoundColor(0.0f, 95.0f/ 255.0f, 184.0f /255.0f, 1.0f);
    const glm::vec2 contentSize = getSize();

    float p0X = (3.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.x;
    float p0Y = (7.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.y;
    float p1X = (6.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.x;
    float p1Y = (4.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.y;
    float p2X = (11.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.x;
    float p2Y = (10.0f / CHECKBOX_DEFAULT_SIZE) * contentSize.y;

    m_pButtonBackground->clear();
    m_pCheckMark->clear();

    m_pButtonBackground->drawFillRect(glm::vec2(0, 0), contentSize, backgoundColor);

    if (m_isSelected) {
        m_pButtonBackground->drawRect(glm::vec2(0, 0), contentSize, Color4f::WHITE);

        m_pCheckMark->drawLine(glm::vec2(p0X, p0Y), glm::vec2(p1X, p1Y), Color4f::WHITE);
        m_pCheckMark->drawLine(glm::vec2(p1X, p1Y), glm::vec2(p2X, p2Y), Color4f::WHITE);
    }
    else {
        m_pButtonBackground->drawRect(glm::vec2(0, 0), contentSize, outlineColor);
    }

}

void CheckBox::onMouseReleased()
{ m_isSelected = !m_isSelected; }

} // namespace ui

NS_OCF_END