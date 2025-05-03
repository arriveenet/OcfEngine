#include "UISlider.h"
#include "2d/Camera.h"

NS_OCF_BEGIN

namespace ui {

Slider* ui::Slider::create(float widht, float height)
{
    Slider* slider = new Slider();
    if (slider->init()) {
        slider->setSize(widht, height);
        return slider;
    }
    OCF_SAFE_DELETE(slider);
    return nullptr;
}

Slider::Slider()
    : m_pSliderBackground(nullptr)
    , m_isDirty(true)
    , m_isDragging(false)
    , m_lastMousePosition(0.0f, 0.0f)
{
}

Slider::~Slider()
{
}

bool Slider::init()
{
    if (Widget::init()) {
        return true;
    }
    return false;
}

void Slider::updateNode(float /*deltaTime*/)
{
    glm::vec2 mousePos = Input::getMousePosition();
    Camera* camera = Camera::getDefaultCamera();

    if (isScreenPointInRect(mousePos, camera, getWorldToNodeTransform(), m_thumbRect, nullptr)) {
        if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Pressed) {
            m_isDragging = true;
        }
        else if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Released) {
            m_isDragging = false;
        }
    }
    else if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Released) {
        m_isDragging = false;
    }

    if (m_isDragging) {
        float deltaX = mousePos.x - m_lastMousePosition.x;
        m_thumbRect.m_position.x += deltaX;

        const float maxWidth = m_size.x - m_thumbRect.m_size.x;
        m_thumbRect.m_position.x = std::max(m_thumbRect.m_position.x, 0.0f);
        m_thumbRect.m_position.x = std::min(m_thumbRect.m_position.x, maxWidth);

        const float raito = m_thumbRect.m_position.x / maxWidth;
        m_value = static_cast<int>(raito * (m_maxValue - m_minValue) + m_minValue);
        m_isDirty = true;

        if(m_onValueChanged)
            m_onValueChanged(m_value);
    }

    updateSlider();

    m_lastMousePosition = mousePos;
}

void Slider::setSize(const glm::vec2& size)
{
    this->setSize(size.x, size.y);
}

void Slider::setSize(float width, float height)
{
    const float thumbX = width * (m_value - m_minValue) / (m_maxValue - m_minValue);
    const float thumbWidth = width * 0.05f;
    const float thumbHeight = height * 2.0f;
    m_thumbRect = Rect(thumbX, -thumbHeight / 4.0f, thumbWidth, thumbHeight);

    Node::setSize(glm::vec2(width, height));
}

void Slider::setValue(int value)
{
    if (value > m_minValue && value < m_maxValue) {
        m_value = value;
    }
    else if (value <= m_minValue) {
        m_value = m_minValue;
    }
    else if (value >= m_maxValue) {
        m_value = m_maxValue;
    }

    m_thumbRect.m_position.x = m_size.x * (m_value - m_minValue) / (m_maxValue - m_minValue);

    m_isDirty = true;
}

void Slider::setOnValueChangedCallback(std::function<void(int)> onValueChanged)
{
    m_onValueChanged = onValueChanged;
}

void Slider::initRenderer()
{
    m_pSliderBackground = DrawShape::create();

    addChild(m_pSliderBackground);
}

void Slider::updateSlider()
{
    if (!m_isDirty)
        return;

    m_pSliderBackground->clear();

    const float raito = static_cast<float>(m_value - m_minValue) / (m_maxValue - m_minValue);

    m_pSliderBackground->drawFilledRect(
        glm::vec2(0.0f, 0.0f),
        m_size,
        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

    if (raito > 0.0f) {
        m_pSliderBackground->drawFilledRect(
            glm::vec2(0.0f, 0.0f),
            glm::vec2(m_size.x * raito, m_size.y),
            glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
    }

    m_pSliderBackground->drawFilledRect(
        m_thumbRect.m_position,
        glm::vec2(m_thumbRect.getMaxX(), m_thumbRect.getMaxY()),
        glm::vec4(0.0f, 162.0f / 255.0f, 232.0f / 255.0f, 1.0f));

    m_isDirty = false;
}

} // namespace ui

NS_OCF_END
